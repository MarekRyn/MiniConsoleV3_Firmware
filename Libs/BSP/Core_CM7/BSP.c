/*******************************************************************
 * MiniConsole V3 - Board Support Package - CM7
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

#define HSEM_ID_0 (0U) // HW semaphore 0

uint8_t BSP_BOARD_Init_CM7() {
	uint32_t timeout;

	// Config MPU
	BSP_STM32_MPU_Init();

	// Enable I-Cache
	SCB_EnableICache();

	// Enable D-Cache
	SCB_EnableDCache();

	// Wait until CPU2 boots and enters in stop mode or timeout
	timeout = 0xFFFF;
	while (((RCC->CR & RCC_CR_D2CKRDY) != RESET) && (timeout-- > 0));
	if ( timeout < 0 ) return BSP_ERROR;

	// Setting NVIC grouping
	BSP_STM32_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	// STM32 Initialization - System Clocks
	if (BSP_STM32_Init_SysClocks()) return BSP_ERROR;

	// When system initialization is finished, Cortex-M7 will release Cortex-M4 by means of HSEM notification
	__BSP_RCC_HSEM_CLK_ENABLE();			// HW semaphore Clock enable
	BSP_STM32_HSEM_FastTake(HSEM_ID_0); 	// Take HSEM
	BSP_STM32_HSEM_FastRelease(HSEM_ID_0);	// Release HSEM in order to notify the CPU2(CM4)
	timeout = 0xFFFF;						// Wait until CPU2 wakes up from stop mode
	while(((RCC->CR & RCC_CR_D2CKRDY) == RESET) && (timeout-- > 0));
	if ( timeout < 0 ) return BSP_ERROR;

	// STM32 Initialization - Peripheral Clocks
	if (BSP_STM32_Init_PeriphClocks()) return BSP_ERROR;

	// STM32 Initialization - GPIO Pins
	if (BSP_STM32_Init_GPIO()) return BSP_ERROR;

	// STM32 Initialization - NVIC - Interrupts
	if (BSP_STM32_Init_NVIC()) return BSP_ERROR;

	// SDRAM Initialization
	if (BSP_SDRAM_Init(120)) return BSP_ERROR;

	// QSPI Initialization
	if (BSP_QSPI_Init()) return BSP_ERROR;

	// Touch Panel Initialization
	if (BSP_LCD_TP_Init()) return BSP_ERROR;

	// IMU Initialization
	if (BSP_IMU_Init()) return BSP_ERROR;

	// Joystick and Keyboard Initialization
	if (BSP_Inputs_Init()) return BSP_ERROR;

	// SDCARD Initialization
	if (BSP_SDCARD_Init()) return BSP_ERROR;

	// USART2 Initialization - communication through debug connection
	if (BSP_Serial_Init()) return BSP_ERROR;

	// Audio Initialization
//	if (BSP_Audio_Init()) return BSP_ERROR;

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

void BSP_Error_Handler(void) {

  __disable_irq();
  while (1) {}

}
