/*******************************************************************
 * MiniConsole V3 - Board Support Package - STM32 RTC
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 *******************************************************************/

#ifndef STM32H7_BSP_STM32_RTC_H_
#define STM32H7_BSP_STM32_RTC_H_

#include "BSP_Common.h"
#include "BSP_STM32.h"

#define RTC_TIMEOUT_VALUE					500

#define RTC_ISR_RESERVED_MASK               0x0003FFFFu
#define RTC_RSF_MASK                        (~(RTC_ISR_INIT | RTC_ISR_RSF))
#define RTC_INIT_MASK           			0xFFFFFFFFu
#define RTC_TR_RESERVED_MASK    			(RTC_TR_PM  | RTC_TR_HT | RTC_TR_HU | RTC_TR_MNT | RTC_TR_MNU| RTC_TR_ST | RTC_TR_SU)
#define RTC_DR_RESERVED_MASK    			(RTC_DR_YT | RTC_DR_YU | RTC_DR_WDU | RTC_DR_MT | RTC_DR_MU | RTC_DR_DT  | RTC_DR_DU)

#define RTC_HOURFORMAT_24             		0x00000000u
#define RTC_HOURFORMAT_12              		RTC_CR_FMT

#define RTC_OUTPUT_DISABLE             		0x00000000u
#define RTC_OUTPUT_ALARMA              		RTC_CR_OSEL_0
#define RTC_OUTPUT_ALARMB              		RTC_CR_OSEL_1
#define RTC_OUTPUT_WAKEUP              		RTC_CR_OSEL

#define RTC_OUTPUT_TYPE_PUSHPULL       		RTC_OR_ALARMOUTTYPE
#define RTC_OUTPUT_TYPE_OPENDRAIN      		0x00000000u

#define RTC_OUTPUT_REMAP_NONE          		0x00000000u
#define RTC_OUTPUT_REMAP_POS1          		RTC_OR_OUT_RMP

#define RTC_OUTPUT_POLARITY_HIGH       		0x00000000u
#define RTC_OUTPUT_POLARITY_LOW        		RTC_CR_POL

#define RTC_DAYLIGHTSAVING_SUB1H       		RTC_CR_SUB1H
#define RTC_DAYLIGHTSAVING_ADD1H       		RTC_CR_ADD1H
#define RTC_DAYLIGHTSAVING_NONE        		0x00000000u

#define RTC_STOREOPERATION_RESET        	0x00000000u
#define RTC_STOREOPERATION_SET          	RTC_CR_BKP


uint8_t BSP_STM32_RTC_Init(RTC_TypeDef * hrtc, uint32_t AsynchPrediv, uint32_t SynchPrediv);

uint8_t BSP_STM32_RTC_SetTime(RTC_TypeDef * hrtc, uint8_t hour, uint8_t minute, uint8_t second);
uint8_t BSP_STM32_RTC_SetDate(RTC_TypeDef * hrtc, uint16_t year, uint8_t month, uint8_t day);

uint8_t BSP_STM32_RTC_GetTime(RTC_TypeDef * hrtc, uint8_t * phour, uint8_t * pminute, uint8_t * psecond);
uint8_t BSP_STM32_RTC_GetDate(RTC_TypeDef * hrtc, uint16_t * pyear, uint8_t * pmonth, uint8_t * pday, uint8_t * pweekday);

uint8_t BSP_STM32_RTC_SetBackupReg(RTC_TypeDef * hrtc, uint8_t reg, uint32_t value);
uint32_t BSP_STM32_RTC_GetBackupReg(RTC_TypeDef * hrtc, uint8_t reg);

#endif /* STM32H7_BSP_STM32_RTC_H_ */
