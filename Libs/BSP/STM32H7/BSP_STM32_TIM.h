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

#ifndef STM32H7_BSP_STM32_TIM_H_
#define STM32H7_BSP_STM32_TIM_H_

#include "BSP_Common.h"

#define TIM_CLOCKDIVISION_DIV1             0x00000000U                          // Clock division: tDTS=tCK_INT
#define TIM_CLOCKDIVISION_DIV2             TIM_CR1_CKD_0                        // Clock division: tDTS=2*tCK_INT
#define TIM_CLOCKDIVISION_DIV4             TIM_CR1_CKD_1                        // Clock division: tDTS=4*tCK_INT


uint8_t BSP_STM32_TIM_Init(TIM_TypeDef *htim, uint8_t clk_division, uint16_t prescaler, uint16_t counter_limit);
uint8_t BSP_STM32_TIM_ConfigChannel(TIM_TypeDef *htim, uint8_t channel, uint8_t mode, uint16_t init_value);
uint8_t BSP_STM32_TIM_Start(TIM_TypeDef *htim);
uint8_t BSP_STM32_TIM_Stop(TIM_TypeDef *htim);
uint8_t BSP_STM32_TIM_SetChannelValue(TIM_TypeDef *htim, uint8_t channel, uint16_t value);
uint16_t BSP_STM32_TIM_GetChannelValue(TIM_TypeDef *htim, uint8_t channel);

#endif /* STM32H7_BSP_STM32_TIM_H_ */
