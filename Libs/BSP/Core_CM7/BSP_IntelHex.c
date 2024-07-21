/*
 * BSP_IntelHex.c
 *
 *  Created on: Jul 11, 2024
 *      Author: marek
 */

#include "BSP_IntelHex.h"

enum IH_STATE {IH_SEEKLINE, IH_BUFSYNC0, IH_BUFSYNC1, IH_READHEADER, IH_DATA, IH_EOF, IH_EOB, IH_ADDR, IH_ENTRY, IH_CRC, IH_ERROR};

typedef struct {
	uint8_t			ByteCount;
	uint16_t		Offset;
	uint8_t			Type;
	uint8_t			CRC8;
	uint8_t			Data[256];
} IHexLine_TypeDef;


typedef struct {
	FIL						file;
	uint8_t					progress;
	enum IH_STATE			state;
	uint32_t				addr0;
	uint32_t				addr0_end;
	uint32_t				addr1;
	uint32_t				i0;
	uint32_t				i1;
	uint32_t				entrypoint;
	IHexLine_TypeDef		line;
} IHexCtx_TypeDef;

static IHexCtx_TypeDef IHexCtx = {0};


static uint32_t _Hex(uint8_t hex) {
	uint32_t b = 0;
	if (hex >= '0') b = hex - '0';
	if (hex >= 'A') b = (hex - 'A') + 10;
	if (hex >= 'a') b = (hex - 'a') + 10;
	b %= 16;
	return b;
}

static uint8_t _BufSum(void * buf, uint8_t size) {
	uint8_t sum = 0;
	uint8_t * pbyte = (uint8_t *)buf;
	while (size > 0) {
		sum += ((size % 2) ? (_Hex(*pbyte)) : (_Hex(*pbyte) << 4));
		pbyte++;
		size--;
	}
	return sum;
}

static uint8_t _SeekLine(void) {

	uint8_t bytebuf;
	uint32_t readsize;

	FRESULT res = {0};

	while (1) {
		res = f_read(&IHexCtx.file, &bytebuf, 1, &readsize);
		if (res) return BSP_ERROR;
		if (readsize != 1) return BSP_ERROR;
		if (bytebuf == ':') return BSP_OK;
	}

	return BSP_ERROR;
}

static uint8_t _ReadLineHeader(void) {

	uint8_t	bytebuf[8];
	FRESULT res = {0};
	uint32_t readsize;

	IHexCtx.line.Type = 0xFF;
	IHexCtx.line.CRC8 = 0x00;

	res = f_read(&IHexCtx.file, &bytebuf, 8, &readsize);
	if (res) return BSP_ERROR;
	if (readsize != 8) return BSP_ERROR;

	IHexCtx.line.ByteCount = (_Hex(bytebuf[0]) << 4) + _Hex(bytebuf[1]);
	IHexCtx.line.Offset = (_Hex(bytebuf[2]) << 12) + (_Hex(bytebuf[3]) << 8) + (_Hex(bytebuf[4]) << 4) + _Hex(bytebuf[5]);
	IHexCtx.line.Type = (_Hex(bytebuf[6]) << 4) + _Hex(bytebuf[7]);
	IHexCtx.line.CRC8 += _BufSum(bytebuf, 8);

	return BSP_OK;
}


static uint8_t _ReadAddr(void) {

	uint8_t	bytebuf[4];
	FRESULT res = {0};
	uint32_t readsize;

	if (IHexCtx.line.ByteCount != 2) return BSP_ERROR;
	if (IHexCtx.line.Type != 0x04) return BSP_ERROR;
	if (IHexCtx.line.Offset != 0) return BSP_ERROR;

	res = f_read(&IHexCtx.file, &bytebuf, 4, &readsize);
	if (res) return BSP_ERROR;
	if (readsize != 4) return BSP_ERROR;

	IHexCtx.addr1 = (_Hex(bytebuf[0]) << 28) + (_Hex(bytebuf[1]) << 24) + (_Hex(bytebuf[2]) << 20) + (_Hex(bytebuf[3]) << 16);
	IHexCtx.line.CRC8 += _BufSum(bytebuf, 4);

	return BSP_OK;
}


static uint8_t _ReadEntry(void) {

	uint8_t	bytebuf[8];
	FRESULT res = {0};
	uint32_t readsize;

	if (IHexCtx.line.ByteCount != 4) return BSP_ERROR;
	if (IHexCtx.line.Type != 0x05) return BSP_ERROR;
	if (IHexCtx.line.Offset != 0) return BSP_ERROR;

	res = f_read(&IHexCtx.file, &bytebuf, 8, &readsize);
	if (res) return BSP_ERROR;
	if (readsize != 8) return BSP_ERROR;

	IHexCtx.entrypoint = (_Hex(bytebuf[0]) << 28) + (_Hex(bytebuf[1]) << 24) + (_Hex(bytebuf[2]) << 20) + (_Hex(bytebuf[3]) << 16);
	IHexCtx.entrypoint += (_Hex(bytebuf[4]) << 12) + (_Hex(bytebuf[5]) << 8) + (_Hex(bytebuf[6]) << 4) + _Hex(bytebuf[7]);
	IHexCtx.line.CRC8 += _BufSum(bytebuf, 8);

	return BSP_OK;
}


static uint8_t _ReadCRC(void) {

	uint8_t bytebuf[2];
	FRESULT res = {0};
	uint32_t readsize;

	res = f_read(&IHexCtx.file, bytebuf, 2, &readsize);
	if (res) return BSP_ERROR;
	if (readsize != 2) return BSP_ERROR;

	IHexCtx.line.CRC8 += _BufSum(bytebuf, 2);

	return BSP_OK;
}


static uint8_t _ReadData(void) {

	uint8_t bytebuf[512];
	FRESULT res = {0};
	uint32_t readsize;

	if (IHexCtx.line.ByteCount == 0) return BSP_ERROR;
	if (IHexCtx.line.Type != 0x00) return BSP_ERROR;

	res = f_read(&IHexCtx.file, bytebuf, (IHexCtx.line.ByteCount << 1), &readsize);
	if (res) return BSP_ERROR;
	if (readsize != (IHexCtx.line.ByteCount << 1)) return BSP_ERROR;

	uint32_t i = 0;
	while (i < (IHexCtx.line.ByteCount)) {
		IHexCtx.line.Data[i] = (_Hex(bytebuf[(i << 1)]) << 4) + _Hex(bytebuf[(i << 1) + 1]);
		i+=1;
	}

	IHexCtx.line.CRC8 += _BufSum(bytebuf, (IHexCtx.line.ByteCount << 1));

	return BSP_OK;
}


uint8_t BSP_IHex_Init(char *filename, uint32_t addr_start, uint32_t addr_end) {

	FRESULT res = {0};

	// Opening file

	res = f_open(&IHexCtx.file, filename, FA_READ);
	if (res) return BSP_ERROR;

	IHexCtx.progress = 0;
	IHexCtx.state = IH_SEEKLINE;
	IHexCtx.addr0 = addr_start;
	IHexCtx.addr0_end = addr_end;
	IHexCtx.addr1 = 0;
	IHexCtx.entrypoint = 0;
	IHexCtx.i0 = 0;
	IHexCtx.i1 = 0;

	return BSP_OK;
}

uint8_t BSP_IHex_FillBuf(void * pbuf, uint32_t bufsize) {

	uint8_t * pbuf0 = (uint8_t *)pbuf;

	IHexCtx.i0 = 0;

	memset(pbuf, 0, bufsize);

	while (1) {
		switch (IHexCtx.state) {

		case IH_SEEKLINE:
			if (_SeekLine()) { IHexCtx.state = IH_ERROR; break; }
			IHexCtx.state = IH_READHEADER;
			break;

		case IH_READHEADER:
			if (_ReadLineHeader()) { IHexCtx.state = IH_ERROR; break; }
			IHexCtx.state = IH_ERROR;
			if (IHexCtx.line.Type == 0x00)	IHexCtx.state = IH_BUFSYNC0;
			if (IHexCtx.line.Type == 0x01)	IHexCtx.state = IH_EOF;
			if (IHexCtx.line.Type == 0x04)	IHexCtx.state = IH_ADDR;
			if (IHexCtx.line.Type == 0x05)	IHexCtx.state = IH_ENTRY;
			break;

		case IH_BUFSYNC0:
			// Updating addr1 with offset
			IHexCtx.addr1 &= 0xFFFF0000;
			IHexCtx.addr1 += IHexCtx.line.Offset;

			if (IHexCtx.addr0 < IHexCtx.addr1) {
				*pbuf0 = 0x00;
				pbuf0++;
				IHexCtx.i0++;
				IHexCtx.addr0++;
			}

			if (IHexCtx.addr0 == IHexCtx.addr0_end) { IHexCtx.state = IH_EOB; break; }
			if (IHexCtx.addr0 >= IHexCtx.addr1) IHexCtx.state = IH_DATA;

			break;

		case IH_DATA:
			if (_ReadData()) { IHexCtx.state = IH_ERROR; break; }
			IHexCtx.i1 = 0;
			IHexCtx.state = IH_CRC;
			break;

		case IH_BUFSYNC1:
			if (IHexCtx.addr0 > IHexCtx.addr1) {
				IHexCtx.i1++;
				IHexCtx.addr1++;
			}
			if (IHexCtx.i1 == IHexCtx.line.ByteCount) { IHexCtx.state = IH_SEEKLINE; break; }
			if (IHexCtx.addr0 == IHexCtx.addr1) {
				*pbuf0 = IHexCtx.line.Data[IHexCtx.i1];
				pbuf0++;
				IHexCtx.i0++;
				IHexCtx.i1++;
				IHexCtx.addr0++;
				IHexCtx.addr1++;
			}
			if (IHexCtx.addr0 == IHexCtx.addr0_end) { IHexCtx.state = IH_EOB; break; }
			break;

		case IH_ADDR:
			if (_ReadAddr()) { IHexCtx.state = IH_ERROR; break; }
			IHexCtx.state = IH_CRC;
			break;

		case IH_ENTRY:
			if (_ReadEntry()) { IHexCtx.state = IH_ERROR; break; }
			IHexCtx.state = IH_CRC;
			break;

		case IH_CRC:
			if (_ReadCRC()) { IHexCtx.state = IH_ERROR; break; }
			IHexCtx.state = IH_SEEKLINE;
			if (IHexCtx.line.Type == 0x00) IHexCtx.state = IH_BUFSYNC1;
			if (IHexCtx.line.CRC8 != 0x00) IHexCtx.state = IH_ERROR;
			break;

		case IH_EOF:
			if (_ReadCRC()) { IHexCtx.state = IH_ERROR; break; }
			IHexCtx.progress = (f_tell(&IHexCtx.file) * 100) / f_size(&IHexCtx.file);
			return BSP_OK;

		case IH_EOB:
			IHexCtx.progress = (f_tell(&IHexCtx.file) * 100) / f_size(&IHexCtx.file);
			return BSP_OK;

		case IH_ERROR:
			return BSP_ERROR;
		}

		if (IHexCtx.i0 == bufsize) {
			IHexCtx.progress = (f_tell(&IHexCtx.file) * 100) / f_size(&IHexCtx.file);
			return BSP_OK;
		}

	}

	IHexCtx.progress = (f_tell(&IHexCtx.file) * 100) / f_size(&IHexCtx.file);
	return BSP_OK;
}


uint8_t BSP_IHex_DeInit(void) {

	FRESULT res = {0};

	// Closing file

	res = f_close(&IHexCtx.file);
	if (res) return BSP_ERROR;
	return BSP_OK;
}


uint8_t	BSP_IHex_IsEndOfFile(void) {
	return (IHexCtx.state == IH_EOF);
}


uint8_t	BSP_IHex_IsError(void) {
	return (IHexCtx.state == IH_ERROR);
}


uint8_t	BSP_IHex_IsEndOfBlock(void) {
	return (IHexCtx.state == IH_EOB);
}

uint8_t BSP_IHex_GetProgress(void) {
	return IHexCtx.progress;
}

uint32_t BSP_IHex_GetFileSize(void) {
	if (IHexCtx.state == IH_ERROR) return 0;
	return f_size(&IHexCtx.file);
}
