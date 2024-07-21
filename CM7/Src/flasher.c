/*
 * flasher.c
 *
 *  Created on: Jul 19, 2024
 *      Author: marek
 */

#include "flasher.h"

enum FL_STATE {FL_IDLE, FL_LOADFILE, FL_ERASEMEMORY, FL_WRITEMEMORY, FL_VERIFY, FL_DEINIT, FL_ERROR};

#define FL_BUFSIZE	1024

typedef struct {

	enum FL_STATE	state;
	char 			path[32];
	uint32_t		progress;
	uint32_t		progress_0;
	uint32_t		progress_1;
	uint32_t		progress_2;
	uint32_t		sec_no;
	uint32_t		sec_i;
	uint32_t		page_no;
	uint32_t		page_i;
	uint32_t		offset;
	uint32_t		crc32_0;
	uint32_t		crc32_1;
	uint8_t			buf[FL_BUFSIZE];

} FlasherCtx_TypeDef;

static FlasherCtx_TypeDef	flctx;



uint8_t	FLASHER_Init(char * fpath) {
	if (flctx.state != FL_IDLE) return BSP_ERROR;
	memset(flctx.path, 0x00, sizeof(flctx.path));
	strcpy(flctx.path, fpath);
	flctx.progress = 0;
	flctx.progress_0 = 0;
	flctx.progress_1 = 0;
	flctx.progress_2 = 0;
	flctx.sec_i = 0;
	flctx.offset = 0;
	flctx.page_i = 0;
	flctx.page_no = 0;
	flctx.crc32_0 = 0;
	flctx.crc32_1 = 0;
	flctx.state = FL_LOADFILE;
	return BSP_OK;
}

void _crc32_0(void) {
	for (uint32_t i = 0; i < FL_BUFSIZE; i++) flctx.crc32_0 += flctx.buf[i];
}

void _crc32_1(void) {
	for (uint32_t i = 0; i < FL_BUFSIZE; i++) flctx.crc32_1 += flctx.buf[i];
}


uint8_t FLASHER_Task(void) {

	char fhex[64];

	switch (flctx.state) {
	case FL_IDLE:
		break;

	case FL_LOADFILE:
		memset(fhex, 0x00, sizeof(fhex));
		strcpy(fhex, flctx.path);
		strcat(fhex, "app.hex");
		if (BSP_IHex_Init(fhex, 0x90000000, 0x91000000)) { flctx.state = FL_ERROR; break; }
		flctx.sec_no = ((BSP_IHex_GetFileSize() >> 1) / QSPI_CFG_SECTOR_SIZE) + 1;
		if (flctx.sec_no > (QSPI_CFG_BLOCK_COUNT * QSPI_CFG_BLOCK_SIZE)) flctx.sec_no = (QSPI_CFG_BLOCK_COUNT * QSPI_CFG_BLOCK_SIZE);
		flctx.state = FL_ERASEMEMORY;
		break;

	case FL_ERASEMEMORY:
		if (BSP_QSPI_SectorErase(flctx.sec_i * QSPI_CFG_SECTOR_SIZE)) { flctx.state = FL_ERROR; break; }
		flctx.sec_i++;
		flctx.progress_0 = (flctx.sec_i * 20) / flctx.sec_no;
		if (flctx.sec_i >= flctx.sec_no) flctx.state = FL_WRITEMEMORY;
		break;

	case FL_WRITEMEMORY:
		BSP_IHex_FillBuf(flctx.buf, FL_BUFSIZE);
		if (BSP_IHex_IsError()) {flctx.state = FL_ERROR; break; }

		if (flctx.page_no == 0) {
			// Updating home directory inside flash payload
			memcpy(&flctx.buf[8], flctx.path, 32);
		}
		if (BSP_QSPI_WriteBuf(flctx.buf, flctx.offset, FL_BUFSIZE)) { flctx.state = FL_ERROR; break; }
		if (BSP_IHex_IsEndOfBlock()) {flctx.state = FL_ERROR; break; }
		_crc32_0();
		flctx.offset+=FL_BUFSIZE;
		flctx.page_no++;
		flctx.progress_1 = BSP_IHex_GetProgress() * 50 / 100;
		if (BSP_IHex_IsEndOfFile()) { flctx.progress_1 = 50; flctx.offset = 0; flctx.state = FL_VERIFY; }
		break;

	case FL_VERIFY:
		BSP_QSPI_ReadBuf(flctx.buf, flctx.offset, FL_BUFSIZE);
		_crc32_1();
		flctx.offset+=FL_BUFSIZE;
		flctx.page_i++;
		flctx.progress_2 = (flctx.page_i * 30) / flctx.page_no;
		if (flctx.page_i >= flctx.page_no) flctx.state = FL_DEINIT;
		break;

	case FL_DEINIT:
		if (BSP_IHex_DeInit()) { flctx.state = FL_ERROR; break; }
		// Checking crc
		if (flctx.crc32_0 != flctx.crc32_1) { flctx.state = FL_ERROR; break; }
		flctx.state = FL_IDLE;
		break;

	case FL_ERROR:
		break;
	}

	flctx.progress = flctx.progress_0 + flctx.progress_1 + flctx.progress_2;

	return BSP_OK;
}


uint8_t	FLASHER_GetProgress(void) {
	return flctx.progress;
}


uint8_t FLASHER_Reset(void) {
	flctx.progress = 0;
	flctx.progress_0 = 0;
	flctx.progress_1 = 0;
	flctx.progress_2 = 0;
	flctx.sec_i = 0;
	flctx.offset = 0;
	flctx.page_i = 0;
	flctx.page_no = 0;
	flctx.crc32_0 = 0;
	flctx.crc32_1 = 0;
	flctx.state = FL_IDLE;

	return BSP_OK;
}

uint8_t FLASHER_IsError(void) {
	return (flctx.state == FL_ERROR);
}
