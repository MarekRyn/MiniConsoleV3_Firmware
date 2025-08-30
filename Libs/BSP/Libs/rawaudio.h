/*
 * rawaudio.h
 *
 *  Created on: Jan 16, 2024
 *      Author: marek
 */

#include "stdint.h"

#ifndef BSP_LIBS_RAWAUDIO_H_
#define BSP_LIBS_RAWAUDIO_H_

#define RAW_OK		0
#define RAW_ERROR	1

#define AUDIO_SH0_CM4_ADDR		0x10040000
#define AUDIO_SH0_CM7_ADDR		0x30040000

typedef struct {
	int16_t *	pdata;
	uint32_t	size;
	uint32_t	length;
	uint32_t	index;
	uint8_t		chno;
	uint8_t		bitformat;
	uint32_t	freq;
	uint32_t	inc;
	uint32_t	flag;
} rawaudioctx_TypeDef;


typedef struct {
	uint8_t * 	buf;
	uint32_t	buf_size;
	uint32_t	buf_modulo;
	uint32_t	req_size;
	uint32_t	i_uploaded;
	uint32_t	i_consumed;
	uint8_t		chno;
	uint8_t		bitformat;
	uint32_t	freq;
	uint32_t	inc;
	uint32_t	flag;
	uint32_t	samplediv;
} srawaudioctx_TypeDef;

uint8_t RAW_Init(rawaudioctx_TypeDef * ractx, int16_t * addr, uint32_t size, uint8_t chno, uint8_t bitformat, uint32_t freq);
uint8_t RAW_Deinit(rawaudioctx_TypeDef * ractx);
uint8_t RAW_Seek(rawaudioctx_TypeDef * ractx, uint32_t pos);
uint8_t RAW_Read(rawaudioctx_TypeDef * ractx, uint32_t len, int16_t * dst_buf);

uint8_t SRAW_Init(srawaudioctx_TypeDef * sractx, void * strbuf, uint32_t strbufsize, uint8_t chno, uint8_t bitformat, uint32_t freq);
uint8_t SRAW_Deinit(srawaudioctx_TypeDef * sractx);
uint8_t SRAW_Read(srawaudioctx_TypeDef * sractx, uint32_t len, int16_t *dst_buf);
void * SRAW_GetBufAddr(srawaudioctx_TypeDef * sractx, uint32_t size);
uint8_t SRAW_BufUpdateComplete(srawaudioctx_TypeDef * sractx);




#endif /* BSP_LIBS_RAWAUDIO_H_ */
