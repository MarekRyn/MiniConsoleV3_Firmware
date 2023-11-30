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

// Include
#include "BSP_diskio.h"
#include "BSP_ff_gen_drv.h"

#if defined ( __GNUC__ )
#ifndef __weak
#define __weak __attribute__((weak))
#endif
#endif

extern Disk_drvTypeDef  disk;

DSTATUS disk_status (BYTE pdrv) {
  DSTATUS stat;

  stat = disk.drv[pdrv]->disk_status(disk.lun[pdrv]);
  return stat;
}


DSTATUS disk_initialize (BYTE pdrv) {
  DSTATUS stat = RES_OK;

  if(disk.is_initialized[pdrv] == 0)
  {
    disk.is_initialized[pdrv] = 1;
    stat = disk.drv[pdrv]->disk_initialize(disk.lun[pdrv]);
  }
  return stat;
}


DRESULT disk_read (BYTE pdrv, BYTE *buff, DWORD sector,	UINT count) {
  DRESULT res;

  res = disk.drv[pdrv]->disk_read(disk.lun[pdrv], buff, sector, count);
  return res;
}


#if FF_FS_READONLY == 0

DRESULT disk_write (BYTE pdrv, const BYTE *buff, DWORD sector, UINT count) {
  DRESULT res;

  res = disk.drv[pdrv]->disk_write(disk.lun[pdrv], buff, sector, count);
  return res;
}

#endif /* FF_FS_READONLY */


DRESULT disk_ioctl (BYTE pdrv, BYTE cmd, void *buff) {
  DRESULT res;

  res = disk.drv[pdrv]->disk_ioctl(disk.lun[pdrv], cmd, buff);
  return res;
}

__weak DWORD get_fattime (void) {
  return 0;
}

