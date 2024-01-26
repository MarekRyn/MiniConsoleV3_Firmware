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

#ifndef __BSP_fatfs_H
#define __BSP_fatfs_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "ff.h"
#include "BSP_ff_gen_drv.h"
#include "BSP_sd_diskio.h"

uint8_t BSP_FatFS_Init(char * homeDir);

#ifdef __cplusplus
}
#endif
#endif /*__BSP_fatfs_H */
