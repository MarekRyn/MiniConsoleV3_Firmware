/*******************************************************************
 * MiniConsole V3 - Board Support Package - STM32 ADC
 *******************************************************************/

#include "BSP_STM32_ADC.h"

/*******************************************************************
* I2C Functions
 *******************************************************************/

uint8_t BSP_ADC_Init(ADC_TypeDef * hadc, uint32_t resolution, uint32_t nb_conversions) {

	uint32_t t_start = BSP_GetTick();
	uint32_t tmpCFGR = 0;

	// Exiting from deep power-down
	if (hadc->CR & ADC_CR_DEEPPWD) hadc->CR &= ~(ADC_CR_DEEPPWD);

	// Starting internal voltage regulator
	hadc->CR |= ADC_CR_ADVREGEN;

	// Waiting for voltage regulator to start
	while ((hadc->ISR & ADC_ISR_LDORDY)==0) if ((BSP_GetTick() - t_start) > 100) return BSP_ERROR;

	// Setting ADC clock prescaler
	ADC_Common_TypeDef * ADCxy_common;
	if ((hadc == ADC1) || (hadc == ADC2)) ADCxy_common = ADC12_COMMON; else ADCxy_common = ADC3_COMMON;
	MODIFY_REG(ADCxy_common->CCR, ADC_CCR_CKMODE | ADC_CCR_PRESC, ADC_CLOCK_ASYNC_DIV1);

	// Enable continuous conversion mode
	tmpCFGR = ADC_CFGR_CONT;

	// Configure conversion overrun
	tmpCFGR |= ADC_OVR_DATA_OVERWRITTEN;

	// Configure ADC resolution
	tmpCFGR |= resolution;

	// Enabling DMA data management
	tmpCFGR |= ADC_CONVERSIONDATA_DMA_CIRCULAR;

	// Update Configuration Register CFGR
	hadc->CFGR = tmpCFGR;

	// Disable ADC oversampling scope on ADC group regular
	CLEAR_BIT(hadc->CFGR2, ADC_CFGR2_ROVSE);

	// Set the LeftShift parameter: it is applied to the final result with or without oversampling
	MODIFY_REG(hadc->CFGR2, ADC_CFGR2_LSHIFT, ADC_LEFTBITSHIFT_NONE);

	// Setting boost:
	// 00: used when ADC clock ≤ 6.25 MHz
	// 01: used when 6.25 MHz < ADC clock frequency ≤ 12.5 MHz
	// 10: used when 12.5 MHz < ADC clock ≤ 25.0 MHz <<<<<<<<< SELECTED (Clock for ADC is 15MHz)
	// 11: used when 25.0 MHz < ADC clock ≤ 50.0 MHz
	MODIFY_REG(hadc->CR, ADC_CR_BOOST, ADC_CR_BOOST_1);

	// Set number of ranks in regular group sequencer
	MODIFY_REG(hadc->SQR1, ADC_SQR1_L, (nb_conversions - (uint8_t)1));

	// Auto-calibration - requesting single-ended calibration
	hadc->CR &= ~(ADC_CR_ADCALDIF);

	// Auto-calibration - requesting calibration of offset and linearity
	hadc->CR |= ADC_CR_ADCALLIN;

	// Starting calibration
	hadc->CR |= ADC_CR_ADCAL;

	// Waiting for completion of calibration
	while (hadc->CR & ADC_CR_ADCAL) if ((BSP_GetTick() - t_start) > 100) return BSP_ERROR;

	// Enabling ADC
	hadc->CR |= ADC_CR_ADEN;

	// Waiting until ADC started
	while ((hadc->ISR & ADC_ISR_ADRDY)==0) if ((BSP_GetTick() - t_start) > 100) return BSP_ERROR;
	// Clearing ADRDY flag
	hadc->ISR &= ~(ADC_ISR_ADRDY);

	return BSP_OK;
}


uint8_t BSP_ADC_ConfigChannel(ADC_TypeDef * hadc, uint32_t channel, uint32_t sequence) {

	if (channel>19) return BSP_ERROR;
	if (sequence == 0) return BSP_ERROR;
	if (sequence > ((hadc->SQR1) & 0x0F)+1) return BSP_ERROR;

	// ADC channels preselection
	hadc->PCSEL |= (1UL << channel);

	// Set ADC group regular sequence: channel on the selected scan sequence rank
	uint8_t sqr_shift = (sequence % 5) * 6;
	uint32_t * sqr_reg = (uint32_t *)&hadc->SQR1;
	sqr_reg += (sequence / 5);
	*sqr_reg |= (channel << sqr_shift);

	// Set sampling time of the selected ADC channel
	uint8_t smpr_shift = (channel % 10) * 3;
	uint32_t * smpr_reg = (uint32_t *)&hadc->SMPR1;
	smpr_reg += (channel / 10);
	*smpr_reg |= (ADC_SAMPLINGTIME_64CYCLES_5 << smpr_shift);

	// For this application all other parameters remains default

	return BSP_OK;
}


uint8_t BSP_ADC_StartDMA(ADC_TypeDef * hadc, TxRxContext_TypeDef *ctx, uint32_t *pData, uint32_t Length) {

	BSP_STM32_DMA_StartIT(ctx, (uint32_t)&hadc->DR, (uint32_t)pData, Length);
	MODIFY_REG(hadc->CR, ADC_CR_BITS_PROPERTY_RS, ADC_CR_ADSTART);
	return BSP_OK;
}
