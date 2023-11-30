/*******************************************************************
 * MiniConsole V3 - Board Support Package - STM32 JPEG
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 *******************************************************************/

#ifndef STM32H7_BSP_STM32_JPEG_H_
#define STM32H7_BSP_STM32_JPEG_H_

#include "BSP_Common.h"

#define JPEG_INTERRUPT_MASK  ((uint32_t)0x0000007EU)

#define JPEG_FLAG_IFTF     ((uint32_t)JPEG_SR_IFTF)   // Input FIFO is not full and is bellow its threshold flag
#define JPEG_FLAG_IFNFF    ((uint32_t)JPEG_SR_IFNFF)  // Input FIFO Not Full Flag, a data can be written
#define JPEG_FLAG_OFTF     ((uint32_t)JPEG_SR_OFTF)   // Output FIFO is not empty and has reach its threshold
#define JPEG_FLAG_OFNEF    ((uint32_t)JPEG_SR_OFNEF)  // Output FIFO is not empty, a data is available
#define JPEG_FLAG_EOCF     ((uint32_t)JPEG_SR_EOCF)   // JPEG Codec core has finished the encoding or the decoding process and than last data has been sent to the output FIFO
#define JPEG_FLAG_HPDF     ((uint32_t)JPEG_SR_HPDF)   // JPEG Codec has finished the parsing of the headers and the internal registers have been updated
#define JPEG_FLAG_COF      ((uint32_t)JPEG_SR_COF)    // JPEG Codec operation on going  flag

#define JPEG_FLAG_ALL      ((uint32_t)0x000000FEU)    // JPEG Codec All previous flag

#define JPEG_GRAYSCALE_COLORSPACE     ((uint32_t)0x00000000U)	// Grayscale color space
#define JPEG_YCBCR_COLORSPACE         JPEG_CONFR1_COLORSPACE_0	// YCbCr color space
#define JPEG_CMYK_COLORSPACE          JPEG_CONFR1_COLORSPACE	// CMYK color space

#define JPEG_444_SUBSAMPLING     ((uint32_t)0x00000000U)   // Chroma Subsampling 4:4:4
#define JPEG_420_SUBSAMPLING     ((uint32_t)0x00000001U)   // Chroma Subsampling 4:2:0
#define JPEG_422_SUBSAMPLING     ((uint32_t)0x00000002U)   // Chroma Subsampling 4:2:2

typedef struct _JPEGContext {
	uint32_t	JPEGAddr;
	uint32_t	JPEGSize;
	uint32_t	JPEGEnd;
	uint32_t	OutputBufAddr;
	uint32_t	OutputBufSize;
	uint32_t	CSSType;
	uint32_t	ColorSpace;
	uint32_t	ImageWidth;
	uint32_t	ImageHeight;
} JPEGContext_TypeDef;


uint8_t BSP_STM32_JPEG_Init(JPEG_TypeDef * _hjpeg);
uint8_t BSP_STM32_JPEG_Decode(JPEG_TypeDef * _hjpeg, uint32_t JPEG_Addr, uint32_t JPEG_Size, uint32_t OutputBuf_Addr, uint32_t OutputBuf_Size);
uint32_t BSP_STM32_JPEG_GetWidth(JPEG_TypeDef * _hjpeg);
uint32_t BSP_STM32_JPEG_GetHeight(JPEG_TypeDef * _hjpeg);
uint32_t BSP_STM32_JPEG_GetCSS(JPEG_TypeDef * _hjpeg);
uint32_t BSP_STM32_JPEG_GetColorSpace(JPEG_TypeDef * _hjpeg);

#endif /* STM32H7_BSP_STM32_JPEG_H_ */
