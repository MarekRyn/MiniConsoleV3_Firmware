/*
 * midiaudio.h
 *
 *  Created on: Aug 28, 2024
 *      Author: marek
 */

#ifndef BSP_LIBS_MIDIAUDIO_H_
#define BSP_LIBS_MIDIAUDIO_H_

#include "stdint.h"

#define MID_OK		0
#define MID_ERROR	1

typedef struct {

} midctx_TypeDef;


uint8_t MID_Init(midctx_TypeDef * midictx, void * sfaddr, uint32_t sfsize, void * midiaddr, uint32_t midisize);
uint8_t MID_Reset(midctx_TypeDef * midictx);
uint8_t MID_Read(midctx_TypeDef * midctx, uint32_t len, int16_t * dst_buf);
uint8_t MID_Uninit(midctx_TypeDef * midictx);

#endif /* BSP_LIBS_MIDIAUDIO_H_ */
