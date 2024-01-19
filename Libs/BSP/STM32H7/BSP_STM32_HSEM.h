/*******************************************************************
 * MiniConsole V3 - Board Support Package - STM32 HSEM
 *
 * Author: Marek Ryn
 * Version: 1.0
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 * - 1.0	- Initial version
 *******************************************************************/

#ifndef STM32H7_BSP_STM32_HSEM_H_
#define STM32H7_BSP_STM32_HSEM_H_

#include "BSP_Common.h"

uint8_t BSP_STM32_HSEM_Take(uint32_t SemID, uint32_t ProcessID);
uint8_t BSP_STM32_HSEM_FastTake(uint32_t SemID);
uint8_t BSP_STM32_HSEM_IsSemTaken(uint32_t SemID);
uint8_t BSP_STM32_HSEM_Release(uint32_t SemID, uint32_t ProcessID);
uint8_t BSP_STM32_HSEM_FastRelease(uint32_t SemID);
uint8_t BSP_STM32_HSEM_ActivateNotification(uint32_t SemID);
uint8_t BSP_STM32_HSEM_DeactivateNotification(uint32_t SemID);
uint8_t BSP_STM32_HSEM_ClearFlag(uint32_t SemID);

#endif /* STM32H7_BSP_STM32_HSEM_H_ */
