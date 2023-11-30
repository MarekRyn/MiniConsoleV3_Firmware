/*******************************************************************
 * MiniConsole V3 - Board Support Package - LCD Touch Panel
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 *******************************************************************/

#ifndef BSP_LCD_TP_H_
#define BSP_LCD_TP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "BSP_STM32.h"
#include "../Components/RVT50AQTNWC00.h"	// LCD Screen driver - model RVT50AQTNWC00


#define BSP_LCD_TP_RST_PORT		GPIOG
#define BSP_LCD_TP_RST_PIN		GPIO_PIN_14


extern BUF_RAM	LCD_TP_HandleTypeDef 	BSP_hlcdtp;
extern 			TxRxContext_TypeDef		BSP_hlcdtp_ctx;


uint8_t BSP_LCD_TP_Init();
uint8_t BSP_LCD_TP_Reset();

void BSP_LCD_TP_INTHandler();

#ifdef __cplusplus
}
#endif

#endif /* BSP_LCD_TP_H_ */
