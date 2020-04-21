/*
 * Copyright (c) 2013, Sony Mobile Communications AB.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#ifndef __PINCTRL_MSM_H__
#define __PINCTRL_MSM_H__

#include <linux/pinctrl/qcom-pinctrl.h>

struct pinctrl_pin_desc;

/**
 * struct msm_function - a pinmux function
 * @name:    Name of the pinmux function.
 * @groups:  List of pingroups for this function.
 * @ngroups: Number of entries in @groups.
 */
struct msm_function {
	const char *name;
	const char * const *groups;
	unsigned ngroups;
};

/**
 * struct msm_pingroup - Qualcomm pingroup definition
 * @name:                 Name of the pingroup.
 * @pins:	          A list of pins assigned to this pingroup.
 * @npins:	          Number of entries in @pins.
 * @funcs:                A list of pinmux functions that can be selected for
 *                        this group. The index of the selected function is used
 *                        for programming the function selector.
 *                        Entries should be indices into the groups list of the
 *                        struct msm_pinctrl_soc_data.
 * @ctl_reg:              Offset of the register holding control bits for this group.
 * @io_reg:               Offset of the register holding input/output bits for this group.
 * @intr_cfg_reg:         Offset of the register holding interrupt configuration bits.
 * @intr_status_reg:      Offset of the register holding the status bits for this group.
 * @intr_target_reg:      Offset of the register specifying routing of the interrupts
 *                        from this group.
 * @dir_conn_reg:         Offset of the register hmss setup in tile.
 * @mux_bit:              Offset in @ctl_reg for the pinmux function selection.
 * @pull_bit:             Offset in @ctl_reg for the bias configuration.
 * @drv_bit:              Offset in @ctl_reg for the drive strength configuration.
 * @oe_bit:               Offset in @ctl_reg for controlling output enable.
 * @in_bit:               Offset in @io_reg for the input bit value.
 * @out_bit:              Offset in @io_reg for the output bit value.
 * @intr_enable_bit:      Offset in @intr_cfg_reg for enabling the interrupt for this group.
 * @intr_status_bit:      Offset in @intr_status_reg for reading and acking the interrupt
 *                        status.
 * @intr_target_bit:      Offset in @intr_target_reg for configuring the interrupt routing.
 * @intr_target_kpss_val: Value in @intr_target_bit for specifying that the interrupt from
 *                        this gpio should get routed to the KPSS processor.
 * @intr_raw_status_bit:  Offset in @intr_cfg_reg for the raw status bit.
 * @intr_polarity_bit:    Offset in @intr_cfg_reg for specifying polarity of the interrupt.
 * @intr_detection_bit:   Offset in @intr_cfg_reg for specifying interrupt type.
 * @intr_detection_width: Number of bits used for specifying interrupt type,
 *                        Should be 2 for SoCs that can detect both edges in hardware,
 *                        otherwise 1.
 * @dir_conn_en_bit:      Offset in @intr_cfg_reg for direct connect enable bit
 * @wake_reg:             Offset of the WAKEUP_INT_EN register from base tile
 * @wake_bit:             Bit number for the corresponding gpio
 */
struct msm_pingroup {
	const char *name;
	const unsigned *pins;
	unsigned npins;

	unsigned *funcs;
	unsigned nfuncs;

	u32 ctl_reg;
	u32 io_reg;
	u32 intr_cfg_reg;
	u32 intr_status_reg;
	u32 intr_target_reg;
	u32 dir_conn_reg;

	unsigned mux_bit:5;

	unsigned pull_bit:5;
	unsigned drv_bit:5;

	unsigned egpio_enable:5;
	unsigned egpio_present:5;
	unsigned oe_bit:5;
	unsigned in_bit:5;
	unsigned out_bit:5;

	unsigned intr_enable_bit:5;
	unsigned intr_status_bit:5;
	unsigned intr_ack_high:1;

	unsigned intr_target_bit:5;
	unsigned intr_target_kpss_val:5;
	unsigned intr_raw_status_bit:5;
	unsigned intr_polarity_bit:5;
	unsigned intr_detection_bit:5;
	unsigned intr_detection_width:5;
	unsigned dir_conn_en_bit:8;

	u32 wake_reg;
	unsigned int wake_bit;
};

/**
 * struct msm_dir_conn - TLMM Direct GPIO connect configuration
 * @gpio:	GPIO pin number
 * @irq:	The GIC interrupt that the pin is connected to
 */
struct msm_dir_conn {
	int gpio;
	int irq;
};

/*
 * struct pinctrl_qup - Qup mode configuration
 * @mode:	Qup i3c mode
 * @offset:	Offset of the register
 */
struct pinctrl_qup {
	u32 mode;
	u32 offset;
};

/**
 * struct msm_pinctrl_soc_data - Qualcomm pin controller driver configuration
 * @pins:	    An array describing all pins the pin controller affects.
 * @npins:	    The number of entries in @pins.
 * @functions:	    An array describing all mux functions the SoC supports.
 * @nfunctions:	    The number of entries in @functions.
 * @groups:	    An array describing all pin groups the pin SoC supports.
 * @ngroups:	    The numbmer of entries in @groups.
 * @ngpio:	    The number of pingroups the driver should expose as GPIOs.
 * @pull_no_keeper: The SoC does not support keeper bias.
 * @wakeirq_map:    The map of wakeup capable GPIOs and the pin at PDC/MPM
 * @nwakeirq_map:   The number of entries in @hierarchy_map
 * @dir_conn:       An array describing all the pins directly connected to GIC.
 */
struct msm_pinctrl_soc_data {
	const struct pinctrl_pin_desc *pins;
	unsigned npins;
	const struct msm_function *functions;
	unsigned nfunctions;
	const struct msm_pingroup *groups;
	unsigned ngroups;
	unsigned ngpios;
	bool pull_no_keeper;
	struct pinctrl_qup *qup_regs;
	unsigned int nqup_regs;
	const int *reserved_gpios;
	struct msm_dir_conn *dir_conn;
};

int msm_pinctrl_probe(struct platform_device *pdev,
		      const struct msm_pinctrl_soc_data *soc_data);
int msm_pinctrl_remove(struct platform_device *pdev);

#ifdef CONFIG_QCOM_SHOW_RESUME_IRQ
extern int msm_show_resume_irq_mask;
#else
#define msm_show_resume_irq_mask 0
#endif

#endif
