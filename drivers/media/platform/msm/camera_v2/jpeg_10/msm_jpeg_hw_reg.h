/* SPDX-License-Identifier: GPL-2.0-only */
/* Copyright (c) 2012-2015, 2018,2020 The Linux Foundation. All rights reserved.
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

#ifndef MSM_JPEG_HW_REG_H
#define MSM_JPEG_HW_REG_H

#define JPEG_REG_BASE 0

#define MSM_JPEG_HW_IRQ_MASK_ADDR 0x00000018
#define MSM_JPEG_HW_IRQ_MASK_RMSK 0xFFFFFFFF
#define MSM_JPEG_HW_IRQ_ENABLE 0xFFFFFFFF

#define MSM_JPEG_HW_IRQ_STATUS_FRAMEDONE_MASK 0x00000001
#define MSM_JPEG_HW_IRQ_STATUS_FRAMEDONE_SHIFT 0x00000000

#define MSM_JPEG_HW_IRQ_STATUS_FE_RD_DONE_MASK 0x00000010
#define MSM_JPEG_HW_IRQ_STATUS_FE_RD_DONE_SHIFT 0x00000001

#define MSM_JPEG_HW_IRQ_STATUS_FE_RTOVF_MASK 0x00000004
#define MSM_JPEG_HW_IRQ_STATUS_FE_RTOVF_SHIFT 0x00000002

#define MSM_JPEG_HW_IRQ_STATUS_FE_VFE_OVERFLOW_MASK 0x00000008
#define MSM_JPEG_HW_IRQ_STATUS_FE_VFE_OVERFLOW_SHIFT 0x00000003

#define MSM_JPEG_HW_IRQ_STATUS_WE_Y_PINGPONG_MASK 0x00000010
#define MSM_JPEG_HW_IRQ_STATUS_WE_Y_PINGPONG_SHIFT 0x00000004

#define MSM_JPEG_HW_IRQ_STATUS_WE_CBCR_PINGPONG_MASK 0x00000020
#define MSM_JPEG_HW_IRQ_STATUS_WE_CBCR_PINGPONG_SHIFT 0x00000005

#define MSM_JPEG_HW_IRQ_STATUS_RESET_ACK_MASK 0x10000000
#define MSM_JPEG_HW_IRQ_STATUS_RESET_ACK_SHIFT 0x0000000a

#define MSM_JPEG_HW_IRQ_STATUS_BUS_ERROR_MASK 0x00000800
#define MSM_JPEG_HW_IRQ_STATUS_BUS_ERROR_SHIFT 0x0000000b

#define MSM_JPEG_HW_IRQ_STATUS_DCD_UNESCAPED_FF      (0x1<<19)
#define MSM_JPEG_HW_IRQ_STATUS_DCD_HUFFMAN_ERROR     (0x1<<20)
#define MSM_JPEG_HW_IRQ_STATUS_DCD_COEFFICIENT_ERR   (0x1<<21)
#define MSM_JPEG_HW_IRQ_STATUS_DCD_MISSING_BIT_STUFF (0x1<<22)
#define MSM_JPEG_HW_IRQ_STATUS_DCD_SCAN_UNDERFLOW    (0x1<<23)
#define MSM_JPEG_HW_IRQ_STATUS_DCD_INVALID_RSM       (0x1<<24)
#define MSM_JPEG_HW_IRQ_STATUS_DCD_INVALID_RSM_SEQ   (0x1<<25)
#define MSM_JPEG_HW_IRQ_STATUS_DCD_MISSING_RSM       (0x1<<26)
#define MSM_JPEG_HW_IRQ_STATUS_VIOLATION_MASK        (0x1<<29)

#define JPEG_OFFLINE_CMD_START 0x00000001

#define JPEG_RESET_DEFAULT 0x00032093

#define JPEG_IRQ_DISABLE_ALL 0x00000000
#define JPEG_IRQ_CLEAR_ALL 0xFFFFFFFF

#define JPEG_PLN0_RD_PNTR_ADDR (JPEG_REG_BASE + 0x00000038)
#define JPEG_PLN0_RD_PNTR_BMSK  0xFFFFFFFF

#define JPEG_PLN0_RD_OFFSET_ADDR 0x0000003C
#define JPEG_PLN0_RD_OFFSET_BMSK 0xFFFFFFFF

#define JPEG_PLN1_RD_PNTR_ADDR (JPEG_REG_BASE + 0x00000044)
#define JPEG_PLN1_RD_PNTR_BMSK 0xFFFFFFFF

#define JPEG_PLN1_RD_OFFSET_ADDR 0x00000048
#define JPEG_PLN1_RD_OFFSET_BMSK 0xFFFFFFFF

#define JPEG_PLN2_RD_PNTR_ADDR (JPEG_REG_BASE + 0x00000050)
#define JPEG_PLN2_RD_PNTR_BMSK 0xFFFFFFFF

#define JPEG_PLN2_RD_OFFSET_ADDR 0x00000054
#define JPEG_PLN2_RD_OFFSET_BMSK 0xFFFFFFFF

#define JPEG_CMD_ADDR (JPEG_REG_BASE + 0x00000010)
#define JPEG_CMD_BMSK 0xFFFFFFFF
#define JPEG_CMD_CLEAR_WRITE_PLN_QUEUES 0x700

#define JPEG_PLN0_WR_PNTR_ADDR (JPEG_REG_BASE + 0x000000cc)
#define JPEG_PLN0_WR_PNTR_BMSK 0xFFFFFFFF

#define JPEG_PLN1_WR_PNTR_ADDR (JPEG_REG_BASE + 0x000000D0)
#define JPEG_PLN1_WR_PNTR_BMSK 0xFFFFFFFF

#define JPEG_PLN2_WR_PNTR_ADDR (JPEG_REG_BASE + 0x000000D4)
#define JPEG_PLN2_WR_PNTR_BMSK 0xFFFFFFFF

#define JPEG_IRQ_MASK_ADDR (JPEG_REG_BASE + 0x00000018)
#define JPEG_IRQ_MASK_BMSK 0xFFFFFFFF
#define JPEG_IRQ_ALLSOURCES_ENABLE 0xFFFFFFFF

#define JPEG_IRQ_CLEAR_ADDR (JPEG_REG_BASE + 0x0000001c)
#define JPEG_IRQ_CLEAR_BMSK 0xFFFFFFFF

#define JPEG_RESET_CMD_ADDR (JPEG_REG_BASE + 0x00000008)
#define JPEG_RESET_CMD_RMSK 0xFFFFFFFF

#define JPEG_IRQ_STATUS_ADDR (JPEG_REG_BASE + 0x00000020)
#define JPEG_IRQ_STATUS_BMSK 0xFFFFFFFF

#define MSM_JPEG_S0_MMU_PF_ADDR_MIN (JPEG_REG_BASE + 0x00000310)
#define MSM_JPEG_S0_MMU_PF_ADDR_MIN_BMSK 0xFFFFFFFF

#define MSM_JPEG_S0_MMU_PF_ADDR_MAX (JPEG_REG_BASE + 0x00000314)
#define MSM_JPEG_S0_MMU_PF_ADDR_MAX_BMSK 0xFFFFFFFF

#define MSM_JPEG_S1_MMU_PF_ADDR_MIN (JPEG_REG_BASE + 0x0000031C)
#define MSM_JPEG_S1_MMU_PF_ADDR_MIN_BMSK 0xFFFFFFFF

#define MSM_JPEG_S1_MMU_PF_ADDR_MAX (JPEG_REG_BASE + 0x00000320)
#define MSM_JPEG_S1_MMU_PF_ADDR_MAX_BMSK 0xFFFFFFFF

#define MSM_JPEG_S2_MMU_PF_ADDR_MIN (JPEG_REG_BASE + 0x00000328)
#define MSM_JPEG_S2_MMU_PF_ADDR_MIN_BMSK 0xFFFFFFFF

#define MSM_JPEG_S2_MMU_PF_ADDR_MAX (JPEG_REG_BASE + 0x0000032C)
#define MSM_JPEG_S2_MMU_PF_ADDR_MAX_BMSK 0xFFFFFFFF

#define MSM_JPEG_S3_MMU_PF_ADDR_MIN (JPEG_REG_BASE + 0x00000334)
#define MSM_JPEG_S3_MMU_PF_ADDR_MIN_BMSK 0xFFFFFFFF

#define MSM_JPEG_S3_MMU_PF_ADDR_MAX (JPEG_REG_BASE + 0x00000338)
#define MSM_JPEG_S3_MMU_PF_ADDR_MAX_BMSK 0xFFFFFFFF

#define JPEG_ENCODE_OUTPUT_SIZE_STATUS_ADDR (JPEG_REG_BASE + 0x00000180)
#define JPEG_ENCODE_OUTPUT_SIZE_STATUS_BMSK 0x1FFFFFFF

#define JPEG_DECODE_MCUS_DECODED_STATUS   (JPEG_REG_BASE + 0x00000258)
#define JPEG_DECODE_BITS_CONSUMED_STATUS  (JPEG_REG_BASE + 0x0000025C)
#define JPEG_DECODE_PRED_Y_STATE          (JPEG_REG_BASE + 0x00000260)
#define JPEG_DECODE_PRED_C_STATE          (JPEG_REG_BASE + 0x00000264)
#define JPEG_DECODE_RSM_STATE             (JPEG_REG_BASE + 0x00000268)

#define JPEG_HW_VERSION                   (JPEG_REG_BASE + 0x00000000)

#define VBIF_BASE_ADDRESS                      0xFDA60000
#define VBIF_REGION_SIZE                       0xC30
#define JPEG_VBIF_CLKON                        0x4
#define JPEG_VBIF_IN_RD_LIM_CONF0              0xB0
#define JPEG_VBIF_IN_RD_LIM_CONF1              0xB4
#define JPEG_VBIF_IN_RD_LIM_CONF2              0xB8
#define JPEG_VBIF_IN_WR_LIM_CONF0              0xC0
#define JPEG_VBIF_IN_WR_LIM_CONF1              0xC4
#define JPEG_VBIF_IN_WR_LIM_CONF2              0xC8
#define JPEG_VBIF_OUT_RD_LIM_CONF0             0xD0
#define JPEG_VBIF_OUT_WR_LIM_CONF0             0xD4
#define JPEG_VBIF_DDR_OUT_MAX_BURST            0xD8
#define JPEG_VBIF_OCMEM_OUT_MAX_BURST          0xDC
#define JPEG_VBIF_ARB_CTL                      0xF0
#define JPEG_VBIF_OUT_AXI_AOOO_EN              0x178
#define JPEG_VBIF_OUT_AXI_AOOO                 0x17c
#define JPEG_VBIF_ROUND_ROBIN_QOS_ARB          0x124
#define JPEG_VBIF_OUT_AXI_AMEMTYPE_CONF0       0x160
#define JPEG_VBIF_OUT_AXI_AMEMTYPE_CONF1       0x164

#define JPEGDMA_IRQ_MASK_ADDR (JPEG_REG_BASE + 0x0000000C)
#define JPEGDMA_IRQ_MASK_BMSK 0xFFFFFFFF
#define JPEGDMA_IRQ_ALLSOURCES_ENABLE 0xFFFFFFFF

#define JPEGDMA_IRQ_CLEAR_ADDR (JPEG_REG_BASE + 0x00000014)
#define JPEGDMA_IRQ_CLEAR_BMSK 0xFFFFFFFF

#define JPEGDMA_RESET_CMD_ADDR (JPEG_REG_BASE + 0x00000008)
#define JPEGDMA_RESET_CMD_BMSK 0xFFFFFFFF

#define JPEGDMA_IRQ_STATUS_ADDR (JPEG_REG_BASE + 0x00000010)
#define JPEGDMA_IRQ_STATUS_BMSK 0xFFFFFFFF
#define JPEGDMA_RESET_DEFAULT 0x00032083


#define JPEGDMA_CMD_ADDR (JPEG_REG_BASE + 0x0000001C)
#define JPEGDMA_CMD_BMSK (0xFFFFFFFF)
#define JPEGDMA_CMD_CLEAR_READ_PLN_QUEUES  0x030
#define JPEGDMA_CMD_CLEAR_WRITE_PLN_QUEUES 0x300

#define JPEGDMA_IRQ_DISABLE_ALL 0x00000000
#define JPEGDMA_IRQ_CLEAR_ALL 0x00001FFF
#define MSM_JPEGDMA_HW_IRQ_STATUS_FRAMEDONE_MASK  0x00000001
#define MSM_JPEGDMA_HW_IRQ_STATUS_FRAMEDONE_SHIFT 0x00000000
#define MSM_JPEGDMA_HW_IRQ_STATUS_FE_RD_DONE_MASK 0x00000006
#define MSM_JPEGDMA_HW_IRQ_STATUS_FE_RD_DONE_SHIFT 0x00000001
#define MSM_JPEGDMA_HW_IRQ_STATUS_WE_WR_DONE_MASK 0x00000060
#define MSM_JPEGDMA_HW_IRQ_STATUS_WE_WR_DONE_SHIFT 0x00000005
#define MSM_JPEGDMA_HW_IRQ_STATUS_RESET_ACK_MASK  0x00000400
#define MSM_JPEGDMA_HW_IRQ_STATUS_RESET_ACK_SHIFT 0x0000000a

#define MSM_JPEGDMA_FE_0_RD_PNTR (JPEG_REG_BASE + 0x00000034)
#define MSM_JPEGDMA_FE_1_RD_PNTR (JPEG_REG_BASE + 0x00000078)
#define MSM_JPEGDMA_WE_0_WR_PNTR (JPEG_REG_BASE + 0x000000BC)
#define MSM_JPEGDMA_WE_1_WR_PNTR (JPEG_REG_BASE + 0x000000EC)

#define MSM_JPEGDMA_S0_MMU_PF_ADDR_MIN (JPEG_REG_BASE + 0x00000190)
#define MSM_JPEGDMA_S0_MMU_PF_ADDR_MIN_BMSK 0xFFFFFFFF

#define MSM_JPEGDMA_S0_MMU_PF_ADDR_MAX (JPEG_REG_BASE + 0x00000198)
#define MSM_JPEGDMA_S0_MMU_PF_ADDR_MAX_BMSK 0xFFFFFFFF

#define MSM_JPEGDMA_S1_MMU_PF_ADDR_MIN (JPEG_REG_BASE + 0x000001A4)
#define MSM_JPEGDMA_S1_MMU_PF_ADDR_MIN_BMSK 0xFFFFFFFF

#define MSM_JPEGDMA_S1_MMU_PF_ADDR_MAX (JPEG_REG_BASE + 0x000001AC)
#define MSM_JPEGDMA_S1_MMU_PF_ADDR_MAX_BMSK 0xFFFFFFFF

#endif /* MSM_JPEG_HW_REG_H */
