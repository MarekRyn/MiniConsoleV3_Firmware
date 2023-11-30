/*******************************************************************
 * MiniConsole V3 - Board Support Package - STM32 ADC
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 *******************************************************************/

#ifndef STM32H7_BSP_STM32_ADC_H_
#define STM32H7_BSP_STM32_ADC_H_

#include "BSP_Common.h"
#include "BSP_STM32_DMA.h"

#endif /* STM32H7_BSP_STM32_ADC_H_ */

#define ADC_RESOLUTION_16B                 (0x00000000UL)						// ADC resolution 16 bits, On devices STM32H72xx and STM32H73xx, parameter available only on ADC instance: ADC1, ADC2
#define ADC_RESOLUTION_14B                 (ADC_CFGR_RES_0)  					// ADC resolution 14 bits, On devices STM32H72xx and STM32H73xx, parameter available only on ADC instance: ADC1, ADC2
#define ADC_RESOLUTION_12B                 (ADC_CFGR_RES_1)  					// ADC resolution 12 bits
#define ADC_RESOLUTION_10B                 (ADC_CFGR_RES_1 | ADC_CFGR_RES_0)  	// ADC resolution 10 bits

#define ADC_CLOCK_ASYNC_DIV1            (0x00000000UL)                                        			// ADC asynchronous clock without prescaler
#define ADC_CLOCK_ASYNC_DIV2            (ADC_CCR_PRESC_0)                                     			// ADC asynchronous clock with prescaler division by 2
#define ADC_CLOCK_ASYNC_DIV4            (ADC_CCR_PRESC_1)                   							// ADC asynchronous clock with prescaler division by 4
#define ADC_CLOCK_ASYNC_DIV6            (ADC_CCR_PRESC_1 | ADC_CCR_PRESC_0)                			   	// ADC asynchronous clock with prescaler division by 6
#define ADC_CLOCK_ASYNC_DIV8            (ADC_CCR_PRESC_2) 												// ADC asynchronous clock with prescaler division by 8
#define ADC_CLOCK_ASYNC_DIV10           (ADC_CCR_PRESC_2 | ADC_CCR_PRESC_0) 							// ADC asynchronous clock with prescaler division by 10
#define ADC_CLOCK_ASYNC_DIV12           (ADC_CCR_PRESC_2 | ADC_CCR_PRESC_1) 							// ADC asynchronous clock with prescaler division by 12
#define ADC_CLOCK_ASYNC_DIV16           (ADC_CCR_PRESC_2 | ADC_CCR_PRESC_1 | ADC_CCR_PRESC_0) 			// ADC asynchronous clock with prescaler division by 16
#define ADC_CLOCK_ASYNC_DIV32           (ADC_CCR_PRESC_3)                                     			// ADC asynchronous clock with prescaler division by 32
#define ADC_CLOCK_ASYNC_DIV64           (ADC_CCR_PRESC_3 | ADC_CCR_PRESC_0)                   			// ADC asynchronous clock with prescaler division by 64
#define ADC_CLOCK_ASYNC_DIV128          (ADC_CCR_PRESC_3 | ADC_CCR_PRESC_1)                   			// ADC asynchronous clock with prescaler division by 128
#define ADC_CLOCK_ASYNC_DIV256          (ADC_CCR_PRESC_3 | ADC_CCR_PRESC_1 | ADC_CCR_PRESC_0) 			// ADC asynchronous clock with prescaler division by 256

#define ADC_OVR_DATA_PRESERVED      (0x00000000UL)         												// ADC group regular behavior in case of overrun: data preserved
#define ADC_OVR_DATA_OVERWRITTEN    (ADC_CFGR_OVRMOD)      												// ADC group regular behavior in case of overrun: data overwritten

#define ADC_CONVERSIONDATA_DR                  ((uint32_t)0x00000000)                            		// Regular Conversion data stored in DR register only
#define ADC_CONVERSIONDATA_DFSDM               ((uint32_t)ADC_CFGR_DMNGT_1)                      		// DFSDM mode selected
#define ADC_CONVERSIONDATA_DMA_ONESHOT         ((uint32_t)ADC_CFGR_DMNGT_0)                      		// DMA one shot mode selected
#define ADC_CONVERSIONDATA_DMA_CIRCULAR        ((uint32_t)(ADC_CFGR_DMNGT_0 | ADC_CFGR_DMNGT_1)) 		// DMA circular mode selected

#define ADC_LEFTBITSHIFT_NONE  (0x00000000UL)                                                    		// ADC no bit shift left applied on the final ADC conversion data

#define ADC_SAMPLINGTIME_1CYCLE_5       (0x00000000UL)                                              	// Sampling time 1.5 ADC clock cycles
#define ADC_SAMPLINGTIME_2CYCLES_5      (ADC_SMPR2_SMP10_0) 											// Sampling time 2.5 ADC clock cycles
#define ADC_SAMPLINGTIME_8CYCLES_5      (ADC_SMPR2_SMP10_1) 											// Sampling time 8.5 ADC clock cycles
#define ADC_SAMPLINGTIME_16CYCLES_5     (ADC_SMPR2_SMP10_1 | ADC_SMPR2_SMP10_0) 						// Sampling time 16.5 ADC clock cycles
#define ADC_SAMPLINGTIME_32CYCLES_5     (ADC_SMPR2_SMP10_2) 											// Sampling time 32.5 ADC clock cycles
#define ADC_SAMPLINGTIME_64CYCLES_5     (ADC_SMPR2_SMP10_2 | ADC_SMPR2_SMP10_0) 						// Sampling time 64.5 ADC clock cycles
#define ADC_SAMPLINGTIME_387CYCLES_5    (ADC_SMPR2_SMP10_2 | ADC_SMPR2_SMP10_1) 						// Sampling time 387.5 ADC clock cycles
#define ADC_SAMPLINGTIME_810CYCLES_5    (ADC_SMPR2_SMP10_2 | ADC_SMPR2_SMP10_1 | ADC_SMPR2_SMP10_0) 	// Sampling time 810.5 ADC clock cycles

#define ADC_CR_BITS_PROPERTY_RS            (ADC_CR_ADCAL | ADC_CR_JADSTP | ADC_CR_ADSTP | ADC_CR_JADSTART | ADC_CR_ADSTART | ADC_CR_ADDIS | ADC_CR_ADEN) // ADC register CR bits with HW property "rs": Software can read as well as set this bit. Writing '0' has no effect on the bit value

#define ADC_CHANNEL_TO_DECIMAL(__CHANNEL__)	((((__CHANNEL__) & ADC_AWD2CR_AWD2CH) == 0UL) ? (((__CHANNEL__) & ADC_CFGR_AWD1CH) >> (26UL)) : ((uint32_t)POSITION_VAL((__CHANNEL__))))


uint8_t BSP_ADC_Init(ADC_TypeDef * hadc, uint32_t resolution, uint32_t nb_conversions);
uint8_t BSP_ADC_ConfigChannel(ADC_TypeDef * hadc, uint32_t channel, uint32_t sequence);
uint8_t BSP_ADC_StartDMA(ADC_TypeDef * hadc, TxRxContext_TypeDef *ctx, uint32_t *pData, uint32_t Length);
