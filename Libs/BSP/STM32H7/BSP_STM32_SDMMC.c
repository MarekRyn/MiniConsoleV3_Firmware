/*******************************************************************
 * MiniConsole V3 - Board Support Package - STM32 SDMMC
 *
 * Author: Marek Ryn
 * Version: 1.0
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 * - 1.0	- Initial version
 *******************************************************************/

#include "BSP_STM32_SDMMC.h"

typedef struct {
  __IO uint8_t  CSDStruct;            // CSD structure
  __IO uint8_t  SysSpecVersion;       // System specification version
  __IO uint8_t  Reserved1;            // Reserved
  __IO uint8_t  TAAC;                 // Data read access time 1
  __IO uint8_t  NSAC;                 // Data read access time 2 in CLK cycles
  __IO uint8_t  MaxBusClkFrec;        // Max. bus clock frequency
  __IO uint16_t CardComdClasses;      // Card command classes
  __IO uint8_t  RdBlockLen;           // Max. read data block length
  __IO uint8_t  PartBlockRead;        // Partial blocks for read allowed
  __IO uint8_t  WrBlockMisalign;      // Write block misalignment
  __IO uint8_t  RdBlockMisalign;      // Read block misalignment
  __IO uint8_t  DSRImpl;              // DSR implemented
  __IO uint8_t  Reserved2;            // Reserved
  __IO uint32_t DeviceSize;           // Device Size
  __IO uint8_t  MaxRdCurrentVDDMin;   // Max. read current @ VDD min
  __IO uint8_t  MaxRdCurrentVDDMax;   // Max. read current @ VDD max
  __IO uint8_t  MaxWrCurrentVDDMin;   // Max. write current @ VDD min
  __IO uint8_t  MaxWrCurrentVDDMax;   // Max. write current @ VDD max
  __IO uint8_t  DeviceSizeMul;        // Device size multiplier
  __IO uint8_t  EraseGrSize;          // Erase group size
  __IO uint8_t  EraseGrMul;           // Erase group size multiplier
  __IO uint8_t  WrProtectGrSize;      // Write protect group size
  __IO uint8_t  WrProtectGrEnable;    // Write protect group enable
  __IO uint8_t  ManDeflECC;           // Manufacturer default ECC
  __IO uint8_t  WrSpeedFact;          // Write speed factor
  __IO uint8_t  MaxWrBlockLen;        // Max. write data block length
  __IO uint8_t  WriteBlockPaPartial;  // Partial blocks for write allowed
  __IO uint8_t  Reserved3;            // Reserved
  __IO uint8_t  ContentProtectAppli;  // Content protection application
  __IO uint8_t  FileFormatGroup;      // File format group
  __IO uint8_t  CopyFlag;             // Copy flag (OTP)
  __IO uint8_t  PermWrProtect;        // Permanent write protection
  __IO uint8_t  TempWrProtect;        // Temporary write protection
  __IO uint8_t  FileFormat;           // File format
  __IO uint8_t  ECC;                  // ECC code
  __IO uint8_t  CSD_CRC;              // CSD CRC
  __IO uint8_t  Reserved4;            // Always 1
} SDMMC_CSD_TypeDef;


typedef struct
{
  __IO uint8_t  DataBusWidth;           // Shows the currently defined data bus width
  __IO uint8_t  SecuredMode;            // Card is in secured mode of operation
  __IO uint16_t CardType;               // Carries information about card type
  __IO uint32_t ProtectedAreaSize;      // Carries information about the capacity of protected area
  __IO uint8_t  SpeedClass;             // Carries information about the speed class of the card
  __IO uint8_t  PerformanceMove;        // Carries information about the card's performance move
  __IO uint8_t  AllocationUnitSize;     // Carries information about the card's allocation unit size
  __IO uint16_t EraseSize;              // Determines the number of AUs to be erased in one operation
  __IO uint8_t  EraseTimeout;           // Determines the timeout for any number of AU erase
  __IO uint8_t  EraseOffset;            // Carries information about the erase offset
  __IO uint8_t  UhsSpeedGrade;          // Carries information about the speed grade of UHS card
  __IO uint8_t  UhsAllocationUnitSize;  // Carries information about the UHS card's allocation unit size
  __IO uint8_t  VideoSpeedClass;        // Carries information about the Video Speed Class of UHS card
} SDMMC_CardStatus_TypeDef;




static uint8_t _SD_IdentifyCard(SDMMC_TypeDef * hsdmmc, TxRxContext_TypeDef * ctx) {

	SDMMCContext_TypeDef * sdmmc_ctx = (SDMMCContext_TypeDef *)ctx->ctxmem;

	__IO uint32_t count = 0U;
	uint32_t response = 0U;
	uint32_t validvoltage = 0U;

	// CMD0: GO_IDLE_STATE
	if (SDMMC_CmdGoIdleState(hsdmmc)) return BSP_ERROR;

	// CMD8: SEND_IF_COND: Command available only on V2.0 cards
	if (SDMMC_CmdOperCond(hsdmmc) == SDMMC_ERROR_TIMEOUT) {

		// No response to CMD8
		sdmmc_ctx->SDCardVersion = CARD_V1_X;
		// CMD0: GO_IDLE_STATE
		if (SDMMC_CmdGoIdleState(hsdmmc)) return BSP_ERROR;

	} else {

		sdmmc_ctx->SDCardVersion = CARD_V2_X;
		// SEND CMD55 APP_CMD with RCA as 0
		if (SDMMC_CmdAppCommand(hsdmmc, 0)) return BSP_ERROR;

	}

	// Send ACMD41 SD_APP_OP_COND with Argument 0x80100000
	while ((count < SDMMC_MAX_VOLT_TRIAL) && (validvoltage == 0U)) {

		// SEND CMD55 APP_CMD with RCA as 0
		if (SDMMC_CmdAppCommand(hsdmmc, 0)) return BSP_ERROR;

		// Send CMD41
		if (SDMMC_CmdAppOperCommand(hsdmmc, SDMMC_VOLTAGE_WINDOW_SD | SDMMC_HIGH_CAPACITY | SD_SWITCH_1_8V_CAPACITY)) return BSP_ERROR;

		// Get command response
		response = SDMMC_GetResponse(hsdmmc, SDMMC_RESP1);

		// Get operating voltage
		validvoltage = (((response >> 31U) == 1U) ? 1U : 0U);

		count++;
	}

	if (count >= SDMMC_MAX_VOLT_TRIAL) return BSP_ERROR;

  	// Set default card type
	sdmmc_ctx->SDCardType = ((response & SDMMC_HIGH_CAPACITY) == SDMMC_HIGH_CAPACITY) ? CARD_SDHC_SDXC : CARD_SDSC;

	return BSP_OK;
}


static uint8_t _SD_GetCardCSD(SDMMC_TypeDef * hsdmmc, TxRxContext_TypeDef * ctx, SDMMC_CSD_TypeDef * CSD) {

	SDMMCContext_TypeDef * sdmmc_ctx = (SDMMCContext_TypeDef *)ctx->ctxmem;

	CSD->CSDStruct = (uint8_t)((sdmmc_ctx->SDCardSD[0] & 0xC0000000U) >> 30U);
	CSD->SysSpecVersion = (uint8_t)((sdmmc_ctx->SDCardSD[0] & 0x3C000000U) >> 26U);
	CSD->Reserved1 = (uint8_t)((sdmmc_ctx->SDCardSD[0] & 0x03000000U) >> 24U);
	CSD->TAAC = (uint8_t)((sdmmc_ctx->SDCardSD[0] & 0x00FF0000U) >> 16U);
	CSD->NSAC = (uint8_t)((sdmmc_ctx->SDCardSD[0] & 0x0000FF00U) >> 8U);
	CSD->MaxBusClkFrec = (uint8_t)(sdmmc_ctx->SDCardSD[0] & 0x000000FFU);
	CSD->CardComdClasses = (uint16_t)((sdmmc_ctx->SDCardSD[1] & 0xFFF00000U) >> 20U);
	CSD->RdBlockLen = (uint8_t)((sdmmc_ctx->SDCardSD[1] & 0x000F0000U) >> 16U);
	CSD->PartBlockRead   = (uint8_t)((sdmmc_ctx->SDCardSD[1] & 0x00008000U) >> 15U);
	CSD->WrBlockMisalign = (uint8_t)((sdmmc_ctx->SDCardSD[1] & 0x00004000U) >> 14U);
	CSD->RdBlockMisalign = (uint8_t)((sdmmc_ctx->SDCardSD[1] & 0x00002000U) >> 13U);
	CSD->DSRImpl = (uint8_t)((sdmmc_ctx->SDCardSD[1] & 0x00001000U) >> 12U);
	CSD->Reserved2 = 0U; /*!< Reserved */

	switch (sdmmc_ctx->SDCardType) {
	case CARD_SDSC:
		CSD->DeviceSize = (((sdmmc_ctx->SDCardSD[1] & 0x000003FFU) << 2U) | ((sdmmc_ctx->SDCardSD[2] & 0xC0000000U) >> 30U));
		CSD->MaxRdCurrentVDDMin = (uint8_t)((sdmmc_ctx->SDCardSD[2] & 0x38000000U) >> 27U);
		CSD->MaxRdCurrentVDDMax = (uint8_t)((sdmmc_ctx->SDCardSD[2] & 0x07000000U) >> 24U);
		CSD->MaxWrCurrentVDDMin = (uint8_t)((sdmmc_ctx->SDCardSD[2] & 0x00E00000U) >> 21U);
		CSD->MaxWrCurrentVDDMax = (uint8_t)((sdmmc_ctx->SDCardSD[2] & 0x001C0000U) >> 18U);
		CSD->DeviceSizeMul = (uint8_t)((sdmmc_ctx->SDCardSD[2] & 0x00038000U) >> 15U);
		sdmmc_ctx->SDCardBlockNbr  = (CSD->DeviceSize + 1U) ;
		sdmmc_ctx->SDCardBlockNbr *= (1UL << ((CSD->DeviceSizeMul & 0x07U) + 2U));
		sdmmc_ctx->SDCardBlockSize = (1UL << (CSD->RdBlockLen & 0x0FU));
		sdmmc_ctx->SDCardLogBlockNbr = (sdmmc_ctx->SDCardBlockNbr) * ((sdmmc_ctx->SDCardBlockSize) / 512U);
		sdmmc_ctx->SDCardLogBlockSize = 512U;
		break;
	case CARD_SDHC_SDXC:
		/* Byte 7 */
		CSD->DeviceSize = (((sdmmc_ctx->SDCardSD[1] & 0x0000003FU) << 16U) | ((sdmmc_ctx->SDCardSD[2] & 0xFFFF0000U) >> 16U));
		sdmmc_ctx->SDCardBlockNbr = ((CSD->DeviceSize + 1U) * 1024U);
		sdmmc_ctx->SDCardLogBlockNbr = sdmmc_ctx->SDCardBlockNbr;
		sdmmc_ctx->SDCardBlockSize = 512U;
		sdmmc_ctx->SDCardLogBlockSize = sdmmc_ctx->SDCardBlockSize;
		break;
	default:
		// Clear all the static flags
		__SDMMC_CLEAR_FLAG(hsdmmc, SDMMC_STATIC_FLAGS);
		return BSP_ERROR;
	}

	CSD->EraseGrSize = (uint8_t)((sdmmc_ctx->SDCardSD[2] & 0x00004000U) >> 14U);
	CSD->EraseGrMul = (uint8_t)((sdmmc_ctx->SDCardSD[2] & 0x00003F80U) >> 7U);
	CSD->WrProtectGrSize = (uint8_t)(sdmmc_ctx->SDCardSD[2] & 0x0000007FU);
	CSD->WrProtectGrEnable = (uint8_t)((sdmmc_ctx->SDCardSD[3] & 0x80000000U) >> 31U);
	CSD->ManDeflECC = (uint8_t)((sdmmc_ctx->SDCardSD[3] & 0x60000000U) >> 29U);
	CSD->WrSpeedFact = (uint8_t)((sdmmc_ctx->SDCardSD[3] & 0x1C000000U) >> 26U);
	CSD->MaxWrBlockLen = (uint8_t)((sdmmc_ctx->SDCardSD[3] & 0x03C00000U) >> 22U);
	CSD->WriteBlockPaPartial = (uint8_t)((sdmmc_ctx->SDCardSD[3] & 0x00200000U) >> 21U);
	CSD->Reserved3 = 0;
	CSD->ContentProtectAppli = (uint8_t)((sdmmc_ctx->SDCardSD[3] & 0x00010000U) >> 16U);
	CSD->FileFormatGroup = (uint8_t)((sdmmc_ctx->SDCardSD[3] & 0x00008000U) >> 15U);
	CSD->CopyFlag = (uint8_t)((sdmmc_ctx->SDCardSD[3] & 0x00004000U) >> 14U);
	CSD->PermWrProtect = (uint8_t)((sdmmc_ctx->SDCardSD[3] & 0x00002000U) >> 13U);
	CSD->TempWrProtect = (uint8_t)((sdmmc_ctx->SDCardSD[3] & 0x00001000U) >> 12U);
	CSD->FileFormat = (uint8_t)((sdmmc_ctx->SDCardSD[3] & 0x00000C00U) >> 10U);
	CSD->ECC = (uint8_t)((sdmmc_ctx->SDCardSD[3] & 0x00000300U) >> 8U);
	CSD->CSD_CRC = (uint8_t)((sdmmc_ctx->SDCardSD[3] & 0x000000FEU) >> 1U);
	CSD->Reserved4 = 1;

	return BSP_OK;
}


static uint8_t _SD_InitCard(SDMMC_TypeDef * hsdmmc, TxRxContext_TypeDef * ctx) {

	SDMMCContext_TypeDef * sdmmc_ctx = (SDMMCContext_TypeDef *)ctx->ctxmem;

	SDMMC_CSD_TypeDef CSD = {0};

	uint16_t sd_rca = 0;
	uint32_t tickstart = BSP_GetTick();

	// Chheck the power State
	if (SDMMC_GetPowerState(hsdmmc) == 0U) return BSP_ERROR;

	// Check if card is secured
	if (sdmmc_ctx->SDCardType == CARD_SECURED) return BSP_ERROR;

    // Send CMD2 ALL_SEND_CID
    if (SDMMC_CmdSendCID(hsdmmc)) return BSP_ERROR;

    //Get Card identification number data
    sdmmc_ctx->SDCardID[0] = SDMMC_GetResponse(hsdmmc, SDMMC_RESP1);
    sdmmc_ctx->SDCardID[1] = SDMMC_GetResponse(hsdmmc, SDMMC_RESP2);
    sdmmc_ctx->SDCardID[2] = SDMMC_GetResponse(hsdmmc, SDMMC_RESP3);
    sdmmc_ctx->SDCardID[3] = SDMMC_GetResponse(hsdmmc, SDMMC_RESP4);

    // Send CMD3 SET_REL_ADDR with argument 0
    // SD Card publishes its RCA
    while (sd_rca == 0U) {
    	if (SDMMC_CmdSetRelAdd(hsdmmc, &sd_rca)) return BSP_ERROR;
    	if ((BSP_GetTick() - tickstart) >=  SDMMC_CMDTIMEOUT) return BSP_ERROR;
    }

    // Get the SD card RCA
    sdmmc_ctx->SDCardRelAdd = sd_rca;

    // Send CMD9 SEND_CSD with argument as card's RCA
    if (SDMMC_CmdSendCSD(hsdmmc, (uint32_t)(sdmmc_ctx->SDCardRelAdd << 16U))) return BSP_ERROR;

    // Get Card Specific Data
    sdmmc_ctx->SDCardSD[0] = SDMMC_GetResponse(hsdmmc, SDMMC_RESP1);
    sdmmc_ctx->SDCardSD[1] = SDMMC_GetResponse(hsdmmc, SDMMC_RESP2);
    sdmmc_ctx->SDCardSD[2] = SDMMC_GetResponse(hsdmmc, SDMMC_RESP3);
    sdmmc_ctx->SDCardSD[3] = SDMMC_GetResponse(hsdmmc, SDMMC_RESP4);

    // Get the Card Class
    sdmmc_ctx->SDCardClass = (SDMMC_GetResponse(hsdmmc, SDMMC_RESP2) >> 20U);

    // Get CSD parameters
    if (_SD_GetCardCSD(hsdmmc, ctx, &CSD)) return BSP_ERROR;

    // Select the Card
    if (SDMMC_CmdSelDesel(hsdmmc, (uint32_t)(((uint32_t)sdmmc_ctx->SDCardRelAdd) << 16U))) return BSP_ERROR;

    return BSP_OK;
}


static uint8_t _SD_SendSDStatus(SDMMC_TypeDef * hsdmmc, TxRxContext_TypeDef * ctx, uint32_t *pSDstatus) {

	SDMMC_DataInitTypeDef config;
	uint32_t tickstart = BSP_GetTick();
	uint32_t count;
	uint32_t *pData = pSDstatus;
	__IO SDMMCContext_TypeDef * sdmmc_ctx = (SDMMCContext_TypeDef *)ctx->ctxmem;

	// Check SD response
	if ((SDMMC_GetResponse(hsdmmc, SDMMC_RESP1) & SDMMC_CARD_LOCKED) == SDMMC_CARD_LOCKED) return BSP_ERROR;

	// Set block size for card if it is not equal to current block size for card
	if (SDMMC_CmdBlockLength(hsdmmc, 64U)) return BSP_ERROR;

	// Send CMD55
	if (SDMMC_CmdAppCommand(hsdmmc, (uint32_t)(sdmmc_ctx->SDCardRelAdd << 16U))) return BSP_ERROR;

	// Configure the SD DPSM (Data Path State Machine)
	config.DataTimeOut   = SDMMC_DATATIMEOUT;
	config.DataLength    = 64U;
	config.DataBlockSize = SDMMC_DATABLOCK_SIZE_64B;
	config.TransferDir   = SDMMC_TRANSFER_DIR_TO_SDMMC;
	config.TransferMode  = SDMMC_TRANSFER_MODE_BLOCK;
	config.DPSM          = SDMMC_DPSM_ENABLE;
	(void)SDMMC_ConfigData(hsdmmc, &config);

	// Send ACMD13 (SD_APP_STAUS)  with argument as card's RCA
	if (SDMMC_CmdStatusRegister(hsdmmc)) return BSP_ERROR;

	// Get status data
	while (!__SDMMC_GET_FLAG(hsdmmc, SDMMC_FLAG_RXOVERR | SDMMC_FLAG_DCRCFAIL | SDMMC_FLAG_DTIMEOUT | SDMMC_FLAG_DATAEND)) {
		if (__SDMMC_GET_FLAG(hsdmmc, SDMMC_FLAG_RXFIFOHF)) {
			for (count = 0U; count < 8U; count++) {
				*pData = SDMMC_ReadFIFO(hsdmmc);
				pData++;
			}
		}
		if ((BSP_GetTick() - tickstart) >=  SDMMC_DATATIMEOUT) return BSP_ERROR;
	}

	if (__SDMMC_GET_FLAG(hsdmmc, (SDMMC_FLAG_DTIMEOUT | SDMMC_FLAG_DCRCFAIL | SDMMC_FLAG_RXOVERR))) return BSP_ERROR;

	while ((__SDMMC_GET_FLAG(hsdmmc, SDMMC_FLAG_DPSMACT))) {
		*pData = SDMMC_ReadFIFO(hsdmmc);
		pData++;
		if ((BSP_GetTick() - tickstart) >=  SDMMC_DATATIMEOUT) return BSP_ERROR;
	}

	// Clear all the static status flags
	__SDMMC_CLEAR_FLAG(hsdmmc, SDMMC_STATIC_FLAGS);

	return BSP_OK;
}


static uint8_t _SD_GetCardStatus(SDMMC_TypeDef * hsdmmc, TxRxContext_TypeDef * ctx, SDMMC_CardStatus_TypeDef * status) {

	uint32_t sd_status[16] = {0};

	if (_SD_SendSDStatus(hsdmmc, ctx, sd_status)) {
		// Clear all the static flags
		__SDMMC_CLEAR_FLAG(hsdmmc, SDMMC_STATIC_FLAGS);
		return BSP_ERROR;
	}

    status->DataBusWidth = (uint8_t)((sd_status[0] & 0xC0U) >> 6U);
    status->SecuredMode = (uint8_t)((sd_status[0] & 0x20U) >> 5U);
    status->CardType = (uint16_t)(((sd_status[0] & 0x00FF0000U) >> 8U) | ((sd_status[0] & 0xFF000000U) >> 24U));
    status->ProtectedAreaSize = (((sd_status[1] & 0xFFU) << 24U)    | ((sd_status[1] & 0xFF00U) << 8U) |
                                ((sd_status[1] & 0xFF0000U) >> 8U) | ((sd_status[1] & 0xFF000000U) >> 24U));
    status->SpeedClass = (uint8_t)(sd_status[2] & 0xFFU);
    status->PerformanceMove = (uint8_t)((sd_status[2] & 0xFF00U) >> 8U);
    status->AllocationUnitSize = (uint8_t)((sd_status[2] & 0xF00000U) >> 20U);
    status->EraseSize = (uint16_t)(((sd_status[2] & 0xFF000000U) >> 16U) | (sd_status[3] & 0xFFU));
    status->EraseTimeout = (uint8_t)((sd_status[3] & 0xFC00U) >> 10U);
    status->EraseOffset = (uint8_t)((sd_status[3] & 0x0300U) >> 8U);
    status->UhsSpeedGrade = (uint8_t)((sd_status[3] & 0x00F0U) >> 4U);
    status->UhsAllocationUnitSize = (uint8_t)(sd_status[3] & 0x000FU) ;
    status->VideoSpeedClass = (uint8_t)((sd_status[4] & 0xFF000000U) >> 24U);

    if (SDMMC_CmdBlockLength(hsdmmc, 512)) {
    	// Clear all the static flags
    	__SDMMC_CLEAR_FLAG(hsdmmc, SDMMC_STATIC_FLAGS);
    	return BSP_ERROR;
    }

    return BSP_OK;
}


static uint8_t _SD_FindSCR(SDMMC_TypeDef * hsdmmc, TxRxContext_TypeDef * ctx, uint32_t *pSCR) {
	SDMMC_DataInitTypeDef config = {0};
	uint32_t tickstart = BSP_GetTick();
	uint32_t index = 0U;
	uint32_t tempscr[2U] = {0UL, 0UL};
	uint32_t *scr = pSCR;

	__IO SDMMCContext_TypeDef * sdmmc_ctx = (SDMMCContext_TypeDef *)ctx->ctxmem;

	// Set Block Size To 8 Bytes
	if (SDMMC_CmdBlockLength(hsdmmc, 8U)) return BSP_ERROR;

	// Send CMD55 APP_CMD with argument as card's RCA
	if (SDMMC_CmdAppCommand(hsdmmc, (uint32_t)((sdmmc_ctx->SDCardRelAdd) << 16U))) return BSP_ERROR;

	config.DataTimeOut   = SDMMC_DATATIMEOUT;
	config.DataLength    = 8U;
	config.DataBlockSize = SDMMC_DATABLOCK_SIZE_8B;
	config.TransferDir   = SDMMC_TRANSFER_DIR_TO_SDMMC;
	config.TransferMode  = SDMMC_TRANSFER_MODE_BLOCK;
	config.DPSM          = SDMMC_DPSM_ENABLE;
	SDMMC_ConfigData(hsdmmc, &config);

	// Send ACMD51 SD_APP_SEND_SCR with argument as 0
	if (SDMMC_CmdSendSCR(hsdmmc)) return BSP_ERROR;

	while (!__SDMMC_GET_FLAG(hsdmmc, SDMMC_FLAG_RXOVERR | SDMMC_FLAG_DCRCFAIL | SDMMC_FLAG_DTIMEOUT | SDMMC_FLAG_DBCKEND | SDMMC_FLAG_DATAEND)) {
		if ((!__SDMMC_GET_FLAG(hsdmmc, SDMMC_FLAG_RXFIFOE)) && (index == 0U)) {
			tempscr[0] = SDMMC_ReadFIFO(hsdmmc);
			tempscr[1] = SDMMC_ReadFIFO(hsdmmc);
			index++;
		}

		if ((BSP_GetTick() - tickstart) >=  SDMMC_DATATIMEOUT) return BSP_ERROR;
	}

	// Checking for error flags
	if (__SDMMC_GET_FLAG(hsdmmc, (SDMMC_FLAG_DTIMEOUT | SDMMC_FLAG_DCRCFAIL | SDMMC_FLAG_RXOVERR))) {
		__SDMMC_CLEAR_FLAG(hsdmmc, (SDMMC_FLAG_DTIMEOUT | SDMMC_FLAG_DCRCFAIL | SDMMC_FLAG_RXOVERR));
		return BSP_ERROR;
	}

    // No error flag set
    // Clear all the static flags
	__SDMMC_CLEAR_FLAG(hsdmmc, SDMMC_STATIC_DATA_FLAGS);

    *scr = (((tempscr[1] & SDMMC_0TO7BITS) << 24)  | ((tempscr[1] & SDMMC_8TO15BITS) << 8) | \
            ((tempscr[1] & SDMMC_16TO23BITS) >> 8) | ((tempscr[1] & SDMMC_24TO31BITS) >> 24));
    scr++;
    *scr = (((tempscr[0] & SDMMC_0TO7BITS) << 24)  | ((tempscr[0] & SDMMC_8TO15BITS) << 8) | \
            ((tempscr[0] & SDMMC_16TO23BITS) >> 8) | ((tempscr[0] & SDMMC_24TO31BITS) >> 24));

    return BSP_OK;
}


static uint8_t _SD_WideBus_Enable(SDMMC_TypeDef * hsdmmc, TxRxContext_TypeDef * ctx) {
	uint32_t scr[2U] = {0UL, 0UL};

	__IO SDMMCContext_TypeDef * sdmmc_ctx = (SDMMCContext_TypeDef *)ctx->ctxmem;

	if ((SDMMC_GetResponse(hsdmmc, SDMMC_RESP1) & SDMMC_CARD_LOCKED) == SDMMC_CARD_LOCKED) return BSP_ERROR;

	// Get SCR Register
	if (_SD_FindSCR(hsdmmc, ctx, scr)) return BSP_ERROR;

	// If requested card supports wide bus operation
	if ((scr[1U] & SDMMC_WIDE_BUS_SUPPORT) == SDMMC_ALLZERO) return BSP_ERROR;

    // Send CMD55 APP_CMD with argument as card's RCA
    if (SDMMC_CmdAppCommand(hsdmmc, (uint32_t)(sdmmc_ctx->SDCardRelAdd << 16U))) return BSP_ERROR;

    // Send ACMD6 APP_CMD with argument as 2 for wide bus mode
    if (SDMMC_CmdBusWidth(hsdmmc, 2U)) return BSP_ERROR;

    return BSP_OK;
}

uint32_t BSP_STM32_SDMMC_GetCardState(SDMMC_TypeDef * hsdmmc, TxRxContext_TypeDef * ctx) {

	__IO SDMMCContext_TypeDef * sdmmc_ctx = (SDMMCContext_TypeDef *)ctx->ctxmem;

	uint32_t resp1 = 0;

	SDMMC_CmdSendStatus(hsdmmc, (uint32_t)(sdmmc_ctx->SDCardRelAdd << 16U));
	resp1 = SDMMC_GetResponse(hsdmmc, SDMMC_RESP1);

	return ((resp1 >> 9U) & 0x0FU);
}



uint8_t BSP_STM32_SDMMC_Init(SDMMC_TypeDef * hsdmmc, TxRxContext_TypeDef * ctx) {

	__IO SDMMCContext_TypeDef * sdmmc_ctx = (SDMMCContext_TypeDef *)ctx->ctxmem;

	SDMMC_CardStatus_TypeDef cardstatus = {0};
	uint32_t speedgrade = 0;
	uint32_t unitsize = 0;

	// 1. Initialize Card parameters

	// Starting up in 1B bus and 400kHz
	SDMMC_InitTypeDef Init = {0};
	Init.ClockEdge        		= SDMMC_CLOCK_EDGE_RISING;
	Init.ClockPowerSave   		= SDMMC_CLOCK_POWER_SAVE_ENABLE;
	Init.BusWide          		= SDMMC_BUS_WIDE_1B;
	Init.HardwareFlowControl 	= SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;

	// For freq to 400kHz div = sdmmc_clk / (2 * 400000) -> For 60MHz div = 75
	Init.ClockDiv 				= 75;

	if (SDMMC_Init(hsdmmc, Init)) return BSP_ERROR;

	// Set Power STate to ON
	if (SDMMC_PowerState_ON(hsdmmc)) return BSP_ERROR;

	// Wait min 74 card cycles - 1ms is more than enough
	BSP_Delay(1);

	// Identify card version, type and voltage
	if (_SD_IdentifyCard(hsdmmc, ctx)) return BSP_ERROR;

	// Card initialization
	if (_SD_InitCard(hsdmmc, ctx)) return BSP_ERROR;

	// Set Block Size for Card (always 512 bytes)
	if (SDMMC_CmdBlockLength(hsdmmc, 512)) return BSP_ERROR;

	// 2. Getting Card Status and obtaining card speed
	if (_SD_GetCardStatus(hsdmmc, ctx, &cardstatus)) return BSP_ERROR;

	speedgrade = cardstatus.UhsSpeedGrade;
	unitsize = cardstatus.UhsAllocationUnitSize;

	sdmmc_ctx->SDCardSpeed = CARD_NORMAL_SPEED;
	if (sdmmc_ctx->SDCardType == CARD_SDHC_SDXC) {
		sdmmc_ctx->SDCardSpeed = CARD_HIGH_SPEED;
		if ((speedgrade != 0U) || (unitsize != 0U)) sdmmc_ctx->SDCardSpeed = CARD_ULTRA_HIGH_SPEED;
	}

	// 3. Configure 4-bit communication bus
	if (sdmmc_ctx->SDCardType == CARD_SECURED) return BSP_ERROR;

	if (_SD_WideBus_Enable(hsdmmc, ctx)) {
		// Clear all the static flags
		__SDMMC_CLEAR_FLAG(hsdmmc, SDMMC_STATIC_FLAGS);
		return BSP_ERROR;
	}

    // Configure the SDMMC peripheral for 4bit bus operation
	Init.ClockEdge        		= SDMMC_CLOCK_EDGE_RISING;
	Init.ClockPowerSave   		= SDMMC_CLOCK_POWER_SAVE_ENABLE;
	Init.BusWide          		= SDMMC_BUS_WIDE_4B;
	Init.HardwareFlowControl 	= SDMMC_HARDWARE_FLOW_CONTROL_ENABLE;

	// Card frequency = 60MHz / (2 * ClockDiv)
	// ClockDiv = 1 -> 30MHz 	= ~14.3MB/s
	// ClockDiv = 2 -> 15MHz 	= ~7.1MB/s
	// ClockDiv = 3 -> 10MHz 	= ~4.7MB/s
	// ClockDiv = 4 -> 7.5MHz 	= ~3.5MB/s

	Init.ClockDiv				= 4;
	if ((sdmmc_ctx->SDCardSpeed == CARD_HIGH_SPEED) || (sdmmc_ctx->SDCardSpeed == CARD_ULTRA_HIGH_SPEED)) Init.ClockDiv = 1;

    if (SDMMC_Init(hsdmmc, Init)) return BSP_ERROR;

    if (SDMMC_CmdBlockLength(hsdmmc, 512)) {
    	// Clear all the static flags
    	__SDMMC_CLEAR_FLAG(hsdmmc, SDMMC_STATIC_FLAGS);
    	return BSP_ERROR;
    }

    // 4. Verify that SD card is ready to use after Initialization
    while (BSP_STM32_SDMMC_GetCardState(hsdmmc, ctx) != SDMMC_CARD_TRANSFER) {};

    return BSP_OK;
}


uint8_t BSP_STM32_SDMMC_ReadBlocks(SDMMC_TypeDef * hsdmmc, TxRxContext_TypeDef * ctx, uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks) {

	__IO SDMMCContext_TypeDef * sdmmc_ctx = (SDMMCContext_TypeDef *)ctx->ctxmem;

	SDMMC_DataInitTypeDef config = {0};
	uint32_t errorstate = 0;
	uint32_t tickstart = BSP_GetTick();
	uint32_t count = 0;
	uint32_t data = 0;
	uint32_t dataremaining = 0;
	uint32_t add = BlockAdd;
	uint8_t *tempbuff = pData;

	if ((add + NumberOfBlocks) > (sdmmc_ctx->SDCardLogBlockNbr)) return BSP_ERROR;

	// Initialize data control register
	hsdmmc->DCTRL = 0U;

	if (sdmmc_ctx->SDCardType != CARD_SDHC_SDXC) add *= 512U;

	// Configure the SD DPSM (Data Path State Machine)
	config.DataTimeOut   = SDMMC_DATATIMEOUT;
	config.DataLength    = NumberOfBlocks * 512U;
	config.DataBlockSize = SDMMC_DATABLOCK_SIZE_512B;
	config.TransferDir   = SDMMC_TRANSFER_DIR_TO_SDMMC;
	config.TransferMode  = SDMMC_TRANSFER_MODE_BLOCK;
	config.DPSM          = SDMMC_DPSM_DISABLE;
	SDMMC_ConfigData(hsdmmc, &config);
	__SDMMC_CMDTRANS_ENABLE(hsdmmc);

	// Read block(s) in polling mode
	if (NumberOfBlocks > 1U) {
		// Read Multi Block command
		errorstate = SDMMC_CmdReadMultiBlock(hsdmmc, add);
	} else {
		// Read Single Block command
		errorstate = SDMMC_CmdReadSingleBlock(hsdmmc, add);
	}

	// Handle error
	if (errorstate) {hsdmmc->ICR = SDMMC_STATIC_FLAGS; return BSP_ERROR; }

	// Poll on SDMMC flags
	dataremaining = config.DataLength;
	while (!(hsdmmc->STA & (SDMMC_FLAG_RXOVERR | SDMMC_FLAG_DCRCFAIL | SDMMC_FLAG_DTIMEOUT | SDMMC_FLAG_DATAEND))) {
		if ((hsdmmc->STA & (SDMMC_FLAG_RXFIFOHF)) && (dataremaining >= 32U)) {
			// Read data from SDMMC Rx FIFO
			for (count = 0U; count < 8U; count++) {
				data = SDMMC_ReadFIFO(hsdmmc);
				*tempbuff = (uint8_t)(data & 0xFFU);
				tempbuff++;
				*tempbuff = (uint8_t)((data >> 8U) & 0xFFU);
				tempbuff++;
				*tempbuff = (uint8_t)((data >> 16U) & 0xFFU);
				tempbuff++;
				*tempbuff = (uint8_t)((data >> 24U) & 0xFFU);
				tempbuff++;
			}
			dataremaining -= 32U;
		}

		// Handle timeout
		if ((BSP_GetTick() - tickstart) >=  1000) {hsdmmc->ICR = SDMMC_STATIC_FLAGS; return BSP_ERROR; }
	}

	__SDMMC_CMDTRANS_DISABLE(hsdmmc);

	// Send stop transmission command in case of multiblock read
	if ((hsdmmc->STA & (SDMMC_FLAG_DATAEND)) && (NumberOfBlocks > 1U)) {
		// Send stop transmission command
		errorstate = SDMMC_CmdStopTransfer(hsdmmc);
		// Handle error
		if (errorstate) {hsdmmc->ICR = SDMMC_STATIC_FLAGS; return BSP_ERROR; }
	}

	// Handle error state
	if (hsdmmc->STA & (SDMMC_FLAG_DTIMEOUT | SDMMC_FLAG_DCRCFAIL | SDMMC_FLAG_RXOVERR)) {
		// Clear all the static flags
		hsdmmc->ICR = SDMMC_STATIC_FLAGS;
		return BSP_ERROR;
	}

	// Clear all the static flags
	hsdmmc->ICR = SDMMC_STATIC_FLAGS;
	return BSP_OK;
}


uint8_t BSP_STM32_SDMMC_WriteBlocks(SDMMC_TypeDef * hsdmmc, TxRxContext_TypeDef * ctx, const uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks) {

	__IO SDMMCContext_TypeDef * sdmmc_ctx = (SDMMCContext_TypeDef *)ctx->ctxmem;

	SDMMC_DataInitTypeDef config = {0};
	uint32_t errorstate = 0;
	uint32_t tickstart = BSP_GetTick();
	uint32_t count = 0;
	uint32_t data = 0;
	uint32_t dataremaining;
	uint32_t add = BlockAdd;
	const uint8_t *tempbuff = pData;


	if ((add + NumberOfBlocks) > (sdmmc_ctx->SDCardLogBlockNbr)) return BSP_ERROR;

    // Initialize data control register
    hsdmmc->DCTRL = 0U;

    if (sdmmc_ctx->SDCardType != CARD_SDHC_SDXC) add *= 512U;

    // Configure the SD DPSM (Data Path State Machine)
    config.DataTimeOut   = SDMMC_DATATIMEOUT;
    config.DataLength    = NumberOfBlocks * SDMMC_BLOCKSIZE;
    config.DataBlockSize = SDMMC_DATABLOCK_SIZE_512B;
    config.TransferDir   = SDMMC_TRANSFER_DIR_TO_CARD;
    config.TransferMode  = SDMMC_TRANSFER_MODE_BLOCK;
    config.DPSM          = SDMMC_DPSM_DISABLE;
    (void)SDMMC_ConfigData(hsdmmc, &config);
    __SDMMC_CMDTRANS_ENABLE(hsdmmc);

    // Write Blocks in Polling mode
	if (NumberOfBlocks > 1U) {
		// Write Multi Block command
		errorstate = SDMMC_CmdWriteMultiBlock(hsdmmc, add);
	} else {
		// Read Single Block command
		errorstate = SDMMC_CmdWriteSingleBlock(hsdmmc, add);
	}

    // Handle error
    if (errorstate) {hsdmmc->ICR = SDMMC_STATIC_FLAGS; return BSP_ERROR; }

    // Write block(s) in polling mode
    dataremaining = config.DataLength;

    while (!(hsdmmc->STA & (SDMMC_FLAG_TXUNDERR | SDMMC_FLAG_DCRCFAIL | SDMMC_FLAG_DTIMEOUT | SDMMC_FLAG_DATAEND))) {
    	if ((hsdmmc->STA & (SDMMC_FLAG_TXFIFOHE)) && (dataremaining >= 32U)) {
    		// Write data to SDMMC Tx FIFO
    		for (count = 0U; count < 8U; count++) {
    			data = (uint32_t)(*tempbuff);
    			tempbuff++;
    			data |= ((uint32_t)(*tempbuff) << 8U);
    			tempbuff++;
    			data |= ((uint32_t)(*tempbuff) << 16U);
    			tempbuff++;
    			data |= ((uint32_t)(*tempbuff) << 24U);
    			tempbuff++;
    			(void)SDMMC_WriteFIFO(hsdmmc, &data);
    		}
    		dataremaining -= 32U;
    	}

		// Handle timeout
		if ((BSP_GetTick() - tickstart) >=  1000) {hsdmmc->ICR = SDMMC_STATIC_FLAGS; return BSP_ERROR; }
    }
    __SDMMC_CMDTRANS_DISABLE(hsdmmc);

    // Send stop transmission command in case of multiblock write
    if ((hsdmmc->STA & (SDMMC_FLAG_DATAEND)) && (NumberOfBlocks > 1U)) {
        // Send stop transmission command
        errorstate = SDMMC_CmdStopTransfer(hsdmmc);
        // Handle error
        if (errorstate) {hsdmmc->ICR = SDMMC_STATIC_FLAGS; return BSP_ERROR; }
    }

	// Handle error state
	if (hsdmmc->STA & (SDMMC_FLAG_DTIMEOUT | SDMMC_FLAG_DCRCFAIL | SDMMC_FLAG_TXUNDERR)) {
		// Clear all the static flags
		hsdmmc->ICR = SDMMC_STATIC_FLAGS;
		return BSP_ERROR;
	}

	// Clear all the static flags
	hsdmmc->ICR = SDMMC_STATIC_FLAGS;
	return BSP_OK;
}
