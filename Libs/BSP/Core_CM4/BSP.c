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

#define HSEM_ID_0 (0U) // HW semaphore 0


uint8_t BSP_BOARD_Init() {

	__BSP_RCC_HSEM_CLK_ENABLE();
	// Activate HSEM notification for Cortex-M4
	BSP_STM32_HSEM_ActivateNotification(HSEM_ID_0);

	// Domain D2 goes to STOP mode (Cortex-M4 in deep-sleep)
	BSP_STM32_PWR_ClearPendingEvent();
	BSP_STM32_PWR_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFE, PWR_D2_DOMAIN);

	// Waiting for CM7 to complete initialization
	// ...
	// Domain D2 wakes up from STOP mode

	// Clear HSEM flag
	BSP_STM32_HSEM_ClearFlag(HSEM_ID_0);

	// Configure ART Accelerator
	__BSP_RCC_ART_CLK_ENABLE();                   // Enable the Cortex-M4 ART Clock
	__BSP_ART_CONFIG_BASE_ADDRESS(0x08100000UL);  // Configure the Cortex-M4 ART Base address to the Flash Bank 2
	__BSP_ART_ENABLE();                           // Enable the Cortex-M4 ART

	// Set Interrupt Group Priority
	BSP_STM32_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	// Configure system clocks
	uint32_t common_system_clock = BSP_STM32_RCC_GetSysClockFreq() >> ((D1CorePrescTable[(RCC->D1CFGR & RCC_D1CFGR_D1CPRE)>> RCC_D1CFGR_D1CPRE_Pos]) & 0x1FU);
	SystemD2Clock = (common_system_clock >> ((D1CorePrescTable[(RCC->D1CFGR & RCC_D1CFGR_HPRE)>> RCC_D1CFGR_HPRE_Pos]) & 0x1FU));
	SystemCoreClock = SystemD2Clock;

	BSP_TickInit(SystemCoreClock, 1, TICK_INT_PRIORITY);

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
