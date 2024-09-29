/*
 * rawaudio.c
 *
 *  Created on: Jan 16, 2024
 *      Author: marek
 */

#include "rawaudio.h"
#include "stdlib.h"

uint8_t RA_Init(rawaudioctx_TypeDef * ractx, int16_t * addr, uint32_t size, uint8_t chno, uint8_t bitformat, uint32_t freq) {

	if (((uint32_t)addr % 4) > 0) return RA_ERROR;
	if ((chno != 1) && (chno !=2)) return RA_ERROR;	// Only mono or stereo supported
	if ((bitformat != 8) && (bitformat != 16)) return RA_ERROR;	// Only 8 or 16 bit signed supported

	ractx->pdata = addr;
	ractx->size = size;
	ractx->length = size / ((chno * bitformat) / 8); // Number of samples
	ractx->index = 0;
	ractx->chno = chno;
	ractx->bitformat = bitformat;
	ractx->freq = freq;
	ractx->inc = (1024 * freq) / 44100;
	ractx->flag = 0;

	return RA_OK;
}

uint8_t RA_Deinit(rawaudioctx_TypeDef * ractx) {
	free(ractx);
	return RA_OK;
}

uint8_t RA_Seek(rawaudioctx_TypeDef * ractx, uint32_t pos) {
	if (pos > ractx->length) return RA_ERROR;
	ractx->index = pos;
	return RA_OK;
}

uint8_t RA_Read(rawaudioctx_TypeDef * ractx, uint32_t len, int16_t * dst_buf) {

	uint32_t reminder = len;
	uint32_t * dst = (uint32_t *)dst_buf;
	uint32_t * src32;
	uint16_t * src16;
	uint8_t  * src8;
	uint32_t sample = 0;
	uint32_t s = 0;
	uint32_t s1 = 0;
	uint32_t s2 = 0;
	uint32_t i = 0;

	switch (ractx->bitformat) {
	case 16:

		switch (ractx->chno) {
		case 1:
			src16 = (uint16_t *)ractx->pdata + ractx->index;
			while(1) {
				s = *src16;
				sample = (s << 16) | s;
				*dst = sample;
				ractx->flag+=ractx->inc;
				if (ractx->flag >= 1024) {
					ractx->flag-=1024;
					ractx->index++;
					src16++;
				}
				dst++;
				reminder--;
				if (ractx->index >= ractx->length) break;
				if (reminder == 0) break;
			}
			break;

		case 2:
			src32 = (uint32_t *)ractx->pdata + ractx->index;
			while(1) {
				sample = *src32;
				*dst = sample;
				ractx->flag+=ractx->inc;
				if (ractx->flag >= 1024) {
					ractx->flag-=1024;
					ractx->index++;
					src32++;
				}
				dst++;
				reminder--;
				if (ractx->index >= ractx->length) break;
				if (reminder == 0) break;
			}
			break;

		default:
			break;
		}

		break;
	case 8:

		switch (ractx->chno) {
		case 1:
			src8 = (uint8_t *)ractx->pdata + ractx->index;
			while(1) {
				s = *src8;
				sample = (s << 24) | (s << 8);
				*dst = sample;
				ractx->flag+=ractx->inc;
				if (ractx->flag >= 1024) {
					ractx->flag-=1024;
					ractx->index++;
					src8++;
				}
				dst++;
				reminder--;
				if (ractx->index >= ractx->length) break;
				if (reminder == 0) break;
			}
			break;

		case 2:
			src16 = (uint16_t *)ractx->pdata + ractx->index;
			while(1) {
				s1 = (*src16 >> 8) & 0x00FF;
				s2 = (*src16) & 0x00FF;
				sample = (s1 << 16) | (s2 << 8);
				*dst = sample;
				ractx->flag+=ractx->inc;
				if (ractx->flag >= 1024) {
					ractx->flag-=1024;
					ractx->index++;
					src16++;
				}
				dst++;
				reminder--;
				if (ractx->index >= ractx->length) break;
				if (reminder == 0) break;
			}
			break;

		default:
			break;
		}


		break;
	default:
		break;
	}

	// If required, filling remaining buffer with 0
	for (i = 0; i < reminder; i++) *dst = 0;

	return RA_OK;
}
