/*******************************************************************
 * MiniConsole V3 - Board Support Package - Keyboard and Joystick
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 *******************************************************************/

#ifndef BSP_INPUTS_H_
#define BSP_INPUTS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "BSP_STM32.h"

extern BUF_RAM	INPUTS_HandleTypeDef	BSP_hinputs;
extern 			TxRxContext_TypeDef		BSP_hinputs_ctx;


uint8_t BSP_Inputs_Init();
uint8_t BSP_Inputs_ParseData();

#ifdef __cplusplus
}
#endif

#endif /* BSP_INPUTS_H_ */
