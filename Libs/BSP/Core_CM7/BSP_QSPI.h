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

#ifndef BSP_QSPI_H_
#define BSP_QSPI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "BSP_STM32.h"
#include "IS25LP128F.h"

#define BSP_QSPI				QUADSPI				// STM32 QSPI peripheral address
#define BSP_QSPI_PRESCALER		2					// Clock = 240MHz / (_2_ + 1) = 80 MHz
#define BSP_QSPI_CLOCKMODE		QSPI_CLOCK_MODE_3	// Clock mode
#define BSP_QSPI_MEMSIZE		23					// Memory size = 2^(_23_ + 1) = 16777216 B = 128 Mbit

uint8_t BSP_QSPI_Init();
uint8_t BSP_QSPI_WriteBuf(uint8_t * pData, uint32_t address, uint32_t length);
uint8_t BSP_QSPI_ReadBuf(uint8_t * pData, uint32_t address, uint32_t length);
uint8_t BSP_QSPI_ChipErase();
uint8_t BSP_QSPI_SectorErase(uint32_t address);
uint8_t BSP_QSPI_ReadUID(uint8_t * pData);
uint8_t BSP_QSPI_MemMappedEnable();

#ifdef __cplusplus
}
#endif

#endif /* BSP_QSPI_H_ */
