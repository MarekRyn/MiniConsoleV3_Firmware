/*******************************************************************
 * MiniConsole V3 - Board Support Package - Keyboard and Joystick
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 *******************************************************************/

#include "BSP_Inputs.h"

BUF_RAM  INPUTS_HandleTypeDef	BSP_hinputs = {0};
		TxRxContext_TypeDef		BSP_hinputs_ctx = {0};

uint8_t BSP_Inputs_Init() {
	// Initializing ADC1 for Joystick
	BSP_ADC_Init(ADC1, ADC_RESOLUTION_16B, 2);
	BSP_ADC_ConfigChannel(ADC1, 4, 1);
	BSP_ADC_ConfigChannel(ADC1, 8, 2);
	BSP_STM32_DMA_Init(DMA1_Stream1, DMA_PERIPH_TO_MEMORY, DMA_PINC_DISABLE, DMA_MINC_ENABLE, DMA_PDATAALIGN_HALFWORD, DMA_MDATAALIGN_HALFWORD, DMA_CIRCULAR, DMA_PRIORITY_LOW);
	BSP_STM32_DMA_CtxLink(DMA1_Stream1, &BSP_hinputs_ctx, DMA_REQUEST_ADC1);
	BSP_ADC_StartDMA(ADC1, &BSP_hinputs_ctx, (uint32_t *)BSP_hinputs.raw_data_joy, 2);

	// Init calibration values
	BSP_hinputs.joy_cal.x_max = 49600;
	BSP_hinputs.joy_cal.x_min = 7600;
	BSP_hinputs.joy_cal.y_max = 58000;
	BSP_hinputs.joy_cal.y_min = 12300;

	BSP_hinputs.joy_cal.x_centre_max = 33000;
	BSP_hinputs.joy_cal.x_centre_min = 30000;
	BSP_hinputs.joy_cal.y_centre_max = 35500;
	BSP_hinputs.joy_cal.y_centre_min = 31500;


	// Configuring TIM2 for parsing joystick and keyboard data
	// Option 1: PWM Frequency = 200MHz Input clock / Prescaler (199+1) / Reload value (9999+1) = 100Hz
	// Option 2: PWM Frequency = 240MHz Input clock / Prescaler (239+1) / Reload value (9999+1) = 100Hz

	BSP_STM32_TIM_Init(TIM2, TIM_CLOCKDIVISION_DIV1, 239, 9999);
	BSP_STM32_TIM_Start(TIM2);
	return BSP_OK;
}


uint8_t BSP_Inputs_ParseData() {
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


	if (BSP_hinputs.raw_data_joy[1] > BSP_hinputs.joy_cal.x_centre_max) {
		joy_X = ((BSP_hinputs.raw_data_joy[1] - BSP_hinputs.joy_cal.x_centre_max) * 128)/ (BSP_hinputs.joy_cal.x_max - BSP_hinputs.joy_cal.x_centre_max);
	}

	if (BSP_hinputs.raw_data_joy[1] < BSP_hinputs.joy_cal.x_centre_min) {
		joy_X = ((BSP_hinputs.raw_data_joy[1] - BSP_hinputs.joy_cal.x_centre_min) * 128)/ (BSP_hinputs.joy_cal.x_centre_min - BSP_hinputs.joy_cal.x_min);
	}

	if (BSP_hinputs.raw_data_joy[0] > BSP_hinputs.joy_cal.y_centre_max) {
		joy_Y = ((BSP_hinputs.raw_data_joy[0] - BSP_hinputs.joy_cal.y_centre_max) * 128)/ (BSP_hinputs.joy_cal.y_max - BSP_hinputs.joy_cal.y_centre_max);
	}

	if (BSP_hinputs.raw_data_joy[0] < BSP_hinputs.joy_cal.y_centre_min) {
		joy_Y = ((BSP_hinputs.raw_data_joy[0] - BSP_hinputs.joy_cal.y_centre_min) * 128)/ (BSP_hinputs.joy_cal.y_centre_min - BSP_hinputs.joy_cal.y_min);
	}

	BSP_hinputs.joy.joy_X = -joy_X;
	BSP_hinputs.joy.joy_Y = joy_Y;

	return BSP_OK;
}


void TIM2_IRQHandler() {
	// Update event interrupt
	if (TIM2->SR & TIM_SR_UIF) {
		// Reseting interrupt
		TIM2->SR &= ~(TIM_SR_UIF);

		BSP_Inputs_ParseData();
	}
}




