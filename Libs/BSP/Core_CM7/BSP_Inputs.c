/*******************************************************************
 * MiniConsole V3 - Board Support Package - Keyboard and Joystick
 *
 * Author: Marek Ryn
 * Version: 1.0
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 * - 1.0	- Added calibration functions
 *******************************************************************/

#include "BSP_Inputs.h"

BUF_RAM INPUTS_HandleTypeDef	BSP_hinputs = {0};
static 	TxRxContext_TypeDef		BSP_hinputs_ctx = {0};


static void _Inputs_Calibration(void) {
	switch (BSP_hinputs.mode) {
	case 1:
		// Initialization of calibration data
		BSP_hinputs.joy_cal.x_max = 0x0000;
		BSP_hinputs.joy_cal.x_min = 0xFFFF;
		BSP_hinputs.joy_cal.y_max = 0x0000;
		BSP_hinputs.joy_cal.y_min = 0xFFFF;

		BSP_hinputs.joy_cal.x_center_max = 0x0000;
		BSP_hinputs.joy_cal.x_center_min = 0xFFFF;
		BSP_hinputs.joy_cal.y_center_max = 0x0000;
		BSP_hinputs.joy_cal.y_center_min = 0xFFFF;
		break;
	case 2:
		// Calibrating dead zone
		if (BSP_hinputs.raw_data_joy[1] < BSP_hinputs.joy_cal.x_center_min) BSP_hinputs.joy_cal.x_center_min = BSP_hinputs.raw_data_joy[1];
		if (BSP_hinputs.raw_data_joy[1] > BSP_hinputs.joy_cal.x_center_max) BSP_hinputs.joy_cal.x_center_max = BSP_hinputs.raw_data_joy[1];
		if (BSP_hinputs.raw_data_joy[0] < BSP_hinputs.joy_cal.y_center_min) BSP_hinputs.joy_cal.y_center_min = BSP_hinputs.raw_data_joy[0];
		if (BSP_hinputs.raw_data_joy[0] > BSP_hinputs.joy_cal.y_center_max) BSP_hinputs.joy_cal.y_center_max = BSP_hinputs.raw_data_joy[0];
		break;
	case 3:
		// Calibrating maximum range of joy axes
		if (BSP_hinputs.raw_data_joy[1] < BSP_hinputs.joy_cal.x_min) BSP_hinputs.joy_cal.x_min = BSP_hinputs.raw_data_joy[1];
		if (BSP_hinputs.raw_data_joy[1] > BSP_hinputs.joy_cal.x_max) BSP_hinputs.joy_cal.x_max = BSP_hinputs.raw_data_joy[1];
		if (BSP_hinputs.raw_data_joy[0] < BSP_hinputs.joy_cal.y_min) BSP_hinputs.joy_cal.y_min = BSP_hinputs.raw_data_joy[0];
		if (BSP_hinputs.raw_data_joy[0] > BSP_hinputs.joy_cal.y_max) BSP_hinputs.joy_cal.y_max = BSP_hinputs.raw_data_joy[0];
		break;
	case 4:
		// Do nothing
		break;
	}
}


uint8_t BSP_Inputs_LoadCalData(void) {
	uint32_t reg0 = BSP_STM32_RTC_GetBackupReg(RTC, 0);
	uint32_t reg1 = BSP_STM32_RTC_GetBackupReg(RTC, 1);
	uint32_t reg2 = BSP_STM32_RTC_GetBackupReg(RTC, 2);
	uint32_t reg3 = BSP_STM32_RTC_GetBackupReg(RTC, 3);

	uint16_t x_max = (uint16_t)(reg0 >> 16);
	uint16_t x_min = (uint16_t)(reg0 & 0x0000FFFF);
	uint16_t y_max = (uint16_t)(reg1 >> 16);
	uint16_t y_min = (uint16_t)(reg1 & 0x0000FFFF);

	uint16_t x_centre_max = (uint16_t)(reg2 >> 16);
	uint16_t x_centre_min = (uint16_t)(reg2 & 0x0000FFFF);
	uint16_t y_centre_max = (uint16_t)(reg3 >> 16);
	uint16_t y_centre_min = (uint16_t)(reg3 & 0x0000FFFF);

	uint8_t eflag = 0;


	// Validating loaded values
	if (x_max <= x_centre_max) eflag = 1;
	if (y_max <= y_centre_max) eflag = 1;
	if (x_min >= x_centre_min) eflag = 1;
	if (y_min >= y_centre_min) eflag = 1;
	if (x_centre_min >= y_centre_max) eflag = 1;
	if (y_centre_min >= y_centre_max) eflag = 1;

	if (!eflag) {

		// If data valid, than load configuration
		BSP_hinputs.joy_cal.x_max = x_max;
		BSP_hinputs.joy_cal.x_min = x_min;
		BSP_hinputs.joy_cal.y_max = y_max;
		BSP_hinputs.joy_cal.y_min = y_min;

		BSP_hinputs.joy_cal.x_center_max = x_centre_max;
		BSP_hinputs.joy_cal.x_center_min = x_centre_min;
		BSP_hinputs.joy_cal.y_center_max = y_centre_max;
		BSP_hinputs.joy_cal.y_center_min = y_centre_min;

	} else {

		// If error than load default values
		BSP_hinputs.joy_cal.x_max = 0xFFFF;
		BSP_hinputs.joy_cal.x_min = 0;
		BSP_hinputs.joy_cal.y_max = 0xFFFF;
		BSP_hinputs.joy_cal.y_min = 0;

		BSP_hinputs.joy_cal.x_center_max = 0x9000;
		BSP_hinputs.joy_cal.x_center_min = 0x7000;
		BSP_hinputs.joy_cal.y_center_max = 0x9000;
		BSP_hinputs.joy_cal.y_center_min = 0x7000;

	}

	return BSP_OK;
}


uint8_t BSP_Inputs_SaveCalData(void) {

	BSP_hinputs.mode = 4;

	uint32_t x_max = BSP_hinputs.joy_cal.x_max;
	uint32_t x_min = BSP_hinputs.joy_cal.x_min;
	uint32_t y_max = BSP_hinputs.joy_cal.y_max;
	uint32_t y_min = BSP_hinputs.joy_cal.y_min;

	uint32_t x_centre_max = BSP_hinputs.joy_cal.x_center_max;
	uint32_t x_centre_min = BSP_hinputs.joy_cal.x_center_min;
	uint32_t y_centre_max = BSP_hinputs.joy_cal.y_center_max;
	uint32_t y_centre_min = BSP_hinputs.joy_cal.y_center_min;

	uint8_t eflag = 0;

	// Validating calibration data
	if (x_max <= x_centre_max) eflag = 1;
	if (y_max <= y_centre_max) eflag = 1;
	if (x_min >= x_centre_min) eflag = 1;
	if (y_min >= y_centre_min) eflag = 1;
	if (x_centre_min >= y_centre_max) eflag = 1;
	if (y_centre_min >= y_centre_max) eflag = 1;

	if (eflag) return BSP_ERROR;

	// Saving calibration data
	uint32_t reg0 = (x_max << 16) | x_min;
	uint32_t reg1 = (y_max << 16) | y_min;
	uint32_t reg2 = (x_centre_max << 16) | x_centre_min;
	uint32_t reg3 = (y_centre_max << 16) | y_centre_min;

	BSP_STM32_RTC_SetBackupReg(RTC, 0, reg0);
	BSP_STM32_RTC_SetBackupReg(RTC, 1, reg1);
	BSP_STM32_RTC_SetBackupReg(RTC, 2, reg2);
	BSP_STM32_RTC_SetBackupReg(RTC, 3, reg3);

	BSP_hinputs.mode = 0;

	return BSP_OK;
}


uint8_t BSP_Inputs_Init(void) {
	// Initializing ADC1 for Joystick
	BSP_ADC_Init(ADC1, ADC_RESOLUTION_16B, 2);
	BSP_ADC_ConfigChannel(ADC1, 4, 1);
	BSP_ADC_ConfigChannel(ADC1, 8, 2);
	BSP_STM32_DMA_Init(DMA1_Stream1, DMA_PERIPH_TO_MEMORY, DMA_PINC_DISABLE, DMA_MINC_ENABLE, DMA_PDATAALIGN_HALFWORD, DMA_MDATAALIGN_HALFWORD, DMA_CIRCULAR, DMA_PRIORITY_LOW);
	BSP_STM32_DMA_CtxLink(DMA1_Stream1, &BSP_hinputs_ctx, DMA_REQUEST_ADC1);
	BSP_ADC_StartDMA(ADC1, &BSP_hinputs_ctx, (uint32_t *)BSP_hinputs.raw_data_joy, 2);

	// Normal mode operation
	BSP_hinputs.mode = 0;

	// Init calibration values
	BSP_Inputs_LoadCalData();

	// Configuring TIM2 for parsing joystick and keyboard data
	// Option 1: PWM Frequency = 200MHz Input clock / Prescaler (199+1) / Reload value (9999+1) = 100Hz
	// Option 2: PWM Frequency = 240MHz Input clock / Prescaler (239+1) / Reload value (9999+1) = 100Hz

	BSP_STM32_TIM_Init(TIM2, TIM_CLOCKDIVISION_DIV1, 239, 9999);
	BSP_STM32_TIM_Start(TIM2);
	return BSP_OK;
}


uint8_t BSP_Inputs_ParseData(void) {
	// Button A - PD4
	BSP_hinputs.buttons.btn_A = (BSP_STM32_GPIO_ReadPin(GPIOD, GPIO_PIN_4) == 0);

	// Button B - PD2
	BSP_hinputs.buttons.btn_B = (BSP_STM32_GPIO_ReadPin(GPIOD, GPIO_PIN_2) == 0);

	// Button C - PI8
	BSP_hinputs.buttons.btn_C = (BSP_STM32_GPIO_ReadPin(GPIOI, GPIO_PIN_8) == 0);

	// Button D - PC13
	BSP_hinputs.buttons.btn_D = (BSP_STM32_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == 0);

	// Button XU - PC11
	BSP_hinputs.buttons.btn_X_U = (BSP_STM32_GPIO_ReadPin(GPIOC, GPIO_PIN_11) == 0);

	// Button XD - PG3
	BSP_hinputs.buttons.btn_X_D = (BSP_STM32_GPIO_ReadPin(GPIOG, GPIO_PIN_3) == 0);

	// Button XL - PA9
	BSP_hinputs.buttons.btn_X_L = (BSP_STM32_GPIO_ReadPin(GPIOA, GPIO_PIN_9) == 0);

	// Button XR - PC7
	BSP_hinputs.buttons.btn_X_R = (BSP_STM32_GPIO_ReadPin(GPIOC, GPIO_PIN_7) == 0);

	// Button MENU - PE3
	BSP_hinputs.buttons.btn_MENU = (BSP_STM32_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == 0);

	// Button PWR - PB5
	BSP_hinputs.buttons.btn_PWR = (BSP_STM32_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == 0);

	// Button JOY - PH5
	BSP_hinputs.buttons.btn_JOY = (BSP_STM32_GPIO_ReadPin(GPIOH, GPIO_PIN_5) == 0);

	// Parsing joystick XY axis
	int16_t joy_X = 0;
	int16_t joy_Y = 0;

	if (BSP_hinputs.mode == 0) {

		// Normal mode

		if (BSP_hinputs.raw_data_joy[1] > BSP_hinputs.joy_cal.x_center_max) {
			joy_X = ((BSP_hinputs.raw_data_joy[1] - BSP_hinputs.joy_cal.x_center_max) * 128)/ (BSP_hinputs.joy_cal.x_max - BSP_hinputs.joy_cal.x_center_max);
		}

		if (BSP_hinputs.raw_data_joy[1] < BSP_hinputs.joy_cal.x_center_min) {
			joy_X = ((BSP_hinputs.raw_data_joy[1] - BSP_hinputs.joy_cal.x_center_min) * 128)/ (BSP_hinputs.joy_cal.x_center_min - BSP_hinputs.joy_cal.x_min);
		}

		if (BSP_hinputs.raw_data_joy[0] > BSP_hinputs.joy_cal.y_center_max) {
			joy_Y = ((BSP_hinputs.raw_data_joy[0] - BSP_hinputs.joy_cal.y_center_max) * 128)/ (BSP_hinputs.joy_cal.y_max - BSP_hinputs.joy_cal.y_center_max);
		}

		if (BSP_hinputs.raw_data_joy[0] < BSP_hinputs.joy_cal.y_center_min) {
			joy_Y = ((BSP_hinputs.raw_data_joy[0] - BSP_hinputs.joy_cal.y_center_min) * 128)/ (BSP_hinputs.joy_cal.y_center_min - BSP_hinputs.joy_cal.y_min);
		}

		// Clipping values to <-128, 128> range
		if (joy_X < -128) joy_X = -128;
		if (joy_X > 128) joy_X = 128;
		if (joy_Y < -128) joy_Y = -128;
		if (joy_Y > 128) joy_Y = 128;

		BSP_hinputs.joy.joy_X = -joy_X;
		BSP_hinputs.joy.joy_Y = joy_Y;

	} else {

		// Calibration mode in progress
		_Inputs_Calibration();

	}

	return BSP_OK;
}


uint8_t BSP_Inputs_CalibrateJoyInit(void) {
	BSP_hinputs.mode = 1;
	return BSP_OK;
}


uint8_t BSP_Inputs_CalibrateJoyDeadZone(void) {
	BSP_hinputs.mode = 2;
	return BSP_OK;
}


uint8_t BSP_Inputs_CalibrateJoyRange(void) {
	BSP_hinputs.mode = 3;
	return BSP_OK;
}


uint8_t BSP_Inputs_CancelCallibration(void) {
	BSP_hinputs.mode = 4;
	BSP_Inputs_LoadCalData();
	BSP_hinputs.mode = 0;
	return BSP_OK;
}




void TIM2_IRQHandler(void) {
	// Update event interrupt
	if (TIM2->SR & TIM_SR_UIF) {
		// Reseting interrupt
		TIM2->SR &= ~(TIM_SR_UIF);

		BSP_Inputs_ParseData();
	}
}




