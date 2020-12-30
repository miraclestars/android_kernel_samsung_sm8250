// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2015, 2018, 2020, The Linux Foundation. All rights reserved.
 *
 */

#include <linux/i2c.h>
#include <linux/list.h>
#include <linux/string.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/delay.h>
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/stat.h>

#include <video/msm_dba.h>
#include "msm_dba_internal.h"

static inline struct msm_dba_device_info *to_dba_dev(struct device *dev)
{
	if (!dev) {
		pr_err("%s: dev is NULL\n", __func__);
		return NULL;
	}
	return dev_get_drvdata(dev);
}

static ssize_t device_name_show(struct device *dev,
				    struct device_attribute *attr,
				    char *buf)
{
	struct msm_dba_device_info *device = to_dba_dev(dev);

	if (!device) {
		pr_err("%s: device is NULL\n", __func__);
		return -EINVAL;
	}

	return scnprintf(buf, PAGE_SIZE, "%s:%d\n", device->chip_name,
						   device->instance_id);
}

static ssize_t client_list_show(struct device *dev,
				    struct device_attribute *attr,
				    char *buf)
{
	struct msm_dba_device_info *device = to_dba_dev(dev);
	struct msm_dba_client_info *c;
	struct list_head *pos = NULL;
	ssize_t bytes = 0;

	if (!device) {
		pr_err("%s: device is NULL\n", __func__);
		return -EINVAL;
	}

	mutex_lock(&device->dev_mutex);

	list_for_each(pos, &device->client_list) {
		c = list_entry(pos, struct msm_dba_client_info, list);
		bytes += scnprintf(buf + bytes, (PAGE_SIZE - bytes), "%s\n",
				c->client_name);
	}

	mutex_unlock(&device->dev_mutex);

	return bytes;
}

static ssize_t power_status_show(struct device *dev,
				     struct device_attribute *attr,
				     char *buf)
{
	struct msm_dba_device_info *device = to_dba_dev(dev);
	struct msm_dba_client_info *c;
	struct list_head *pos = NULL;
	ssize_t bytes = 0;

	if (!device) {
		pr_err("%s: device is NULL\n", __func__);
		return -EINVAL;
	}

	mutex_lock(&device->dev_mutex);
	bytes = scnprintf(buf, PAGE_SIZE, "power_status:%d\n",
			 device->power_status);

	list_for_each(pos, &device->client_list) {
		c = list_entry(pos, struct msm_dba_client_info, list);
		bytes += scnprintf(buf + bytes, (PAGE_SIZE - bytes),
				  "client: %s, status = %d\n",
				  c->client_name, c->power_on);
	}

	mutex_unlock(&device->dev_mutex);
	return bytes;
}

static ssize_t video_status_show(struct device *dev,
				     struct device_attribute *attr,
				     char *buf)
{
	struct msm_dba_device_info *device = to_dba_dev(dev);
	struct msm_dba_client_info *c;
	struct list_head *pos = NULL;
	ssize_t bytes = 0;

	if (!device) {
		pr_err("%s: device is NULL\n", __func__);
		return -EINVAL;
	}

	mutex_lock(&device->dev_mutex);
	bytes = scnprintf(buf, PAGE_SIZE, "video_status:%d\n",
			 device->video_status);

	list_for_each(pos, &device->client_list) {
		c = list_entry(pos, struct msm_dba_client_info, list);
		bytes += scnprintf(buf + bytes, (PAGE_SIZE - bytes),
				  "client: %s, status = %d\n",
				  c->client_name, c->video_on);
	}

	mutex_unlock(&device->dev_mutex);
	return bytes;
}

static ssize_t audio_status_show(struct device *dev,
				     struct device_attribute *attr,
				     char *buf)
{
	struct msm_dba_device_info *device = to_dba_dev(dev);
	struct msm_dba_client_info *c;
	struct list_head *pos = NULL;
	ssize_t bytes = 0;

	if (!device) {
		pr_err("%s: device is NULL\n", __func__);
		return -EINVAL;
	}

	mutex_lock(&device->dev_mutex);
	bytes = scnprintf(buf, PAGE_SIZE, "audio_status:%d\n",
			 device->audio_status);

	list_for_each(pos, &device->client_list) {
		c = list_entry(pos, struct msm_dba_client_info, list);
		bytes += scnprintf(buf + bytes, (PAGE_SIZE - bytes),
				  "client: %s, status = %d\n",
				  c->client_name, c->audio_on);
	}

	mutex_unlock(&device->dev_mutex);
	return bytes;
}

static ssize_t write_reg_store(struct device *dev,
				  struct device_attribute *attr,
				  const char *buf,
				  size_t count)
{
	struct msm_dba_device_info *device = to_dba_dev(dev);
	char *regstr, *valstr, *ptr;
	char str[20];
	long reg = 0;
	long val = 0;
	int rc = 0;
	int len;

	if (!device) {
		pr_err("%s: device is NULL\n", __func__);
		return -EINVAL;
	}

	len = strlen(buf);
	strlcpy(str, buf, 20);
	if (len < 20)
		str[len] = '\0';
	else
		str[19] = '\0';

	ptr = str;
	regstr = strsep(&ptr, ":");
	valstr = strsep(&ptr, ":");

	rc = kstrtol(regstr, 0, &reg);
	if (rc) {
		pr_err("%s: kstrol error %d\n", __func__, rc);
	} else {
		rc = kstrtol(valstr, 0, &val);
		if (rc)
			pr_err("%s: kstrol error for val %d\n", __func__, rc);
	}

	if (!rc) {
		mutex_lock(&device->dev_mutex);

		if (device->dev_ops.write_reg) {
			rc = device->dev_ops.write_reg(device,
						       (u32)reg,
						       (u32)val);

			if (rc) {
				pr_err("%s: failed to write reg %d\n", __func__,
				       rc);
			}
		} else {
			pr_err("%s: not supported\n", __func__);
		}

		mutex_unlock(&device->dev_mutex);
	}

	return count;
}

static ssize_t read_reg_show(struct device *dev,
				 struct device_attribute *attr,
				 char *buf)
{
	struct msm_dba_device_info *device = to_dba_dev(dev);
	ssize_t bytes;

	if (!device) {
		pr_err("%s: device is NULL\n", __func__);
		return -EINVAL;
	}

	mutex_lock(&device->dev_mutex);

	bytes = scnprintf(buf, PAGE_SIZE, "0x%x\n", device->register_val);

	mutex_unlock(&device->dev_mutex);

	return bytes;
}

static ssize_t read_reg_store(struct device *dev,
				 struct device_attribute *attr,
				 const char *buf,
				 size_t count)
{
	struct msm_dba_device_info *device = to_dba_dev(dev);
	long reg = 0;
	int rc = 0;
	u32 val = 0;

	if (!device) {
		pr_err("%s: device is NULL\n", __func__);
		return count;
	}

	rc = kstrtol(buf, 0, &reg);
	if (rc) {
		pr_err("%s: kstrol error %d\n", __func__, rc);
	} else {
		mutex_lock(&device->dev_mutex);

		if (device->dev_ops.read_reg) {
			rc = device->dev_ops.read_reg(device,
						      (u32)reg,
						      &val);

			if (rc) {
				pr_err("%s: failed to write reg %d\n", __func__,
				       rc);
			} else {
				device->register_val = val;
			}
		} else {
			pr_err("%s: not supported\n", __func__);
		}

		mutex_unlock(&device->dev_mutex);
	}

	return count;
}

static ssize_t dump_info_store(struct device *dev,
				  struct device_attribute *attr,
				  const char *buf,
				  size_t count)
{
	struct msm_dba_device_info *device = to_dba_dev(dev);
	int rc;

	if (!device) {
		pr_err("%s: device is NULL\n", __func__);
		return -EINVAL;
	}

	if (device->dev_ops.dump_debug_info) {
		rc = device->dev_ops.dump_debug_info(device, 0x00);
		if (rc)
			pr_err("%s: failed to dump debug data\n", __func__);
	} else {
		pr_err("%s: not supported\n", __func__);
	}

	return count;
}

static DEVICE_ATTR_RO(device_name);
static DEVICE_ATTR_RO(client_list);
static DEVICE_ATTR_RO(power_status);
static DEVICE_ATTR_RO(video_status);
static DEVICE_ATTR_RO(audio_status);
static DEVICE_ATTR_WO(write_reg);
static DEVICE_ATTR_RW(read_reg);
static DEVICE_ATTR_WO(dump_info);

static struct attribute *msm_dba_sysfs_attrs[] = {
	&dev_attr_device_name.attr,
	&dev_attr_client_list.attr,
	&dev_attr_power_status.attr,
	&dev_attr_video_status.attr,
	&dev_attr_audio_status.attr,
	&dev_attr_write_reg.attr,
	&dev_attr_read_reg.attr,
	&dev_attr_dump_info.attr,
	NULL,
};

static struct attribute_group msm_dba_sysfs_attr_grp = {
	.attrs = msm_dba_sysfs_attrs,
};

int msm_dba_helper_sysfs_init(struct device *dev)
{
	int rc = 0;

	if (!dev) {
		pr_err("%s: Invalid params\n", __func__);
		return -EINVAL;
	}

	rc = sysfs_create_group(&dev->kobj, &msm_dba_sysfs_attr_grp);
	if (rc)
		pr_err("%s: sysfs group creation failed %d\n", __func__, rc);

	return rc;
}

void msm_dba_helper_sysfs_remove(struct device *dev)
{
	if (!dev) {
		pr_err("%s: Invalid params\n", __func__);
		return;
	}

	sysfs_remove_group(&dev->kobj, &msm_dba_sysfs_attr_grp);
}
