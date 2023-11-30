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


#ifndef __BSP_FF_GEN_DRV_H
#define __BSP_FF_GEN_DRV_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>
#include "ff.h"
#include "BSP_diskio.h"




typedef struct
{
  DSTATUS (*disk_initialize) (BYTE);                     		// Initialize Disk Drive
  DSTATUS (*disk_status)     (BYTE);                     		// Get Disk Status
  DRESULT (*disk_read)       (BYTE, BYTE*, DWORD, UINT);       	// Read Sector(s)
#if FF_FS_READONLY == 0
  DRESULT (*disk_write)      (BYTE, const BYTE*, DWORD, UINT); 	// Write Sector(s) when FF_FS_READONLY = 0
#endif /* FF_FS_READONLY */
  DRESULT (*disk_ioctl)      (BYTE, BYTE, void*);              	// I/O control

}Diskio_drvTypeDef;


typedef struct {
  uint8_t                 is_initialized[FF_VOLUMES];
  const Diskio_drvTypeDef *drv[FF_VOLUMES];
  uint8_t                 lun[FF_VOLUMES];
  volatile uint8_t        nbr;
} Disk_drvTypeDef;


uint8_t FATFS_LinkDriver(const Diskio_drvTypeDef *drv, char *path);
uint8_t FATFS_UnLinkDriver(char *path);
uint8_t FATFS_LinkDriverEx(const Diskio_drvTypeDef *drv, char *path, BYTE lun);
uint8_t FATFS_UnLinkDriverEx(char *path, BYTE lun);
uint8_t FATFS_GetAttachedDriversNbr(void);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_FF_GEN_DRV_H */

