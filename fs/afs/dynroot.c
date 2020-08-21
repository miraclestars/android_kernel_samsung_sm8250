/* AFS dynamic root handling
 *
 * Copyright (C) 2018 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
 */

#include <linux/fs.h>
#include <linux/namei.h>
#include <linux/dns_resolver.h>
#include "internal.h"

const struct file_operations afs_dynroot_file_operations = {
	.open		= dcache_dir_open,
	.release	= dcache_dir_close,
	.iterate_shared	= dcache_readdir,
	.llseek		= dcache_dir_lseek,
};

/*
 * Probe to see if a cell may exist.  This prevents positive dentries from
 * being created unnecessarily.
 */
static int afs_probe_cell_name(struct dentry *dentry)
{
	struct afs_cell *cell;
	const char *name = dentry->d_name.name;
	size_t len = dentry->d_name.len;
	int ret;

	/* Names prefixed with a dot are R/W mounts. */
	if (name[0] == '.') {
		if (len == 1)
			return -EINVAL;
		name++;
		len--;
	}

	cell = afs_lookup_cell_rcu(afs_d2net(dentry), name, len);
	if (!IS_ERR(cell)) {
		afs_put_cell(afs_d2net(dentry), cell);
		return 0;
	}

	ret = dns_query("afsdb", name, len, "", NULL, NULL);
	if (ret == -ENODATA)
		ret = -EDESTADDRREQ;
	return ret;
}

/*
 * Try to auto mount the mountpoint with pseudo directory, if the autocell
 * operation is setted.
 */
struct inode *afs_try_auto_mntpt(struct dentry *dentry, struct inode *dir)
{
	struct afs_vnode *vnode = AFS_FS_I(dir);
	struct inode *inode;
	int ret = -ENOENT;

	_enter("%p{%pd}, {%x:%u}",
	       dentry, dentry, vnode->fid.vid, vnode->fid.vnode);

	if (!test_bit(AFS_VNODE_AUTOCELL, &vnode->flags))
		goto out;

	ret = afs_probe_cell_name(dentry);
	if (ret < 0)
		goto out;

	inode = afs_iget_pseudo_dir(dir->i_sb, false);
	if (IS_ERR(inode)) {
		ret = PTR_ERR(inode);
		goto out;
	}

	_leave("= %p", inode);
	return inode;

out:
	_leave("= %d", ret);
	return ret == -ENOENT ? NULL : ERR_PTR(ret);
}

/*
 * Look up @cell in a dynroot directory.  This is a substitution for the
 * local cell name for the net namespace.
 */
static struct dentry *afs_lookup_atcell(struct dentry *dentry)
{
	struct afs_cell *cell;
	struct afs_net *net = afs_d2net(dentry);
	struct dentry *ret;
	unsigned int seq = 0;
	char *name;
	int len;

	if (!net->ws_cell)
		return ERR_PTR(-ENOENT);

	ret = ERR_PTR(-ENOMEM);
	name = kmalloc(AFS_MAXCELLNAME + 1, GFP_KERNEL);
	if (!name)
		goto out_p;

	rcu_read_lock();
	do {
		read_seqbegin_or_lock(&net->cells_lock, &seq);
		cell = rcu_dereference_raw(net->ws_cell);
		if (cell) {
			len = cell->name_len;
			memcpy(name, cell->name, len + 1);
		}
	} while (need_seqretry(&net->cells_lock, seq));
	done_seqretry(&net->cells_lock, seq);
	rcu_read_unlock();

	ret = ERR_PTR(-ENOENT);
	if (!cell)
		goto out_n;

	ret = lookup_one_len(name, dentry->d_parent, len);

	/* We don't want to d_add() the @cell dentry here as we don't want to
	 * the cached dentry to hide changes to the local cell name.
	 */

out_n:
	kfree(name);
out_p:
	return ret;
}

/*
 * Look up an entry in a dynroot directory.
 */
static struct dentry *afs_dynroot_lookup(struct inode *dir, struct dentry *dentry,
					 unsigned int flags)
{
	_enter("%pd", dentry);

	ASSERTCMP(d_inode(dentry), ==, NULL);

	if (flags & LOOKUP_CREATE)
		return ERR_PTR(-EOPNOTSUPP);

	if (dentry->d_name.len >= AFSNAMEMAX) {
		_leave(" = -ENAMETOOLONG");
		return ERR_PTR(-ENAMETOOLONG);
	}

	if (dentry->d_name.len == 5 &&
	    memcmp(dentry->d_name.name, "@cell", 5) == 0)
		return afs_lookup_atcell(dentry);

	return d_splice_alias(afs_try_auto_mntpt(dentry, dir), dentry);
}

const struct inode_operations afs_dynroot_inode_operations = {
	.lookup		= afs_dynroot_lookup,
};

/*
 * Dirs in the dynamic root don't need revalidation.
 */
static int afs_dynroot_d_revalidate(struct dentry *dentry, unsigned int flags)
{
	return 1;
}

/*
 * Allow the VFS to enquire as to whether a dentry should be unhashed (mustn't
 * sleep)
 * - called from dput() when d_count is going to 0.
 * - return 1 to request dentry be unhashed, 0 otherwise
 */
static int afs_dynroot_d_delete(const struct dentry *dentry)
{
	return d_really_is_positive(dentry);
}

const struct dentry_operations afs_dynroot_dentry_operations = {
	.d_revalidate	= afs_dynroot_d_revalidate,
	.d_delete	= afs_dynroot_d_delete,
	.d_release	= afs_d_release,
	.d_automount	= afs_d_automount,
};

/*
 * Create a manually added cell mount directory.
 * - The caller must hold net->proc_cells_lock
 */
int afs_dynroot_mkdir(struct afs_net *net, struct afs_cell *cell)
{
	struct super_block *sb = net->dynroot_sb;
	struct dentry *root, *subdir;
	int ret;

	if (!sb || atomic_read(&sb->s_active) == 0)
		return 0;

	/* Let the ->lookup op do the creation */
	root = sb->s_root;
	inode_lock(root->d_inode);
	subdir = lookup_one_len(cell->name, root, cell->name_len);
	if (IS_ERR(subdir)) {
		ret = PTR_ERR(subdir);
		goto unlock;
	}

	/* Note that we're retaining an extra ref on the dentry */
	subdir->d_fsdata = (void *)1UL;
	ret = 0;
unlock:
	inode_unlock(root->d_inode);
	return ret;
}

/*
 * Remove a manually added cell mount directory.
 * - The caller must hold net->proc_cells_lock
 */
void afs_dynroot_rmdir(struct afs_net *net, struct afs_cell *cell)
{
	struct super_block *sb = net->dynroot_sb;
	struct dentry *root, *subdir;

	if (!sb || atomic_read(&sb->s_active) == 0)
		return;

	root = sb->s_root;
	inode_lock(root->d_inode);

	/* Don't want to trigger a lookup call, which will re-add the cell */
	subdir = try_lookup_one_len(cell->name, root, cell->name_len);
	if (IS_ERR_OR_NULL(subdir)) {
		_debug("lookup %ld", PTR_ERR(subdir));
		goto no_dentry;
	}

	_debug("rmdir %pd %u", subdir, d_count(subdir));

	if (subdir->d_fsdata) {
		_debug("unpin %u", d_count(subdir));
		subdir->d_fsdata = NULL;
		dput(subdir);
	}
	dput(subdir);
no_dentry:
	inode_unlock(root->d_inode);
	_leave("");
}

/*
 * Populate a newly created dynamic root with cell names.
 */
int afs_dynroot_populate(struct super_block *sb)
{
	struct afs_cell *cell;
	struct afs_net *net = afs_sb2net(sb);
	int ret;

	if (mutex_lock_interruptible(&net->proc_cells_lock) < 0)
		return -ERESTARTSYS;

	net->dynroot_sb = sb;
	hlist_for_each_entry(cell, &net->proc_cells, proc_link) {
		ret = afs_dynroot_mkdir(net, cell);
		if (ret < 0)
			goto error;
	}

	ret = 0;
out:
	mutex_unlock(&net->proc_cells_lock);
	return ret;

error:
	net->dynroot_sb = NULL;
	goto out;
}

/*
 * When a dynamic root that's in the process of being destroyed, depopulate it
 * of pinned directories.
 */
void afs_dynroot_depopulate(struct super_block *sb)
{
	struct afs_net *net = afs_sb2net(sb);
	struct dentry *root = sb->s_root, *subdir, *tmp;

	/* Prevent more subdirs from being created */
	mutex_lock(&net->proc_cells_lock);
	if (net->dynroot_sb == sb)
		net->dynroot_sb = NULL;
	mutex_unlock(&net->proc_cells_lock);

	if (root) {
		inode_lock(root->d_inode);

		/* Remove all the pins for dirs created for manually added cells */
		list_for_each_entry_safe(subdir, tmp, &root->d_subdirs, d_child) {
			if (subdir->d_fsdata) {
				subdir->d_fsdata = NULL;
				dput(subdir);
			}
		}

		inode_unlock(root->d_inode);
	}
}
