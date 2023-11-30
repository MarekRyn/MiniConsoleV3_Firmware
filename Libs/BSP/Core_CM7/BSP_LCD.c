/*******************************************************************
 * MiniConsole V3 - Board Support Package - LCD
 *
 * Author: Marek Ryn
 * Version: 0.3b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 * - 0.2b 	- Added hardware JPEG decoding
 * - 0.3b	- Added ARGB1555 and ARGB4444 modes
 *******************************************************************/

#include "BSP_LCD.h"
#include <string.h>

#define NOT(x)		(~x) & 0x01
#define MIN(A, B)	((A) > (B) ? (B) : (A))
#define MAX(A, B)	((A) > (B) ? (A) : (B))

LCD_HandleTypeDef BSP_hlcd = {0};

uint32_t (*BSP_LCD_Color)(uint32_t color, uint8_t alpha); // Calculating color value and include alpha in modes with alpha channel
uint32_t (*BSP_LCD_Alpha)(uint32_t color, uint8_t alpha); // Updating alpha channel in pre-calculated color value
void (*BSP_LCD_DMA2D_Wait)();
void (*BSP_LCD_UpdatePixel)(LCD_HandleTypeDef *lcd, uint32_t offset, int16_t x, int16_t y, uint32_t value);
uint32_t (*BSP_LCD_GetPixel)(LCD_HandleTypeDef *lcd, uint32_t offset,  int16_t x, int16_t y);
void (*BSP_LCD_FillBuf)(LCD_HandleTypeDef *lcd, uint8_t layer, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t offsetline, uint32_t color);
void (*BSP_LCD_CopyBuf)(LCD_HandleTypeDef *lcd, uint8_t layer, uint32_t src_addr, uint16_t offsline_src, uint16_t x_dest, uint16_t y_dest, uint16_t offsline_dest, uint16_t width, uint16_t height);
void (*BSP_LCD_CopyBufBlend)(LCD_HandleTypeDef *lcd, uint8_t layer, uint32_t src_addr, uint16_t offsline_src, uint16_t x_dest, uint16_t y_dest, uint16_t offsline_dest, uint16_t width, uint16_t height, uint8_t alpha);
void (*BSP_LCD_CopyBufJPEG)(LCD_HandleTypeDef *lcd, uint8_t layer, uint16_t x_dest, uint16_t y_dest);

/******************************************************************************
 * Private functions
 ******************************************************************************/

inline static uint32_t _color_dist(uint32_t c1, uint32_t c2) {
	// Color distance is calculated as per formula presented in:
	// https://www.compuphase.com/cmetric.htm, and
	// https://en.wikipedia.org/wiki/Color_difference


	uint8_t c1_b = (c1 & 0x000000ff) >> 0;
	uint8_t c1_g = (c1 & 0x0000ff00) >> 8;
	uint8_t c1_r = (c1 & 0x00ff0000) >> 16;

	uint8_t c2_b = (c2 & 0x000000ff) >> 0;
	uint8_t c2_g = (c2 & 0x0000ff00) >> 8;
	uint8_t c2_r = (c2 & 0x00ff0000) >> 16;

	int16_t av_r = (c1_r + c2_r) >> 1;
	int16_t	dr = (int16_t)c1_r - (int16_t)c2_r;
	int16_t dg = (int16_t)c1_g - (int16_t)c2_g;
	int16_t db = (int16_t)c1_b - (int16_t)c2_b;

	//return sqrt((((512 + av_r) * dr * dr) >> 8) + 4 * dg * dg + (((767 - av_r) * db * db) >> 8));
	// SQRT can be omitted, as it has no effect on finding minimum value of the function
	return (((512 + av_r) * dr * dr) >> 8) + 4 * dg * dg + (((767 - av_r) * db * db) >> 8);
}


/******************************************************************************
 * Functions for various color modes
 ******************************************************************************/

/* ARGB8888 ------------------------------------------------------------------*/

static uint32_t _ARGB8888_color(uint32_t color, uint8_t alpha) {
	// Status: Function Completed
	return (color & 0x00FFFFFF) | ((uint32_t)alpha << 24);
}


static uint32_t _ARGB8888_alpha(uint32_t color, uint8_t alpha) {
	// Status: Function Completed
	return (color & 0x00FFFFFF) | ((uint32_t)alpha << 24);
}


static void _ARGB8888_dma2dwait() {
	// Status: Function Completed
	while (BSP_hlcd.priv_.dma2d_state == LCD_DMA2D_BUSY) {};
}


static void _ARGB8888_updatepixel(LCD_HandleTypeDef *lcd, uint32_t offset, int16_t x, int16_t y, uint32_t value) {
	// Status: Function Completed
	if (x >= LCD_WIDTH) return;
	if (x < 0) return;
	if (y >= LCD_HEIGHT) return;
	if (y < 0) return;

	uint32_t *addr = (uint32_t *)offset + (x + y * LCD_WIDTH);
	*addr = value;
}


static uint32_t _ARGB8888_getpixel(LCD_HandleTypeDef *lcd, uint32_t offset,  int16_t x, int16_t y) {
	// Status: Function Completed
	if (x >= LCD_WIDTH) return 0x00000000;
	if (x < 0) return 0x00000000;
	if (y >= LCD_HEIGHT) return 0x00000000;
	if (y < 0) return 0x00000000;

	uint32_t *addr = (uint32_t *)offset + (x + y * LCD_WIDTH);
	return *addr;
}


static void _ARGB8888_fillbuf(LCD_HandleTypeDef *lcd, uint8_t layer, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t offsetline, uint32_t color) {
	// Status: Function Completed
	uint8_t eframe = lcd->Layers[layer].Frame_EDIT;
	uint32_t dest_addr = lcd->Layers[layer].Frames[eframe] + ((x + y * LCD_WIDTH) << 2);

	_ARGB8888_dma2dwait();

	// Starting DMA2D
	BSP_hlcd.priv_.dma2d_state = LCD_DMA2D_BUSY;

	BSP_STM32_DMA2D_FillBuff(DMA2D, DMA2D_ARGB8888, width, height, offsetline, dest_addr, color);
}


static void _ARGB8888_copybuf(LCD_HandleTypeDef *lcd, uint8_t layer, uint32_t src_addr, uint16_t offsline_src, uint16_t x_dest, uint16_t y_dest, uint16_t offsline_dest, uint16_t width, uint16_t height) {
	// Status: Function Completed
	uint8_t eframe = lcd->Layers[layer].Frame_EDIT;
	uint32_t dest_addr = lcd->Layers[layer].Frames[eframe] + ((x_dest + y_dest * LCD_WIDTH) << 2);

	_ARGB8888_dma2dwait();

	// Starting DMA2D
	BSP_hlcd.priv_.dma2d_state = LCD_DMA2D_BUSY;

	BSP_STM32_DMA2D_CopyBuf(DMA2D, DMA2D_ARGB8888, width, height, offsline_src, src_addr, offsline_dest, dest_addr);
}


static void _ARGB8888_copybufblend(LCD_HandleTypeDef *lcd, uint8_t layer, uint32_t src_addr, uint16_t offsline_src, uint16_t x_dest, uint16_t y_dest, uint16_t offsline_dest, uint16_t width, uint16_t height, uint8_t alpha) {
	// Status: Function Completed
	uint8_t eframe = lcd->Layers[layer].Frame_EDIT;
	uint32_t dest_addr = lcd->Layers[layer].Frames[eframe] + ((x_dest + y_dest * LCD_WIDTH) << 2);

	_ARGB8888_dma2dwait();

	// Starting DMA2D
	BSP_hlcd.priv_.dma2d_state = LCD_DMA2D_BUSY;

	BSP_STM32_DMA2D_CopyBufBlend(DMA2D, DMA2D_ARGB8888, width, height, offsline_src, src_addr, offsline_dest, dest_addr, alpha);
}


static void _ARGB8888_copybufJPEG(LCD_HandleTypeDef *lcd, uint8_t layer, uint16_t x_dest, uint16_t y_dest) {
	uint8_t eframe = lcd->Layers[layer].Frame_EDIT;
	uint32_t dest_addr = lcd->Layers[layer].Frames[eframe] + ((x_dest + y_dest * LCD_WIDTH) << 2);
	uint32_t chroma = 0;

	if (BSP_STM32_JPEG_GetColorSpace(JPEG) != JPEG_YCBCR_COLORSPACE) return;

	uint32_t width = BSP_STM32_JPEG_GetWidth(JPEG);
	uint32_t offsline_dest = LCD_WIDTH - BSP_STM32_JPEG_GetWidth(JPEG);
	uint32_t offsline_src = 0;

	switch (BSP_STM32_JPEG_GetCSS(JPEG)) {
	case JPEG_420_SUBSAMPLING:
		chroma = DMA2D_CSS_420;
		if (width % 16) offsline_src = 16 - (width % 16);
		break;
	case JPEG_422_SUBSAMPLING:
		chroma = DMA2D_CSS_422;
		if (width % 16) offsline_src = 16 - (width % 16);
		break;
	case JPEG_444_SUBSAMPLING:
		chroma = DMA2D_CSS_444;
		if (width % 8) offsline_src = 8 - (width % 8);
		break;
	default:
		return;
	}

	// Wait for any previous DMA2D operation to complete
	_ARGB8888_dma2dwait();

	// Starting DMA2D color space conversion and copy to frame buffer.
	BSP_hlcd.priv_.dma2d_state = LCD_DMA2D_BUSY;

	BSP_STM32_DMA2D_CopyBufJPEG(DMA2D, DMA2D_ARGB8888, BSP_STM32_JPEG_GetWidth(JPEG), BSP_STM32_JPEG_GetHeight(JPEG), offsline_src, lcd->JPEGBuf, offsline_dest, dest_addr, chroma);
}

/* ARGB1555 ------------------------------------------------------------------*/

static uint32_t _ARGB1555_color(uint32_t color, uint8_t alpha) {
	// Status: Function Completed
	uint32_t r = ((color & 0x000000FF) >> 3 ) << 0;
	uint32_t g = ((color & 0x0000FF00) >> 11) << 5;
	uint32_t b = ((color & 0x00FF0000) >> 19) << 10;
	uint32_t a = (uint32_t)(alpha == 0?0:1) << 15;
	return r | g | b | a;
}


static uint32_t _ARGB1555_alpha(uint32_t color, uint8_t alpha) {
	// Status: Function Completed
	uint32_t c = color & 0x0007FFF;
	uint32_t a = (uint32_t)(alpha == 0?0:1) << 15;
	return c | a;
}


static void _ARGB1555_dma2dwait() {
	// Status: Function Completed
	while (BSP_hlcd.priv_.dma2d_state == LCD_DMA2D_BUSY) {};
}


static void _ARGB1555_updatepixel(LCD_HandleTypeDef *lcd, uint32_t offset, int16_t x, int16_t y, uint32_t value) {
	// Status: Function Completed
	if (x >= LCD_WIDTH) return;
	if (x < 0) return;
	if (y >= LCD_HEIGHT) return;
	if (y < 0) return;

	uint16_t *addr = (uint16_t *)offset + (x + y * LCD_WIDTH);
	*addr = (uint16_t)value;
}


static uint32_t _ARGB1555_getpixel(LCD_HandleTypeDef *lcd, uint32_t offset,  int16_t x, int16_t y) {
	// Status: Function Completed
	if (x >= LCD_WIDTH) return 0x00000000;
	if (x < 0) return 0x00000000;
	if (y >= LCD_HEIGHT) return 0x00000000;
	if (y < 0) return 0x00000000;

	uint16_t *addr = (uint16_t *)offset + (x + y * LCD_WIDTH);
	return *addr;
}


static void _ARGB1555_fillbuf(LCD_HandleTypeDef *lcd, uint8_t layer, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t offsetline, uint32_t color) {
	// Status: Function Completed
	uint8_t eframe = lcd->Layers[layer].Frame_EDIT;
	uint32_t dest_addr = lcd->Layers[layer].Frames[eframe] + ((x + y * LCD_WIDTH) << 1);

	_ARGB1555_dma2dwait();

	// Starting DMA2D
	BSP_hlcd.priv_.dma2d_state = LCD_DMA2D_BUSY;

	BSP_STM32_DMA2D_FillBuff(DMA2D, DMA2D_ARGB1555, width, height, offsetline, dest_addr, color);
}


static void _ARGB1555_copybuf(LCD_HandleTypeDef *lcd, uint8_t layer, uint32_t src_addr, uint16_t offsline_src, uint16_t x_dest, uint16_t y_dest, uint16_t offsline_dest, uint16_t width, uint16_t height) {
	// Status: Function Completed
	uint8_t eframe = lcd->Layers[layer].Frame_EDIT;
	uint32_t dest_addr = lcd->Layers[layer].Frames[eframe] + ((x_dest + y_dest * LCD_WIDTH) << 1);

	_ARGB1555_dma2dwait();

	// Starting DMA2D
	BSP_hlcd.priv_.dma2d_state = LCD_DMA2D_BUSY;

	BSP_STM32_DMA2D_CopyBuf(DMA2D, DMA2D_ARGB1555, width, height, offsline_src, src_addr, offsline_dest, dest_addr);
}


static void _ARGB1555_copybufblend(LCD_HandleTypeDef *lcd, uint8_t layer, uint32_t src_addr, uint16_t offsline_src, uint16_t x_dest, uint16_t y_dest, uint16_t offsline_dest, uint16_t width, uint16_t height, uint8_t alpha) {
	// Status: Function Completed
	uint8_t eframe = lcd->Layers[layer].Frame_EDIT;
	uint32_t dest_addr = lcd->Layers[layer].Frames[eframe] + ((x_dest + y_dest * LCD_WIDTH) << 1);

	_ARGB1555_dma2dwait();

	// Starting DMA2D
	BSP_hlcd.priv_.dma2d_state = LCD_DMA2D_BUSY;

	BSP_STM32_DMA2D_CopyBufBlend(DMA2D, DMA2D_ARGB1555, width, height, offsline_src, src_addr, offsline_dest, dest_addr, alpha);
}


static void _ARGB1555_copybufJPEG(LCD_HandleTypeDef *lcd, uint8_t layer, uint16_t x_dest, uint16_t y_dest) {
	uint8_t eframe = lcd->Layers[layer].Frame_EDIT;
	uint32_t dest_addr = lcd->Layers[layer].Frames[eframe] + ((x_dest + y_dest * LCD_WIDTH) << 1);
	uint32_t chroma = 0;

	if (BSP_STM32_JPEG_GetColorSpace(JPEG) != JPEG_YCBCR_COLORSPACE) return;

	uint32_t width = BSP_STM32_JPEG_GetWidth(JPEG);
	uint32_t offsline_dest = LCD_WIDTH - BSP_STM32_JPEG_GetWidth(JPEG);
	uint32_t offsline_src = 0;

	switch (BSP_STM32_JPEG_GetCSS(JPEG)) {
	case JPEG_420_SUBSAMPLING:
		chroma = DMA2D_CSS_420;
		if (width % 16) offsline_src = 16 - (width % 16);
		break;
	case JPEG_422_SUBSAMPLING:
		chroma = DMA2D_CSS_422;
		if (width % 16) offsline_src = 16 - (width % 16);
		break;
	case JPEG_444_SUBSAMPLING:
		chroma = DMA2D_CSS_444;
		if (width % 8) offsline_src = 8 - (width % 8);
		break;
	default:
		return;
	}

	// Wait for any previous DMA2D operation to complete
	_ARGB1555_dma2dwait();

	// Starting DMA2D color space conversion and copy to frame buffer.
	BSP_hlcd.priv_.dma2d_state = LCD_DMA2D_BUSY;

	BSP_STM32_DMA2D_CopyBufJPEG(DMA2D, DMA2D_ARGB1555, BSP_STM32_JPEG_GetWidth(JPEG), BSP_STM32_JPEG_GetHeight(JPEG), offsline_src, lcd->JPEGBuf, offsline_dest, dest_addr, chroma);
}


/* ARGB4444 ------------------------------------------------------------------*/

static uint32_t _ARGB4444_color(uint32_t color, uint8_t alpha) {
	// Status: Function Completed
	uint32_t r = ((color & 0x000000FF) >> 4 ) << 0;
	uint32_t g = ((color & 0x0000FF00) >> 12) << 4;
	uint32_t b = ((color & 0x00FF0000) >> 20) << 8;
	uint32_t a = ((uint32_t)alpha >> 4) << 12;
	return r | g | b | a;
}


static uint32_t _ARGB4444_alpha(uint32_t color, uint8_t alpha) {
	// Status: Function Completed
	uint32_t c = color & 0x0000FFF;
	uint32_t a = ((uint32_t)alpha >> 4) << 12;
	return c | a;
}


static void _ARGB4444_dma2dwait() {
	// Status: Function Completed
	while (BSP_hlcd.priv_.dma2d_state == LCD_DMA2D_BUSY) {};
}


static void _ARGB4444_updatepixel(LCD_HandleTypeDef *lcd, uint32_t offset, int16_t x, int16_t y, uint32_t value) {
	// Status: Function Completed
	if (x >= LCD_WIDTH) return;
	if (x < 0) return;
	if (y >= LCD_HEIGHT) return;
	if (y < 0) return;

	uint16_t *addr = (uint16_t *)offset + (x + y * LCD_WIDTH);
	*addr = (uint16_t)value;
}


static uint32_t _ARGB4444_getpixel(LCD_HandleTypeDef *lcd, uint32_t offset,  int16_t x, int16_t y) {
	// Status: Function Completed
	if (x >= LCD_WIDTH) return 0x00000000;
	if (x < 0) return 0x00000000;
	if (y >= LCD_HEIGHT) return 0x00000000;
	if (y < 0) return 0x00000000;

	uint16_t *addr = (uint16_t *)offset + (x + y * LCD_WIDTH);
	return *addr;
}


static void _ARGB4444_fillbuf(LCD_HandleTypeDef *lcd, uint8_t layer, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t offsetline, uint32_t color) {
	// Status: Function Completed
	uint8_t eframe = lcd->Layers[layer].Frame_EDIT;
	uint32_t dest_addr = lcd->Layers[layer].Frames[eframe] + ((x + y * LCD_WIDTH) << 1);

	_ARGB4444_dma2dwait();

	// Starting DMA2D
	BSP_hlcd.priv_.dma2d_state = LCD_DMA2D_BUSY;

	BSP_STM32_DMA2D_FillBuff(DMA2D, DMA2D_ARGB4444, width, height, offsetline, dest_addr, color);
}


static void _ARGB4444_copybuf(LCD_HandleTypeDef *lcd, uint8_t layer, uint32_t src_addr, uint16_t offsline_src, uint16_t x_dest, uint16_t y_dest, uint16_t offsline_dest, uint16_t width, uint16_t height) {
	// Status: Function Completed
	uint8_t eframe = lcd->Layers[layer].Frame_EDIT;
	uint32_t dest_addr = lcd->Layers[layer].Frames[eframe] + ((x_dest + y_dest * LCD_WIDTH) << 1);

	_ARGB4444_dma2dwait();

	// Starting DMA2D
	BSP_hlcd.priv_.dma2d_state = LCD_DMA2D_BUSY;

	BSP_STM32_DMA2D_CopyBuf(DMA2D, DMA2D_ARGB4444, width, height, offsline_src, src_addr, offsline_dest, dest_addr);
}


static void _ARGB4444_copybufblend(LCD_HandleTypeDef *lcd, uint8_t layer, uint32_t src_addr, uint16_t offsline_src, uint16_t x_dest, uint16_t y_dest, uint16_t offsline_dest, uint16_t width, uint16_t height, uint8_t alpha) {
	// Status: Function Completed
	uint8_t eframe = lcd->Layers[layer].Frame_EDIT;
	uint32_t dest_addr = lcd->Layers[layer].Frames[eframe] + ((x_dest + y_dest * LCD_WIDTH) << 1);

	_ARGB4444_dma2dwait();

	// Starting DMA2D
	BSP_hlcd.priv_.dma2d_state = LCD_DMA2D_BUSY;

	BSP_STM32_DMA2D_CopyBufBlend(DMA2D, DMA2D_ARGB4444, width, height, offsline_src, src_addr, offsline_dest, dest_addr, alpha);
}


static void _ARGB4444_copybufJPEG(LCD_HandleTypeDef *lcd, uint8_t layer, uint16_t x_dest, uint16_t y_dest) {
	uint8_t eframe = lcd->Layers[layer].Frame_EDIT;
	uint32_t dest_addr = lcd->Layers[layer].Frames[eframe] + ((x_dest + y_dest * LCD_WIDTH) << 1);
	uint32_t chroma = 0;

	if (BSP_STM32_JPEG_GetColorSpace(JPEG) != JPEG_YCBCR_COLORSPACE) return;

	uint32_t width = BSP_STM32_JPEG_GetWidth(JPEG);
	uint32_t offsline_dest = LCD_WIDTH - BSP_STM32_JPEG_GetWidth(JPEG);
	uint32_t offsline_src = 0;

	switch (BSP_STM32_JPEG_GetCSS(JPEG)) {
	case JPEG_420_SUBSAMPLING:
		chroma = DMA2D_CSS_420;
		if (width % 16) offsline_src = 16 - (width % 16);
		break;
	case JPEG_422_SUBSAMPLING:
		chroma = DMA2D_CSS_422;
		if (width % 16) offsline_src = 16 - (width % 16);
		break;
	case JPEG_444_SUBSAMPLING:
		chroma = DMA2D_CSS_444;
		if (width % 8) offsline_src = 8 - (width % 8);
		break;
	default:
		return;
	}

	// Wait for any previous DMA2D operation to complete
	_ARGB4444_dma2dwait();

	// Starting DMA2D color space conversion and copy to frame buffer.
	BSP_hlcd.priv_.dma2d_state = LCD_DMA2D_BUSY;

	BSP_STM32_DMA2D_CopyBufJPEG(DMA2D, DMA2D_ARGB4444, BSP_STM32_JPEG_GetWidth(JPEG), BSP_STM32_JPEG_GetHeight(JPEG), offsline_src, lcd->JPEGBuf, offsline_dest, dest_addr, chroma);
}



/* RGB888 --------------------------------------------------------------------*/

static uint32_t _RGB888_color(uint32_t color, uint8_t alpha) {
	// Status: Function Completed
	// alpha is ignored
	return color;
}


static uint32_t _RGB888_alpha(uint32_t color, uint8_t alpha) {
	// Status: Function Completed
	// alpha is ignored
	return color;
}


static void _RGB888_dma2dwait() {
	// Status: Function Completed
	while (BSP_hlcd.priv_.dma2d_state == LCD_DMA2D_BUSY) {};
}


static void _RGB888_updatepixel(LCD_HandleTypeDef *lcd, uint32_t offset, int16_t x, int16_t y, uint32_t value) {
	// Status: Function Completed
	if (x >= LCD_WIDTH) return;
	if (x < 0) return;
	if (y >= LCD_HEIGHT) return;
	if (y < 0) return;

	uint32_t pvalue = (uint32_t)&value;
	uint32_t addr = offset + ((x + y * LCD_WIDTH) * 3);

	*(uint8_t *)(addr++) = *(uint8_t *)(pvalue++);
	*(uint8_t *)(addr++) = *(uint8_t *)(pvalue++);
	*(uint8_t *)(addr  ) = *(uint8_t *)(pvalue  );
}


static uint32_t _RGB888_getpixel(LCD_HandleTypeDef *lcd, uint32_t offset,  int16_t x, int16_t y) {
	// Status: Function Completed
	if (x >= LCD_WIDTH) return 0x00000000;
	if (x < 0) return 0x00000000;
	if (y >= LCD_HEIGHT) return 0x00000000;
	if (y < 0) return 0x00000000;

	uint32_t addr = offset + ((x + y * LCD_WIDTH) * 3);

	if (addr & 1) return (*(uint8_t *)(addr + 1)) | (*(uint16_t *)(addr) << 8);
	return (*(uint16_t *)(addr + 1)) | (*(uint8_t *)(addr) << 16);
}


static void _RGB888_fillbuf(LCD_HandleTypeDef *lcd, uint8_t layer, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t offsetline, uint32_t color) {
	// Status: Function Completed
	uint8_t eframe = lcd->Layers[layer].Frame_EDIT;
	uint32_t dest_addr = lcd->Layers[layer].Frames[eframe] + (x + y * LCD_WIDTH) * 3;

	_RGB888_dma2dwait();

	// Starting DMA2D
	BSP_hlcd.priv_.dma2d_state = LCD_DMA2D_BUSY;

	BSP_STM32_DMA2D_FillBuff(DMA2D, DMA2D_RGB888, width, height, offsetline, dest_addr, color);
}


static void _RGB888_copybuf(LCD_HandleTypeDef *lcd, uint8_t layer, uint32_t src_addr, uint16_t offsline_src, uint16_t x_dest, uint16_t y_dest, uint16_t offsline_dest, uint16_t width, uint16_t height) {
	// Status: Function Completed
	uint8_t eframe = lcd->Layers[layer].Frame_EDIT;
	uint32_t dest_addr = lcd->Layers[layer].Frames[eframe] + ((x_dest + y_dest * LCD_WIDTH) * 3);

	_RGB888_dma2dwait();

	// Starting DMA2D
	BSP_hlcd.priv_.dma2d_state = LCD_DMA2D_BUSY;

	BSP_STM32_DMA2D_CopyBuf(DMA2D, DMA2D_RGB888, width, height, offsline_src, src_addr, offsline_dest, dest_addr);
}


static void _RGB888_copybufblend(LCD_HandleTypeDef *lcd, uint8_t layer, uint32_t src_addr, uint16_t offsline_src, uint16_t x_dest, uint16_t y_dest, uint16_t offsline_dest, uint16_t width, uint16_t height, uint8_t alpha) {
	// Status: Function Completed

	// Blending not available in modes without ALPHA channel
	_RGB888_copybuf(lcd, layer, src_addr, offsline_src, x_dest, y_dest, offsline_dest, width, height);
}


static void _RGB888_copybufJPEG(LCD_HandleTypeDef *lcd, uint8_t layer, uint16_t x_dest, uint16_t y_dest) {
	uint8_t eframe = lcd->Layers[layer].Frame_EDIT;
	uint32_t dest_addr = lcd->Layers[layer].Frames[eframe] + ((x_dest + y_dest * LCD_WIDTH) * 3);
	uint32_t chroma = 0;

	if (BSP_STM32_JPEG_GetColorSpace(JPEG) != JPEG_YCBCR_COLORSPACE) return;

	uint32_t width = BSP_STM32_JPEG_GetWidth(JPEG);
	uint32_t offsline_dest = LCD_WIDTH - BSP_STM32_JPEG_GetWidth(JPEG);
	uint32_t offsline_src = 0;

	switch (BSP_STM32_JPEG_GetCSS(JPEG)) {
	case JPEG_420_SUBSAMPLING:
		chroma = DMA2D_CSS_420;
		if (width % 16) offsline_src = 16 - (width % 16);
		break;
	case JPEG_422_SUBSAMPLING:
		chroma = DMA2D_CSS_422;
		if (width % 16) offsline_src = 16 - (width % 16);
		break;
	case JPEG_444_SUBSAMPLING:
		chroma = DMA2D_CSS_444;
		if (width % 8) offsline_src = 8 - (width % 8);
		break;
	default:
		return;
	}

	// Wait for any previous DMA2D operation to complete
	_ARGB8888_dma2dwait();

	// Starting DMA2D color space conversion and copy to frame buffer.
	BSP_hlcd.priv_.dma2d_state = LCD_DMA2D_BUSY;

	BSP_STM32_DMA2D_CopyBufJPEG(DMA2D, DMA2D_RGB888, BSP_STM32_JPEG_GetWidth(JPEG), BSP_STM32_JPEG_GetHeight(JPEG), offsline_src, lcd->JPEGBuf, offsline_dest, dest_addr, chroma);
}


/* AL88 ----------------------------------------------------------------------*/

static uint32_t _AL88_color(uint32_t color, uint8_t alpha) {
	// Status: Function Completed
	uint32_t match_c = 0;
	uint32_t min_dist = 0xffffffff;
	uint32_t dist;

	for (uint16_t m=0; m<256; m++) {
		dist = _color_dist(color, C_LUT_COLOR[m]);
		if (dist < min_dist) {
			min_dist = dist;
			match_c = m;
		}
	}

	return (match_c) | ((uint32_t)alpha << 8) | (match_c << 16) | ((uint32_t)alpha << 24);
}


static uint32_t _AL88_alpha(uint32_t color, uint8_t alpha) {
	// Status: Function Completed
	return (color & 0x00FF00FF) | ((uint32_t)alpha << 8) | ((uint32_t)alpha << 24);
}


static void _AL88_dma2dwait() {
	// Status: Function Completed
	while (BSP_hlcd.priv_.dma2d_state == LCD_DMA2D_BUSY) {};
}


static void _AL88_updatepixel(LCD_HandleTypeDef *lcd, uint32_t offset, int16_t x, int16_t y, uint32_t value) {
	// Status: Function Completed
	if (x >= LCD_WIDTH) return;
	if (x < 0) return;
	if (y >= LCD_HEIGHT) return;
	if (y < 0) return;

	uint16_t *pvalue16 = (uint16_t *)&value;
	uint16_t *addr = (uint16_t *)offset + (x + y * LCD_WIDTH);
	*addr = *pvalue16;
}

static uint32_t _AL88_getpixel(LCD_HandleTypeDef *lcd, uint32_t offset,  int16_t x, int16_t y) {
	// Status: Function Completed
	if (x >= LCD_WIDTH) return 0x00000000;
	if (x < 0) return 0x00000000;
	if (y >= LCD_HEIGHT) return 0x00000000;
	if (y < 0) return 0x00000000;

	uint16_t *addr = (uint16_t *)offset + (x + y * LCD_WIDTH);
	return *addr;
}


static void _AL88_fillbuf(LCD_HandleTypeDef *lcd, uint8_t layer, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t offsetline, uint32_t color) {
	// Status: Function Completed

	uint8_t eframe = lcd->Layers[layer].Frame_EDIT;
	uint32_t dest_addr = lcd->Layers[layer].Frames[eframe] + ((x + y * LCD_WIDTH) << 1);

	_AL88_dma2dwait();

	// Starting DMA2D
	BSP_hlcd.priv_.dma2d_state = LCD_DMA2D_BUSY;

	// RGB5656 mode used for 16 bits transfer (officially AL mode is not supported).
	BSP_STM32_DMA2D_FillBuff(DMA2D, DMA2D_RGB565, width, height, offsetline, dest_addr, color);

}


static void _AL88_copybuf(LCD_HandleTypeDef *lcd, uint8_t layer, uint32_t src_addr, uint16_t offsline_src, uint16_t x_dest, uint16_t y_dest, uint16_t offsline_dest, uint16_t width, uint16_t height) {
	// Status: Function Completed

	uint8_t eframe = lcd->Layers[layer].Frame_EDIT;
	uint16_t *dest_addr = (uint16_t *)lcd->Layers[layer].Frames[eframe] + (x_dest + y_dest * LCD_WIDTH);

	_AL88_dma2dwait();

	// Starting DMA2D
	BSP_hlcd.priv_.dma2d_state = LCD_DMA2D_BUSY;

	// RGB5656 mode used for 16 bits transfer (officially AL mode is not supported).
	BSP_STM32_DMA2D_CopyBuf(DMA2D, DMA2D_RGB565, width, height, offsline_src, src_addr, offsline_dest, (uint32_t)dest_addr);
}


static void _AL88_copybufblend(LCD_HandleTypeDef *lcd, uint8_t layer, uint32_t src_addr, uint16_t offsline_src, uint16_t x_dest, uint16_t y_dest, uint16_t offsline_dest, uint16_t width, uint16_t height, uint8_t alpha) {
	// Status: Function Completed
	// Effective and fast blending is not possible with CLUT mode
	_AL88_copybuf(lcd, layer, src_addr, offsline_src, x_dest, y_dest, offsline_dest, width, height);
}


static void _AL88_copybufJPEG(LCD_HandleTypeDef *lcd, uint8_t layer, uint16_t x_dest, uint16_t y_dest) {
	// Function not available in AL color mode. Instead area is filled with black color.
	uint32_t width = BSP_STM32_JPEG_GetWidth(JPEG);
	uint32_t height = BSP_STM32_JPEG_GetHeight(JPEG);

	_AL88_fillbuf(lcd, layer, x_dest, y_dest, width, height, LCD_WIDTH - width, BSP_LCD_Color(C_BLACK, 255));
}

/* L8 ------------------------------------------------------------------------*/

static uint32_t _L8_color(uint32_t color, uint8_t alpha) {
	// Status: Function Completed
	// Alpha is ignored in this color mode

	uint32_t match_c = 0;
	uint32_t min_dist = 0xffffffff;
	uint32_t dist;

	for (uint16_t m=0; m<256; m++) {
		dist = _color_dist(color, C_LUT_COLOR[m]);
		if (dist < min_dist) {
			min_dist = dist;
			match_c = m;
		}
	}

	return (match_c) | (match_c << 8) | (match_c << 16) | (match_c << 24);
}


static uint32_t _L8_alpha(uint32_t color, uint8_t alpha) {
	// Status: Function Completed
	// Alpha is ignored in this color mode

	return color;
}


static void _L8_dma2dwait() {
	// Status: Function Completed
	while (BSP_hlcd.priv_.dma2d_state == LCD_DMA2D_BUSY) {};
}


static void _L8_updatepixel(LCD_HandleTypeDef *lcd, uint32_t offset, int16_t x, int16_t y, uint32_t value) {
	// Status: Function Completed
	if (x >= LCD_WIDTH) return;
	if (x < 0) return;
	if (y >= LCD_HEIGHT) return;
	if (y < 0) return;

	uint8_t *pvalue8 = (uint8_t *)&value;
	uint8_t *addr = (uint8_t *)offset + ((x + y * LCD_WIDTH));
	*addr = *pvalue8;
}


static uint32_t _L8_getpixel(LCD_HandleTypeDef *lcd, uint32_t offset,  int16_t x, int16_t y) {
	// Status: Function Completed
	if (x >= LCD_WIDTH) return 0x00000000;
	if (x < 0) return 0x00000000;
	if (y >= LCD_HEIGHT) return 0x00000000;
	if (y < 0) return 0x00000000;

	uint8_t *addr = (uint8_t *)offset + ((x + y * LCD_WIDTH));
	return *addr;
}


static void _L8_fillbuf(LCD_HandleTypeDef *lcd, uint8_t layer, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t offsetline, uint32_t color) {
	// Status: Function Completed

	// Because DMA2D cannot handle 8bit formats, below workaround was required

	uint8_t eframe = lcd->Layers[layer].Frame_EDIT;

	// Finding x0 and x1 so it will be within filled area and are divided by 4 (required by DMA2D in 32bit mode)
	uint16_t x0 = x & 0xFFFC;
	if (x0 < x) x0 += 4;
	uint16_t x1 = (x + width) & 0xFFFC;

	uint8_t *color8 = (uint8_t *)&color;
	uint8_t *addr;

	// Waiting for completion of previous DMA2D (accessing memory concurrently with DMA2D slows everything down)
	_L8_dma2dwait();

	//Manual filling areas outside DMA2D range (not aligned to 4 bytes)

	for (uint32_t yy=y; yy<(y+height); yy++) {
		for (uint32_t xx=x; xx<MIN(x0, x+width); xx++) {
			addr = (uint8_t *)(lcd->Layers[layer].Frames[eframe]) + (xx + yy*LCD_WIDTH);
			*addr = *color8;
		}
		if (x1<x0) continue;
		for (uint32_t xx=x1; xx<(x+width); xx++) {
			addr = (uint8_t *)(lcd->Layers[layer].Frames[eframe]) + (xx + yy*LCD_WIDTH);
			*addr = *color8;
		}
	}

	// Testing condition to start DMA2D
	if (x1<=x0) return;

	// Calculating width for DMA2D
	uint16_t w = x1-x0;

	// Calculating offset line for DMA2D
	uint16_t ol = offsetline + width - w;

	// Setting destination address for DMA2D
	uint32_t dest_addr = lcd->Layers[layer].Frames[eframe] + (x0 + y * LCD_WIDTH);

	// Starting DMA2D
	BSP_hlcd.priv_.dma2d_state = LCD_DMA2D_BUSY;
	BSP_STM32_DMA2D_FillBuff(DMA2D, DMA2D_ARGB8888, (w >> 2), height, (ol >> 2), dest_addr, color);
}


static void _L8_copybuf(LCD_HandleTypeDef *lcd, uint8_t layer, uint32_t src_addr, uint16_t offsline_src, uint16_t x_dest, uint16_t y_dest, uint16_t offsline_dest, uint16_t width, uint16_t height) {

	// Because DMA2D cannot handle 8bit formats, below workaround was required

	uint8_t eframe = lcd->Layers[layer].Frame_EDIT;

	uint8_t *d_addr = (uint8_t *)lcd->Layers[layer].Frames[eframe] + ((x_dest + y_dest * LCD_WIDTH));
	uint8_t *s_addr = (uint8_t *)src_addr;
	uint32_t size = 0;

	_L8_dma2dwait();

	if (((uint32_t)s_addr % 2) || ((uint32_t)d_addr % 2)>0) {
		// Cannot use DMA2D
		while (size<(width * height)) {
			*d_addr = *s_addr;
			size++;
			if (!(size % width)) {
				d_addr+=offsline_dest;
				s_addr+=offsline_src;
			}
			d_addr++;
			s_addr++;
		}
		return;

	} else {
		// Can use DMA2D
		uint16_t w = width & 0xFFFE;
		uint16_t os = offsline_src + width - w;
		uint16_t od = offsline_dest + width - w;

		// Fill area not covered by DMA2D (if required)
		if (w < width) {
			uint8_t *dd_addr = d_addr + width - 1;
			uint8_t *ss_addr = s_addr + width - 1;
			for (uint16_t yy = 0; yy< height; yy++) {
				*dd_addr = *ss_addr;
				dd_addr+=offsline_dest;
				ss_addr+=offsline_src;
			}
		}

		// DMA2D Start
		BSP_hlcd.priv_.dma2d_state = LCD_DMA2D_BUSY;

		BSP_STM32_DMA2D_CopyBuf(DMA2D, DMA2D_RGB565, (w >> 1), height, (os >> 1), (uint32_t)s_addr, (od >> 1), (uint32_t)d_addr);

	}


}


static void _L8_copybufblend(LCD_HandleTypeDef *lcd, uint8_t layer, uint32_t src_addr, uint16_t offsline_src, uint16_t x_dest, uint16_t y_dest, uint16_t offsline_dest, uint16_t width, uint16_t height, uint8_t alpha) {
	// Status: Function Completed
	// Effective and fast blending is not possible with CLUT mode
	_L8_copybuf(lcd, layer, src_addr, offsline_src, x_dest, y_dest, offsline_dest, width, height);
}


static void _L8_copybufJPEG(LCD_HandleTypeDef *lcd, uint8_t layer, uint16_t x_dest, uint16_t y_dest) {
	// Function not available in AL color mode. Instead area is filled with black color.
	uint32_t width = BSP_STM32_JPEG_GetWidth(JPEG);
	uint32_t height = BSP_STM32_JPEG_GetHeight(JPEG);

	_L8_fillbuf(lcd, layer, x_dest, y_dest, width, height, LCD_WIDTH - width, BSP_LCD_Color(C_BLACK, 255));
}


/******************************************************************************
 * Config functions for various modes
 ******************************************************************************/

static void _config_triplebuf(LCD_HandleTypeDef *lcd) {
	// Setting up layer 0 for 3 buffers
	lcd->Layers[0].Frame_IDLE = 2;
	lcd->Layers[0].Frame_EDIT = 1;
	lcd->Layers[0].Frame_READY = 255;
	lcd->Layers[0].Frame_ACTIVE = 0;
	lcd->Layers[0].Frame_NOUSE = 255;
	lcd->Layers[0].Frame_PREV = 0;
	lcd->Layers[0].Frames[0] = LCD_FRAMEBUFFER_END_ADDR - 1 * lcd->config_.framesize;
	lcd->Layers[0].Frames[1] = LCD_FRAMEBUFFER_END_ADDR - 2 * lcd->config_.framesize;
	lcd->Layers[0].Frames[2] = LCD_FRAMEBUFFER_END_ADDR - 3 * lcd->config_.framesize;
	lcd->JPEGBuf = LCD_FRAMEBUFFER_END_ADDR - (3 * lcd->config_.framesize) - LCD_JPEGBUF_SIZE;

	if (lcd->config_.layerno == LCD_LAYER_NO_ONE) {
		// Setting up layer 1 as idle
		lcd->Layers[1].Frame_IDLE = 0;
		lcd->Layers[1].Frame_EDIT = 0;
		lcd->Layers[1].Frame_READY = 0;
		lcd->Layers[1].Frame_ACTIVE = 0;
		lcd->Layers[1].Frame_NOUSE = 0;
		lcd->Layers[1].Frame_PREV = 0;
		lcd->Layers[1].Frames[0] = 0;
		lcd->Layers[1].Frames[1] = 0;
		lcd->Layers[1].Frames[2] = 0;
	} else {
		// Setting up layer 1 for 3 buffers
		lcd->Layers[1].Frame_IDLE = 2;
		lcd->Layers[1].Frame_EDIT = 1;
		lcd->Layers[1].Frame_READY = 255;
		lcd->Layers[1].Frame_ACTIVE = 0;
		lcd->Layers[1].Frame_NOUSE = 255;
		lcd->Layers[1].Frame_PREV = 0;
		lcd->Layers[1].Frames[0] = LCD_FRAMEBUFFER_END_ADDR - 4 * lcd->config_.framesize;
		lcd->Layers[1].Frames[1] = LCD_FRAMEBUFFER_END_ADDR - 5 * lcd->config_.framesize;
		lcd->Layers[1].Frames[2] = LCD_FRAMEBUFFER_END_ADDR - 6 * lcd->config_.framesize;
		lcd->JPEGBuf = LCD_FRAMEBUFFER_END_ADDR - (6 * lcd->config_.framesize) - LCD_JPEGBUF_SIZE;
	}
}


static void _config_doublebuf(LCD_HandleTypeDef *lcd) {
	// Setting up layer 0 for 2 buffers
	lcd->Layers[0].Frame_IDLE = 255;
	lcd->Layers[0].Frame_EDIT = 1;
	lcd->Layers[0].Frame_READY = 255;
	lcd->Layers[0].Frame_ACTIVE = 0;
	lcd->Layers[0].Frame_NOUSE = 2;
	lcd->Layers[0].Frame_PREV = 0;
	lcd->Layers[0].Frames[0] = LCD_FRAMEBUFFER_END_ADDR - 1 * lcd->config_.framesize;
	lcd->Layers[0].Frames[1] = LCD_FRAMEBUFFER_END_ADDR - 2 * lcd->config_.framesize;
	lcd->Layers[0].Frames[2] = 0;
	lcd->JPEGBuf = LCD_FRAMEBUFFER_END_ADDR - (2 * lcd->config_.framesize) - LCD_JPEGBUF_SIZE;

	if (lcd->config_.layerno == LCD_LAYER_NO_ONE) {
		// Setting up layer 1 as idle
		lcd->Layers[1].Frame_IDLE = 0;
		lcd->Layers[1].Frame_EDIT = 0;
		lcd->Layers[1].Frame_READY = 0;
		lcd->Layers[1].Frame_ACTIVE = 0;
		lcd->Layers[1].Frame_NOUSE = 0;
		lcd->Layers[1].Frame_PREV = 0;
		lcd->Layers[1].Frames[0] = 0;
		lcd->Layers[1].Frames[1] = 0;
		lcd->Layers[1].Frames[2] = 0;
	} else {
		// Setting up layer 1 for 2 buffers
		lcd->Layers[1].Frame_IDLE = 255;
		lcd->Layers[1].Frame_EDIT = 1;
		lcd->Layers[1].Frame_READY = 255;
		lcd->Layers[1].Frame_ACTIVE = 0;
		lcd->Layers[1].Frame_NOUSE = 2;
		lcd->Layers[1].Frame_PREV = 0;
		lcd->Layers[1].Frames[0] = LCD_FRAMEBUFFER_END_ADDR - 3 * lcd->config_.framesize;
		lcd->Layers[1].Frames[1] = LCD_FRAMEBUFFER_END_ADDR - 4 * lcd->config_.framesize;
		lcd->Layers[1].Frames[2] = 0;
		lcd->JPEGBuf = LCD_FRAMEBUFFER_END_ADDR - (4 * lcd->config_.framesize) - LCD_JPEGBUF_SIZE;
	}
}


static void _ARGB8888_config(LCD_HandleTypeDef *lcd) {
	lcd->config_.bytesperpixel = 4;
	lcd->config_.framesize *=4;
	lcd->config_.framebuffersize = lcd->config_.framesize * lcd->config_.buffermode * lcd->config_.layerno;

	// Updating function pointers
	BSP_LCD_Color = _ARGB8888_color;
	BSP_LCD_Alpha = _ARGB8888_alpha;
	BSP_LCD_DMA2D_Wait = _ARGB8888_dma2dwait;
	BSP_LCD_UpdatePixel = _ARGB8888_updatepixel;
	BSP_LCD_GetPixel = _ARGB8888_getpixel;
	BSP_LCD_FillBuf = _ARGB8888_fillbuf;
	BSP_LCD_CopyBuf = _ARGB8888_copybuf;
	BSP_LCD_CopyBufBlend = _ARGB8888_copybufblend;
	BSP_LCD_CopyBufJPEG = _ARGB8888_copybufJPEG;
}

static void _ARGB8888_config_layer(LCD_HandleTypeDef *lcd, uint8_t layer) {

	// Configure Layer
	BSP_STM32_LTDC_ConfigLayer(
			LTDC,
			layer,
			255,
			0,
			0x00000000,
			LTDC_BLENDING_FACTOR1_PAxCA,
			LTDC_BLENDING_FACTOR2_PAxCA,
			lcd->Layers[layer].Frames[0],
			LCD_HEIGHT, LCD_WIDTH,
			LTDC_PIXEL_FORMAT_ARGB8888,
			0,
			LCD_WIDTH,
			0,
			LCD_HEIGHT);

}

static void _ARGB1555_config(LCD_HandleTypeDef *lcd) {
	lcd->config_.bytesperpixel = 2;
	lcd->config_.framesize *=2;
	lcd->config_.framebuffersize = lcd->config_.framesize * lcd->config_.buffermode * lcd->config_.layerno;

	// Updating function pointers
	BSP_LCD_Color = _ARGB1555_color;
	BSP_LCD_Alpha = _ARGB1555_alpha;
	BSP_LCD_DMA2D_Wait = _ARGB1555_dma2dwait;
	BSP_LCD_UpdatePixel = _ARGB1555_updatepixel;
	BSP_LCD_GetPixel = _ARGB1555_getpixel;
	BSP_LCD_FillBuf = _ARGB1555_fillbuf;
	BSP_LCD_CopyBuf = _ARGB1555_copybuf;
	BSP_LCD_CopyBufBlend = _ARGB1555_copybufblend;
	BSP_LCD_CopyBufJPEG = _ARGB1555_copybufJPEG;
}

static void _ARGB1555_config_layer(LCD_HandleTypeDef *lcd, uint8_t layer) {

	// Configure Layer
	BSP_STM32_LTDC_ConfigLayer(
			LTDC,
			layer,
			255,
			0,
			0x00000000,
			LTDC_BLENDING_FACTOR1_PAxCA,
			LTDC_BLENDING_FACTOR2_PAxCA,
			lcd->Layers[layer].Frames[0],
			LCD_HEIGHT, LCD_WIDTH,
			LTDC_PIXEL_FORMAT_ARGB1555,
			0,
			LCD_WIDTH,
			0,
			LCD_HEIGHT);

}


static void _ARGB4444_config(LCD_HandleTypeDef *lcd) {
	lcd->config_.bytesperpixel = 2;
	lcd->config_.framesize *=2;
	lcd->config_.framebuffersize = lcd->config_.framesize * lcd->config_.buffermode * lcd->config_.layerno;

	// Updating function pointers
	BSP_LCD_Color = _ARGB4444_color;
	BSP_LCD_Alpha = _ARGB4444_alpha;
	BSP_LCD_DMA2D_Wait = _ARGB4444_dma2dwait;
	BSP_LCD_UpdatePixel = _ARGB4444_updatepixel;
	BSP_LCD_GetPixel = _ARGB4444_getpixel;
	BSP_LCD_FillBuf = _ARGB4444_fillbuf;
	BSP_LCD_CopyBuf = _ARGB4444_copybuf;
	BSP_LCD_CopyBufBlend = _ARGB4444_copybufblend;
	BSP_LCD_CopyBufJPEG = _ARGB4444_copybufJPEG;
}

static void _ARGB4444_config_layer(LCD_HandleTypeDef *lcd, uint8_t layer) {

	// Configure Layer
	BSP_STM32_LTDC_ConfigLayer(
			LTDC,
			layer,
			255,
			0,
			0x00000000,
			LTDC_BLENDING_FACTOR1_PAxCA,
			LTDC_BLENDING_FACTOR2_PAxCA,
			lcd->Layers[layer].Frames[0],
			LCD_HEIGHT, LCD_WIDTH,
			LTDC_PIXEL_FORMAT_ARGB4444,
			0,
			LCD_WIDTH,
			0,
			LCD_HEIGHT);

}

static void _RGB888_config(LCD_HandleTypeDef *lcd) {
	lcd->config_.bytesperpixel = 3;
	lcd->config_.framesize *=3;
	lcd->config_.framebuffersize = lcd->config_.framesize * lcd->config_.buffermode * lcd->config_.layerno;

	// Updating function pointers
	BSP_LCD_Color = _RGB888_color;
	BSP_LCD_Alpha = _RGB888_alpha;
	BSP_LCD_DMA2D_Wait = _RGB888_dma2dwait;
	BSP_LCD_UpdatePixel = _RGB888_updatepixel;
	BSP_LCD_GetPixel = _RGB888_getpixel;
	BSP_LCD_FillBuf = _RGB888_fillbuf;
	BSP_LCD_CopyBuf = _RGB888_copybuf;
	BSP_LCD_CopyBufBlend = _RGB888_copybufblend;
	BSP_LCD_CopyBufJPEG = _RGB888_copybufJPEG;
}

static void _RGB888_config_layer(LCD_HandleTypeDef *lcd, uint8_t layer) {

	// Configure Layer
	BSP_STM32_LTDC_ConfigLayer(
			LTDC,
			layer,
			255,
			0,
			0x00000000,
			LTDC_BLENDING_FACTOR1_CA,
			LTDC_BLENDING_FACTOR2_CA,
			lcd->Layers[layer].Frames[0],
			LCD_HEIGHT,
			LCD_WIDTH,
			LTDC_PIXEL_FORMAT_RGB888,
			0,
			LCD_WIDTH,
			0,
			LCD_HEIGHT);

	// Configure color keying so 0x00000000 is transparent
	BSP_STM32_LTDC_SetTransparentColor(LTDC, layer, 0x00000000);
}

static void _AL88_config(LCD_HandleTypeDef *lcd) {
	lcd->config_.bytesperpixel = 2;
	lcd->config_.framesize *=2;
	lcd->config_.framebuffersize = lcd->config_.framesize * lcd->config_.buffermode * lcd->config_.layerno;

	// Updating function pointers
	BSP_LCD_Color = _AL88_color;
	BSP_LCD_Alpha = _AL88_alpha;
	BSP_LCD_DMA2D_Wait = _AL88_dma2dwait;
	BSP_LCD_UpdatePixel = _AL88_updatepixel;
	BSP_LCD_GetPixel = _AL88_getpixel;
	BSP_LCD_FillBuf = _AL88_fillbuf;
	BSP_LCD_CopyBuf = _AL88_copybuf;
	BSP_LCD_CopyBufBlend = _AL88_copybufblend;
	BSP_LCD_CopyBufJPEG = _AL88_copybufJPEG;
}

static void _AL88_config_layer(LCD_HandleTypeDef *lcd, uint8_t layer, uint32_t *clut) {

	// Configure Layer
	BSP_STM32_LTDC_ConfigLayer(
			LTDC,
			layer,
			255,
			0,
			0x00000000,
			LTDC_BLENDING_FACTOR1_PAxCA,
			LTDC_BLENDING_FACTOR2_PAxCA,
			lcd->Layers[layer].Frames[0],
			LCD_HEIGHT,
			LCD_WIDTH,
			LTDC_PIXEL_FORMAT_AL88,
			0,
			LCD_WIDTH,
			0,
			LCD_HEIGHT);

	BSP_STM32_LTDC_ConfigCLUT(LTDC, layer, clut);
}

static void _L8_config(LCD_HandleTypeDef *lcd) {
	lcd->config_.bytesperpixel = 1;
	lcd->config_.framesize *=1;
	lcd->config_.framebuffersize = lcd->config_.framesize * lcd->config_.buffermode * lcd->config_.layerno;

	// Updating function pointers
	BSP_LCD_Color = _L8_color;
	BSP_LCD_Alpha = _L8_alpha;
	BSP_LCD_DMA2D_Wait = _L8_dma2dwait;
	BSP_LCD_UpdatePixel = _L8_updatepixel;
	BSP_LCD_GetPixel = _L8_getpixel;
	BSP_LCD_FillBuf = _L8_fillbuf;
	BSP_LCD_CopyBuf = _L8_copybuf;
	BSP_LCD_CopyBufBlend = _L8_copybufblend;
	BSP_LCD_CopyBufJPEG = _L8_copybufJPEG;
}

static void _L8_config_layer(LCD_HandleTypeDef *lcd, uint8_t layer, uint32_t *clut) {

	// Configure Layer
	BSP_STM32_LTDC_ConfigLayer(
			LTDC,
			layer,
			255,
			0,
			0x00000000,
			LTDC_BLENDING_FACTOR1_CA,
			LTDC_BLENDING_FACTOR2_CA,
			lcd->Layers[layer].Frames[0],
			LCD_HEIGHT,
			LCD_WIDTH,
			LTDC_PIXEL_FORMAT_L8,
			0,
			LCD_WIDTH,
			0,
			LCD_HEIGHT);

	BSP_STM32_LTDC_ConfigCLUT(LTDC, layer, clut);

}


/******************************************************************************
 * Public functions
 ******************************************************************************/


void BSP_LCD_Init(LCD_HandleTypeDef *lcd, uint8_t color_mode, uint8_t buffer_mode, uint8_t layer_no, uint32_t bgcolor, uint32_t *clut) {
	// CLUT parameter is ignored in ARGB and RGB modes

	// Setting up config variables
	lcd->frametime[0] = 0;
	lcd->frametime[1] = 0;
	lcd->priv_.dma2d_state = LCD_DMA2D_READY;
	lcd->priv_.l_timestamp[0] = 0;
	lcd->priv_.l_timestamp[1] = 0;
	lcd->config_.colormode = color_mode;
	lcd->config_.buffermode = buffer_mode;
	lcd->config_.layerno = layer_no;
	lcd->config_.bgcolor = bgcolor;
	lcd->config_.framesize = LCD_WIDTH * LCD_HEIGHT;

	// Configuring color mode
	switch (lcd->config_.colormode) {
	case LCD_COLOR_MODE_ARGB8888:
		_ARGB8888_config(lcd);
		break;
	case LCD_COLOR_MODE_ARGB1555:
		_ARGB1555_config(lcd);
		break;
	case LCD_COLOR_MODE_ARGB4444:
		_ARGB4444_config(lcd);
		break;
	case LCD_COLOR_MODE_RGB888:
		_RGB888_config(lcd);
		break;
	case LCD_COLOR_MODE_AL88:
		_AL88_config(lcd);
		break;
	case LCD_COLOR_MODE_L8:
		_L8_config(lcd);
		break;
	}

	// Clearing memory for frame buffer
	memset((void *)(LCD_FRAMEBUFFER_END_ADDR - lcd->config_.framebuffersize - LCD_JPEGBUF_SIZE),0x00, (lcd->config_.framebuffersize + LCD_JPEGBUF_SIZE));

	// Configuring frame buffer parameters
	switch (lcd->config_.buffermode) {
	case LCD_BUFFER_MODE_TRIPLE:
		_config_triplebuf(lcd);
		break;
	case LCD_BUFFER_MODE_DOUBLE:
		_config_doublebuf(lcd);
		break;
	}

	// Configuring LTDC controller
	BSP_STM32_LTDC_Init(LTDC, LCD_H_SYNC, LCD_V_SYNC, LCD_ACC_H_BACK_PORCH_WIDTH, LCD_ACC_V_BACK_PORCH_HEIGHT,
			LCD_ACC_ACTIVE_WIDTH, LCD_ACC_ACTIVE_HEIGHT, LCD_TOTAL_WIDTH, LCD_TOTAL_HEIGHT, bgcolor);

	// Configuring JPEG hardware codec
	BSP_STM32_JPEG_Init(JPEG);

	// Disabling both layers
	BSP_STM32_LTDC_DisableLayer(LTDC, 0);
	BSP_STM32_LTDC_DisableLayer(LTDC, 1);

	// Configuring LTDC Layers
	switch (lcd->config_.colormode) {
	case LCD_COLOR_MODE_ARGB8888:
		_ARGB8888_config_layer(lcd, 0);
		if (lcd->config_.layerno == LCD_LAYER_NO_TWO) _ARGB8888_config_layer(lcd, 1);
		break;
	case LCD_COLOR_MODE_ARGB1555:
		_ARGB1555_config_layer(lcd, 0);
		if (lcd->config_.layerno == LCD_LAYER_NO_TWO) _ARGB1555_config_layer(lcd, 1);
		break;
	case LCD_COLOR_MODE_ARGB4444:
		_ARGB4444_config_layer(lcd, 0);
		if (lcd->config_.layerno == LCD_LAYER_NO_TWO) _ARGB4444_config_layer(lcd, 1);
		break;
	case LCD_COLOR_MODE_RGB888:
		_RGB888_config_layer(lcd, 0);
		if (lcd->config_.layerno == LCD_LAYER_NO_TWO) _RGB888_config_layer(lcd, 1);
		break;
	case LCD_COLOR_MODE_AL88:
		_AL88_config_layer(lcd, 0, clut);
		if (lcd->config_.layerno == LCD_LAYER_NO_TWO) _AL88_config_layer(lcd, 1, clut);
		break;
	case LCD_COLOR_MODE_L8:
		_L8_config_layer(lcd, 0, clut);
		if (lcd->config_.layerno == LCD_LAYER_NO_TWO) _L8_config_layer(lcd, 1, clut);
		break;
	}

	// Enabling required layers
	BSP_STM32_LTDC_EnableLayer(LTDC, 0);
	if (lcd->config_.layerno == LCD_LAYER_NO_TWO) BSP_STM32_LTDC_EnableLayer(LTDC, 1);

	// Programming LTDC line interrupt
	BSP_STM32_LTDC_SetLineInt(LTDC, (uint32_t)LCD_HEIGHT);
}


void BSP_LCD_FrameReady(LCD_HandleTypeDef *lcd, uint8_t layer) {
	// Marking EDIT frame as READY for display
	// Marking previous READY frame (if exists) as EDIT, else IDLE becoming EDIT

	// Waiting for andy DMA2D in progress
	BSP_LCD_DMA2D_Wait();

	uint8_t t = lcd->Layers[layer].Frame_READY;
	lcd->Layers[layer].Frame_READY = lcd->Layers[layer].Frame_EDIT;
	lcd->Layers[layer].Frame_PREV = lcd->Layers[layer].Frame_EDIT;
	if (t<255) lcd->Layers[layer].Frame_EDIT = t;
		else {
			lcd->Layers[layer].Frame_EDIT = lcd->Layers[layer].Frame_IDLE;
			lcd->Layers[layer].Frame_IDLE = t;
		}

	// Calculating frame time [ms]. FPS = 1000 / frametime.
	uint32_t timestamp = BSP_GetTick();
	uint32_t dt = timestamp - lcd->priv_.l_timestamp[layer];
	lcd->priv_.l_timestamp[layer] = timestamp;
	lcd->frametime[layer] = (lcd->frametime[layer] * 768 + (dt << 8)) >> 10; // Moving average
}

uint8_t BSP_LCD_GetEditPermission(LCD_HandleTypeDef *lcd) {
	// Returns True when engine is ready for generating next frame (used for double frame buffer / not applicable for triple buffering)
	return ((lcd->Layers[0].Frame_EDIT<255) && (lcd->Layers[1].Frame_EDIT<255));
}

void BSP_LCD_SetLayerAlpha(LCD_HandleTypeDef *lcd, uint8_t layer, uint8_t alpha) {
	// Setting transparency of layer
	LTDC_Layer_TypeDef *l = (LTDC_Layer_TypeDef *)(((uint32_t)LTDC) + 0x84U + (0x80U*(layer)));
	// Specifies the constant alpha value
	l->CACR &= ~(LTDC_LxCACR_CONSTA);
	l->CACR = (uint32_t)alpha;
	// Set the Vertical Blanking Reload type
	LTDC->SRCR = LTDC_SRCR_VBR;
}


uint8_t BSP_LCD_GetLayerAlpha(LCD_HandleTypeDef *lcd, uint8_t layer) {
	// Returns layer's transparency
	LTDC_Layer_TypeDef *l = (LTDC_Layer_TypeDef *)(((uint32_t)LTDC) + 0x84U + (0x80U*(layer)));
	return (uint8_t)(l->CACR & LTDC_LxCACR_CONSTA);
}


void BSP_LCD_InitBackLight(LCD_HandleTypeDef *lcd, uint8_t value) {
	// PWM Timer - base configration
	// Option 1: PWM Frequency = 200MHz Input clock / Prescaler (7+1) / Reload value (999+1) = 25kHz
	// Option 2: PWM Frequency = 240MHz Input clock / Prescaler (7+1) / Reload value (999+1) = 30kHz

	lcd->priv_.bklt_setting = (int32_t)value << 6;
	lcd->priv_.bklt_value = lcd->priv_.bklt_setting;
	lcd->priv_.bklt_dimspeed = 0;

	uint32_t val = (((uint32_t)(lcd->priv_.bklt_value >> 6) * (LCD_BKL_MAX_PWM - LCD_BKL_MIN_PWM)) / 100) + LCD_BKL_MIN_PWM;

	BSP_STM32_TIM_Init(LCD_BKL_TIM, TIM_CLOCKDIVISION_DIV1, 7, 999);
	BSP_STM32_TIM_ConfigChannel(LCD_BKL_TIM, 3, 0b110, val);
	BSP_STM32_TIM_Start(LCD_BKL_TIM);
}


void BSP_LCD_SetBackLight(LCD_HandleTypeDef *lcd, uint8_t value, uint8_t dimspeed) {
	// Sets backlight value 0 - 100%

	lcd->priv_.bklt_setting = (int32_t)value << 6;
	lcd->priv_.bklt_dimspeed = (int32_t)dimspeed;

}


uint8_t LCD_GetBackLight(LCD_HandleTypeDef *lcd) {
	// Returns backlight value 0 - 100%

	uint32_t val = BSP_STM32_TIM_GetChannelValue(LCD_BKL_TIM, 3);

	uint32_t value = ((val - LCD_BKL_MIN_PWM) * 100) / (LCD_BKL_MAX_PWM - LCD_BKL_MIN_PWM);

	return value;
}


void BSP_LCD_DecodeJPEG(LCD_HandleTypeDef *lcd, uint32_t jpeg_addr, uint32_t jpeg_size) {
	// Decode JPEG into intermediate buffer (organized in YCbCr MCU blocks)
	BSP_STM32_JPEG_Decode(JPEG, jpeg_addr, jpeg_size, lcd->JPEGBuf, LCD_JPEGBUF_SIZE);
}


/******************************************************************************
 * Callback for blanking interval. Updating display buffer.
 ******************************************************************************/

// INT Handler for LTDC controller
void LTDC_IRQHandler(void)
{
	if (BSP_STM32_LTDC_IRQHandler(LTDC)==BSP_OK) {

		// Updating backlight brightness
		if (BSP_hlcd.priv_.bklt_value != BSP_hlcd.priv_.bklt_setting) {
			if (BSP_hlcd.priv_.bklt_value < BSP_hlcd.priv_.bklt_setting) {
				BSP_hlcd.priv_.bklt_value += BSP_hlcd.priv_.bklt_dimspeed;
				if (BSP_hlcd.priv_.bklt_value > BSP_hlcd.priv_.bklt_setting) BSP_hlcd.priv_.bklt_value = BSP_hlcd.priv_.bklt_setting;
			}

			if (BSP_hlcd.priv_.bklt_value > BSP_hlcd.priv_.bklt_setting) {
				BSP_hlcd.priv_.bklt_value -= BSP_hlcd.priv_.bklt_dimspeed;
				if (BSP_hlcd.priv_.bklt_value < BSP_hlcd.priv_.bklt_setting) BSP_hlcd.priv_.bklt_value = BSP_hlcd.priv_.bklt_setting;
			}

			uint32_t val = (((uint32_t)(BSP_hlcd.priv_.bklt_value >> 6) * (LCD_BKL_MAX_PWM - LCD_BKL_MIN_PWM)) / 100) + LCD_BKL_MIN_PWM;
			BSP_STM32_TIM_SetChannelValue(LCD_BKL_TIM, 3, val);
		}

		// Switching frames
		switch (BSP_hlcd.config_.buffermode) {
		case LCD_BUFFER_MODE_TRIPLE:

			for (uint8_t l=0;l<BSP_hlcd.config_.layerno; l++) if (BSP_hlcd.Layers[l].Frame_READY < 255) {
				uint8_t t = BSP_hlcd.Layers[l].Frame_ACTIVE;
				BSP_hlcd.Layers[l].Frame_ACTIVE = BSP_hlcd.Layers[l].Frame_READY;
				BSP_hlcd.Layers[l].Frame_IDLE = t;
				BSP_hlcd.Layers[l].Frame_READY = 255;

				// Updating Framebuffer address
				BSP_STM32_LTDC_UpdateFrameBufAddr(LTDC, l, BSP_hlcd.Layers[l].Frames[BSP_hlcd.Layers[l].Frame_ACTIVE]);

			}
			return;

		case LCD_BUFFER_MODE_DOUBLE:

			for (uint8_t l=0;l<BSP_hlcd.config_.layerno; l++) if (BSP_hlcd.Layers[l].Frame_READY < 255) {
				uint8_t t = BSP_hlcd.Layers[l].Frame_ACTIVE;
				BSP_hlcd.Layers[l].Frame_ACTIVE = BSP_hlcd.Layers[l].Frame_READY;
				BSP_hlcd.Layers[l].Frame_EDIT = t;
				BSP_hlcd.Layers[l].Frame_READY = 255;

				// Updating Framebuffer address
				BSP_STM32_LTDC_UpdateFrameBufAddr(LTDC, l, BSP_hlcd.Layers[l].Frames[BSP_hlcd.Layers[l].Frame_ACTIVE]);

			}
			return;

		}

	}

}

// INT Handler for DMA2D
void DMA2D_IRQHandler(void) {

	// Update state flag
	if (BSP_STM32_DMA2D_IRQHandler(DMA2D) == BSP_OK) BSP_hlcd.priv_.dma2d_state = LCD_DMA2D_READY;
}



/******************************************************************************
 * Default Color Look-Up Tables
 ******************************************************************************/


const uint32_t C_LUT_COLOR[256] = {
		0x00000000, 0x00000055, 0x000000AB, 0x000000FF, 0x00002400, 0x00002455, 0x000024AB, 0x000024FF,  //  8
		0x00004900, 0x00004955, 0x000049AB, 0x000049FF, 0x00006D00, 0x00006D55, 0x00006DAB, 0x00006DFF,  //  16
		0x00009200, 0x00009255, 0x000092AB, 0x000092FF, 0x0000B600, 0x0000B655, 0x0000B6AB, 0x0000B6FF,  //  24
		0x0000DB00, 0x0000DB55, 0x0000DBAB, 0x0000DBFF, 0x0000FF00, 0x0000FF55, 0x0000FFAB, 0x0000FFFF,  //  32
		0x00240000, 0x00240055, 0x002400AB, 0x002400FF, 0x00242400, 0x00242455, 0x002424AB, 0x002424FF,  //  40
		0x00244900, 0x00244955, 0x002449AB, 0x002449FF, 0x00246D00, 0x00246D55, 0x00246DAB, 0x00246DFF,  //  48
		0x00249200, 0x00249255, 0x002492AB, 0x002492FF, 0x0024B600, 0x0024B655, 0x0024B6AB, 0x0024B6FF,  //  56
		0x0024DB00, 0x0024DB55, 0x0024DBAB, 0x0024DBFF, 0x0024FF00, 0x0024FF55, 0x0024FFAB, 0x0024FFFF,  //  64
		0x00490000, 0x00490055, 0x004900AB, 0x004900FF, 0x00492400, 0x00492455, 0x004924AB, 0x004924FF,  //  72
		0x00494900, 0x00494955, 0x004949AB, 0x004949FF, 0x00496D00, 0x00496D55, 0x00496DAB, 0x00496DFF,  //  80
		0x00499200, 0x00499255, 0x004992AB, 0x004992FF, 0x0049B600, 0x0049B655, 0x0049B6AB, 0x0049B6FF,  //  88
		0x0049DB00, 0x0049DB55, 0x0049DBAB, 0x0049DBFF, 0x0049FF00, 0x0049FF55, 0x0049FFAB, 0x0049FFFF,  //  96
		0x006D0000, 0x006D0055, 0x006D00AB, 0x006D00FF, 0x006D2400, 0x006D2455, 0x006D24AB, 0x006D24FF,  //  104
		0x006D4900, 0x006D4955, 0x006D49AB, 0x006D49FF, 0x006D6D00, 0x006D6D55, 0x006D6DAB, 0x006D6DFF,  //  112
		0x006D9200, 0x006D9255, 0x006D92AB, 0x006D92FF, 0x006DB600, 0x006DB655, 0x006DB6AB, 0x006DB6FF,  //  120
		0x006DDB00, 0x006DDB55, 0x006DDBAB, 0x006DDBFF, 0x006DFF00, 0x006DFF55, 0x006DFFAB, 0x006DFFFF,  //  128
		0x00920000, 0x00920055, 0x009200AB, 0x009200FF, 0x00922400, 0x00922455, 0x009224AB, 0x009224FF,  //  136
		0x00924900, 0x00924955, 0x009249AB, 0x009249FF, 0x00926D00, 0x00926D55, 0x00926DAB, 0x00926DFF,  //  144
		0x00929200, 0x00929255, 0x009292AB, 0x009292FF, 0x0092B600, 0x0092B655, 0x0092B6AB, 0x0092B6FF,  //  152
		0x0092DB00, 0x0092DB55, 0x0092DBAB, 0x0092DBFF, 0x0092FF00, 0x0092FF55, 0x0092FFAB, 0x0092FFFF,  //  160
		0x00B60000, 0x00B60055, 0x00B600AB, 0x00B600FF, 0x00B62400, 0x00B62455, 0x00B624AB, 0x00B624FF,  //  168
		0x00B64900, 0x00B64955, 0x00B649AB, 0x00B649FF, 0x00B66D00, 0x00B66D55, 0x00B66DAB, 0x00B66DFF,  //  176
		0x00B69200, 0x00B69255, 0x00B692AB, 0x00B692FF, 0x00B6B600, 0x00B6B655, 0x00B6B6AB, 0x00B6B6FF,  //  184
		0x00B6DB00, 0x00B6DB55, 0x00B6DBAB, 0x00B6DBFF, 0x00B6FF00, 0x00B6FF55, 0x00B6FFAB, 0x00B6FFFF,  //  192
		0x00DB0000, 0x00DB0055, 0x00DB00AB, 0x00DB00FF, 0x00DB2400, 0x00DB2455, 0x00DB24AB, 0x00DB24FF,  //  200
		0x00DB4900, 0x00DB4955, 0x00DB49AB, 0x00DB49FF, 0x00DB6D00, 0x00DB6D55, 0x00DB6DAB, 0x00DB6DFF,  //  208
		0x00DB9200, 0x00DB9255, 0x00DB92AB, 0x00DB92FF, 0x00DBB600, 0x00DBB655, 0x00DBB6AB, 0x00DBB6FF,  //  216
		0x00DBDB00, 0x00DBDB55, 0x00DBDBAB, 0x00DBDBFF, 0x00DBFF00, 0x00DBFF55, 0x00DBFFAB, 0x00DBFFFF,  //  224
		0x00FF0000, 0x00FF0055, 0x00FF00AB, 0x00FF00FF, 0x00FF2400, 0x00FF2455, 0x00FF24AB, 0x00FF24FF,  //  232
		0x00FF4900, 0x00FF4955, 0x00FF49AB, 0x00FF49FF, 0x00FF6D00, 0x00FF6D55, 0x00FF6DAB, 0x00FF6DFF,  //  240
		0x00FF9200, 0x00FF9255, 0x00FF92AB, 0x00FF92FF, 0x00FFB600, 0x00FFB655, 0x00FFB6AB, 0x00FFB6FF,  //  248
		0x00FFDB00, 0x00FFDB55, 0x00FFDBAB, 0x00FFDBFF, 0x00FFFF00, 0x00FFFF55, 0x00FFFFAB, 0x00FFFFFF   //  256
};

const uint32_t C_LUT_GRAY[256] = {
		0x00000000, 0x00010101, 0x00020202, 0x00030303, 0x00040404, 0x00050505, 0x00060606, 0x00070707,
		0x00080808, 0x00090909, 0x000A0A0A, 0x000B0B0B, 0x000C0C0C, 0x000D0D0D, 0x000E0E0E, 0x000F0F0F,
		0x00101010, 0x00111111, 0x00121212, 0x00131313, 0x00141414, 0x00151515, 0x00161616, 0x00171717,
		0x00181818, 0x00191919, 0x001A1A1A, 0x001B1B1B, 0x001C1C1C, 0x001D1D1D, 0x001E1E1E, 0x001F1F1F,
		0x00202020, 0x00212121, 0x00222222, 0x00232323, 0x00242424, 0x00252525, 0x00262626, 0x00272727,
		0x00282828, 0x00292929, 0x002A2A2A, 0x002B2B2B, 0x002C2C2C, 0x002D2D2D, 0x002E2E2E, 0x002F2F2F,
		0x00303030, 0x00313131, 0x00323232, 0x00333333, 0x00343434, 0x00353535, 0x00363636, 0x00373737,
		0x00383838, 0x00393939, 0x003A3A3A, 0x003B3B3B, 0x003C3C3C, 0x003D3D3D, 0x003E3E3E, 0x003F3F3F,
		0x00404040, 0x00414141, 0x00424242, 0x00434343, 0x00444444, 0x00454545, 0x00464646, 0x00474747,
		0x00484848, 0x00494949, 0x004A4A4A, 0x004B4B4B, 0x004C4C4C, 0x004D4D4D, 0x004E4E4E, 0x004F4F4F,
		0x00505050, 0x00515151, 0x00525252, 0x00535353, 0x00545454, 0x00555555, 0x00565656, 0x00575757,
		0x00585858, 0x00595959, 0x005A5A5A, 0x005B5B5B, 0x005C5C5C, 0x005D5D5D, 0x005E5E5E, 0x005F5F5F,
		0x00606060, 0x00616161, 0x00626262, 0x00636363, 0x00646464, 0x00656565, 0x00666666, 0x00676767,
		0x00686868, 0x00696969, 0x006A6A6A, 0x006B6B6B, 0x006C6C6C, 0x006D6D6D, 0x006E6E6E, 0x006F6F6F,
		0x00707070, 0x00717171, 0x00727272, 0x00737373, 0x00747474, 0x00757575, 0x00767676, 0x00777777,
		0x00787878, 0x00797979, 0x007A7A7A, 0x007B7B7B, 0x007C7C7C, 0x007D7D7D, 0x007E7E7E, 0x007F7F7F,
		0x00808080, 0x00818181, 0x00828282, 0x00838383, 0x00848484, 0x00858585, 0x00868686, 0x00878787,
		0x00888888, 0x00898989, 0x008A8A8A, 0x008B8B8B, 0x008C8C8C, 0x008D8D8D, 0x008E8E8E, 0x008F8F8F,
		0x00909090, 0x00919191, 0x00929292, 0x00939393, 0x00949494, 0x00959595, 0x00969696, 0x00979797,
		0x00989898, 0x00999999, 0x009A9A9A, 0x009B9B9B, 0x009C9C9C, 0x009D9D9D, 0x009E9E9E, 0x009F9F9F,
		0x00A0A0A0, 0x00A1A1A1, 0x00A2A2A2, 0x00A3A3A3, 0x00A4A4A4, 0x00A5A5A5, 0x00A6A6A6, 0x00A7A7A7,
		0x00A8A8A8, 0x00A9A9A9, 0x00AAAAAA, 0x00ABABAB, 0x00ACACAC, 0x00ADADAD, 0x00AEAEAE, 0x00AFAFAF,
		0x00B0B0B0, 0x00B1B1B1, 0x00B2B2B2, 0x00B3B3B3, 0x00B4B4B4, 0x00B5B5B5, 0x00B6B6B6, 0x00B7B7B7,
		0x00B8B8B8, 0x00B9B9B9, 0x00BABABA, 0x00BBBBBB, 0x00BCBCBC, 0x00BDBDBD, 0x00BEBEBE, 0x00BFBFBF,
		0x00C0C0C0, 0x00C1C1C1, 0x00C2C2C2, 0x00C3C3C3, 0x00C4C4C4, 0x00C5C5C5, 0x00C6C6C6, 0x00C7C7C7,
		0x00C8C8C8, 0x00C9C9C9, 0x00CACACA, 0x00CBCBCB, 0x00CCCCCC, 0x00CDCDCD, 0x00CECECE, 0x00CFCFCF,
		0x00D0D0D0, 0x00D1D1D1, 0x00D2D2D2, 0x00D3D3D3, 0x00D4D4D4, 0x00D5D5D5, 0x00D6D6D6, 0x00D7D7D7,
		0x00D8D8D8, 0x00D9D9D9, 0x00DADADA, 0x00DBDBDB, 0x00DCDCDC, 0x00DDDDDD, 0x00DEDEDE, 0x00DFDFDF,
		0x00E0E0E0, 0x00E1E1E1, 0x00E2E2E2, 0x00E3E3E3, 0x00E4E4E4, 0x00E5E5E5, 0x00E6E6E6, 0x00E7E7E7,
		0x00E8E8E8, 0x00E9E9E9, 0x00EAEAEA, 0x00EBEBEB, 0x00ECECEC, 0x00EDEDED, 0x00EEEEEE, 0x00EFEFEF,
		0x00F0F0F0, 0x00F1F1F1, 0x00F2F2F2, 0x00F3F3F3, 0x00F4F4F4, 0x00F5F5F5, 0x00F6F6F6, 0x00F7F7F7,
		0x00F8F8F8, 0x00F9F9F9, 0x00FAFAFA, 0x00FBFBFB, 0x00FCFCFC, 0x00FDFDFD, 0x00FEFEFE, 0x00FFFFFF
};

