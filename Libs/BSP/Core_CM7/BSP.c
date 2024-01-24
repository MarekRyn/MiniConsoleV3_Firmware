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

uint8_t BSP_BOARD_Init_0(void) {

	// Config MPU
	BSP_STM32_MPU_Init();

	// Enable I-Cache
	SCB_EnableICache();

	// Enable D-Cache
	SCB_EnableDCache();

	// Wait until CPU2 boots and enters in stop mode or timeout
	if (BSP_WaitForCM4StopMode()) return BSP_ERROR;

	// Setting NVIC grouping
	BSP_STM32_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	// STM32 Initialization - System Clocks
	if (BSP_STM32_Init_SysClocks()) return BSP_ERROR;

	// Enable SYSCFG Clock
	__BSP_RCC_SYSCFG_CLK_ENABLE();

	// Initializing GPIOB i GPIOG clocks
	__BSP_RCC_GPIOB_CLK_ENABLE();
	__BSP_RCC_GPIOG_CLK_ENABLE();

	// Initialization PWR pins (for PWR_BUTTON and PWR_HOLD)
    BSP_STM32_GPIO_WritePin(GPIOG, GPIO_PIN_9, GPIO_PIN_SET);
    BSP_STM32_GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_INPUT, GPIO_NOPULL, 0, 0);
    BSP_STM32_GPIO_Init(GPIOG, GPIO_PIN_9, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, 0);

	return BSP_OK;
}


uint8_t BSP_BOARD_Init_1(void) {
	// STM32 Initialization - Peripheral Clocks
	if (BSP_STM32_Init_PeriphClocks()) return BSP_ERROR;

	// STM32 Initialization - GPIO Pins
	if (BSP_STM32_Init_GPIO()) return BSP_ERROR;

	// SDRAM Initialization
	if (BSP_SDRAM_Init(120)) return BSP_ERROR;

	// QSPI Initialization
	if (BSP_QSPI_Init()) return BSP_ERROR;

	// PWR Initialization
	if (BSP_PWR_Init()) return BSP_ERROR;

	// STM32 Initialization - NVIC - Interrupts
	if (BSP_STM32_Init_NVIC()) return BSP_ERROR;

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

	// Wake up core CM4 (Audio System)
	if (BSP_WakeUpCM4()) return BSP_ERROR;

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

void BSP_Error_Handler(void) {

  __disable_irq();
  while (1) {}

}

/*******************************************************************
* INT HANDLERS FOR GPIO GROUP EXTIS shared between subsystems
 *******************************************************************/

// INT Handlers for Touch Panel, MPU6050


__weak void EXTI13_IRQHandler(void) {
	return;
}


__weak void EXTI11_IRQHandler(void) {
	return;
}


void EXTI15_10_IRQHandler(void) {
	if (EXTI->PR1 & GPIO_PIN_13) {
		EXTI->PR1 |= ~(GPIO_PIN_13);
		// Call to Touch Panel INT handler
		EXTI13_IRQHandler();
	}

	if (EXTI->PR1 & GPIO_PIN_11) {
		EXTI->PR1 |= ~(GPIO_PIN_11);
		// Call to IMU INT handler
		EXTI11_IRQHandler();
	}
}

// INT Handler for BLE Module

void EXTI9_5_IRQHandler(void)
{
	if (EXTI->PR1 & GPIO_PIN_6) {
		EXTI->PR1 |= ~(GPIO_PIN_6);
		// Call to BLE module INT handler
		BSP_BLE_INTHandler();
	}
}
