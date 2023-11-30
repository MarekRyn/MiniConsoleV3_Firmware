/*******************************************************************
 * MiniConsole V3 - Board Support Package - QSPI Memory
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 *******************************************************************/

#include "BSP_QSPI.h"

uint8_t BSP_QSPI_Init() {
	if (BSP_STM32_QSPI_Init(BSP_QSPI, BSP_QSPI_PRESCALER, BSP_QSPI_CLOCKMODE, 1, BSP_QSPI_MEMSIZE)) return BSP_ERROR;
	if (BSP_DRV_QSPI_Init(QUADSPI)) return BSP_ERROR;

	return BSP_OK;
}


uint8_t BSP_QSPI_WriteBuf(uint8_t * pData, uint32_t address, uint32_t length) {
	if (BSP_DRV_QSPI_WriteBuf(BSP_QSPI, address, pData, length)) return BSP_ERROR;

	return BSP_OK;
}


uint8_t BSP_QSPI_ReadBuf(uint8_t * pData, uint32_t address, uint32_t length) {
	if (BSP_DRV_QSPI_ReadBuf(BSP_QSPI, address, pData, length)) return BSP_ERROR;

	return BSP_OK;
}


uint8_t BSP_QSPI_ChipErase() {
	if (BSP_DRV_QSPI_ChipErase(BSP_QSPI)) return BSP_ERROR;

	return BSP_OK;
}


uint8_t BSP_QSPI_SectorErase(uint32_t address) {
	if (BSP_DRV_QSPI_SectorErase(BSP_QSPI, address)) return BSP_ERROR;

	return BSP_OK;
}


uint8_t BSP_QSPI_ReadUID(uint8_t * pData) {
	// Reads 16-byte Unique ID of QSPI Memory Chip
	if (BSP_DRV_QSPI_ReadUID(BSP_QSPI, pData)) return BSP_ERROR;

	return BSP_OK;
}


uint8_t BSP_QSPI_MemMappedEnable() {
	if (BSP_DRV_QSPI_MemMappedEnable(BSP_QSPI)) return BSP_ERROR;

	return BSP_OK;
}
