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

#include "BSP_sd_diskio.h"
#include "BSP_ff_gen_drv.h"
#include "BSP_SDCARD.h"

#define SD_TIMEOUT 30 * 1000
#define SD_DEFAULT_BLOCK_SIZE 512

static volatile DSTATUS Stat = STA_NOINIT;

static DSTATUS SD_CheckStatus(BYTE lun);
DSTATUS SD_initialize (BYTE);
DSTATUS SD_status (BYTE);
DRESULT SD_read (BYTE, BYTE*, DWORD, UINT);
#if FF_READONLY == 0
DRESULT SD_write (BYTE, const BYTE*, DWORD, UINT);
#endif /* _USE_WRITE == 1 */
DRESULT SD_ioctl (BYTE, BYTE, void*);


const Diskio_drvTypeDef  SD_Driver = {
  SD_initialize,
  SD_status,
  SD_read,
#if  FF_READONLY == 0
  SD_write,
#endif /* _USE_WRITE == 1 */
  SD_ioctl,
};


// Private functions

static DSTATUS SD_CheckStatus(BYTE lun) {
	Stat = STA_NOINIT;

	if (!BSP_SDCARD_GetCardState()) {
		Stat &= ~STA_NOINIT;
	}

	return Stat;
}

DSTATUS SD_initialize(BYTE lun) {
	// NOTE: SDCard initialized separately!!!
	return SD_CheckStatus(lun)?STA_NOINIT:~STA_NOINIT;
}


DSTATUS SD_status(BYTE lun) {
	return SD_CheckStatus(lun);
}


DRESULT SD_read(BYTE lun, BYTE *buff, DWORD sector, UINT count) {
	if(BSP_SDCARD_ReadBlocks((uint8_t *)buff, (uint32_t)sector, (uint32_t)count)) return RES_ERROR;
	return RES_OK;
}


#if FF_READONLY == 0

DRESULT SD_write(BYTE lun, const BYTE *buff, DWORD sector, UINT count) {
	if(BSP_SDCARD_WriteBlocks((uint8_t *)buff, (uint32_t)sector, (uint32_t)count)) return RES_ERROR;
	return RES_OK;
}

#endif /* FF_READONLY == 0 */



DRESULT SD_ioctl(BYTE lun, BYTE cmd, void *buff) {
  DRESULT res = RES_ERROR;

  if (Stat & STA_NOINIT) return RES_NOTRDY;

  switch (cmd) {
  // Make sure that no pending write process
  case CTRL_SYNC :
    res = RES_OK;
    break;

  // Get number of sectors on the disk (DWORD)
  case GET_SECTOR_COUNT :
    *(DWORD*)buff = BSP_SDCARD_GetBlockNo();
    res = RES_OK;
    break;

  // Get R/W sector size (WORD)
  case GET_SECTOR_SIZE :
    *(WORD*)buff = BSP_SDCARD_GetBlockSize();
    res = RES_OK;
    break;

  // Get erase block size in unit of sector (DWORD)
  case GET_BLOCK_SIZE :
    *(DWORD*)buff = BSP_SDCARD_GetBlockSize() / SD_DEFAULT_BLOCK_SIZE;
    res = RES_OK;
    break;

  default:
    res = RES_PARERR;
  }

  return res;
}

