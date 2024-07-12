/*
 * BSP_IntelHex.h
 *
 *  Created on: Jul 11, 2024
 *      Author: marek
 */

#ifndef BSP_CORE_CM7_BSP_INTELHEX_H_
#define BSP_CORE_CM7_BSP_INTELHEX_H_

#include "BSP_STM32.h"
#include "BSP_fatfs.h"
#include <stdlib.h>

uint8_t BSP_IHex_Init(char *filename, uint32_t addr_start, uint32_t addr_end);
uint8_t BSP_IHex_FillBuf(void * pbuf, uint32_t bufsize);
uint8_t BSP_IHex_DeInit(void);
uint8_t	BSP_IHex_IsEndOfFile(void);
uint8_t	BSP_IHex_IsError(void);
uint8_t	BSP_IHex_IsEndOfBlock(void);

#endif /* BSP_CORE_CM7_BSP_INTELHEX_H_ */
