/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2018-2019, The Linux Foundation. All rights reserved.
 */

/*
 * This header is for cdspl3 devfreq governor in drivers/devfreq.
 */

#ifndef __QCOM_CDSPRM_H__
#define __QCOM_CDSPRM_H__

/**
 * struct cdsprm_l3 - register with set L3 clock frequency method
 * @set_l3_freq:    Sets desired L3 clock frequency in kilo-hertz.
 *                  cdsprm module would call this method to set L3
 *                  clock frequency as requested by CDSP subsystem.
 */
struct cdsprm_l3 {
	int (*set_l3_freq)(unsigned int freq_khz);
};

/**
 * cdsprm_register_cdspl3gov() - Register a method to set L3 clock
 *                               frequency
 * @arg: cdsprm_l3 structure with set L3 clock frequency method
 *
 * Note: To be called from cdspl3 governor only. Called when the governor is
 *       started.
 */
void cdsprm_register_cdspl3gov(struct cdsprm_l3 *arg);

/**
 * cdsprm_unregister_cdspl3gov() - Unregister the method to set L3 clock
 *                                 frequency
 *
 * Note: To be called from cdspl3 governor only. Called when the governor is
 *       stopped
 */
void cdsprm_unregister_cdspl3gov(void);

#endif
