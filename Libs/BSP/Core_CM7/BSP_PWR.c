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
#include "graph2d.h"

#include <stdlib.h>

static uint32_t timeout_screen_reduced = 0;
static uint32_t timeout_screen_pwroff = 0;
static uint8_t	ps_state0 = 0;
static uint8_t	ps_state1 = 0;


uint8_t BSP_PWR_Animation(void) {
	// Disabling OSD layer
	BSP_LCD_OSD_Hide();

	// Configure OSD Layer to cover whole screen area
	// The layer memory will excess size of initial memory designated for OSD and overflow to graph cache memory,
	// but in this case (closing animation) it does not matter.
	BSP_STM32_LTDC_ConfigLayerWindow(LTDC, 1, 0, LCD_WIDTH, 0, LCD_HEIGHT);
	BSP_LCD_OSD_SetAlpha(255);
	BSP_LCD_OSD_Show();

	// Clear Layer
	G2D_OSD_DrawFillRect(0, 0, LCD_WIDTH, LCD_HEIGHT, BSP_LCD_OSD_Color(C_BLACK, 0));

	// Animation 1
	int16_t y0 = 0;
	int16_t y1 = LCD_HEIGHT - 1;
	uint8_t a = 255 - 240;
	uint8_t v = BSP_Audio_GetMasterVolume();
	while (y0 < 240) {
		G2D_OSD_DrawFillRect(0, 0, LCD_WIDTH, y0, BSP_LCD_OSD_Color(C_BLACK, 255));
		G2D_OSD_DrawFillRect(0, y1, LCD_WIDTH, LCD_HEIGHT - y1, BSP_LCD_OSD_Color(C_BLACK, 255));
		G2D_OSD_DrawFillRect(0, y0, LCD_WIDTH, y1 - y0, BSP_LCD_OSD_Color(C_WHITE, a));
		y0++;
		y1--;
		a++;
		if (v > 0) v--;
		BSP_Audio_SetMasterVolume(v);
		BSP_Delay(1);
	}

	return BSP_OK;
}


uint8_t BSP_PWR_Init(void) {
	// Configuring TIM4 for parsing joystick and keyboard data
	// Option 1: Frequency = 200MHz Input clock / Prescaler (19999+1) / Reload value (4999+1) = 2Hz
	// Option 2: Frequency = 240MHz Input clock / Prescaler (23999+1) / Reload value (4999+1) = 2Hz

	ps_state0 = 0;
	ps_state1 = 0;
	BSP_STM32_TIM_Init(TIM4, TIM_CLOCKDIVISION_DIV1, 23999, 4999);
	BSP_STM32_TIM_Start(TIM4);

	return BSP_OK;
}


uint8_t BSP_PWR_Restart(void) {

	// Power Animation
	BSP_PWR_Animation();

	NVIC_SystemReset();

	while (1) {}
	return BSP_OK;
}


uint8_t BSP_PWR_ShutDown(void) {

	// Power Animation
	BSP_PWR_Animation();

	// Pulling power hold pin to low state
	BSP_STM32_GPIO_WritePin(PWR_Hold_Port, PWR_Hold_Pin, GPIO_PIN_RESET);

	while (1) {}
	return BSP_OK;
}


uint8_t BSP_PWR_ShutDownNoAnim(void) {
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


uint8_t BSP_PWR_GetBatteryState(void) {
	return BSP_STM32_GPIO_ReadPin(PWR_Low_Port, PWR_Low_Pin);
}


// IRQ Handler for TIM2

void TIM4_IRQHandler(void) {
	if (TIM4->SR & TIM_SR_UIF) {
		// Reseting interrupt
		TIM4->SR &= ~(TIM_SR_UIF);

		// Counting power saving timeout
		uint32_t timestamp0 = BSP_GetTick();
		uint32_t timestamp1 = BSP_hlcdtp.gest_data.start_t;
		if (timestamp1 < BSP_hinputs.timestamp) timestamp1 = BSP_hinputs.timestamp;
		if (timestamp1 < BSP_himu.timestamp) timestamp1 = BSP_himu.timestamp;

		ps_state0 = 0;
		if (timeout_screen_reduced > 0) if (timestamp0 > (timestamp1 + timeout_screen_reduced)) ps_state0 = 1;
		if (timeout_screen_pwroff > 0) if (timestamp0 > (timestamp1 + timeout_screen_pwroff)) ps_state0 = 2;

		if (ps_state0 != ps_state1) switch (ps_state0) {
		case 0:
			BSP_LCD_BackLightOn();
			break;
		case 1:
			BSP_LCD_BackLightLo();
			break;
		case 2:
			BSP_LCD_BacklLightOff();
			break;
		}

		ps_state1 = ps_state0;
	}
}
