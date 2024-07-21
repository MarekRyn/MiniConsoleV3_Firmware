/*******************************************************************
 * MiniConsole V3 - Board Support Package - FatFS I/O driver
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 * - 1.0 	- Initial version
 *******************************************************************/
#include "BSP_fatfs.h"
#include "BSP_STM32_RTC.h"

uint8_t retSD;
char SDPath[4];
FATFS SDFatFS;
FIL SDFile;

static uint8_t homedirflag = 0;

uint8_t BSP_FatFS_Init(void) {
	retSD = FATFS_LinkDriver(&SD_Driver, SDPath);
	if (f_mount(&SDFatFS, "0:/", 1)) return FR_DENIED;
	return FR_OK;
}


uint8_t BSP_SetHomeDir(char * homeDir) {
	// Only one time successful update is allowed
	// home dir must starts with "0:/"
	if (homedirflag) return FR_DENIED;
	if (homeDir[0] != 48) return FR_DENIED;
	if (homeDir[1] != 58) return FR_DENIED;
	if (homeDir[2] != 47) return FR_DENIED;
	if (f_chdir(homeDir)) return FR_DENIED;
	homedirflag = 1;
	return FR_OK;
}


DWORD get_fattime(void) {
	uint16_t year;
	uint8_t month;
	uint8_t	day;
	uint8_t weekday;
	uint8_t	hour;
	uint8_t	minute;
	uint8_t	second;

	BSP_STM32_RTC_GetDate(RTC, &year, &month, &day, &weekday);
	BSP_STM32_RTC_GetTime(RTC, &hour, &minute, &second);

	DWORD fattime = 0;
	fattime |= (((uint32_t)(year - 1980) & 0x7F) << 25);
	fattime |= (((uint32_t)month & 0x0F) << 21);
	fattime |= (((uint32_t)day & 0x1F) << 16);
	fattime |= (((uint32_t)hour & 0x1F) << 11);
	fattime |= (((uint32_t)minute & 0x3F) << 5);
	fattime |= (((uint32_t)second & 0x1F) << 0);

	return 0;
}
