/*******************************************************************
 * MiniConsole V3 - Driver - IS25LP128F - QSPI Memory
 *
 * Author: Marek Ryn
 * Version: 1.0
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 * - 1.0	- Initial version
 *******************************************************************/

#include "IS25LP128F.h"


static uint8_t	_DRV_QSPI_CheckStatus(QUADSPI_TypeDef * hqspi, uint8_t mode, uint8_t mask, uint8_t match) {

	// 1. Configure command
	BSP_STM32_QSPI_CfgNewCommand();
	BSP_STM32_QSPI_CfgOptions(QSPI_DDR_MODE_DISABLE, QSPI_DDR_HHC_ANALOG_DELAY, QSPI_SIOO_INST_EVERY_CMD);
	BSP_STM32_QSPI_CfgAutoPooling(mask, match, 0x10, QSPI_MATCH_MODE_AND);

	switch (mode) {
	case QSPI_MODE_SPI:
	case QSPI_MODE_QSPI:
		BSP_STM32_QSPI_CfgInstruction(QSPI_INSTRUCTION_1_LINE, QSPI_CMD_RDSR);
		BSP_STM32_QSPI_CfgData(QSPI_DATA_1_LINE, 0x00000000, 1);
		break;
	case QSPI_MODE_QPI:
		BSP_STM32_QSPI_CfgInstruction(QSPI_INSTRUCTION_4_LINES, QSPI_CMD_RDSR);
		BSP_STM32_QSPI_CfgData(QSPI_DATA_4_LINES, 0x00000000, 1);
		break;
	default:
		return BSP_ERROR;
	}

	// 2. Execute command in Auto Pooling Mode
	if (BSP_STM32_QSPI_StartAutoPooling(hqspi)) return BSP_TIMEOUT;

	return BSP_OK;

}



static uint8_t	_DRV_QSPI_WriteEnable(QUADSPI_TypeDef * hqspi, uint8_t mode) {

	// 1. COnfigure command
	BSP_STM32_QSPI_CfgNewCommand();
	BSP_STM32_QSPI_CfgOptions(QSPI_DDR_MODE_DISABLE, QSPI_DDR_HHC_ANALOG_DELAY, QSPI_SIOO_INST_EVERY_CMD);

	switch (mode) {
	case QSPI_MODE_SPI:
	case QSPI_MODE_QSPI:
		BSP_STM32_QSPI_CfgInstruction(QSPI_INSTRUCTION_1_LINE, QSPI_CMD_WREN);
		break;
	case QSPI_MODE_QPI:
		BSP_STM32_QSPI_CfgInstruction(QSPI_INSTRUCTION_4_LINES, QSPI_CMD_WREN);
		break;
	}

	// 2. Execute command
	if (BSP_STM32_QSPI_StartTransmit(hqspi)) return BSP_ERROR;

    return BSP_OK;
}



uint8_t	BSP_DRV_QSPI_Reset(QUADSPI_TypeDef * hqspi) {

	// 1. Exit QPI mode and fallback to SPI 1-bit mode
	BSP_STM32_QSPI_CfgNewCommand();
	BSP_STM32_QSPI_CfgInstruction(QSPI_INSTRUCTION_4_LINES, QSPI_CMD_QPIDI);
	BSP_STM32_QSPI_CfgOptions(QSPI_DDR_MODE_DISABLE, QSPI_DDR_HHC_ANALOG_DELAY, QSPI_SIOO_INST_EVERY_CMD);
	BSP_STM32_QSPI_StartTransmit(hqspi);  // This function can exit with error if chip was not in QPI mode - just ignore it

	// 2. Sending Reset Enable command

    BSP_STM32_QSPI_CfgNewCommand();
    BSP_STM32_QSPI_CfgInstruction(QSPI_INSTRUCTION_1_LINE, QSPI_CMD_RSTEN);
    BSP_STM32_QSPI_CfgOptions(QSPI_DDR_MODE_DISABLE, QSPI_DDR_HHC_ANALOG_DELAY, QSPI_SIOO_INST_EVERY_CMD);
    if (BSP_STM32_QSPI_StartTransmit(hqspi)) return BSP_ERROR;

	// 3. Sending Reset command

    BSP_STM32_QSPI_CfgNewCommand();
    BSP_STM32_QSPI_CfgInstruction(QSPI_INSTRUCTION_1_LINE, QSPI_CMD_RST);
    BSP_STM32_QSPI_CfgOptions(QSPI_DDR_MODE_DISABLE, QSPI_DDR_HHC_ANALOG_DELAY, QSPI_SIOO_INST_EVERY_CMD);
    if (BSP_STM32_QSPI_StartTransmit(hqspi)) return BSP_ERROR;

	// 4. Waiting for chip ready status
    BSP_Delay(10); // Delay so CE line can go HIGH
    if (_DRV_QSPI_CheckStatus(hqspi, QSPI_MODE_SPI, 0x01, 0x00)) return BSP_ERROR;

    return BSP_OK;
}



uint8_t BSP_DRV_QSPI_Init(QUADSPI_TypeDef * hqspi) {
    uint8_t chip_id[2] = {0};
    uint8_t	reg = 0;
    uint8_t	status_reg = 0;
    uint8_t try = 0;

    // For some reason memory chip is not working properly after every reset
    // Therefore operation is repeated as required (but not more than 4 times)
    do {

    	try++;
		if (try >= 4) return BSP_ERROR;

		// 1. Reset chip
		if (BSP_DRV_QSPI_Reset(hqspi)) continue;

		// 2. Checking communication and identify chip
		BSP_STM32_QSPI_CfgNewCommand();
		BSP_STM32_QSPI_CfgInstruction(QSPI_INSTRUCTION_1_LINE, QSPI_CMD_RDMDID);
		BSP_STM32_QSPI_CfgAddress(QSPI_ADDRESS_1_LINE, QSPI_ADDRESS_24_BITS, 0x00);
		BSP_STM32_QSPI_CfgData(QSPI_DATA_1_LINE, chip_id, 2);
		BSP_STM32_QSPI_CfgOptions(QSPI_DDR_MODE_DISABLE, QSPI_DDR_HHC_ANALOG_DELAY, QSPI_SIOO_INST_EVERY_CMD);
		if (BSP_STM32_QSPI_StartReceive(hqspi)) continue;

		if (chip_id[0] != QSPI_CHIP_MID) continue;

		if (chip_id[1] != QSPI_CHIP_CID) continue;

		break;

    } while (1);

	// 3. Setup dummy cycles
    BSP_STM32_QSPI_CfgNewCommand();
    BSP_STM32_QSPI_CfgInstruction(QSPI_INSTRUCTION_1_LINE, QSPI_CMD_RDRP);
    BSP_STM32_QSPI_CfgData(QSPI_DATA_1_LINE, &reg, 1);
	BSP_STM32_QSPI_CfgOptions(QSPI_DDR_MODE_DISABLE, QSPI_DDR_HHC_ANALOG_DELAY, QSPI_SIOO_INST_EVERY_CMD);
	if (BSP_STM32_QSPI_StartReceive(hqspi)) return BSP_ERROR;

    MODIFY_REG(reg, 0b01111000, (QSPI_CFG_DUMMY_CLKS << 3));

    if (_DRV_QSPI_WriteEnable(hqspi, QSPI_MODE_SPI)) return BSP_ERROR;

    BSP_STM32_QSPI_CfgNewCommand();
    BSP_STM32_QSPI_CfgInstruction(QSPI_INSTRUCTION_1_LINE, QSPI_CMD_SRPV);
    BSP_STM32_QSPI_CfgData(QSPI_DATA_1_LINE, &reg, 1);
	BSP_STM32_QSPI_CfgOptions(QSPI_DDR_MODE_DISABLE, QSPI_DDR_HHC_ANALOG_DELAY, QSPI_SIOO_INST_EVERY_CMD);
	if (BSP_STM32_QSPI_StartTransmit(hqspi)) return BSP_ERROR;

	// Waiting for chip ready status
    if (_DRV_QSPI_CheckStatus(hqspi, QSPI_MODE_SPI, 0x01, 0x00)) return BSP_ERROR;

    // 4. Setting driver output power
    reg = QSPI_CFG_DRV_PWR;
    BSP_STM32_QSPI_CfgNewCommand();
    BSP_STM32_QSPI_CfgInstruction(QSPI_INSTRUCTION_1_LINE, QSPI_CMD_SERPV);
    BSP_STM32_QSPI_CfgData(QSPI_DATA_1_LINE, &reg, 1);
    BSP_STM32_QSPI_CfgOptions(QSPI_DDR_MODE_DISABLE, QSPI_DDR_HHC_ANALOG_DELAY, QSPI_SIOO_INST_EVERY_CMD);
    if (BSP_STM32_QSPI_StartTransmit(hqspi)) return BSP_ERROR;

	// Waiting for chip ready status
    if (_DRV_QSPI_CheckStatus(hqspi, QSPI_MODE_SPI, 0x01, 0x00)) return BSP_ERROR;

	// 5. Enable 4bytes addresses (required for mem-mapped mode)
    BSP_STM32_QSPI_CfgNewCommand();
    BSP_STM32_QSPI_CfgInstruction(QSPI_INSTRUCTION_1_LINE, QSPI_CMD_EN4B);
    BSP_STM32_QSPI_CfgOptions(QSPI_DDR_MODE_DISABLE, QSPI_DDR_HHC_ANALOG_DELAY, QSPI_SIOO_INST_EVERY_CMD);
    if (BSP_STM32_QSPI_StartTransmit(hqspi)) return BSP_ERROR;

	// 6. Enable quad mode
    BSP_STM32_QSPI_CfgNewCommand();
    BSP_STM32_QSPI_CfgInstruction(QSPI_INSTRUCTION_1_LINE, QSPI_CMD_RDSR);
    BSP_STM32_QSPI_CfgData(QSPI_DATA_1_LINE, &status_reg, 1);
    BSP_STM32_QSPI_CfgOptions(QSPI_DDR_MODE_DISABLE, QSPI_DDR_HHC_ANALOG_DELAY, QSPI_SIOO_INST_EVERY_CMD);
    if (BSP_STM32_QSPI_StartReceive(hqspi)) return BSP_ERROR;

    // The quad mode option bit is non-volatile. If it is already set, there is no need to write it again.
    if ((status_reg & (0b01000000)) == 0) {

    	MODIFY_REG(status_reg, 0b01000000, (1 << 6));

		if (_DRV_QSPI_WriteEnable(hqspi, QSPI_MODE_SPI)) return BSP_ERROR;

		BSP_STM32_QSPI_CfgNewCommand();
		BSP_STM32_QSPI_CfgInstruction(QSPI_INSTRUCTION_1_LINE, QSPI_CMD_WRSR);
		BSP_STM32_QSPI_CfgData(QSPI_DATA_1_LINE, &status_reg, 1);
		BSP_STM32_QSPI_CfgOptions(QSPI_DDR_MODE_DISABLE, QSPI_DDR_HHC_ANALOG_DELAY, QSPI_SIOO_INST_EVERY_CMD);
		if (BSP_STM32_QSPI_StartTransmit(hqspi)) return BSP_ERROR;

		BSP_Delay(10); // Delay so CE line can go HIGH

		// Waiting for chip ready status
		if (_DRV_QSPI_CheckStatus(hqspi, QSPI_MODE_SPI, 0x01, 0x00)) return BSP_ERROR;

    }

	// 7. Enable QPI mode
    BSP_STM32_QSPI_CfgNewCommand();
    BSP_STM32_QSPI_CfgInstruction(QSPI_INSTRUCTION_1_LINE, QSPI_CMD_QPIEN);
	BSP_STM32_QSPI_CfgOptions(QSPI_DDR_MODE_DISABLE, QSPI_DDR_HHC_ANALOG_DELAY, QSPI_SIOO_INST_EVERY_CMD);
	if (BSP_STM32_QSPI_StartTransmit(hqspi)) return BSP_ERROR;

	return BSP_OK;
}


uint8_t BSP_DRV_QSPI_MemMappedEnable(QUADSPI_TypeDef * hqspi) {

    // Enable Memory-Mapped mode
	BSP_STM32_QSPI_CfgNewCommand();
	BSP_STM32_QSPI_CfgInstruction(QSPI_INSTRUCTION_4_LINES, QSPI_CMD_FRQDTR);
	BSP_STM32_QSPI_CfgAddress(QSPI_ADDRESS_4_LINES, QSPI_ADDRESS_32_BITS, 0);
	// To activate SIOO mode byte must be in 0xAX format, where X are not important.
	BSP_STM32_QSPI_CfgAltBytes(QSPI_ALTERNATE_BYTES_4_LINES, QSPI_ALTERNATE_BYTES_8_BITS, 0xAA);
	// One dummy cycle used for alternate byte (On 4 lines and DDR mode)
	BSP_STM32_QSPI_CfgDummyCycles(QSPI_CFG_DUMMY_CLKS - 1);
	// Pointer to buffer and data length will be set automatically in memory mapped mode
	BSP_STM32_QSPI_CfgData(QSPI_DATA_4_LINES, 0x00000000, 0x00);
	// Activating DDR and SIOO mode
	BSP_STM32_QSPI_CfgOptions(QSPI_DDR_MODE_ENABLE, QSPI_DDR_HHC_HALF_CLK_DELAY, QSPI_SIOO_INST_ONLY_FIRST_CMD);

	if (BSP_STM32_QSPI_StartMemMapped(hqspi)) return BSP_ERROR;

	return BSP_OK;
}


uint8_t BSP_DRV_QSPI_WriteBuf(QUADSPI_TypeDef * hqspi, uint32_t address, uint8_t * buf, uint32_t length) {

	uint32_t index = length;
	uint32_t adr = address;
	uint32_t page = 0;
	uint32_t offset = 0;
	uint32_t txsize = 0;

	if ((address + length)> QSPI_CFG_MEM_SIZE) return BSP_ERROR;

	// Waiting for chip ready status
	if (_DRV_QSPI_CheckStatus(hqspi, QSPI_MODE_QPI, 0x01, 0x00)) return BSP_ERROR;

	while (index > 0) {

		// Enable write
		if (_DRV_QSPI_WriteEnable(hqspi, QSPI_MODE_QPI)) return BSP_ERROR;

		// Configuring command
		BSP_STM32_QSPI_CfgNewCommand();
		BSP_STM32_QSPI_CfgInstruction(QSPI_INSTRUCTION_4_LINES, QSPI_CMD_PP);
		BSP_STM32_QSPI_CfgOptions(QSPI_DDR_MODE_DISABLE, QSPI_DDR_HHC_ANALOG_DELAY, QSPI_SIOO_INST_EVERY_CMD);

		adr += txsize;

		page = adr / QSPI_CFG_PAGE_SIZE;
		offset = adr % QSPI_CFG_PAGE_SIZE;
		txsize = (index < QSPI_CFG_PAGE_SIZE)?index:(QSPI_CFG_PAGE_SIZE - offset);

		BSP_STM32_QSPI_CfgAddress(QSPI_ADDRESS_4_LINES, QSPI_ADDRESS_32_BITS, ((page * QSPI_CFG_PAGE_SIZE) + offset));
		BSP_STM32_QSPI_CfgData(QSPI_DATA_4_LINES, buf, txsize);

		// Transmitting data
		if (BSP_STM32_QSPI_StartTransmit(hqspi)) return BSP_ERROR;

		// Waiting for chip ready status
		BSP_Delay(10); // Delay so CE line can go HIGH
		if (_DRV_QSPI_CheckStatus(hqspi, QSPI_MODE_QPI, 0x01, 0x00)) return BSP_ERROR;

		index -= txsize;
		buf += txsize;
	}

	return BSP_OK;
}


uint8_t BSP_DRV_QSPI_ReadBuf(QUADSPI_TypeDef * hqspi, uint32_t address, uint8_t * buf, uint32_t length) {

	// Waiting for chip ready status
	if (_DRV_QSPI_CheckStatus(hqspi, QSPI_MODE_QPI, 0x01, 0x00)) return BSP_ERROR;

	// Configuring command
	BSP_STM32_QSPI_CfgNewCommand();
	BSP_STM32_QSPI_CfgInstruction(QSPI_INSTRUCTION_4_LINES, QSPI_CMD_FRQDTR);
	BSP_STM32_QSPI_CfgAddress(QSPI_ADDRESS_4_LINES, QSPI_ADDRESS_32_BITS, address);
	BSP_STM32_QSPI_CfgDummyCycles(QSPI_CFG_DUMMY_CLKS);
	BSP_STM32_QSPI_CfgData(QSPI_DATA_4_LINES, buf, length);
	BSP_STM32_QSPI_CfgOptions(QSPI_DDR_MODE_ENABLE, QSPI_DDR_HHC_HALF_CLK_DELAY, QSPI_SIOO_INST_EVERY_CMD);

	// Receiving data
	if (BSP_STM32_QSPI_StartReceive(hqspi)) return BSP_ERROR;

	return BSP_OK;
}


uint8_t BSP_DRV_QSPI_ReadUID(QUADSPI_TypeDef * hqspi, uint8_t * buf) {

	// Waiting for chip ready status
	if (_DRV_QSPI_CheckStatus(hqspi, QSPI_MODE_QPI, 0x01, 0x00)) return BSP_ERROR;

	// Configuring command
	BSP_STM32_QSPI_CfgNewCommand();
	BSP_STM32_QSPI_CfgInstruction(QSPI_INSTRUCTION_4_LINES, QSPI_CMD_RDUID);
	BSP_STM32_QSPI_CfgAddress(QSPI_ADDRESS_4_LINES, QSPI_ADDRESS_24_BITS, 0x00);
	BSP_STM32_QSPI_CfgDummyCycles(QSPI_CFG_DUMMY_CLKS);
	BSP_STM32_QSPI_CfgData(QSPI_DATA_4_LINES, buf, 16);
	BSP_STM32_QSPI_CfgOptions(QSPI_DDR_MODE_DISABLE, QSPI_DDR_HHC_ANALOG_DELAY, QSPI_SIOO_INST_EVERY_CMD);

	// Receiving data
	if (BSP_STM32_QSPI_StartReceive(hqspi)) return BSP_ERROR;

	return BSP_OK;
}


uint8_t	BSP_DRV_QSPI_ChipErase(QUADSPI_TypeDef * hqspi) {

	// Waiting for chip ready status
	if (_DRV_QSPI_CheckStatus(hqspi, QSPI_MODE_QPI, 0x01, 0x00)) return BSP_ERROR;

	// Enable write
	if (_DRV_QSPI_WriteEnable(hqspi, QSPI_MODE_QPI)) return BSP_ERROR;

	// Configure command
	BSP_STM32_QSPI_CfgNewCommand();
	BSP_STM32_QSPI_CfgInstruction(QSPI_INSTRUCTION_4_LINES, QSPI_CMD_CER);
	BSP_STM32_QSPI_CfgOptions(QSPI_DDR_MODE_DISABLE, QSPI_DDR_HHC_ANALOG_DELAY, QSPI_SIOO_INST_EVERY_CMD);

	// Send command
	if (BSP_STM32_QSPI_StartTransmit(hqspi)) return BSP_ERROR;

	// Waiting for chip ready status
	BSP_Delay(10); // Delay so CE line can go HIGH
	if (_DRV_QSPI_CheckStatus(hqspi, QSPI_MODE_QPI, 0x01, 0x00)) return BSP_ERROR;

	return BSP_OK;
}


uint8_t BSP_DRV_QSPI_SectorErase(QUADSPI_TypeDef * hqspi, uint32_t address) {

	uint32_t adr = (address / QSPI_CFG_SECTOR_SIZE) * QSPI_CFG_SECTOR_SIZE;
	if ((address / QSPI_CFG_SECTOR_SIZE) > (QSPI_CFG_BLOCK_COUNT * QSPI_CFG_BLOCK_SIZE)) return BSP_ERROR;

	// Waiting for chip ready status
	if (_DRV_QSPI_CheckStatus(hqspi, QSPI_MODE_QPI, 0x01, 0x00)) return BSP_ERROR;

	// Enable write
	if (_DRV_QSPI_WriteEnable(hqspi, QSPI_MODE_QPI)) return BSP_ERROR;

	// Configure command
	BSP_STM32_QSPI_CfgNewCommand();
	BSP_STM32_QSPI_CfgInstruction(QSPI_INSTRUCTION_4_LINES, QSPI_CMD_SER);
	BSP_STM32_QSPI_CfgAddress(QSPI_ADDRESS_4_LINES, QSPI_ADDRESS_32_BITS, adr);
	BSP_STM32_QSPI_CfgOptions(QSPI_DDR_MODE_DISABLE, QSPI_DDR_HHC_ANALOG_DELAY, QSPI_SIOO_INST_EVERY_CMD);

	// Send command
	if (BSP_STM32_QSPI_StartTransmit(hqspi)) return BSP_ERROR;

	// Waiting for chip ready status
	BSP_Delay(10); // Delay so CE line can go HIGH
	if (_DRV_QSPI_CheckStatus(hqspi, QSPI_MODE_QPI, 0x01, 0x00)) return BSP_ERROR;

	return BSP_OK;
}
