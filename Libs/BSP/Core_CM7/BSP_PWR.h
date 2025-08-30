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
#include "BSP_Inputs.h"
#include "BSP_LCD_TP.h"
#include "BSP_LCD.h"
#include "BSP_IMU.h"

#define PWR_Hold_Port	GPIOG
#define PWR_Hold_Pin	GPIO_PIN_9

#define PWR_Low_Port	GPIOD
#define PWR_Low_Pin		GPIO_PIN_7

uint8_t BSP_PWR_Animation(void);
uint8_t BSP_PWR_Init(void);
uint8_t BSP_PWR_Restart(void);
uint8_t BSP_PWR_ShutDown(void);
uint8_t BSP_PWR_ShutDownNoAnim(void);
uint8_t BSP_PWR_SaveConfig(uint8_t screen_reduced_timeout, uint8_t screen_pwroff_timeout);
uint8_t BSP_PWR_LoadConfig(void);


#ifdef __cplusplus
}
#endif

#endif /* BSP_CORE_CM7_BSP_PWR_H_ */
