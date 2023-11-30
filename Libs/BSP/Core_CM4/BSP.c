/*******************************************************************
 * MiniConsole V3 - Board Support Package - CM4
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 *******************************************************************/

// Includes

#include "BSP.h"

uint8_t BSP_BOARD_Init_CM4() {

	// Config MPU
	BSP_STM32_MPU_Init();

	// STM32 Initialization - RCC
	if (BSP_STM32_Init_PeriphClocks()) return BSP_ERROR;

	// STM32 Initialization - GPIO Pins
	if (BSP_STM32_Init_GPIO()) return BSP_ERROR;

	// STM32 Initialization - NVIC - Interrupts
	if (BSP_STM32_Init_NVIC()) return BSP_ERROR;

	// Audio Initialization
	if (BSP_Audio_Init()) return BSP_ERROR;

	return BSP_OK;
}

// Configuring Basic Interrupts

void NMI_Handler(void) {

  while (1) {}

}

void HardFault_Handler(void) {

  while (1) {}

}

void MemManage_Handler(void) {

  while (1) {}

}

void BusFault_Handler(void) {

  while (1) {}

}

void UsageFault_Handler(void) {

  while (1) {}

}

void SVC_Handler(void) {

}

void DebugMon_Handler(void) {

}

void PendSV_Handler(void) {

}

void SysTick_Handler(void) {

  BSP_IncTick();

}
