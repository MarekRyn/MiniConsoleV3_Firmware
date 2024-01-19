/*******************************************************************
 * MiniConsole V3 - Board Support Package - STM32 DMA2D
 *
 * Author: Marek Ryn
 * Version: 1.0
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 * - 1.0	- Initial version
 *******************************************************************/

#ifndef STM32H7_BSP_STM32_DMA2D_H_
#define STM32H7_BSP_STM32_DMA2D_H_

#include "BSP_Common.h"

#define DMA2D_M2M                   0x00000000U                         // DMA2D memory to memory transfer mode
#define DMA2D_M2M_PFC               DMA2D_CR_MODE_0                     // DMA2D memory to memory with pixel format conversion transfer mode
#define DMA2D_M2M_BLEND             DMA2D_CR_MODE_1                     // DMA2D memory to memory with blending transfer mode
#define DMA2D_R2M                   (DMA2D_CR_MODE_1 | DMA2D_CR_MODE_0) // DMA2D register to memory transfer mode
#define DMA2D_M2M_BLEND_FG          DMA2D_CR_MODE_2                     // DMA2D memory to memory with blending transfer mode and fixed color FG
#define DMA2D_M2M_BLEND_BG          (DMA2D_CR_MODE_2 | DMA2D_CR_MODE_0) // DMA2D memory to memory with blending transfer mode and fixed color BG
#define DMA2D_NO_MODIF_ALPHA        0x00000000U  						// No modification of the alpha channel value
#define DMA2D_REPLACE_ALPHA         0x00000001U							// Replace original alpha channel value by programmed alpha value
#define DMA2D_COMBINE_ALPHA         0x00000002U  						// Replace original alpha channel value by programmed alpha value

#define DMA2D_ARGB8888          	0b0000
#define DMA2D_RGB888            	0b0001
#define DMA2D_RGB565            	0b0010
#define DMA2D_ARGB1555          	0b0011
#define DMA2D_ARGB4444          	0b0100
#define DMA2D_YCBCR           		0b1011

#define DMA2D_CSS_444				0b00
#define DMA2D_CSS_422				0b01
#define DMA2D_CSS_420				0b10

uint8_t BSP_STM32_DMA2D_FillBuff(DMA2D_TypeDef * hdma2d, uint32_t colormode, uint16_t width, uint16_t height, uint16_t offsetline, uint32_t dest_addr, uint32_t color);
uint8_t BSP_STM32_DMA2D_FillBuffBlend(DMA2D_TypeDef * hdma2d, uint32_t colormode, uint16_t width, uint16_t height, uint16_t offsetline, uint32_t dest_addr, uint32_t color, uint8_t alpha);
uint8_t BSP_STM32_DMA2D_CopyBuf(DMA2D_TypeDef * hdma2d, uint32_t colormode, uint16_t width, uint16_t height, uint16_t src_offsetline, uint32_t src_addr, uint16_t dest_offsetline, uint32_t dest_addr);
uint8_t BSP_STM32_DMA2D_CopyBufJPEG(DMA2D_TypeDef * hdma2d, uint32_t colormode, uint16_t width, uint16_t height, uint16_t src_offsetline, uint32_t src_addr, uint16_t dest_offsetline, uint32_t dest_addr, uint32_t chroma);
uint8_t BSP_STM32_DMA2D_CopyBufBlend(DMA2D_TypeDef * hdma2d, uint32_t colormode, uint16_t width, uint16_t height, uint16_t src_offsetline, uint32_t src_addr, uint16_t dest_offsetline, uint32_t dest_addr, uint8_t alpha);

uint8_t BSP_STM32_DMA2D_IRQHandler(DMA2D_TypeDef * hdma2d);

#endif /* STM32H7_BSP_STM32_DMA2D_H_ */
