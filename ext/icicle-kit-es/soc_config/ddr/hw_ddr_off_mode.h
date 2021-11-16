/*******************************************************************************
 * Copyright 2019-2021 Microchip FPGA Embedded Systems Solutions.
 * 
 * SPDX-License-Identifier: MIT
 * 
 * @file hw_ddr_off_mode.h
 * @author Microchip-FPGA Embedded Systems Solutions
 * 
 * Generated using Libero version: 2.0
 * Libero design name: ICICLE_MSS
 * MPFS part number used in design: MPFS250T_ES
 * Date generated by Libero: 12-14-2020_09:32:06
 * Format version of XML description: 0.4.2
 * PolarFire SoC Configuration Generator version: 0.5.1
 * 
 * Note 1: This file should not be edited. If you need to modify a parameter,
 * without going through the Libero flow or editing the associated xml file,
 * the following method is recommended:
 *   1. edit the file platform//config//software//mpfs_hal//mss_sw_config.h
 *   2. define the value you want to override there. (Note: There is a 
 *      commented example in mss_sw_config.h)
 * Note 2: The definition in mss_sw_config.h takes precedence, as 
 * mss_sw_config.h is included prior to the hw_ddr_off_mode.h in the hal 
 * (see platform//mpfs_hal//mss_hal.h)
 *
 */ 

#ifndef HW_DDR_OFF_MODE_H_
#define HW_DDR_OFF_MODE_H_


#ifdef __cplusplus
extern  "C" {
#endif

#if !defined (LIBERO_SETTING_DDRPHY_MODE_OFF)
/*DDRPHY MODE Register, ddr off */ 
#define LIBERO_SETTING_DDRPHY_MODE_OFF    0x00000000UL
    /* DDRMODE                           [0:3]   RW value= 0x0 */ 
    /* ECC                               [3:1]   RW value= 0x0 */ 
    /* CRC                               [4:1]   RW value= 0x0 */ 
    /* BUS_WIDTH                         [5:3]   RW value= 0x0 */ 
    /* DMI_DBI                           [8:1]   RW value= 0x0 */ 
    /* DQ_DRIVE                          [9:2]   RW value= 0x0 */ 
    /* DQS_DRIVE                         [11:2]  RW value= 0x0 */ 
    /* ADD_CMD_DRIVE                     [13:2]  RW value= 0x0 */ 
    /* CLOCK_OUT_DRIVE                   [15:2]  RW value= 0x0 */ 
    /* DQ_TERMINATION                    [17:2]  RW value= 0x0 */ 
    /* DQS_TERMINATION                   [19:2]  RW value= 0x0 */ 
    /* ADD_CMD_INPUT_PIN_TERMINATION     [21:2]  RW value= 0x0 */ 
    /* PRESET_ODT_CLK                    [23:2]  RW value= 0x0 */ 
    /* POWER_DOWN                        [25:1]  RW value= 0x0 */ 
    /* RANK                              [26:1]  RW value= 0x0 */ 
    /* RESERVED                          [27:5]  RSVD */ 
#endif
#if !defined (LIBERO_SETTING_DPC_BITS_OFF_MODE)
/*DPC Bits Register off mode */ 
#define LIBERO_SETTING_DPC_BITS_OFF_MODE    0x00000000UL
    /* DPC_VS                            [0:4]   RW value= 0x0 */ 
    /* DPC_VRGEN_H                       [4:6]   RW value= 0x0 */ 
    /* DPC_VRGEN_EN_H                    [10:1]  RW value= 0x0 */ 
    /* DPC_MOVE_EN_H                     [11:1]  RW value= 0x0 */ 
    /* DPC_VRGEN_V                       [12:6]  RW value= 0x0 */ 
    /* DPC_VRGEN_EN_V                    [18:1]  RW value= 0x0 */ 
    /* DPC_MOVE_EN_V                     [19:1]  RW value= 0x0 */ 
    /* RESERVE01                         [20:12] RSVD */ 
#endif

#ifdef __cplusplus
}
#endif


#endif /* #ifdef HW_DDR_OFF_MODE_H_ */

