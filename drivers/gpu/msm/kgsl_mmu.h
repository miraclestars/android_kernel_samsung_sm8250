/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2002,2007-2019, The Linux Foundation. All rights reserved.
 */
#ifndef __KGSL_MMU_H
#define __KGSL_MMU_H

#include <linux/platform_device.h>

#include "kgsl_iommu.h"

/* Identifier for the global page table */
/*
 * Per process page tables will probably pass in the thread group
 *  as an identifier
 */
#define KGSL_MMU_GLOBAL_PT 0
#define KGSL_MMU_SECURE_PT 1

#define MMU_DEFAULT_TTBR0(_d) \
	(kgsl_mmu_pagetable_get_ttbr0((_d)->mmu.defaultpagetable))

#define MMU_DEFAULT_CONTEXTIDR(_d) \
	(kgsl_mmu_pagetable_get_contextidr((_d)->mmu.defaultpagetable))

struct kgsl_device;

enum kgsl_mmutype {
	KGSL_MMU_TYPE_IOMMU = 0,
	KGSL_MMU_TYPE_NONE
};

#define KGSL_IOMMU_SMMU_V500 1

struct kgsl_pagetable {
	spinlock_t lock;
	struct kref refcount;
	struct list_head list;
	unsigned int name;
	struct kobject *kobj;
	struct work_struct destroy_ws;

	struct {
		atomic_t entries;
		atomic_long_t mapped;
		atomic_long_t max_mapped;
	} stats;
	const struct kgsl_mmu_pt_ops *pt_ops;
	uint64_t fault_addr;
	void *priv;
	struct kgsl_mmu *mmu;
};

struct kgsl_mmu;

struct kgsl_mmu_ops {
	int (*probe)(struct kgsl_device *device);
	int (*mmu_init)(struct kgsl_mmu *mmu);
	void (*mmu_close)(struct kgsl_mmu *mmu);
	int (*mmu_start)(struct kgsl_mmu *mmu);
	void (*mmu_stop)(struct kgsl_mmu *mmu);
	int (*mmu_set_pt)(struct kgsl_mmu *mmu, struct kgsl_pagetable *pt);
	uint64_t (*mmu_get_current_ttbr0)(struct kgsl_mmu *mmu);
	void (*mmu_pagefault_resume)(struct kgsl_mmu *mmu);
	void (*mmu_clear_fsr)(struct kgsl_mmu *mmu);
	void (*mmu_enable_clk)(struct kgsl_mmu *mmu);
	void (*mmu_disable_clk)(struct kgsl_mmu *mmu);
	bool (*mmu_pt_equal)(struct kgsl_mmu *mmu,
			struct kgsl_pagetable *pt, u64 ttbr0);
	int (*mmu_set_pf_policy)(struct kgsl_mmu *mmu, unsigned long pf_policy);
	int (*mmu_init_pt)(struct kgsl_mmu *mmu, struct kgsl_pagetable *pt);
	void (*mmu_add_global)(struct kgsl_mmu *mmu,
			struct kgsl_memdesc *memdesc, const char *name);
	void (*mmu_remove_global)(struct kgsl_mmu *mmu,
			struct kgsl_memdesc *memdesc);
	struct kgsl_pagetable * (*mmu_getpagetable)(struct kgsl_mmu *mmu,
			unsigned long name);
	struct kgsl_memdesc* (*mmu_get_qdss_global_entry)(void);
	struct kgsl_memdesc* (*mmu_get_qtimer_global_entry)(void);
};

struct kgsl_mmu_pt_ops {
	int (*mmu_map)(struct kgsl_pagetable *pt,
			struct kgsl_memdesc *memdesc);
	int (*mmu_unmap)(struct kgsl_pagetable *pt,
			struct kgsl_memdesc *memdesc);
	void (*mmu_destroy_pagetable)(struct kgsl_pagetable *pt);
	u64 (*get_ttbr0)(struct kgsl_pagetable *pt);
	u32 (*get_contextidr)(struct kgsl_pagetable *pt);
	int (*get_gpuaddr)(struct kgsl_pagetable *pt,
				struct kgsl_memdesc *memdesc);
	void (*put_gpuaddr)(struct kgsl_memdesc *memdesc);
	uint64_t (*find_svm_region)(struct kgsl_pagetable *pt, uint64_t start,
		uint64_t end, uint64_t size, uint64_t align);
	int (*set_svm_region)(struct kgsl_pagetable *pt,
				uint64_t gpuaddr, uint64_t size);
	int (*svm_range)(struct kgsl_pagetable *pt, uint64_t *lo, uint64_t *hi,
			uint64_t memflags);
	bool (*addr_in_range)(struct kgsl_pagetable *pagetable,
			uint64_t gpuaddr);
	int (*mmu_map_offset)(struct kgsl_pagetable *pt,
			uint64_t virtaddr, uint64_t virtoffset,
			struct kgsl_memdesc *memdesc, uint64_t physoffset,
			uint64_t size, uint64_t flags);
	int (*mmu_unmap_offset)(struct kgsl_pagetable *pt,
			struct kgsl_memdesc *memdesc, uint64_t addr,
			uint64_t offset, uint64_t size);
	int (*mmu_sparse_dummy_map)(struct kgsl_pagetable *pt,
			struct kgsl_memdesc *memdesc, uint64_t offset,
			uint64_t size);
};

/*
 * MMU_FEATURE - return true if the specified feature is supported by the GPU
 * MMU
 */
#define MMU_FEATURE(_mmu, _bit) \
	((_mmu)->features & (_bit))

/* MMU has register retention */
#define KGSL_MMU_RETENTION  BIT(1)
/* MMU requires the TLB to be flushed on map */
#define KGSL_MMU_FLUSH_TLB_ON_MAP BIT(2)
/* MMU uses global pagetable */
#define KGSL_MMU_GLOBAL_PAGETABLE BIT(3)
/* MMU uses hypervisor for content protection */
#define KGSL_MMU_HYP_SECURE_ALLOC BIT(4)
/* Force 32 bit, even if the MMU can do 64 bit */
#define KGSL_MMU_FORCE_32BIT BIT(5)
/* 64 bit address is live */
#define KGSL_MMU_64BIT BIT(6)
/* The MMU supports non-contigious pages */
#define KGSL_MMU_PAGED BIT(8)
/* The device requires a guard page */
#define KGSL_MMU_NEED_GUARD_PAGE BIT(9)
/* The device supports IO coherency */
#define KGSL_MMU_IO_COHERENT BIT(10)

/**
 * struct kgsl_mmu - Master definition for KGSL MMU devices
 * @flags: MMU device flags
 * @type: Type of MMU that is attached
 * @subtype: Sub Type of MMU that is attached
 * @defaultpagetable: Default pagetable object for the MMU
 * @securepagetable: Default secure pagetable object for the MMU
 * @mmu_ops: Function pointers for the MMU sub-type
 * @secured: True if the MMU needs to be secured
 * @feature: Static list of MMU features
 * @secure_aligned_mask: Mask that secure buffers need to be aligned to
 * @priv: Union of sub-device specific members
 */
struct kgsl_mmu {
	unsigned long flags;
	enum kgsl_mmutype type;
	u32 subtype;
	struct kgsl_pagetable *defaultpagetable;
	struct kgsl_pagetable *securepagetable;
	const struct kgsl_mmu_ops *mmu_ops;
	bool secured;
	unsigned long features;
	unsigned int secure_align_mask;
	union {
		struct kgsl_iommu iommu;
	} priv;
};

/* KGSL MMU FLAGS */
#define KGSL_MMU_STARTED BIT(0)

#define KGSL_IOMMU_PRIV(_device) (&((_device)->mmu.priv.iommu))

extern struct kgsl_mmu_ops kgsl_iommu_ops;

int kgsl_mmu_probe(struct kgsl_device *device);
int kgsl_mmu_start(struct kgsl_device *device);
struct kgsl_pagetable *kgsl_mmu_getpagetable_ptbase(struct kgsl_mmu *mmu,
						u64 ptbase);

int kgsl_iommu_map_global_secure_pt_entry(struct kgsl_device *device,
					struct kgsl_memdesc *memdesc);
void kgsl_iommu_unmap_global_secure_pt_entry(struct kgsl_device *device,
					struct kgsl_memdesc *memdesc);
void kgsl_print_global_pt_entries(struct seq_file *s);
void kgsl_mmu_putpagetable(struct kgsl_pagetable *pagetable);

int kgsl_mmu_get_gpuaddr(struct kgsl_pagetable *pagetable,
		 struct kgsl_memdesc *memdesc);
int kgsl_mmu_map(struct kgsl_pagetable *pagetable,
		 struct kgsl_memdesc *memdesc);
int kgsl_mmu_unmap(struct kgsl_pagetable *pagetable,
		    struct kgsl_memdesc *memdesc);
void kgsl_mmu_put_gpuaddr(struct kgsl_memdesc *memdesc);
unsigned int kgsl_virtaddr_to_physaddr(void *virtaddr);
unsigned int kgsl_mmu_log_fault_addr(struct kgsl_mmu *mmu,
		u64 ttbr0, uint64_t addr);
bool kgsl_mmu_gpuaddr_in_range(struct kgsl_pagetable *pt, uint64_t gpuaddr);

int kgsl_mmu_get_region(struct kgsl_pagetable *pagetable,
		uint64_t gpuaddr, uint64_t size);

int kgsl_mmu_find_region(struct kgsl_pagetable *pagetable,
		uint64_t region_start, uint64_t region_end,
		uint64_t *gpuaddr, uint64_t size, unsigned int align);

struct kgsl_pagetable *kgsl_mmu_get_pt_from_ptname(struct kgsl_mmu *mmu,
							int ptname);
void kgsl_mmu_close(struct kgsl_device *device);

uint64_t kgsl_mmu_find_svm_region(struct kgsl_pagetable *pagetable,
		uint64_t start, uint64_t end, uint64_t size,
		uint64_t alignment);

int kgsl_mmu_set_svm_region(struct kgsl_pagetable *pagetable, uint64_t gpuaddr,
		uint64_t size);

void kgsl_mmu_detach_pagetable(struct kgsl_pagetable *pagetable);

int kgsl_mmu_svm_range(struct kgsl_pagetable *pagetable,
		uint64_t *lo, uint64_t *hi, uint64_t memflags);

struct kgsl_pagetable *kgsl_get_pagetable(unsigned long name);

struct kgsl_pagetable *
kgsl_mmu_createpagetableobject(struct kgsl_mmu *mmu, unsigned int name);

int kgsl_mmu_map_offset(struct kgsl_pagetable *pagetable,
		uint64_t virtaddr, uint64_t virtoffset,
		struct kgsl_memdesc *memdesc, uint64_t physoffset,
		uint64_t size, uint64_t flags);
int kgsl_mmu_unmap_offset(struct kgsl_pagetable *pagetable,
		struct kgsl_memdesc *memdesc, uint64_t addr, uint64_t offset,
		uint64_t size);

struct kgsl_memdesc *kgsl_mmu_get_qdss_global_entry(struct kgsl_device *device);

struct kgsl_memdesc *kgsl_mmu_get_qtimer_global_entry(
		struct kgsl_device *device);

int kgsl_mmu_sparse_dummy_map(struct kgsl_pagetable *pagetable,
		struct kgsl_memdesc *memdesc, uint64_t offset, uint64_t size);

/*
 * Static inline functions of MMU that simply call the SMMU specific
 * function using a function pointer. These functions can be thought
 * of as wrappers around the actual function
 */

#define MMU_OP_VALID(_mmu, _field) \
	(((_mmu) != NULL) && \
	 ((_mmu)->mmu_ops != NULL) && \
	 ((_mmu)->mmu_ops->_field != NULL))

#define PT_OP_VALID(_pt, _field) \
	(((_pt) != NULL) && \
	 ((_pt)->pt_ops != NULL) && \
	 ((_pt)->pt_ops->_field != NULL))

static inline u64 kgsl_mmu_get_current_ttbr0(struct kgsl_mmu *mmu)
{
	if (MMU_OP_VALID(mmu, mmu_get_current_ttbr0))
		return mmu->mmu_ops->mmu_get_current_ttbr0(mmu);

	return 0;
}

static inline struct kgsl_pagetable *kgsl_mmu_getpagetable(struct kgsl_mmu *mmu,
		unsigned long name)
{
	if (MMU_OP_VALID(mmu, mmu_getpagetable))
		return mmu->mmu_ops->mmu_getpagetable(mmu, name);

	return NULL;
}

static inline int kgsl_mmu_set_pt(struct kgsl_mmu *mmu,
					struct kgsl_pagetable *pagetable)
{
	if (MMU_OP_VALID(mmu, mmu_set_pt))
		return mmu->mmu_ops->mmu_set_pt(mmu, pagetable);

	return 0;
}

static inline void kgsl_mmu_stop(struct kgsl_mmu *mmu)
{
	if (MMU_OP_VALID(mmu, mmu_stop))
		mmu->mmu_ops->mmu_stop(mmu);
}

static inline bool kgsl_mmu_pt_equal(struct kgsl_mmu *mmu,
			struct kgsl_pagetable *pt, u64 ttbr0)
{
	if (MMU_OP_VALID(mmu, mmu_pt_equal))
		return mmu->mmu_ops->mmu_pt_equal(mmu, pt, ttbr0);

	return false;
}

static inline void kgsl_mmu_enable_clk(struct kgsl_mmu *mmu)
{
	if (MMU_OP_VALID(mmu, mmu_enable_clk))
		mmu->mmu_ops->mmu_enable_clk(mmu);
}

static inline void kgsl_mmu_disable_clk(struct kgsl_mmu *mmu)
{
	if (MMU_OP_VALID(mmu, mmu_disable_clk))
		mmu->mmu_ops->mmu_disable_clk(mmu);
}

static inline int kgsl_mmu_set_pagefault_policy(struct kgsl_mmu *mmu,
						unsigned long pf_policy)
{
	if (MMU_OP_VALID(mmu, mmu_set_pf_policy))
		return mmu->mmu_ops->mmu_set_pf_policy(mmu, pf_policy);

	return 0;
}

static inline void kgsl_mmu_pagefault_resume(struct kgsl_mmu *mmu)
{
	if (MMU_OP_VALID(mmu, mmu_pagefault_resume))
		return mmu->mmu_ops->mmu_pagefault_resume(mmu);
}

static inline void kgsl_mmu_clear_fsr(struct kgsl_mmu *mmu)
{
	if (MMU_OP_VALID(mmu, mmu_clear_fsr))
		return mmu->mmu_ops->mmu_clear_fsr(mmu);
}

static inline int kgsl_mmu_is_perprocess(struct kgsl_mmu *mmu)
{
	return MMU_FEATURE(mmu, KGSL_MMU_GLOBAL_PAGETABLE) ? 0 : 1;
}

static inline int kgsl_mmu_use_cpu_map(struct kgsl_mmu *mmu)
{
	return kgsl_mmu_is_perprocess(mmu);
}

static inline int kgsl_mmu_is_secured(struct kgsl_mmu *mmu)
{
	return mmu && (mmu->secured) && (mmu->securepagetable);
}

static inline u64
kgsl_mmu_pagetable_get_ttbr0(struct kgsl_pagetable *pagetable)
{
	if (PT_OP_VALID(pagetable, get_ttbr0))
		return pagetable->pt_ops->get_ttbr0(pagetable);

	return 0;
}

static inline u32
kgsl_mmu_pagetable_get_contextidr(struct kgsl_pagetable *pagetable)
{
	if (PT_OP_VALID(pagetable, get_contextidr))
		return pagetable->pt_ops->get_contextidr(pagetable);

	return 0;
}

static inline bool kgsl_mmu_bus_secured(struct device *dev)
{
	/*ARM driver contains all context banks on single bus */
	return true;
}

static inline struct bus_type *kgsl_mmu_get_bus(struct device *dev)
{
	return &platform_bus_type;
}
static inline struct device *kgsl_mmu_get_ctx(const char *name)
{
	return ERR_PTR(-ENODEV);
}

#endif /* __KGSL_MMU_H */
