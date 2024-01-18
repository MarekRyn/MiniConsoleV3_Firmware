/*
 * rawaudio.c
 *
 *  Created on: Jan 16, 2024
 *      Author: marek
 */

#include "rawaudio.h"

uint8_t RA_Init(rawaudioctx_TypeDef * ractx, int16_t * addr, uint32_t size) {

	if (((uint32_t)addr % 4) > 0) return RA_ERROR;

	ractx->pdata = addr;
	ractx->size = size;
	ractx->length = size / 4; // 16bit stereo = 4 bytes per sample
	ractx->index = 0;

	return RA_OK;
}

uint8_t RA_Seek(rawaudioctx_TypeDef * ractx, uint32_t pos) {
	if (pos > ractx->length) return RA_ERROR;
	ractx->index = pos;
	return RA_OK;
}

uint8_t RA_Read(rawaudioctx_TypeDef * ractx, uint32_t len, int16_t * dst_buf) {

	uint32_t reminder = 0;

	if (ractx->index > ractx->length) {
		ractx->index = ractx->length;
		reminder = len;
		len = 0;
	}

	if ((ractx->index + len) > ractx->length) {
		reminder = ractx->index + len - ractx->length;
		len = ractx->length - ractx->index;
	}

	int32_t * src = (int32_t *)ractx->pdata + ractx->index;
	int32_t * dst = (int32_t *)dst_buf;
	uint32_t i;

	for (i = 0; i < len; i++) {
		*dst = *src;
		dst++;
		src++;
	}

	for (i = 0; i < reminder; i++) *dst = 0;

	ractx->index+=len;

	return RA_OK;
}
