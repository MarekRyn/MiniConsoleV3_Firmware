/*******************************************************************
 * MiniConsole V3 - Board Support Package - SDCARD
 *
 * Author: Marek Ryn
 * Version: 1.0
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 * - 1.0 	- First stable release
 *******************************************************************/

#ifndef BSP_SDCARD_H_
#define BSP_SDCARD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "BSP_STM32.h"

extern TxRxContext_TypeDef	hsdcardctx;

uint8_t BSP_SDCARD_Init();
uint8_t BSP_SDCARD_ReadBlocks(uint8_t * pData, uint32_t BlockAdd, uint32_t NumberOfBlocks);
uint8_t BSP_SDCARD_WriteBlocks(uint8_t * pData, uint32_t BlockAdd, uint32_t NumberOfBocks);
uint8_t BSP_SDCARD_GetCardState();
uint32_t BSP_SDCARD_GetBlockNo();
uint16_t BSP_SDCARD_GetBlockSize();

#ifdef __cplusplus
}
#endif

#endif /* BSP_SDCARD_H_ */
