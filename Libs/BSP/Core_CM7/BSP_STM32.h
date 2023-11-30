/*******************************************************************
 * MiniConsole V3 - Board Support Package - STM32
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 *******************************************************************/

#ifndef COMPONENTS_BSP_STM32_H_
#define COMPONENTS_BSP_STM32_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "BSP_Common.h"
#include "BSP_STM32_PWR.h"
#include "BSP_STM32_HSEM.h"
#include "BSP_STM32_ADC.h"
#include "BSP_STM32_BDMA.h"
#include "BSP_STM32_CORE.h"
#include "BSP_STM32_DMA.h"
#include "BSP_STM32_DMA2D.h"
#include "BSP_STM32_FMC.h"
#include "BSP_STM32_GPIO.h"
#include "BSP_STM32_I2C.h"
#include "BSP_STM32_I2S.h"
#include "BSP_STM32_JPEG.h"
#include "BSP_STM32_LTDC.h"
#include "BSP_STM32_QUADSPI.h"
#include "BSP_STM32_RCC.h"
#include "BSP_STM32_RTC.h"
#include "BSP_STM32_SDMMC.h"
#include "BSP_STM32_TIM.h"
#include "BSP_STM32_UART.h"
#include "BSP_STM32_PCD.h"

uint8_t BSP_STM32_MPU_Init(void);
uint8_t BSP_STM32_Init_SysClocks(void);
uint8_t BSP_STM32_Init_PeriphClocks(void);
uint8_t BSP_STM32_Init_NVIC(void);
uint8_t BSP_STM32_Init_GPIO(void);

uint8_t BSP_TickInit(uint32_t CoreClock, uint32_t TickFreq, uint32_t TickPriority);
void BSP_IncTick(void);
uint32_t BSP_GetTick(void);
void BSP_Delay(uint32_t delay);


__weak void BSP_LCD_TP_INTHandler();
__weak void BSP_IMU_INTHandler();
__weak void BSP_BLE_INTHandler();

#ifdef __cplusplus
}
#endif


#endif /* COMPONENTS_BSP_STM32_H_ */
