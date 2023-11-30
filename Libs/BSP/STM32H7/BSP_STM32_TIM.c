/*******************************************************************
 * MiniConsole V3 - Board Support Package - STM32 TIM
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 *******************************************************************/

#include "BSP_STM32_TIM.h"



/*******************************************************************
* TIM Functions
 *******************************************************************/

uint8_t BSP_STM32_TIM_Init(TIM_TypeDef *htim, uint8_t clk_division, uint16_t prescaler, uint16_t counter_limit) {
	uint32_t tmp;

	if (clk_division > 3) return BSP_ERROR;

	// Setting CR1 register
	tmp = 0;									// Reset value
	tmp |= 0 << TIM_CR1_CEN_Pos;				// Timer stopped
	tmp |= 0 << TIM_CR1_UDIS_Pos;				// Update event enabled
	tmp |= 0 << TIM_CR1_URS_Pos;				// Update request source
	tmp |= 0 << TIM_CR1_OPM_Pos;				// One pulse mode disabled
	tmp |= 0 << TIM_CR1_DIR_Pos;				// Counting direction UP
	tmp |= 0 << TIM_CR1_CMS_Pos;				// Edge alignment mode
	tmp |= 0 << TIM_CR1_ARPE_Pos;				// Auto-reload preload disabled
	tmp	|= clk_division << TIM_CR1_CKD_Pos;		// Configuring clock division factor
	tmp |= 0 << TIM_CR1_UIFREMAP_Pos;			// No remapping

	htim->CR1 = tmp;

	// Setting CR2 register
	tmp = 0;	// Reset value
	htim->CR2 = tmp;

	// Setting SMCR register
	tmp = 0;	// Reset value
	htim->SMCR = tmp;

	// Setting PSC register (prescaler)
	htim->PSC = prescaler;

	// Setting ARR register (counter limit)
	htim->ARR = counter_limit;

	// Resetting CNT register (counter)
	htim->CNT = 0;

	// Generating update event to reload registers
	htim->EGR = 1 << TIM_EGR_UG_Pos;

	// Enabling update event interrupt
	htim->DIER |= (1 << TIM_DIER_UIE_Pos);

	return BSP_OK;
}

uint8_t BSP_STM32_TIM_ConfigChannel(TIM_TypeDef *htim, uint8_t channel, uint8_t mode, uint16_t init_value) {

	// Mode 0b000 - Frozen
	// Mode 0b001 - Set channel to active on match
	// Mode 0b010 - Set channel to inactive on match
	// Mode 0b011 - Set channel to toggle on match
	// Mode 0b100 - Force channel to inactive level
	// Mode 0b101 - Force channel to active level
	// Mode 0b110 - PWM mode 1
	// Mode 0b111 - PWM mode 2

	channel--;

	if (channel > 3) return BSP_ERROR;
	if (mode > 7) return BSP_ERROR;

	uint8_t ch_shift;
	uint16_t tmp;

	ch_shift = channel << 2;
	tmp = htim->CCER;
	tmp &= ~(0b1011 << ch_shift);	// Resetting channel settings
	tmp |= (0 << (ch_shift + 1));	// Setting positive polarity
	tmp |= (1 << (ch_shift + 0));	// Enable selected channel as output
	htim->CCER = tmp;


	ch_shift = (channel % 2) << 3;
	if (channel < 2) tmp = htim->CCMR1; else tmp = htim->CCMR2;
	tmp &= ~(0b11111111 << ch_shift);	// Resetting channel  setting
	tmp |= (0 << (ch_shift + 0));		// Configure channel as output
	tmp |= (0 << (ch_shift + 2));		// Fast mode disabled
	tmp |= (0 << (ch_shift + 3));		// Preload disabled
	tmp |= ((uint16_t)mode << (ch_shift + 4));	// Setting channel mode
	if (channel < 2) htim->CCMR1 = tmp; else htim->CCMR2 = tmp;

	// Setting value for selected channel
	if (channel == 0) htim->CCR1 = init_value;
	if (channel == 1) htim->CCR2 = init_value;
	if (channel == 2) htim->CCR3 = init_value;
	if (channel == 3) htim->CCR4 = init_value;


	return BSP_OK;
}

uint8_t BSP_STM32_TIM_Start(TIM_TypeDef *htim) {
	htim->CR1 |= 1 << TIM_CR1_CEN_Pos;
	return BSP_OK;
}

uint8_t BSP_STM32_TIM_Stop(TIM_TypeDef *htim) {
	htim->CR1 &= ~(1 << TIM_CR1_CEN_Pos);
	return BSP_OK;
}

uint8_t BSP_STM32_TIM_SetChannelValue(TIM_TypeDef *htim, uint8_t channel, uint16_t value) {
	channel --;
	if (channel > 3) return BSP_ERROR;

	// Setting value for selected channel
	if (channel == 0) htim->CCR1 = value;
	if (channel == 1) htim->CCR2 = value;
	if (channel == 2) htim->CCR3 = value;
	if (channel == 3) htim->CCR4 = value;

	return BSP_OK;
}

uint16_t BSP_STM32_TIM_GetChannelValue(TIM_TypeDef *htim, uint8_t channel) {
	channel --;

	// Setting value for selected channel
	if (channel == 0) return htim->CCR1;
	if (channel == 1) return htim->CCR2;
	if (channel == 2) return htim->CCR3;
	if (channel == 3) return htim->CCR4;

	return 0;
}

