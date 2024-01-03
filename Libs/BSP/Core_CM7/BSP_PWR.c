/*******************************************************************
 * MiniConsole V3 - Board Support Package - Power
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 *******************************************************************/

#include "BSP_PWR.h"

uint8_t BSP_PWR_Restart(void) {

	NVIC_SystemReset();

	while (1) {}
	return BSP_OK;
}


uint8_t BSP_PWR_ShutDown(void) {

	// Pulling power hold pin to low state
	BSP_STM32_GPIO_WritePin(PWR_Hold_Port, PWR_Hold_Pin, GPIO_PIN_RESET);

	while (1) {}
	return BSP_OK;
}
