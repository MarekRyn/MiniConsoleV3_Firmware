/*******************************************************************
 * MiniConsole V3 - Board Support Package - FatFS I/O driver
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 *******************************************************************/
#include "BSP_fatfs.h"

uint8_t retSD;
char SDPath[4];
FATFS SDFatFS;
FIL SDFile;

void f_init(void)
{
  retSD = FATFS_LinkDriver(&SD_Driver, SDPath);
}


DWORD get_fattime(void)
{
  return 0;
}
