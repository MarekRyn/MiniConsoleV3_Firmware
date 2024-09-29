/*******************************************************************
 * MiniConsole V3 - Board Support Package - RTC
 *******************************************************************/

#include "BSP_RTC.h"
#include <time.h>

uint8_t BSP_RTC_GetTime(uint8_t * phour, uint8_t * pminute, uint8_t * psecond) {
	return BSP_STM32_RTC_GetTime(RTC, phour, pminute, psecond);
}

uint8_t BSP_RTC_GetDate(uint16_t * pyear, uint8_t * pmonth, uint8_t * pday, uint8_t * pweekday) {
	return BSP_STM32_RTC_GetDate(RTC, pyear, pmonth, pday, pweekday);
}

int32_t BSP_RTC_GetUnixTimestamp(void) {
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t weekday;

	struct tm time = {0};


	BSP_STM32_RTC_GetDate(RTC, &year, &month, &day, &weekday);
	BSP_STM32_RTC_GetTime(RTC, &hour, &minute, &second);

	time.tm_year = year - 1900;
	time.tm_mon = month - 1;
	time.tm_mday = day;
	time.tm_hour = hour;
	time.tm_min = minute;
	time.tm_sec = second;

	return mktime(&time);
}
