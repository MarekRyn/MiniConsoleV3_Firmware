/*
 * rawaudio.c
 *
 *  Created on: Jan 16, 2024
 *      Author: marek
 */

#include "rawaudio.h"
#include "stdlib.h"

uint8_t RAW_Init(rawaudioctx_TypeDef * ractx, int16_t * addr, uint32_t size, uint8_t chno, uint8_t bitformat, uint32_t freq) {

	if (((uint32_t)addr % 4) > 0) return RAW_ERROR;
	if ((chno != 1) && (chno !=2)) return RAW_ERROR;	// Only mono or stereo supported
	if ((bitformat != 8) && (bitformat != 16)) return RAW_ERROR;	// Only 8 or 16 bit signed supported

	ractx->pdata = addr;
	ractx->size = size;
	ractx->length = size / ((chno * bitformat) / 8); // Number of samples
	ractx->index = 0;
	ractx->chno = chno;
	ractx->bitformat = bitformat;
	ractx->freq = freq;
	ractx->inc = (1024 * freq) / 44100;
	ractx->flag = 0;

	return RAW_OK;
}

uint8_t RAW_Deinit(rawaudioctx_TypeDef * ractx) {
	free(ractx);
	return RAW_OK;
}

uint8_t RAW_Seek(rawaudioctx_TypeDef * ractx, uint32_t pos) {
	if (pos > ractx->length) return RAW_ERROR;
	ractx->index = pos;
	return RAW_OK;
}

uint8_t RAW_Read(rawaudioctx_TypeDef * ractx, uint32_t len, int16_t * dst_buf) {

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

	return RAW_OK;
}



uint8_t SRAW_Init(srawaudioctx_TypeDef * sractx, void * strbuf, uint32_t strbufsize, uint8_t chno, uint8_t bitformat, uint32_t freq) {
	// Allocating stream buffer for mp3 frames;
	sractx->buf = strbuf;
	sractx->buf_size = strbufsize;
	if (sractx->buf == NULL) return RAW_ERROR;
	if (sractx->buf_size == 0) return RAW_ERROR;
	if ((chno != 1) && (chno !=2)) return RAW_ERROR;	// Only mono or stereo supported
	if ((bitformat != 8) && (bitformat != 16)) return RAW_ERROR;	// Only 8 or 16 bit signed supported

	// Initialize offsets for uploaded and consumed data
	sractx->i_consumed = 0;
	sractx->i_uploaded = 0;
	sractx->buf_modulo = strbufsize;
	sractx->chno = chno;
	sractx->bitformat = bitformat;
	sractx->freq = freq;
	sractx->inc = (1024 * freq) / 44100;
	sractx->flag = 0;
	sractx->samplediv = chno * (bitformat / 8);
	sractx->req_size = 0;

	return RAW_OK;
}


uint8_t SRAW_Deinit(srawaudioctx_TypeDef * sractx) {
	free(sractx);
	return RAW_OK;
}


uint8_t SRAW_Read(srawaudioctx_TypeDef * sractx, uint32_t len, int16_t *dst_buf) {

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

	// Calculating required samples
	uint32_t req_samples = (32000 * len) / 44100;

	switch (sractx->bitformat) {
	case 16:

		switch (sractx->chno) {
		case 1:
			// Checking if enough data in stream buffer
			if ((sractx->i_uploaded - sractx->i_consumed) < (req_samples * 2)) break;

			src16 = (uint16_t *)(sractx->buf + AUDIO_SH0_CM4_ADDR + (sractx->i_consumed % sractx->buf_modulo));
			while(1) {
				s = *src16;
				sample = (s << 16) | s;
				*dst = sample;
				sractx->flag+=sractx->inc;
				if (sractx->flag >= 1024) {
					sractx->flag-=1024;
					sractx->i_consumed+=2;
					src16 = (uint16_t *)(sractx->buf + AUDIO_SH0_CM4_ADDR + (sractx->i_consumed % sractx->buf_modulo));
				}
				dst++;
				reminder--;
				if (sractx->i_consumed >= sractx->i_uploaded) break;
				if (reminder == 0) break;
			}
			break;

		case 2:
			// Checking if enough data in stream buffer
			if ((sractx->i_uploaded - sractx->i_consumed) < (req_samples * 4)) break;

			src32 = (uint32_t *)(sractx->buf + AUDIO_SH0_CM4_ADDR + (sractx->i_consumed % sractx->buf_modulo));
			while(1) {
				sample = *src32;
				*dst = sample;
				sractx->flag+=sractx->inc;
				if (sractx->flag >= 1024) {
					sractx->flag-=1024;
					sractx->i_consumed+=4;
					src32 = (uint32_t *)(sractx->buf + AUDIO_SH0_CM4_ADDR + (sractx->i_consumed % sractx->buf_modulo));
				}
				dst++;
				reminder--;
				if (sractx->i_consumed >= sractx->i_uploaded) break;
				if (reminder == 0) break;
			}
			break;

		default:
			break;
		}

		break;
	case 8:

		switch (sractx->chno) {
		case 1:
			// Checking if enough data in stream buffer
			if ((sractx->i_uploaded - sractx->i_consumed) < (req_samples)) break;

			src8 = (uint8_t *)(sractx->buf + AUDIO_SH0_CM4_ADDR + (sractx->i_consumed % sractx->buf_modulo));
			while(1) {
				s = *src8;
				sample = (s << 24) | (s << 8);
				*dst = sample;
				sractx->flag+=sractx->inc;
				if (sractx->flag >= 1024) {
					sractx->flag-=1024;
					sractx->i_consumed += 1;
					src8 = (uint8_t *)(sractx->buf + AUDIO_SH0_CM4_ADDR + (sractx->i_consumed % sractx->buf_modulo));
				}
				dst++;
				reminder--;
				if (sractx->i_consumed >= sractx->i_uploaded) break;
				if (reminder == 0) break;
			}
			break;

		case 2:
			// Checking if enough data in stream buffer
			if ((sractx->i_uploaded - sractx->i_consumed) < (req_samples * 2)) break;

			src16 = (uint16_t *)(sractx->buf + AUDIO_SH0_CM4_ADDR + (sractx->i_consumed % sractx->buf_modulo));
			while(1) {
				s1 = (*src16 >> 8) & 0x00FF;
				s2 = (*src16) & 0x00FF;
				sample = (s1 << 16) | (s2 << 8);
				*dst = sample;
				sractx->flag+=sractx->inc;
				if (sractx->flag >= 1024) {
					sractx->flag-=1024;
					sractx->i_consumed += 2;
					src16 = (uint16_t *)(sractx->buf + AUDIO_SH0_CM4_ADDR + (sractx->i_consumed % sractx->buf_modulo));
				}
				dst++;
				reminder--;
				if (sractx->i_consumed >= sractx->i_uploaded) break;
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

	return RAW_OK;
}


void * SRAW_GetBufAddr(srawaudioctx_TypeDef * sractx, uint32_t size) {
	sractx->req_size = 0;
	if (!sractx->buf) return NULL;
	if ((sractx->i_uploaded - sractx->i_consumed + (size << 1)) > sractx->buf_size) return NULL;

	sractx->req_size = size;
	uint32_t offset = sractx->i_uploaded % sractx->buf_modulo;
	if ((offset + size) > sractx->buf_size) {
		sractx->buf_modulo = offset;
		offset = 0;
	}

	return (void *)sractx->buf + offset + AUDIO_SH0_CM7_ADDR;
}


uint8_t SRAW_BufUpdateComplete(srawaudioctx_TypeDef * sractx) {
	sractx->i_uploaded += sractx->req_size;
	return RAW_OK;
}

