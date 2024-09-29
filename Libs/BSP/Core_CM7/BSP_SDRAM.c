/*******************************************************************
 * MiniConsole V3 - Board Support Package - SDRAM
 *******************************************************************/

#include "BSP_SDRAM.h"

// Configuring SDRAM on the board
uint8_t	BSP_SDRAM_Init(uint32_t Freq) {
	// Freq in MHz

	// Init SDRAM
	if (BSP_DRV_SDRAM_Init(FMC_SDRAM_DEVICE, FMC_SDRAM_BANK1, FMC_SDRAM_CLOCK_PERIOD_2, Freq)) return BSP_ERROR;

	return BSP_OK;
}
