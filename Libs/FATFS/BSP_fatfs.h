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

#ifndef __BSP_fatfs_H
#define __BSP_fatfs_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "ff.h"
#include "BSP_ff_gen_drv.h"
#include "BSP_sd_diskio.h"


extern uint8_t retSD;
extern char SDPath[4];
extern FATFS SDFatFS;
extern FIL SDFile;

void f_init(void);

#ifdef __cplusplus
}
#endif
#endif /*__BSP_fatfs_H */
