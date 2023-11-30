/*******************************************************************
 * MiniConsole V3 - Board Support Package - STM32 HSEM
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 *******************************************************************/

#include "BSP_STM32_HSEM.h"

uint8_t BSP_STM32_HSEM_Take(uint32_t SemID, uint32_t ProcessID) {
	// Validate input parameters
	if (SemID > 31) return BSP_ERROR;
	if (ProcessID > 255) return BSP_ERROR;

	// 1. Write R register with MasterID, ProcessID and take bit=1
	HSEM->R[SemID] = (ProcessID | HSEM_CR_COREID_CURRENT | HSEM_R_LOCK);

	// 2. Read the R register. Take achieved if MasterID and ProcessID match and take bit set to 1
	if (HSEM->R[SemID] == (ProcessID | HSEM_CR_COREID_CURRENT | HSEM_R_LOCK)) return BSP_OK;
	return BSP_ERROR;
}


uint8_t BSP_STM32_HSEM_FastTake(uint32_t SemID) {
	// Validate input parameters
	if (SemID > 31) return BSP_ERROR;

	// Read the RLR register to take the semaphore
	if (HSEM->RLR[SemID] == (HSEM_CR_COREID_CURRENT | HSEM_RLR_LOCK)) return BSP_OK;
	return BSP_ERROR;
}


uint8_t BSP_STM32_HSEM_IsSemTaken(uint32_t SemID) {
	// Validate input parameters
	if (SemID > 31) return SET;

	// Check R register for R_LOCK bit
    return ((HSEM->R[SemID] & HSEM_R_LOCK) ? SET : RESET);
}


uint8_t BSP_STM32_HSEM_Release(uint32_t SemID, uint32_t ProcessID) {
	// Validate input parameters
	if (SemID > 31) return BSP_ERROR;
	if (ProcessID > 255) return BSP_ERROR;

	// Clear the semaphore by writing to the R register: the MasterID, the ProcessID and take bit = 0
	HSEM->R[SemID] = (ProcessID | HSEM_CR_COREID_CURRENT);
	return BSP_OK;
}

uint8_t BSP_STM32_HSEM_FastRelease(uint32_t SemID) {
	// Validate input parameters
	if (SemID > 31) return BSP_ERROR;

	// Clear the semaphore by writing to the R register: the MasterID, the ProcessID = 0 and take bit = 0
	HSEM->R[SemID] = (HSEM_CR_COREID_CURRENT);
	return BSP_OK;
}

uint8_t BSP_STM32_HSEM_ActivateNotification(uint32_t SemID) {
	// Validate input parameters
	if (SemID > 31) return BSP_ERROR;

	HSEM_COMMON->IER |= (1 << SemID);
	return BSP_OK;
}

uint8_t BSP_STM32_HSEM_DeactivateNotification(uint32_t SemID) {
	// Validate input parameters
	if (SemID > 31) return BSP_ERROR;

	HSEM_COMMON->IER &= ~(1 << SemID);
	return BSP_OK;
}

uint8_t BSP_STM32_HSEM_ClearFlag(uint32_t SemID) {
	// Validate input parameters
	if (SemID > 31) return BSP_ERROR;

	HSEM_COMMON->ICR |= (1 << SemID);
	return BSP_OK;
}
