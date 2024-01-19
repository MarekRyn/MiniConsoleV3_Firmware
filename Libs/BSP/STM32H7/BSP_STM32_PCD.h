/*******************************************************************
 * MiniConsole V3 - Board Support Package - STM32 PDC
 *
 * Author: Marek Ryn
 * Version: 1.0
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 * - 1.0	- Initial version
 *******************************************************************/

#ifndef STM32H7_BSP_STM32_PCD_H_
#define STM32H7_BSP_STM32_PCD_H_

#include "BSP_Common.h"


// All initialization of USB peripheral carried out by TinyUSB driver.

uint8_t BSP_STM32_PCD_EnableUSBVoltageDetector();
uint8_t BSP_STM32_PCD_DisableUSBVoltageDetector();

#endif /* STM32H7_BSP_STM32_PCD_H_ */
