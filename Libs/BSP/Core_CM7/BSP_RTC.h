/*******************************************************************
 * MiniConsole V3 - Board Support Package - RTC
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 *******************************************************************/

#ifndef BSP_CORE_CM7_BSP_RTC_H_
#define BSP_CORE_CM7_BSP_RTC_H_

#include "BSP_Common.h"
#include "BSP_STM32.h"

uint8_t BSP_RTC_GetTime(uint8_t * phour, uint8_t * pminute, uint8_t * psecond);
uint8_t BSP_RTC_GetDate(uint16_t * pyear, uint8_t * pmonth, uint8_t * pday, uint8_t * pweekday);
int32_t BSP_RTC_GetUnixTimestamp(void);

#endif /* BSP_CORE_CM7_BSP_RTC_H_ */
