/*******************************************************************
 * MiniConsole V3 - Driver - IS42S32160 - SDRAM Memory
 *
 * Author: Marek Ryn
 * Version: 1.0
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 * - 1.0	- Initial version
 *******************************************************************/

// Includes

#include "IS42S32160F.h"

#include "math.h"

// Private functions
static uint32_t limitvalue(uint32_t value, uint32_t min, uint32_t max) {
	uint32_t val = value;
	if (value < min) val = min;
	if (value > max) val = max;
	return val;
}


// SDRAM Configuration generator
uint8_t	BSP_DRV_SDRAM_Init(FMC_SDRAM_TypeDef *sdram, uint32_t SDBank, uint32_t SDClockPeriod, uint32_t Ck_Freq) {

	// Ck_Freq in MHz

	#define SDRAM_TIMEOUT	(uint32_t)0xFFFFF
	volatile uint32_t timeout = SDRAM_TIMEOUT;

	uint32_t param_CASLatency = 0;
	uint32_t param_ColumnBitsNumber = 0;
	uint32_t param_InternalBankNumber = 0;
	uint32_t param_MemoryDataWidth = 0;
	uint32_t param_ReadBurst = 0;
	uint32_t param_ReadPipeDelay = 0;
	uint32_t param_RowBitsNumber = 0;
	uint32_t param_WriteProtection = 0;


	// Calculating memory parameters (Bank Number and Clock Period are depend on uC and should be configured outside of this driver)
	switch (BSP_SDRAM_CAS_LATENCY) {
		case 1:		param_CASLatency = FMC_SDRAM_CAS_LATENCY_1;		break;
		case 2:		param_CASLatency = FMC_SDRAM_CAS_LATENCY_2;		break;
		case 3: 	param_CASLatency = FMC_SDRAM_CAS_LATENCY_3;		break;
		default:	return BSP_ERROR;
	}

	switch (BSP_SDRAM_COL_ADDR_BITS) {
		case 8:		param_ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_8;	break;
		case 9:		param_ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_9;	break;
		case 10:	param_ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_10;	break;
		case 11:	param_ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_11;	break;
		default:	return BSP_ERROR;
	}

	switch (BSP_SDRAM_BANK_NO) {
		case 2:		param_InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_2; break;
		case 4:		param_InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4; break;
		default:	return BSP_ERROR;
	}

	switch (BSP_SDRAM_DATA_WIDTH) {
		case 8:		param_MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_8; 	break;
		case 16:	param_MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16; break;
		case 32:	param_MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_32;	break;
		default:	return BSP_ERROR;
	}

	switch (BSP_SDRAM_BURST_READ) {
		case 0:		param_ReadBurst = FMC_SDRAM_RBURST_DISABLE;	break;
		case 1:		param_ReadBurst = FMC_SDRAM_RBURST_ENABLE;	break;
		default:	return BSP_ERROR;
	}

	switch (BSP_SDRAM_READ_PIPE_DELAY) {
		case 0:		param_ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_0;	break;
		case 1:		param_ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_1;	break;
		case 2:		param_ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_2;	break;
		default:	return BSP_ERROR;
	}

	switch (BSP_SDRAM_ROW_ADDR_BITS) {
		case 11:	param_RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_11;	break;
		case 12:	param_RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_12;	break;
		case 13:	param_RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_13;	break;
		default:	return BSP_ERROR;
	}

	switch (BSP_SDRAM_WRITE_PROTECTION) {
		case 0:		param_WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;	break;
		case 1:		param_WriteProtection = FMC_SDRAM_WRITE_PROTECTION_ENABLE;	break;
		default:	return BSP_ERROR;
	}

	// Calculating memory timings

	float CPeriod = 1000.0 / (float)Ck_Freq; // Returns period in ns

	uint32_t param_LoadToActiveDelay = limitvalue(round(1.0 * BSP_SDRAM_TMRD / CPeriod), 1, 16);
	uint32_t param_ExitSelfRefreshDelay = limitvalue(round(1.0 * BSP_SDRAM_TXSR / CPeriod), 1, 16);
	uint32_t param_SelfRefreshTime = limitvalue(round(1.0 * BSP_SDRAM_TRAS / CPeriod), 1, 16);
	uint32_t param_RowCycleDelay = limitvalue(round(1.0 * BSP_SDRAM_TRC / CPeriod), 1, 16);
	uint32_t param_WriteRecoveryTime = limitvalue(BSP_SDRAM_TWR, 2, 16);
	uint32_t param_RPDelay = limitvalue(BSP_SDRAM_TRP, 1, 16);
	uint32_t param_RCDDelay = limitvalue(BSP_SDRAM_TRCD, 1, 16);

	// Configuring SDRAM

	if (SDBank == FMC_SDRAM_BANK1) {
	    MODIFY_REG(sdram->SDCR[FMC_SDRAM_BANK1], 0xFFFFFFFF,(param_ColumnBitsNumber | param_RowBitsNumber | param_MemoryDataWidth | param_InternalBankNumber | param_CASLatency |
	    														param_WriteProtection | SDClockPeriod | param_ReadBurst | param_ReadPipeDelay));
	} else {
	    MODIFY_REG(sdram->SDCR[FMC_SDRAM_BANK1], (FMC_SDCRx_SDCLK | FMC_SDCRx_RBURST | FMC_SDCRx_RPIPE), (SDClockPeriod | param_ReadBurst | param_ReadPipeDelay));
	    MODIFY_REG(sdram->SDCR[FMC_SDRAM_BANK2], 0xFFFFFFFF, (param_ColumnBitsNumber | param_RowBitsNumber | param_MemoryDataWidth | param_InternalBankNumber |
	    														param_CASLatency | param_WriteProtection));
	}


	// Configuring SDRAM timings

	if (SDBank == FMC_SDRAM_BANK1) {
	    MODIFY_REG(sdram->SDTR[FMC_SDRAM_BANK1], 0xFFFFFFFF, 	(((param_LoadToActiveDelay) - 1U) | (((param_ExitSelfRefreshDelay) - 1U) << FMC_SDTRx_TXSR_Pos) |
	    															(((param_SelfRefreshTime) - 1U) << FMC_SDTRx_TRAS_Pos) | (((param_RowCycleDelay) - 1U) << FMC_SDTRx_TRC_Pos) |
																	(((param_WriteRecoveryTime) - 1U) << FMC_SDTRx_TWR_Pos) | (((param_RPDelay) - 1U) << FMC_SDTRx_TRP_Pos) |
																	(((param_RCDDelay) - 1U) << FMC_SDTRx_TRCD_Pos)));
	} else {
	    MODIFY_REG(sdram->SDTR[FMC_SDRAM_BANK1], FMC_SDTRx_TRC | FMC_SDTRx_TRP, (((param_RowCycleDelay) - 1U) << FMC_SDTRx_TRC_Pos) | (((param_RPDelay) - 1U) << FMC_SDTRx_TRP_Pos));
	    MODIFY_REG(sdram->SDTR[FMC_SDRAM_BANK2], 0xFFFFFFFF,	(((param_LoadToActiveDelay) - 1U) | (((param_ExitSelfRefreshDelay) - 1U) << FMC_SDTRx_TXSR_Pos) |
	                												(((param_SelfRefreshTime) - 1U) << FMC_SDTRx_TRAS_Pos) | (((param_WriteRecoveryTime) - 1U) << FMC_SDTRx_TWR_Pos) |
																	(((param_RCDDelay) - 1U) << FMC_SDTRx_TRCD_Pos)));
	}

	// Initialize memory IC

	uint32_t CommandTarget = 0;
	if (SDBank == FMC_SDRAM_BANK1) CommandTarget =  FMC_SDRAM_CMD_TARGET_BANK1; else CommandTarget =  FMC_SDRAM_CMD_TARGET_BANK2;

	// Little delay
	timeout = SDRAM_TIMEOUT * 0xF;	while (timeout--);

	// Configure a clock configuration enable command
	MODIFY_REG(sdram->SDCMR, (FMC_SDCMR_MODE | FMC_SDCMR_CTB2 | FMC_SDCMR_CTB1 | FMC_SDCMR_NRFS | FMC_SDCMR_MRD),
				((FMC_SDRAM_CMD_CLK_ENABLE) | (CommandTarget) | (((1) - 1U) << FMC_SDCMR_NRFS_Pos) | ((0) << FMC_SDCMR_MRD_Pos)));

	// Little delay
	timeout = SDRAM_TIMEOUT * 0xF;	while (timeout--);

	// Configure a PALL (precharge all) command
	MODIFY_REG(sdram->SDCMR, (FMC_SDCMR_MODE | FMC_SDCMR_CTB2 | FMC_SDCMR_CTB1 | FMC_SDCMR_NRFS | FMC_SDCMR_MRD),
				((FMC_SDRAM_CMD_PALL) | (CommandTarget) | (((1) - 1U) << FMC_SDCMR_NRFS_Pos) | ((0) << FMC_SDCMR_MRD_Pos)));

	// Configure a Auto-Refresh command
	MODIFY_REG(sdram->SDCMR, (FMC_SDCMR_MODE | FMC_SDCMR_CTB2 | FMC_SDCMR_CTB1 | FMC_SDCMR_NRFS | FMC_SDCMR_MRD),
				((FMC_SDRAM_CMD_AUTOREFRESH_MODE) | (CommandTarget) | (((8) - 1U) << FMC_SDCMR_NRFS_Pos) | ((0) << FMC_SDCMR_MRD_Pos)));

	// Configure a Auto-Refresh command
	MODIFY_REG(sdram->SDCMR, (FMC_SDCMR_MODE | FMC_SDCMR_CTB2 | FMC_SDCMR_CTB1 | FMC_SDCMR_NRFS | FMC_SDCMR_MRD),
				((FMC_SDRAM_CMD_AUTOREFRESH_MODE) | (CommandTarget) | (((8) - 1U) << FMC_SDCMR_NRFS_Pos) | ((0) << FMC_SDCMR_MRD_Pos)));

	// Configure a load Mode register command
	if (BSP_SDRAM_CAS_LATENCY == 3) MODIFY_REG(sdram->SDCMR, (FMC_SDCMR_MODE | FMC_SDCMR_CTB2 | FMC_SDCMR_CTB1 | FMC_SDCMR_NRFS | FMC_SDCMR_MRD),
				((FMC_SDRAM_CMD_LOAD_MODE) | (CommandTarget) | (((1) - 1U) << FMC_SDCMR_NRFS_Pos) | ((0x0230) << FMC_SDCMR_MRD_Pos)));
	if (BSP_SDRAM_CAS_LATENCY == 2) MODIFY_REG(sdram->SDCMR, (FMC_SDCMR_MODE | FMC_SDCMR_CTB2 | FMC_SDCMR_CTB1 | FMC_SDCMR_NRFS | FMC_SDCMR_MRD),
			((FMC_SDRAM_CMD_LOAD_MODE) | (CommandTarget) | (((1) - 1U) << FMC_SDCMR_NRFS_Pos) | ((0x0220) << FMC_SDCMR_MRD_Pos)));

	// Program Refresh Period
	uint32_t param_RefreshPeriod = round(1000.0 * Ck_Freq * BSP_SDRAM_REFRESH_PERIOD / BSP_SDRAM_REFRESH_COUNT) - 20;
	MODIFY_REG(sdram->SDRTR, FMC_SDRTR_COUNT, (param_RefreshPeriod << FMC_SDRTR_COUNT_Pos));

	// Enable FMC Peripheral
	__FMC_ENABLE();

	return BSP_OK;
}
