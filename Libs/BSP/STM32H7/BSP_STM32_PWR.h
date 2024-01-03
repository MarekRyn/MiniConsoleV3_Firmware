/*******************************************************************
 * MiniConsole V3 - Board Support Package - STM32 PWR
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 *******************************************************************/

#ifndef STM32H7_BSP_STM32_PWR_H_
#define STM32H7_BSP_STM32_PWR_H_

#include "BSP_Common.h"
#include "BSP_STM32.h"

#define PWR_LDO_SUPPLY                      PWR_CR3_LDOEN                                                               // Core domains are supplied from the LDO
#define PWR_DIRECT_SMPS_SUPPLY              PWR_CR3_SMPSEN                                                              // Core domains are supplied from the SMPS only
#define PWR_SMPS_1V8_SUPPLIES_LDO           (PWR_CR3_SMPSLEVEL_0 | PWR_CR3_SMPSEN    | PWR_CR3_LDOEN)                   // The SMPS 1.8V output supplies the LDO which supplies the Core domains
#define PWR_SMPS_2V5_SUPPLIES_LDO           (PWR_CR3_SMPSLEVEL_1 | PWR_CR3_SMPSEN    | PWR_CR3_LDOEN)                   // The SMPS 2.5V output supplies the LDO which supplies the Core domains
#define PWR_SMPS_1V8_SUPPLIES_EXT_AND_LDO   (PWR_CR3_SMPSLEVEL_0 | PWR_CR3_SMPSEXTHP | PWR_CR3_SMPSEN | PWR_CR3_LDOEN)  // The SMPS 1.8V output supplies an external circuits and the LDO. The Core domains are supplied from the LDO
#define PWR_SMPS_2V5_SUPPLIES_EXT_AND_LDO   (PWR_CR3_SMPSLEVEL_1 | PWR_CR3_SMPSEXTHP | PWR_CR3_SMPSEN | PWR_CR3_LDOEN)  // The SMPS 2.5V output supplies an external circuits and the LDO. The Core domains are supplied from the LDO
#define PWR_SMPS_1V8_SUPPLIES_EXT           (PWR_CR3_SMPSLEVEL_0 | PWR_CR3_SMPSEXTHP | PWR_CR3_SMPSEN | PWR_CR3_BYPASS) // The SMPS 1.8V output supplies an external source which supplies the Core domains
#define PWR_SMPS_2V5_SUPPLIES_EXT           (PWR_CR3_SMPSLEVEL_1 | PWR_CR3_SMPSEXTHP | PWR_CR3_SMPSEN | PWR_CR3_BYPASS) // The SMPS 2.5V output supplies an external source which supplies the Core domains
#define PWR_EXTERNAL_SOURCE_SUPPLY          PWR_CR3_BYPASS                                                              // The SMPS disabled and the LDO Bypass. The Core domains are supplied from an external source

#define PWR_REGULATOR_VOLTAGE_SCALE0  		(0U)
#define PWR_REGULATOR_VOLTAGE_SCALE1  		(PWR_D3CR_VOS_1 | PWR_D3CR_VOS_0)
#define PWR_REGULATOR_VOLTAGE_SCALE2  		(PWR_D3CR_VOS_1)
#define PWR_REGULATOR_VOLTAGE_SCALE3  		(PWR_D3CR_VOS_0)

#define PWR_MAINREGULATOR_ON      			(0U)
#define PWR_LOWPOWERREGULATOR_ON  			PWR_CR1_LPDS

#define PWR_STOPENTRY_WFI  					(0x01U)
#define PWR_STOPENTRY_WFE  					(0x02U)

#define PWR_D1_DOMAIN						(0x00000000U)
#define PWR_D2_DOMAIN 						(0x00000001U)
#define PWR_D3_DOMAIN 						(0x00000002U)


uint8_t BSP_STM32_PWR_ConfigSupply(uint32_t SupplySource);
uint8_t BSP_STM32_PWR_VoltageScalingConfig(uint32_t Regulator);
uint8_t BSP_STM32_PWR_WaitForVOSRDY(void);
uint8_t BSP_STM32_PWR_EnableBkUpAccess(void);
uint8_t BSP_STM32_PWR_ClearPendingEvent(void);
uint8_t BSP_STM32_PWR_EnterSTOPMode(uint32_t Regulator, uint8_t STOPEntry, uint32_t Domain);
uint8_t BSP_STM32_PWR_Restart(void);



#endif /* STM32H7_BSP_STM32_PWR_H_ */
