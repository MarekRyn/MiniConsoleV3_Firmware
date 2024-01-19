/*******************************************************************
 * MiniConsole V3 - Board Support Package - STM32 JPEG
 *
 * Author: Marek Ryn
 * Version: 1.0
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 * - 1.0	- Initial version
 *******************************************************************/

#include "BSP_STM32_JPEG.h"

static JPEGContext_TypeDef JPEGctx = {0};

static uint8_t _JPEG_DecInit(JPEG_TypeDef * _hjpeg, uint32_t JPEG_Addr, uint32_t JPEG_Size, uint32_t OutputBuf_Addr, uint32_t OutputBuf_Size) {

	// Setup context
	JPEGctx.CSSType = 0;
	JPEGctx.ColorSpace = 0;
	JPEGctx.ImageHeight = 0;
	JPEGctx.ImageWidth = 0;
	JPEGctx.JPEGAddr = JPEG_Addr;
	JPEGctx.JPEGSize = JPEG_Size;
	JPEGctx.JPEGEnd = JPEG_Addr + JPEG_Size;
	JPEGctx.OutputBufAddr = OutputBuf_Addr;
	JPEGctx.OutputBufSize = OutputBuf_Size;

	// Set codec mode to decoding
	_hjpeg->CONFR1 |= JPEG_CONFR1_DE;

	// Stop any decoding process
	_hjpeg->CONFR0 &=  ~JPEG_CONFR0_START;

	// Disable all interrupts
	MODIFY_REG(_hjpeg->CR, JPEG_INTERRUPT_MASK, 0UL);

	// Flush input and output FIFOs
	_hjpeg->CR |= JPEG_CR_IFF;
	_hjpeg->CR |= JPEG_CR_OFF;

	// Clear all flags
	_hjpeg->CFR |= (JPEG_FLAG_ALL & (JPEG_FLAG_EOCF | JPEG_FLAG_HPDF));

	// Start decoding
	_hjpeg->CONFR0 |=  JPEG_CONFR0_START;

	return BSP_OK;
}


static uint8_t _JPEG_DecProcess(JPEG_TypeDef * _hjpeg) {

	uint32_t yblockNb;
	uint32_t cBblockNb;
	uint32_t cRblockNb;

	// Header parsing completed
	if ((_hjpeg->SR) & JPEG_FLAG_HPDF) {

		// Updating JPEG context
		switch ((_hjpeg->CONFR1 & JPEG_CONFR1_NF)) {
		case 0:
			JPEGctx.ColorSpace = JPEG_GRAYSCALE_COLORSPACE;
			JPEGctx.CSSType = JPEG_444_SUBSAMPLING;
			break;
		case JPEG_CONFR1_NF_1:
			JPEGctx.ColorSpace = JPEG_YCBCR_COLORSPACE;
			yblockNb  = (_hjpeg->CONFR4 & JPEG_CONFR4_NB) >> 4;
			cBblockNb = (_hjpeg->CONFR5 & JPEG_CONFR5_NB) >> 4;
			cRblockNb = (_hjpeg->CONFR6 & JPEG_CONFR6_NB) >> 4;
			JPEGctx.CSSType = JPEG_444_SUBSAMPLING;
			if ((yblockNb == 1UL) && (cBblockNb == 0UL) && (cRblockNb == 0UL)) JPEGctx.CSSType = JPEG_422_SUBSAMPLING; // 16x8 block
			if ((yblockNb == 3UL) && (cBblockNb == 0UL) && (cRblockNb == 0UL)) JPEGctx.CSSType = JPEG_420_SUBSAMPLING;
			break;
		case JPEG_CONFR1_NF:
			JPEGctx.ColorSpace = JPEG_CMYK_COLORSPACE;
			yblockNb  = (_hjpeg->CONFR4 & JPEG_CONFR4_NB) >> 4;
			cBblockNb = (_hjpeg->CONFR5 & JPEG_CONFR5_NB) >> 4;
			cRblockNb = (_hjpeg->CONFR6 & JPEG_CONFR6_NB) >> 4;
			JPEGctx.CSSType = JPEG_444_SUBSAMPLING;
			if ((yblockNb == 1UL) && (cBblockNb == 0UL) && (cRblockNb == 0UL)) JPEGctx.CSSType = JPEG_422_SUBSAMPLING; // 16x8 block
			if ((yblockNb == 3UL) && (cBblockNb == 0UL) && (cRblockNb == 0UL)) JPEGctx.CSSType = JPEG_420_SUBSAMPLING;
			break;
		default:
			return BSP_ERROR;
		}

		JPEGctx.ImageHeight = (_hjpeg->CONFR1 & 0xFFFF0000UL) >> 16;
		JPEGctx.ImageWidth  = (_hjpeg->CONFR3 & 0xFFFF0000UL) >> 16;

		//Clear flag
		_hjpeg->CFR |= JPEG_FLAG_HPDF;
	}

	// Input FIFO handling
	while ((((_hjpeg->SR) & JPEG_FLAG_IFNFF) > 0) && (JPEGctx.JPEGAddr < JPEGctx.JPEGEnd)) {
		_hjpeg->DIR = *((uint32_t *)JPEGctx.JPEGAddr);
		JPEGctx.JPEGAddr += 4;
	}

	// Output FIFO handling
	while ((_hjpeg->SR) & JPEG_FLAG_OFNEF) {
		*((uint32_t *)JPEGctx.OutputBufAddr) = _hjpeg->DOR;
		JPEGctx.OutputBufAddr += 4;
	}

	// End of decoding handling (EOC flag high, Output FIFO flags low)
	if (((_hjpeg->SR) & (JPEG_FLAG_EOCF | JPEG_FLAG_OFNEF | JPEG_FLAG_OFTF)) == JPEG_FLAG_EOCF) {

		//Stop decoding process
		_hjpeg->CONFR0 &=  ~JPEG_CONFR0_START;

		// Disable all interrupts
		MODIFY_REG(_hjpeg->CR, JPEG_INTERRUPT_MASK, 0UL);

		// Clear all flags
		_hjpeg->CFR |= (JPEG_FLAG_ALL & (JPEG_FLAG_EOCF | JPEG_FLAG_HPDF));

		return BSP_OK;
	}

	return BSP_BUSY;
}


uint8_t BSP_STM32_JPEG_Init(JPEG_TypeDef * _hjpeg) {

	// Enable JPEG core
	_hjpeg->CR |=  JPEG_CR_JCEN;

	// Stop any decoding process
	_hjpeg->CONFR0 &=  ~JPEG_CONFR0_START;

	// Disable all interrupts
	MODIFY_REG(_hjpeg->CR, JPEG_INTERRUPT_MASK, 0UL);

	// Flush input and output FIFOs
	_hjpeg->CR |= JPEG_CR_IFF;
	_hjpeg->CR |= JPEG_CR_OFF;

	// Clear all flags
	_hjpeg->CFR |= (JPEG_FLAG_ALL & (JPEG_FLAG_EOCF | JPEG_FLAG_HPDF));

	// Enable header processing
	_hjpeg->CONFR1 |= JPEG_CONFR1_HDR;

	return BSP_OK;
}

uint8_t BSP_STM32_JPEG_Decode(JPEG_TypeDef * _hjpeg, uint32_t JPEG_Addr, uint32_t JPEG_Size, uint32_t OutputBuf_Addr, uint32_t OutputBuf_Size) {
	// Init decode process
	_JPEG_DecInit(_hjpeg, JPEG_Addr, JPEG_Size, OutputBuf_Addr, OutputBuf_Size);

	// Decode process
	while (_JPEG_DecProcess(_hjpeg) == BSP_BUSY) {};

	return BSP_OK;
}

uint32_t BSP_STM32_JPEG_GetWidth(JPEG_TypeDef * _hjpeg) {
	UNUSED(_hjpeg);

	return JPEGctx.ImageWidth;
}

uint32_t BSP_STM32_JPEG_GetHeight(JPEG_TypeDef * _hjpeg) {
	UNUSED(_hjpeg);

	return JPEGctx.ImageHeight;
}

uint32_t BSP_STM32_JPEG_GetCSS(JPEG_TypeDef * _hjpeg) {
	UNUSED(_hjpeg);

	return JPEGctx.CSSType;
}

uint32_t BSP_STM32_JPEG_GetColorSpace(JPEG_TypeDef * _hjpeg) {
	UNUSED(_hjpeg);

	return JPEGctx.ColorSpace;
}
