/*******************************************************************
 * MiniConsole V3 - Board Support Package - Power
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 *******************************************************************/

#ifndef BSP_CORE_CM7_BSP_PWR_H_
#define BSP_CORE_CM7_BSP_PWR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "BSP_STM32.h"

#define PWR_Hold_Port	GPIOG
#define PWR_Hold_Pin	GPIO_PIN_9

uint8_t BSP_PWR_Restart(void);
uint8_t BSP_PWR_ShutDown(void);

#ifdef __cplusplus
}
#endif

#endif /* BSP_CORE_CM7_BSP_PWR_H_ */
