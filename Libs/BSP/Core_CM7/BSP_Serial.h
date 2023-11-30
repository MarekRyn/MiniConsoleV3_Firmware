/*******************************************************************
 * MiniConsole V3 - Board Support Package - Serial Connection
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 *******************************************************************/

#ifndef CORE_CM7_BSP_SERIAL_H_
#define CORE_CM7_BSP_SERIAL_H_

#include "BSP_STM32.h"

extern BUF_RAM	SERIAL_HandleTypeDef 	BSP_hserial;
extern 			TxRxContext_TypeDef		BSP_hserial_ctx;

uint8_t BSP_Serial_Init(void);
uint8_t BSP_Serial_Transmit(uint8_t * pData, uint32_t Size);
uint8_t BSP_Serial_Transmit_IT(uint8_t * pData, uint32_t Size);


#endif /* CORE_CM7_BSP_SERIAL_H_ */
