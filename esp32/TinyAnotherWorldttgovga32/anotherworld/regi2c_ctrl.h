/*
 * SPDX-FileCopyrightText: 2015-2021 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <stdint.h>
//#include "regi2c_apll.h"
//#include "regi2c_bbpll.h"





//regi2c_apll.h BEGIN
//
//SPDX-FileCopyrightText: 2015-2021 Espressif Systems (Shanghai) CO LTD
//
//SPDX-License-Identifier: Apache-2.0
//

//#pragma once
//
//@file regi2c_apll.h
//@brief Register definitions for audio PLL (APLL)
//
//This file lists register fields of APLL, located on an internal configuration
//bus. These definitions are used via macros defined in regi2c_ctrl.h, by
//rtc_clk_apll_enable function in rtc_clk.c.

#define I2C_APLL            0X6D
#define I2C_APLL_HOSTID     3

#define I2C_APLL_IR_CAL_DELAY        0
#define I2C_APLL_IR_CAL_DELAY_MSB    3
#define I2C_APLL_IR_CAL_DELAY_LSB    0

#define I2C_APLL_IR_CAL_RSTB        0
#define I2C_APLL_IR_CAL_RSTB_MSB    4
#define I2C_APLL_IR_CAL_RSTB_LSB    4

#define I2C_APLL_IR_CAL_START        0
#define I2C_APLL_IR_CAL_START_MSB    5
#define I2C_APLL_IR_CAL_START_LSB    5

#define I2C_APLL_IR_CAL_UNSTOP        0
#define I2C_APLL_IR_CAL_UNSTOP_MSB    6
#define I2C_APLL_IR_CAL_UNSTOP_LSB    6

#define I2C_APLL_OC_ENB_FCAL        0
#define I2C_APLL_OC_ENB_FCAL_MSB    7
#define I2C_APLL_OC_ENB_FCAL_LSB    7

#define I2C_APLL_IR_CAL_EXT_CAP        1
#define I2C_APLL_IR_CAL_EXT_CAP_MSB    4
#define I2C_APLL_IR_CAL_EXT_CAP_LSB    0

#define I2C_APLL_IR_CAL_ENX_CAP        1
#define I2C_APLL_IR_CAL_ENX_CAP_MSB    5
#define I2C_APLL_IR_CAL_ENX_CAP_LSB    5

#define I2C_APLL_OC_LBW        1
#define I2C_APLL_OC_LBW_MSB    6
#define I2C_APLL_OC_LBW_LSB    6

#define I2C_APLL_IR_CAL_CK_DIV        2
#define I2C_APLL_IR_CAL_CK_DIV_MSB    3
#define I2C_APLL_IR_CAL_CK_DIV_LSB    0

#define I2C_APLL_OC_DCHGP        2
#define I2C_APLL_OC_DCHGP_MSB    6
#define I2C_APLL_OC_DCHGP_LSB    4

#define I2C_APLL_OC_ENB_VCON        2
#define I2C_APLL_OC_ENB_VCON_MSB    7
#define I2C_APLL_OC_ENB_VCON_LSB    7

#define I2C_APLL_OR_CAL_CAP        3
#define I2C_APLL_OR_CAL_CAP_MSB    4
#define I2C_APLL_OR_CAL_CAP_LSB    0

#define I2C_APLL_OR_CAL_UDF        3
#define I2C_APLL_OR_CAL_UDF_MSB    5
#define I2C_APLL_OR_CAL_UDF_LSB    5

#define I2C_APLL_OR_CAL_OVF        3
#define I2C_APLL_OR_CAL_OVF_MSB    6
#define I2C_APLL_OR_CAL_OVF_LSB    6

#define I2C_APLL_OR_CAL_END        3
#define I2C_APLL_OR_CAL_END_MSB    7
#define I2C_APLL_OR_CAL_END_LSB    7

#define I2C_APLL_OR_OUTPUT_DIV        4
#define I2C_APLL_OR_OUTPUT_DIV_MSB    4
#define I2C_APLL_OR_OUTPUT_DIV_LSB    0

#define I2C_APLL_OC_TSCHGP        4
#define I2C_APLL_OC_TSCHGP_MSB    6
#define I2C_APLL_OC_TSCHGP_LSB    6

#define I2C_APLL_EN_FAST_CAL        4
#define I2C_APLL_EN_FAST_CAL_MSB    7
#define I2C_APLL_EN_FAST_CAL_LSB    7

#define I2C_APLL_OC_DHREF_SEL        5
#define I2C_APLL_OC_DHREF_SEL_MSB    1
#define I2C_APLL_OC_DHREF_SEL_LSB    0

#define I2C_APLL_OC_DLREF_SEL        5
#define I2C_APLL_OC_DLREF_SEL_MSB    3
#define I2C_APLL_OC_DLREF_SEL_LSB    2

#define I2C_APLL_SDM_DITHER        5
#define I2C_APLL_SDM_DITHER_MSB    4
#define I2C_APLL_SDM_DITHER_LSB    4

#define I2C_APLL_SDM_STOP        5
#define I2C_APLL_SDM_STOP_MSB    5
#define I2C_APLL_SDM_STOP_LSB    5

#define I2C_APLL_SDM_RSTB        5
#define I2C_APLL_SDM_RSTB_MSB    6
#define I2C_APLL_SDM_RSTB_LSB    6

#define I2C_APLL_OC_DVDD        6
#define I2C_APLL_OC_DVDD_MSB    4
#define I2C_APLL_OC_DVDD_LSB    0

#define I2C_APLL_DSDM2        7
#define I2C_APLL_DSDM2_MSB    5
#define I2C_APLL_DSDM2_LSB    0

#define I2C_APLL_DSDM1        8
#define I2C_APLL_DSDM1_MSB    7
#define I2C_APLL_DSDM1_LSB    0

#define I2C_APLL_DSDM0        9
#define I2C_APLL_DSDM0_MSB    7
#define I2C_APLL_DSDM0_LSB    0
//regi2c_apll.h END





//regi2c_bbpll.h BEGIN
//
//SPDX-FileCopyrightText: 2015-2021 Espressif Systems (Shanghai) CO LTD
//
//SPDX-License-Identifier: Apache-2.0
//

//#pragma once

//
//@file regi2c_apll.h
//@brief Register definitions for digital PLL (BBPLL)
//
//This file lists register fields of BBPLL, located on an internal configuration
//bus. These definitions are used via macros defined in regi2c_ctrl.h, by
//rtc_clk_cpu_freq_set function in rtc_clk.c.

#define I2C_BBPLL           0x66
#define I2C_BBPLL_HOSTID    4

#define I2C_BBPLL_IR_CAL_DELAY        0
#define I2C_BBPLL_IR_CAL_DELAY_MSB    3
#define I2C_BBPLL_IR_CAL_DELAY_LSB    0

#define I2C_BBPLL_IR_CAL_CK_DIV        0
#define I2C_BBPLL_IR_CAL_CK_DIV_MSB    7
#define I2C_BBPLL_IR_CAL_CK_DIV_LSB    4

#define I2C_BBPLL_IR_CAL_EXT_CAP        1
#define I2C_BBPLL_IR_CAL_EXT_CAP_MSB    3
#define I2C_BBPLL_IR_CAL_EXT_CAP_LSB    0

#define I2C_BBPLL_IR_CAL_ENX_CAP        1
#define I2C_BBPLL_IR_CAL_ENX_CAP_MSB    4
#define I2C_BBPLL_IR_CAL_ENX_CAP_LSB    4

#define I2C_BBPLL_IR_CAL_RSTB        1
#define I2C_BBPLL_IR_CAL_RSTB_MSB    5
#define I2C_BBPLL_IR_CAL_RSTB_LSB    5

#define I2C_BBPLL_IR_CAL_START        1
#define I2C_BBPLL_IR_CAL_START_MSB    6
#define I2C_BBPLL_IR_CAL_START_LSB    6

#define I2C_BBPLL_IR_CAL_UNSTOP        1
#define I2C_BBPLL_IR_CAL_UNSTOP_MSB    7
#define I2C_BBPLL_IR_CAL_UNSTOP_LSB    7

#define I2C_BBPLL_OC_REF_DIV        2
#define I2C_BBPLL_OC_REF_DIV_MSB    3
#define I2C_BBPLL_OC_REF_DIV_LSB    0

#define I2C_BBPLL_OC_DIV_10_8        2
#define I2C_BBPLL_OC_DIV_10_8_MSB    6
#define I2C_BBPLL_OC_DIV_10_8_LSB    4

#define I2C_BBPLL_OC_LREF        2
#define I2C_BBPLL_OC_LREF_MSB    7
#define I2C_BBPLL_OC_LREF_LSB    7

#define I2C_BBPLL_OC_DIV_7_0        3
#define I2C_BBPLL_OC_DIV_7_0_MSB    7
#define I2C_BBPLL_OC_DIV_7_0_LSB    0

#define I2C_BBPLL_OC_ENB_FCAL        4
#define I2C_BBPLL_OC_ENB_FCAL_MSB    0
#define I2C_BBPLL_OC_ENB_FCAL_LSB    0

#define I2C_BBPLL_OC_DCHGP        4
#define I2C_BBPLL_OC_DCHGP_MSB    3
#define I2C_BBPLL_OC_DCHGP_LSB    1

#define I2C_BBPLL_OC_DHREF_SEL        4
#define I2C_BBPLL_OC_DHREF_SEL_MSB    5
#define I2C_BBPLL_OC_DHREF_SEL_LSB    4

#define I2C_BBPLL_OC_DLREF_SEL        4
#define I2C_BBPLL_OC_DLREF_SEL_MSB    7
#define I2C_BBPLL_OC_DLREF_SEL_LSB    6

#define I2C_BBPLL_OC_DCUR        5
#define I2C_BBPLL_OC_DCUR_MSB    2
#define I2C_BBPLL_OC_DCUR_LSB    0

#define I2C_BBPLL_OC_BST_DIV        5
#define I2C_BBPLL_OC_BST_DIV_MSB    3
#define I2C_BBPLL_OC_BST_DIV_LSB    3

#define I2C_BBPLL_OC_BST_E2C        5
#define I2C_BBPLL_OC_BST_E2C_MSB    4
#define I2C_BBPLL_OC_BST_E2C_LSB    4

#define I2C_BBPLL_OC_TSCHGP        5
#define I2C_BBPLL_OC_TSCHGP_MSB    5
#define I2C_BBPLL_OC_TSCHGP_LSB    5

#define I2C_BBPLL_OC_BW        5
#define I2C_BBPLL_OC_BW_MSB    7
#define I2C_BBPLL_OC_BW_LSB    6

#define I2C_BBPLL_OR_LOCK1        6
#define I2C_BBPLL_OR_LOCK1_MSB    0
#define I2C_BBPLL_OR_LOCK1_LSB    0

#define I2C_BBPLL_OR_LOCK2        6
#define I2C_BBPLL_OR_LOCK2_MSB    1
#define I2C_BBPLL_OR_LOCK2_LSB    1

#define I2C_BBPLL_OR_CAL_CAP        7
#define I2C_BBPLL_OR_CAL_CAP_MSB    3
#define I2C_BBPLL_OR_CAL_CAP_LSB    0

#define I2C_BBPLL_OR_CAL_UDF        7
#define I2C_BBPLL_OR_CAL_UDF_MSB    4
#define I2C_BBPLL_OR_CAL_UDF_LSB    4

#define I2C_BBPLL_OR_CAL_OVF        7
#define I2C_BBPLL_OR_CAL_OVF_MSB    5
#define I2C_BBPLL_OR_CAL_OVF_LSB    5

#define I2C_BBPLL_OR_CAL_END        7
#define I2C_BBPLL_OR_CAL_END_MSB    6
#define I2C_BBPLL_OR_CAL_END_LSB    6

#define I2C_BBPLL_BBADC_DELAY1        8
#define I2C_BBPLL_BBADC_DELAY1_MSB    1
#define I2C_BBPLL_BBADC_DELAY1_LSB    0

#define I2C_BBPLL_BBADC_DELAY2        8
#define I2C_BBPLL_BBADC_DELAY2_MSB    3
#define I2C_BBPLL_BBADC_DELAY2_LSB    2

#define I2C_BBPLL_BBADC_DELAY3        8
#define I2C_BBPLL_BBADC_DELAY3_MSB    5
#define I2C_BBPLL_BBADC_DELAY3_LSB    4

#define I2C_BBPLL_BBADC_DELAY4        8
#define I2C_BBPLL_BBADC_DELAY4_MSB    7
#define I2C_BBPLL_BBADC_DELAY4_LSB    6

#define I2C_BBPLL_BBADC_DELAY5        9
#define I2C_BBPLL_BBADC_DELAY5_MSB    1
#define I2C_BBPLL_BBADC_DELAY5_LSB    0

#define I2C_BBPLL_BBADC_DELAY6        9
#define I2C_BBPLL_BBADC_DELAY6_MSB    3
#define I2C_BBPLL_BBADC_DELAY6_LSB    2

#define I2C_BBPLL_BBADC_DSMP        9
#define I2C_BBPLL_BBADC_DSMP_MSB    7
#define I2C_BBPLL_BBADC_DSMP_LSB    4

#define I2C_BBPLL_DTEST        10
#define I2C_BBPLL_DTEST_MSB    1
#define I2C_BBPLL_DTEST_LSB    0

#define I2C_BBPLL_ENT_ADC        10
#define I2C_BBPLL_ENT_ADC_MSB    3
#define I2C_BBPLL_ENT_ADC_LSB    2

#define I2C_BBPLL_BBADC_DIV        10
#define I2C_BBPLL_BBADC_DIV_MSB    5
#define I2C_BBPLL_BBADC_DIV_LSB    4

#define I2C_BBPLL_ENT_PLL        10
#define I2C_BBPLL_ENT_PLL_MSB    6
#define I2C_BBPLL_ENT_PLL_LSB    6

#define I2C_BBPLL_OC_ENB_VCON        10
#define I2C_BBPLL_OC_ENB_VCON_MSB    7
#define I2C_BBPLL_OC_ENB_VCON_LSB    7

#define I2C_BBPLL_DIV_DAC        11
#define I2C_BBPLL_DIV_DAC_MSB    0
#define I2C_BBPLL_DIV_DAC_LSB    0

#define I2C_BBPLL_DIV_CPU        11
#define I2C_BBPLL_DIV_CPU_MSB    1
#define I2C_BBPLL_DIV_CPU_LSB    1

#define I2C_BBPLL_BBADC_INPUT_SHORT        11
#define I2C_BBPLL_BBADC_INPUT_SHORT_MSB    2
#define I2C_BBPLL_BBADC_INPUT_SHORT_LSB    2

#define I2C_BBPLL_BBADC_CAL_9_8        11
#define I2C_BBPLL_BBADC_CAL_9_8_MSB    4
#define I2C_BBPLL_BBADC_CAL_9_8_LSB    3

#define I2C_BBPLL_BBADC_DCM        11
#define I2C_BBPLL_BBADC_DCM_MSB    6
#define I2C_BBPLL_BBADC_DCM_LSB    5

#define I2C_BBPLL_ENDIV5        11
#define I2C_BBPLL_ENDIV5_MSB    7
#define I2C_BBPLL_ENDIV5_LSB    7

#define I2C_BBPLL_BBADC_CAL_7_0        12
#define I2C_BBPLL_BBADC_CAL_7_0_MSB    7
#define I2C_BBPLL_BBADC_CAL_7_0_LSB    0
//regi2c_bbpll.h END







#ifdef __cplusplus
extern "C" {
#endif

/* Analog function control register */
#define ANA_CONFIG_REG  0x6000E044
#define ANA_CONFIG_S    (8)
#define ANA_CONFIG_M    (0x3FF)
/* Clear to enable APLL */
#define I2C_APLL_M      (BIT(14))
/* Clear to enable BBPLL */
#define I2C_BBPLL_M     (BIT(17))

/* ROM functions which read/write internal control bus */
uint8_t rom_i2c_readReg(uint8_t block, uint8_t host_id, uint8_t reg_add);
uint8_t rom_i2c_readReg_Mask(uint8_t block, uint8_t host_id, uint8_t reg_add, uint8_t msb, uint8_t lsb);
void rom_i2c_writeReg(uint8_t block, uint8_t host_id, uint8_t reg_add, uint8_t data);
void rom_i2c_writeReg_Mask(uint8_t block, uint8_t host_id, uint8_t reg_add, uint8_t msb, uint8_t lsb, uint8_t data);

#ifdef BOOTLOADER_BUILD

/**
 * If compiling for the bootloader, ROM functions can be called directly,
 * without the need of a lock.
 */
#define regi2c_ctrl_read_reg         rom_i2c_readReg
#define regi2c_ctrl_read_reg_mask    rom_i2c_readReg_Mask
#define regi2c_ctrl_write_reg        rom_i2c_writeReg
#define regi2c_ctrl_write_reg_mask   rom_i2c_writeReg_Mask

#else

#define i2c_read_reg_raw        rom_i2c_readReg
#define i2c_read_reg_mask_raw   rom_i2c_readReg_Mask
#define i2c_write_reg_raw       rom_i2c_writeReg
#define i2c_write_reg_mask_raw  rom_i2c_writeReg_Mask

uint8_t regi2c_ctrl_read_reg(uint8_t block, uint8_t host_id, uint8_t reg_add);
uint8_t regi2c_ctrl_read_reg_mask(uint8_t block, uint8_t host_id, uint8_t reg_add, uint8_t msb, uint8_t lsb);
void regi2c_ctrl_write_reg(uint8_t block, uint8_t host_id, uint8_t reg_add, uint8_t data);
void regi2c_ctrl_write_reg_mask(uint8_t block, uint8_t host_id, uint8_t reg_add, uint8_t msb, uint8_t lsb, uint8_t data);

/* enter the critical section that protects internal registers. Don't use it in SDK. Use the functions above. */
void regi2c_enter_critical(void);
void regi2c_exit_critical(void);

#endif // BOOTLOADER_BUILD

/* Convenience macros for the above functions, these use register definitions
 * from regi2c_apll.h/regi2c_bbpll.h header files.
 */
#define REGI2C_WRITE_MASK(block, reg_add, indata) \
      regi2c_ctrl_write_reg_mask(block, block##_HOSTID,  reg_add,  reg_add##_MSB,  reg_add##_LSB,  indata)

#define REGI2C_READ_MASK(block, reg_add) \
      regi2c_ctrl_read_reg_mask(block, block##_HOSTID,  reg_add,  reg_add##_MSB,  reg_add##_LSB)

#define REGI2C_WRITE(block, reg_add, indata) \
      regi2c_ctrl_write_reg(block, block##_HOSTID,  reg_add, indata)

#define REGI2C_READ(block, reg_add) \
      regi2c_ctrl_read_reg(block, block##_HOSTID,  reg_add)


#ifdef __cplusplus
}
#endif
