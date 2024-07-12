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

static uint32_t timeout_screen_reduced = 0;
static uint32_t timeout_screen_pwroff = 0;
static uint8_t	ps_state0 = 0;
static uint8_t	ps_state1 = 0;

uint8_t BSP_PWR_Animation(void) {
	uint32_t timestart = BSP_GetTick();
	uint32_t dt = 0;
	BSP_LCD_SetBackLight(0, 25);
	while (dt < 240) {
		dt = (BSP_GetTick() - timestart);
		while (!BSP_LCD_GetEditPermission()) {};
		G2D_CopyPrevFrame();
		int16_t y0 = dt + 2;
		int16_t y1 = 479 - y0;
		G2D_DrawFillRect(0, 0, 800, y0, BSP_LCD_Color(C_BLACK, 255));
		G2D_DrawFillRect(0, y1, 800, y1, BSP_LCD_Color(C_BLACK, 255));
		G2D_DrawHLine(0, y0, 800, BSP_LCD_Color(C_WHITE, 255));
		G2D_DrawHLine(0, y1, 800, BSP_LCD_Color(C_WHITE, 255));
		BSP_LCD_FrameReady();
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
