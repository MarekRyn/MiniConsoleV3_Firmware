/*******************************************************************
 * MiniConsole V3 - Board Support Package - STM32 FMC
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 *******************************************************************/

#ifndef STM32H7_BSP_STM32_FMC_H_
#define STM32H7_BSP_STM32_FMC_H_

#include "BSP_Common.h"

#define FMC_SDRAM_TypeDef              FMC_Bank5_6_TypeDef
#define FMC_SDRAM_DEVICE               FMC_Bank5_6_R

#define FMC_SDRAM_BANK1                         (0x00000000U)
#define FMC_SDRAM_BANK2                         (0x00000001U)

#define FMC_SDRAM_COLUMN_BITS_NUM_8             (0x00000000U)
#define FMC_SDRAM_COLUMN_BITS_NUM_9             (0x00000001U)
#define FMC_SDRAM_COLUMN_BITS_NUM_10            (0x00000002U)
#define FMC_SDRAM_COLUMN_BITS_NUM_11            (0x00000003U)

#define FMC_SDRAM_ROW_BITS_NUM_11               (0x00000000U)
#define FMC_SDRAM_ROW_BITS_NUM_12               (0x00000004U)
#define FMC_SDRAM_ROW_BITS_NUM_13               (0x00000008U)

#define FMC_SDRAM_MEM_BUS_WIDTH_8               (0x00000000U)
#define FMC_SDRAM_MEM_BUS_WIDTH_16              (0x00000010U)
#define FMC_SDRAM_MEM_BUS_WIDTH_32              (0x00000020U)

#define FMC_SDRAM_INTERN_BANKS_NUM_2            (0x00000000U)
#define FMC_SDRAM_INTERN_BANKS_NUM_4            (0x00000040U)

#define FMC_SDRAM_CAS_LATENCY_1                 (0x00000080U)
#define FMC_SDRAM_CAS_LATENCY_2                 (0x00000100U)
#define FMC_SDRAM_CAS_LATENCY_3                 (0x00000180U)

#define FMC_SDRAM_WRITE_PROTECTION_DISABLE      (0x00000000U)
#define FMC_SDRAM_WRITE_PROTECTION_ENABLE       (0x00000200U)

#define FMC_SDRAM_CLOCK_DISABLE                 (0x00000000U)
#define FMC_SDRAM_CLOCK_PERIOD_2                (0x00000800U)
#define FMC_SDRAM_CLOCK_PERIOD_3                (0x00000C00U)

#define FMC_SDRAM_RBURST_DISABLE                (0x00000000U)
#define FMC_SDRAM_RBURST_ENABLE                 (0x00001000U)

#define FMC_SDRAM_RPIPE_DELAY_0                 (0x00000000U)
#define FMC_SDRAM_RPIPE_DELAY_1                 (0x00002000U)
#define FMC_SDRAM_RPIPE_DELAY_2                 (0x00004000U)

#define FMC_SDRAM_CMD_NORMAL_MODE               (0x00000000U)
#define FMC_SDRAM_CMD_CLK_ENABLE                (0x00000001U)
#define FMC_SDRAM_CMD_PALL                      (0x00000002U)
#define FMC_SDRAM_CMD_AUTOREFRESH_MODE          (0x00000003U)
#define FMC_SDRAM_CMD_LOAD_MODE                 (0x00000004U)
#define FMC_SDRAM_CMD_SELFREFRESH_MODE          (0x00000005U)
#define FMC_SDRAM_CMD_POWERDOWN_MODE            (0x00000006U)

#define FMC_SDRAM_CMD_TARGET_BANK2              FMC_SDCMR_CTB2
#define FMC_SDRAM_CMD_TARGET_BANK1              FMC_SDCMR_CTB1
#define FMC_SDRAM_CMD_TARGET_BANK1_2            (0x00000018U)

#define FMC_SDRAM_NORMAL_MODE                   (0x00000000U)
#define FMC_SDRAM_SELF_REFRESH_MODE             FMC_SDSR_MODES1_0
#define FMC_SDRAM_POWER_DOWN_MODE               FMC_SDSR_MODES1_1

#define __FMC_ENABLE()  (FMC_Bank1_R->BTCR[0] |= FMC_BCR1_FMCEN)
#define __FMC_DISABLE()  (FMC_Bank1_R->BTCR[0] &= ~FMC_BCR1_FMCEN)

#endif /* STM32H7_BSP_STM32_FMC_H_ */
