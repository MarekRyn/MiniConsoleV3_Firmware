/*******************************************************************
 * MiniConsole V3 - Board Support Package - LCD Touch Panel
 *
 * Author: Marek Ryn
 * Version: 1.0
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 * - 1.0	- Added support for areas
 *******************************************************************/

#ifndef BSP_LCD_TP_H_
#define BSP_LCD_TP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "BSP_STM32.h"
#include "RVT50AQTNWC00.h"	// LCD Screen driver - model RVT50AQTNWC00


#define BSP_LCD_TP_RST_PORT		GPIOG
#define BSP_LCD_TP_RST_PIN		GPIO_PIN_14

extern BUF_RAM	LCD_TP_HandleTypeDef 	BSP_hlcdtp;

uint8_t BSP_LCD_TP_Init(void);
uint8_t BSP_LCD_TP_Reset(void);
uint8_t BSP_LCD_TP_RegisterArea(uint8_t areaid, uint16_t x, uint16_t y, uint16_t width, uint16_t height, void* callback);
uint8_t BSP_LCD_TP_RemoveArea(uint8_t areaid);
uint8_t BSP_CLD_TP_RemoveAreaRange(uint8_t aid_start, uint8_t aid_stop);
uint8_t BSP_LCD_TP_RemoveAllAreas(void);


#ifdef __cplusplus
}
#endif

#endif /* BSP_LCD_TP_H_ */
