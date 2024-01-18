/*
 * rawaudio.h
 *
 *  Created on: Jan 16, 2024
 *      Author: marek
 */

#include "stdint.h"

#ifndef BSP_LIBS_RAWAUDIO_H_
#define BSP_LIBS_RAWAUDIO_H_

#define RA_OK		0
#define RA_ERROR	1

typedef struct {
	int16_t *	pdata;
	uint32_t	size;
	uint32_t	length;
	uint32_t	index;
} rawaudioctx_TypeDef;

uint8_t RA_Init(rawaudioctx_TypeDef * ractx, int16_t * addr, uint32_t size);
uint8_t RA_Seek(rawaudioctx_TypeDef * ractx, uint32_t pos);
uint8_t RA_Read(rawaudioctx_TypeDef * ractx, uint32_t len, int16_t * dst_buf);



#endif /* BSP_LIBS_RAWAUDIO_H_ */
