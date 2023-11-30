/*******************************************************************
 * MiniConsole V3 - Board Support Package - Resources
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 *******************************************************************/

#include "BSP_Resources.h"

uint8_t BSP_Res_Init(ResCtx_TypeDef *resctx, uint32_t resAddr, uint32_t resSize) {
	// resaddr - memory address for resource storage
	// ressize - size of memory available for resource storage [bytes]

	FRESULT res;

	// Initiating variables

	resctx->resAddr = resAddr;
	resctx->resSize = resSize;
	resctx->resFree = resSize;
	resctx->resIndex = (resAddr % 4)?(4 - (resAddr % 4)):0;

	// Cleaning reserved resource space

	memset((uint8_t *)resctx->resAddr, 0, resctx->resSize);

	// Cleaning item list
	for (uint16_t id=0; id<RES_ITEM_NUM; id++) {
		resctx->items[id].addr = 0;
		resctx->items[id].size = 0;
	}

	// Mounting File System
	f_init();
	res = f_mount(&resctx->fs, "0:/", 1);
	if (res) return res;

	return FR_OK;
}

uint8_t BSP_Res_Load(ResCtx_TypeDef *resctx, char *filename, uint16_t id) {

	FIL	file;
	FRESULT res;

	// Opening file

	res = f_open(&file, filename, FA_READ);
	if (res) return res;

	uint32_t fsize = f_size(&file);
	uint32_t findex = 0;
	uint32_t fread = 0;

	if (fsize > resctx->resFree) return FR_DENIED;

	// Calculating start address
	uint32_t startaddress = resctx->resAddr + resctx->resIndex;

	// Reading file into SDRAM
	while (findex < fsize) {
		res = f_read(&file, (uint8_t *)(startaddress + findex), 4096, (UINT *)&fread);
		if (res) return res;
		findex += fread;
	}

	// Updating resource item list
	resctx->items[id].addr = startaddress;
	resctx->items[id].size = fsize;
	resctx->resIndex += fsize;

	// Aligning index to next whole 4 bytes;
	resctx->resIndex &= 0xFFFFFFFC;
	resctx->resIndex += 4;
	resctx->resFree = resctx->resSize - resctx->resIndex;

	// Closing file
	f_close(&file);

	return FR_OK;
}


uint32_t BSP_Res_GetAddr(ResCtx_TypeDef *resctx, uint16_t id) {
	return resctx->items[id].addr;
}

uint32_t BSP_Res_GetSize(ResCtx_TypeDef *resctx, uint16_t id) {
	return resctx->items[id].size;
}
