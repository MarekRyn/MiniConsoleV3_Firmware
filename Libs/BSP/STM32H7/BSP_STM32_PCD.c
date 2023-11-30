/*******************************************************************
 * MiniConsole V3 - Board Support Package - STM32 PDC
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 *******************************************************************/

#include "BSP_STM32_PCD.h"

uint8_t BSP_STM32_PCD_EnableUSBVoltageDetector() {
  // Enable the USB voltage detector
  SET_BIT (PWR->CR3, PWR_CR3_USB33DEN);
  return BSP_OK;
}

uint8_t BSP_STM32_PCD_DisableUSBVoltageDetector() {
	// Enable the USB voltage detector
	CLEAR_BIT (PWR->CR3, PWR_CR3_USB33DEN);
	return BSP_OK;
}
