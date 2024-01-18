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

static uint32_t timeout_screen_reduced = 0;
static uint32_t timeout_screen_pwroff = 0;
static uint8_t	flag_reduced = 0;
static uint8_t	flag_pwroff = 0;


uint8_t BSP_PWR_Init(void) {
	// Configuring TIM4 for parsing joystick and keyboard data
	// Option 1: Frequency = 200MHz Input clock / Prescaler (19999+1) / Reload value (4999+1) = 2Hz
	// Option 2: Frequency = 240MHz Input clock / Prescaler (23999+1) / Reload value (4999+1) = 2Hz

	BSP_STM32_TIM_Init(TIM4, TIM_CLOCKDIVISION_DIV1, 23999, 4999);
	BSP_STM32_TIM_Start(TIM4);
}


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

uint8_t BSP_PWR_SaveConfig(uint8_t screen_reduced_timeout, uint8_t screen_pwroff_timeout) {

	uint32_t tmp = BSP_STM32_RTC_GetBackupReg(RTC, 10);
	tmp &= 0x0000FFFF;
	tmp |= ((uint32_t)screen_reduced_timeout << 16);
	tmp |= ((uint32_t)screen_pwroff_timeout << 24);
	BSP_STM32_RTC_SetBackupReg(RTC, 10, tmp);

	timeout_screen_reduced = screen_reduced_timeout * 60 * 1000;
	timeout_screen_pwroff = screen_pwroff_timeout * 60 * 1000;

	return BSP_OK;
}

uint8_t BSP_PWR_LoadConfig(void) {
	uint32_t tmp = BSP_STM32_RTC_GetBackupReg(RTC, 10);
	timeout_screen_reduced = ((tmp & 0x00FF0000) >> 16) * 60 * 1000;
	timeout_screen_pwroff = ((tmp & 0xFF000000) >> 24) * 60 * 1000;
	return BSP_OK;
}


// IRQ Handler for TIM2

void TIM4_IRQHandler(void) {
	if (TIM4->SR & TIM_SR_UIF) {
		// Reseting interrupt
		TIM4->SR &= ~(TIM_SR_UIF);

		// Counting power saving timeout
		uint32_t timestamp0 = BSP_GetTick();
		uint32_t timestamp1 = BSP_hlcdtp.gest_data.start_t;

		if (timestamp1 > BSP_hinputs.timestamp) timestamp1 = BSP_hinputs.timestamp;

		if (timeout_screen_reduced > 0) {
			if (timestamp0 > (timestamp1 + timeout_screen_reduced)) BSP_LCD_BackLightLo(); else BSP_LCD_BackLightOn();
		}

		if (timeout_screen_pwroff > 0) {
			if (timestamp0 > (timestamp1 + timeout_screen_pwroff)) BSP_LCD_BacklLightOff(); else BSP_LCD_BackLightOn();
		}
	}
}
