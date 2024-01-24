/*******************************************************************
 * MiniConsole V3 - Board Support Package - STM32QUADSPI
 *
 * Author: Marek Ryn
 * Version: 1.0
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 * - 1.0	- Initial version
 *******************************************************************/

#include "BSP_STM32_QUADSPI.h"
#include <string.h>

typedef struct _QSPICtx {
	uint32_t	InstructionMode;
	uint32_t	Instruction;

	uint32_t	AddressMode;
	uint32_t	AddressSize;
	uint32_t	Address;

	uint32_t	AlternateMode;
	uint32_t	AlternateSize;
	uint32_t	AlternateBytes;

	uint32_t	DummyCycles;

	uint32_t	DataMode;
	uint32_t	DataLenth;
	uint8_t*	pData;

	uint32_t	DDRMode;
	uint32_t	SignalDelay;
	uint32_t	SIOOMode;

	uint32_t	AutoPoolingMatch;
	uint32_t	AutoPoolingMask;
	uint32_t	AutoPoolingMode;
	uint32_t	AutoPoolingInterval;

} QSPI_CTX_TypeDef;

static QSPI_CTX_TypeDef qspictx = {0};


static uint8_t _QSPI_WaitForFlag(QUADSPI_TypeDef * hqspi, uint32_t flag, uint32_t state, uint32_t timeout) {
	uint32_t tick = BSP_GetTick();

	while (1) {
		uint32_t reg = ((hqspi->SR & flag) == 0)?RESET:SET;
		if (reg == state) break;
		if ((BSP_GetTick() - tick) > timeout) return BSP_TIMEOUT;
	}

//	while ((((hqspi->SR & flag) == 0)?RESET:SET) != state) {
//		if ((HAL_GetTick() - tick) > timeout) return BSP_TIMEOUT;
//	}

	return BSP_OK;
}


static uint32_t _QSPI_GetConfigCCR(QUADSPI_TypeDef * hqspi, uint32_t FMode) {

	uint32_t reg_ccr = 0x00;

	// Program Instruction Block
	if (qspictx.InstructionMode != QSPI_INSTRUCTION_NONE) {
		reg_ccr |= (qspictx.Instruction & QUADSPI_CCR_INSTRUCTION_Msk);
		reg_ccr |= (qspictx.InstructionMode & QUADSPI_CCR_IMODE_Msk);
	}

	// Program Address Block
	if (qspictx.AddressMode !=QSPI_ADDRESS_NONE) {
		reg_ccr |= (qspictx.AddressMode & QUADSPI_CCR_ADMODE_Msk);
		reg_ccr |= (qspictx.AddressSize & QUADSPI_CCR_ADSIZE_Msk);
		if (FMode != QSPI_FUNCTIONAL_MODE_MEMORY_MAPPED) hqspi->AR = qspictx.Address;
			else hqspi->AR = 0;
	}

	// Program Alternate Bytes
	if (qspictx.AlternateMode != QSPI_ALTERNATE_BYTES_NONE) {
		reg_ccr |= (qspictx.AlternateMode & QUADSPI_CCR_ABMODE_Msk);
		reg_ccr |= (qspictx.AlternateSize & QUADSPI_CCR_ABSIZE_Msk);
		hqspi->ABR = qspictx.AlternateBytes;
	}

	// Program Dummy Cycles
	if (qspictx.DummyCycles > 0) {
		reg_ccr |= (qspictx.DummyCycles << QUADSPI_CCR_DCYC_Pos);
	}

	// Program Data Block
	if (qspictx.DataMode != QSPI_DATA_NONE) {
		reg_ccr |= (qspictx.DataMode & QUADSPI_CCR_DMODE_Msk);
		if (FMode != QSPI_FUNCTIONAL_MODE_MEMORY_MAPPED) hqspi->DLR = qspictx.DataLenth - 1U;
			else hqspi->DLR = 0;
	}

	// Program Additional Options
	reg_ccr |= (qspictx.DDRMode & QUADSPI_CCR_DDRM_Msk); 		// DDR Configuration
	reg_ccr |= (qspictx.SignalDelay & QUADSPI_CCR_DHHC_Msk); 	// Signal Shift
	reg_ccr |= (qspictx.SIOOMode & QUADSPI_CCR_SIOO_Msk);		// SIOO Mode
	reg_ccr |= (FMode & QUADSPI_CCR_FMODE_Msk);					// Functional Mode

	return reg_ccr;
}


uint8_t BSP_STM32_QSPI_Init(QUADSPI_TypeDef * hqspi, uint32_t ClockPrescaler, uint32_t ClockMode, uint32_t FifoThreshold, uint32_t FlashSize) {

	// Configure QSPI FIFO Threshold
	MODIFY_REG(hqspi->CR, QUADSPI_CR_FTHRES, ((FifoThreshold - 1U) << QUADSPI_CR_FTHRES_Pos));

	// Wait till BUSY flag reset
	if (_QSPI_WaitForFlag(hqspi, QSPI_FLAG_BUSY, RESET, 5000)) return BSP_TIMEOUT;

	// Configure QSPI Clock Prescaler and Sample Shift
	MODIFY_REG(hqspi->CR, (QUADSPI_CR_PRESCALER | QUADSPI_CR_SSHIFT | QUADSPI_CR_FSEL | QUADSPI_CR_DFM),
			((ClockPrescaler << QUADSPI_CR_PRESCALER_Pos) | QSPI_SAMPLE_SHIFTING_NONE  | QSPI_FLASH_ID_1 | QSPI_DUALFLASH_DISABLE));

	// Configure QSPI Flash Size, CS High Time and Clock Mode
	MODIFY_REG(hqspi->DCR, (QUADSPI_DCR_FSIZE | QUADSPI_DCR_CSHT | QUADSPI_DCR_CKMODE),
			((FlashSize << QUADSPI_DCR_FSIZE_Pos) | QSPI_CS_HIGH_TIME_1_CYCLE | ClockMode));

	// Enable the QSPI peripheral
	hqspi->CR |= QUADSPI_CR_EN;

    return BSP_OK;
}


uint8_t BSP_STM32_QSPI_CfgNewCommand() {

	memset(&qspictx, 0x00, sizeof(qspictx));

	return BSP_OK;
}

uint8_t BSP_STM32_QSPI_CfgInstruction(uint32_t Mode, uint8_t Instruction) {

	qspictx.InstructionMode = Mode;
	qspictx.Instruction = Instruction;

	return BSP_OK;
}

uint8_t BSP_STM32_QSPI_CfgAddress(uint32_t Mode, uint32_t Size, uint32_t Address) {

	qspictx.AddressMode = Mode;
	qspictx.AddressSize = Size;
	qspictx.Address = Address;

	return BSP_OK;
}

uint8_t BSP_STM32_QSPI_CfgAltBytes(uint32_t Mode, uint32_t Size, uint32_t AltBytes) {

	qspictx.AlternateMode = Mode;
	qspictx.AlternateSize = Size;
	qspictx.AlternateBytes = AltBytes;

	return BSP_OK;
}

uint8_t BSP_STM32_QSPI_CfgDummyCycles(uint32_t DummyCycles) {

	qspictx.DummyCycles = DummyCycles;

	return BSP_OK;
}

uint8_t BSP_STM32_QSPI_CfgData(uint32_t Mode, uint8_t * pData, uint32_t Length) {

	qspictx.DataMode = Mode;
	qspictx.DataLenth = Length;
	qspictx.pData = pData;

	return BSP_OK;
}

uint8_t	BSP_STM32_QSPI_CfgOptions(uint32_t DDRMode, uint32_t SignalDelay, uint32_t SIOOMode) {
	qspictx.DDRMode = DDRMode;
	qspictx.SignalDelay = SignalDelay;
	qspictx.SIOOMode = SIOOMode;

	return BSP_OK;
}

uint8_t BSP_STM32_QSPI_CfgAutoPooling(uint32_t mask, uint32_t match, uint32_t interval, uint32_t matchmode) {
	qspictx.AutoPoolingMask = mask;
	qspictx.AutoPoolingMatch = match;
	qspictx.AutoPoolingInterval = interval;
	qspictx.AutoPoolingMode = matchmode;

	return BSP_OK;
}


uint8_t BSP_STM32_QSPI_StartReceive(QUADSPI_TypeDef * hqspi) {

	// 1. Wait till BUSY flag reset
	if (_QSPI_WaitForFlag(hqspi, QSPI_FLAG_BUSY, RESET, 5000)) return BSP_TIMEOUT;

	// 2. Set CCR configuration register
	hqspi->CCR = _QSPI_GetConfigCCR(hqspi, QSPI_FUNCTIONAL_MODE_INDIRECT_READ);

	// 3. Initiate transmit of data by rewriting Address Register
	if (qspictx.AddressMode != QSPI_ADDRESS_NONE) hqspi->AR = qspictx.Address;

	// 4. If Data block defined than receive data
	if (qspictx.DataMode != QSPI_DATA_NONE) {
		uint32_t xFerCount = qspictx.DataLenth;
		__IO uint8_t * pData = qspictx.pData;
		__IO uint8_t * pRegDR = (uint8_t *)&hqspi->DR;

		while (xFerCount > 0) {

			//Wait until FT or TC flag is set to read received data
			if (_QSPI_WaitForFlag(hqspi, (QSPI_FLAG_FT | QSPI_FLAG_TC), SET, 5000)) return BSP_TIMEOUT;

			// Copy data from DR register
			*pData = *pRegDR;

			pData++;
			xFerCount--;
		}

	}

	// 5. Wait for TC flag to SET
	if (_QSPI_WaitForFlag(hqspi, QSPI_FLAG_TC, SET, 5000)) return BSP_TIMEOUT;

	// 6. Reset TC flag
	hqspi->FCR |= QSPI_FLAG_TC;


	return BSP_OK;
}


uint8_t BSP_STM32_QSPI_StartTransmit(QUADSPI_TypeDef * hqspi) {

	// 1. Wait till BUSY flag reset
	if (_QSPI_WaitForFlag(hqspi, QSPI_FLAG_BUSY, RESET, 5000)) return BSP_TIMEOUT;

	// 2. Set CCR configuration register
	hqspi->CCR = _QSPI_GetConfigCCR(hqspi, QSPI_FUNCTIONAL_MODE_INDIRECT_WRITE);

	// 3. Initiate transmit of data by rewriting Address Register
	if (qspictx.AddressMode != QSPI_ADDRESS_NONE) hqspi->AR = qspictx.Address;

	// 4. If Data block defined than receive data
	if (qspictx.DataMode != QSPI_DATA_NONE) {
		uint32_t xFerCount = qspictx.DataLenth;
		__IO uint8_t * pData = qspictx.pData;
		__IO uint8_t * pRegDR = (uint8_t *)&hqspi->DR;

		while (xFerCount > 0) {

			//Wait until FT is set to transmit data
			if (_QSPI_WaitForFlag(hqspi, QSPI_FLAG_FT, SET, 5000)) return BSP_TIMEOUT;

			// Copy data to DR register
			 *pRegDR = *pData;

			pData++;
			xFerCount--;
		}

	}

	// 5. Wait for TC flag to SET
	if (_QSPI_WaitForFlag(hqspi, QSPI_FLAG_TC, SET, 5000)) return BSP_TIMEOUT;

	// 6. Reset TC flag
	hqspi->FCR |= QSPI_FLAG_TC;


	return BSP_OK;
}


uint8_t BSP_STM32_QSPI_Abort(QUADSPI_TypeDef * hqspi) {
	// 1. Exit any previous operation / mode
	SET_BIT(hqspi->CR, QUADSPI_CR_ABORT);
	CLEAR_BIT(hqspi->CCR, QUADSPI_CCR_FMODE);

	// 2. Wait till BUSY flag reset
	if (_QSPI_WaitForFlag(hqspi, QSPI_FLAG_BUSY, RESET, 5000)) return BSP_TIMEOUT;

	// 3. Set timeout to DISABLE
	 MODIFY_REG(hqspi->CR, QUADSPI_CR_TCEN, QSPI_TIMEOUT_COUNTER_DISABLE);

	 return BSP_OK;
}


uint8_t	BSP_STM32_QSPI_StartMemMapped(QUADSPI_TypeDef * hqspi) {
	// 1. Exit any previous operation / mode
	SET_BIT(hqspi->CR, QUADSPI_CR_ABORT);
	CLEAR_BIT(hqspi->CCR, QUADSPI_CCR_FMODE);

	// 2. Wait till BUSY flag reset
	if (_QSPI_WaitForFlag(hqspi, QSPI_FLAG_BUSY, RESET, 5000)) return BSP_TIMEOUT;

	// 3. Set timeout to DISABLE
	 MODIFY_REG(hqspi->CR, QUADSPI_CR_TCEN, QSPI_TIMEOUT_COUNTER_DISABLE);

	 // 4. Set CCR configuration register
	 hqspi->CCR = _QSPI_GetConfigCCR(hqspi, QSPI_FUNCTIONAL_MODE_MEMORY_MAPPED);

	 return BSP_OK;
}


uint8_t BSP_STM32_QSPI_StartAutoPooling(QUADSPI_TypeDef * hqspi) {
	// 1. Wait till BUSY flag reset
	if (_QSPI_WaitForFlag(hqspi, QSPI_FLAG_BUSY, RESET, 5000)) return BSP_TIMEOUT;

	// 2. Configure Auto Pooling parameters
	hqspi->PSMAR = qspictx.AutoPoolingMatch;
	hqspi->PSMKR = qspictx.AutoPoolingMask;
	hqspi->PIR = qspictx.AutoPoolingInterval;

	// 3. Configure CR register for Auto Pooling mode
    MODIFY_REG(hqspi->CR, (QUADSPI_CR_PMM | QUADSPI_CR_APMS), (qspictx.AutoPoolingMode | QSPI_AUTOMATIC_STOP_ENABLE));

    // 4. Configure CCR register
    hqspi->CCR = _QSPI_GetConfigCCR(hqspi, QSPI_FUNCTIONAL_MODE_AUTO_POLLING);

	// 5. Wait for SM flag to SET
	if (_QSPI_WaitForFlag(hqspi, QSPI_FLAG_SM, SET, 60000)) return BSP_TIMEOUT;

	hqspi->FCR = QSPI_FLAG_SM;

	// 6. Exit autopooling mode
	SET_BIT(hqspi->CR, QUADSPI_CR_ABORT);
	CLEAR_BIT(hqspi->CCR, QUADSPI_CCR_FMODE);

	return BSP_OK;
}
