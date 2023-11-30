/*******************************************************************
 * MiniConsole V3 - Board Support Package - SDCARD
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 *******************************************************************/

#include "BSP_SDCARD.h"

TxRxContext_TypeDef	hsdcardctx = {0};



uint8_t BSP_SDCARD_Init() {
	if (BSP_STM32_SDMMC_Init(SDMMC2, &hsdcardctx)) return BSP_ERROR;
	return BSP_OK;
}

uint8_t BSP_SDCARD_ReadBlocks(uint8_t * pData, uint32_t BlockAdd, uint32_t NumberOfBlocks) {
	if (BSP_STM32_SDMMC_ReadBlocks(SDMMC2, &hsdcardctx, pData, BlockAdd, NumberOfBlocks)) return BSP_ERROR;
	//Wait until operation finish
	while(!(BSP_STM32_SDMMC_GetCardState(SDMMC2, &hsdcardctx) == SDMMC_CARD_TRANSFER)) {}
	return BSP_OK;
}

uint8_t BSP_SDCARD_WriteBlocks(uint8_t * pData, uint32_t BlockAdd, uint32_t NumberOfBlocks) {
	if (BSP_STM32_SDMMC_WriteBlocks(SDMMC2, &hsdcardctx, pData, BlockAdd, NumberOfBlocks)) return BSP_ERROR;
	//Wait until operation finish
	while(!(BSP_STM32_SDMMC_GetCardState(SDMMC2, &hsdcardctx) == SDMMC_CARD_TRANSFER)) {}
	return BSP_OK;
}

uint32_t BSP_SDCARD_GetBlockNo() {
	SDMMCContext_TypeDef * sdcardctx = (SDMMCContext_TypeDef *)hsdcardctx.ctxmem;
	return sdcardctx->SDCardLogBlockNbr;
}

uint16_t BSP_SDCARD_GetBlockSize() {
	SDMMCContext_TypeDef * sdcardctx = (SDMMCContext_TypeDef *)hsdcardctx.ctxmem;
	return (uint16_t)sdcardctx->SDCardLogBlockSize;
}

uint8_t BSP_SDCARD_GetCardState() {
	if (BSP_STM32_SDMMC_GetCardState(SDMMC2, &hsdcardctx) == SDMMC_CARD_TRANSFER) return BSP_OK;
	return BSP_ERROR;
}
