// Copyright 2019 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SRC_GRAPHICS_DISPLAY_DRIVERS_HIKEY_DISPLAY_HIDISPLAY_REGS_H_
#define SRC_GRAPHICS_DISPLAY_DRIVERS_HIKEY_DISPLAY_HIDISPLAY_REGS_H_
#define DW_DSI_PHY_TST_CTRL1_TESTEN (1 << 16)
#define DS_NUM_LANES 4
#define DW_DSI_DPI_COLOR_CODING 0x10 /* configs DPI color coding */
#define DW_DSI_VID_MODE_CFG 0x38     /* Video mode operation config */
#define DSI_NON_BURST_SYNC_PULSES 0
#define DW_DSI_PCKHDL_CFG 0x2c /* how EoTp, BTA, CRC and ECC are to be used */
#define DW_DSI_LPCLK_CTRL 0x94 /* non continuous clock in the clock lane. */

// ADV7533 Registers
#define ADV7533_REG_CHIP_REVISION 0x00
#define ADV7533_REG_POWER 0x41
#define ADV7533_REG_STATUS 0x42
#define ADV7533_REG_EDID_I2C_ADDR 0x43
#define ADV7533_REG_PACKET_ENABLE1 0x44
#define ADV7533_REG_PACKET_I2C_ADDR 0x45
#define ADV7533_REG_INT0_ENABLE 0x94
#define ADV7533_REG_INT1_ENABLE 0x95
#define ADV7533_REG_INT0 0x96
#define ADV7533_REG_INT1 0x96
#define ADV7533_REG_HDCP_HDMI_CFG 0xaf
#define ADV7533_REG_EDID_SEGMENT 0xc4
#define ADV7533_REG_DDC_STATUS 0xc8
#define ADV7533_REG_EDID_READ_CTRL 0xc9
#define ADV7533_REG_POWER2 0xd6
#define ADV7533_REG_CEC_I2C_ADDR 0xe1
#define ADV7533_REG_CEC_CTRL 0xe2
#define ADV7533_REG_CHIP_ID_HIGH 0xf5
#define ADV7533_REG_CHIP_ID_LOW 0xf6

#define EDID_I2C_ADDR (0x3B << 1)
#define PACKET_I2C_ADDR (0x34 << 1)
#define CEC_I2C_ADDR (0x38 << 1)

/* ADV7533_REG_POWER Bit Definitions */
#define REG_POWER_PWR_UP (0x10)
#define REG_POWER_PWR_DWN (0x50)

/* ADV7533_REG_STATUS Bit Definitions */
#define REG_STATUS_HPD_DET (1 << 6)
#define REG_STATUS_MON_SNS_DET (1 << 5)

/* ADV7533_REG_PACKET_ENABLE1 Bit Definitions */
#define PACKET_ENABLE_DISABLE (0x0)

/* ADV7533_REG_INT_ENABLE Bit Definitions */
#define REG_INT0_ENABLE_HPD (1 << 7)
#define REG_INT0_ENABLE_EDID_RDY (1 << 2)
#define REG_INT1_ENABLE_DDC_ERR (1 << 7)

/* ADV7533_REG_INT0 Bit Definitions */
#define REG_INT0_HPD (1 << 7)

/* ADV7533_REG_POWER2 Bit Definitions */
#define REG_POWER2_HPD_ALWAYS_HIGH (0xC0)

/* ADV7533_REG_DDC_STATUS Bit Definition */
#define REG_DDC_STATUS_EDID_READY (0x2)

/* ADV7533_REG_HDCP_HDMI_CFG Bit Definition */
#define REG_HDCP_HDMI_CFG_AVI_MODE (0 << 1)
#define REG_HDCP_HDMI_CFG_HDMI_MODE (1 << 1)
#define REG_HDCP_HDMI_CFG_DEFAULT (0x14)
#define REG_HDCP_HDMI_CFG_ENB_HDMI (REG_HDCP_HDMI_CFG_DEFAULT | REG_HDCP_HDMI_CFG_HDMI_MODE)
#define REG_HDCP_HDMI_CFG_ENB_AVI (REG_HDCP_HDMI_CFG_DEFAULT | REG_HDCP_HDMI_CFG_AVI_MODE)

#endif  // SRC_GRAPHICS_DISPLAY_DRIVERS_HIKEY_DISPLAY_HIDISPLAY_REGS_H_
