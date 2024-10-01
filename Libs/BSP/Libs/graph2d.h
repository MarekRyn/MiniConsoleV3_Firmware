/*******************************************************************
 * MiniConsole V3 - Board Support Package - Graph 2D Library
 *
 * Author: Marek Ryn
 * Version: 1.0
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 * - 0.2b 	- Added hardware JPEG decoding
 * - 0.3b	- Added ARGB1555 and ARGB4444 modes
 * - 0.4b   - Added RoundRect and FillRoundRect
 * 			- Added DrawIconBlend and TextBlend
 * - 1.0	- Major refactoring
 *******************************************************************/

#ifndef INC_GRAPH2D_H_
#define INC_GRAPH2D_H_

#ifdef CORE_CM7

#include <string.h>
#include "BSP_Common.h"
#include "BSP_STM32.h"
#include "BSP_LCD.h"

#define G2D_TEXTBUFSIZE	1024  /* Size of buffer for rendering characters (before blending) */

void G2D_ClearFrame(void);
void G2D_FillFrame(uint32_t color);
void G2D_CopyPrevFrame(void);
void G2D_CopyScrollPrevFrame(int16_t dx, int16_t dy);

void G2D_DrawPixel(int16_t x, int16_t y, uint32_t color);

void G2D_DrawHLine(int16_t x, int16_t y, int16_t length, uint32_t color);
void G2D_DrawHLineBlend(int16_t x, int16_t y, int16_t length, uint32_t color);
void G2D_DrawVLine(int16_t x, int16_t y, int16_t length, uint32_t color);
void G2D_DrawVLineBlend(int16_t x, int16_t y, int16_t length, uint32_t color);

void G2D_DrawLine(int16_t X1, int16_t Y1, int16_t X2, int16_t Y2, uint32_t color);

void G2D_DrawRect(int16_t x, int16_t y, uint16_t width, uint16_t height, uint32_t color);
void G2D_DrawFillRect(int16_t x, int16_t y, uint16_t width, uint16_t height, uint32_t color);
void G2D_DrawFillRectBlend(int16_t x, int16_t y, uint16_t width, uint16_t height, uint32_t color);

void G2D_DrawCircle(int16_t x, int16_t y, uint16_t r, uint32_t color);
void G2D_DrawFillCircle(int16_t x, int16_t y, uint16_t r, uint32_t color);
void G2D_DrawFillCircleBlend(int16_t x, int16_t y, uint16_t r, uint32_t color);

void G2D_DrawRoundRect(int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t radius, uint32_t color);
void G2D_DrawFillRoundRect(int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t radius, uint32_t color);
void G2D_DrawFillRoundRectBlend(int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t radius, uint32_t color);

uint16_t G2D_Text(int16_t x, int16_t y, const uint8_t *font, char *str, uint32_t color, uint32_t bgcolor);
uint16_t G2D_TextBlend(int16_t x, int16_t y, const uint8_t *font, char *str, uint32_t color);
uint8_t G2D_GetTextHeight(const uint8_t *font);

void G2D_DrawBitmapBlend(const void * sourcedata, int16_t x, int16_t y, int16_t width, int16_t height, uint8_t alpha);
void G2D_DrawBitmapBlendC(const void * sourcedata, int16_t x, int16_t y, int16_t width, int16_t height, uint8_t alpha);
void G2D_DrawBitmap(const void * sourcedata, int16_t x, int16_t y, int16_t width, int16_t height);
void G2D_DrawBitmapC(const void * sourcedata, int16_t x, int16_t y, int16_t width, int16_t height);
void G2D_DrawBitmapRotate(const void * sourcedata, int16_t x, int16_t y, int16_t width, int16_t height, float angle);
void G2D_DrawBitmapRotateC(const void * sourcedata, int16_t x, int16_t y, int16_t width, int16_t height, float angle);

void G2D_DrawIcon(const void * iconsource, int16_t x, int16_t y, uint32_t color, uint32_t bgcolor);
void G2D_DrawIconC(const void * iconsource, int16_t x, int16_t y, uint32_t color, uint32_t bgcolor);
void G2D_DrawIconBlend(const void * iconsource, int16_t x, int16_t y, uint32_t color);
void G2D_DrawIconBlendC(const void * iconsource, int16_t x, int16_t y, uint32_t color);
uint16_t G2D_GetIconHeight(const void * iconsource);
uint16_t G2D_GetIconWidth(const void * iconsource);

void G2D_DrawJPEG(const void * jpeg_addr, uint32_t jpeg_size, int16_t x, int16_t y);
void G2D_DrawJPEGC(const void * jpeg_addr, uint32_t jpeg_size, int16_t x, int16_t y);
void G2D_DrawLastJPEG(int16_t x, int16_t y);
void G2D_DrawLastJPEGC(int16_t x, int16_t y);
void G2D_DecodeJPEG(const void * jpeg_addr, uint32_t jpeg_size);

uint32_t G2D_Color(uint32_t color, uint8_t alpha); // Calculating color value and include alpha in modes with alpha channel
uint32_t G2D_Alpha(uint32_t color, uint8_t alpha); // Updating alpha channel in pre-calculated color value

void G2D_CopyBuf(const void * src_addr, uint16_t offsline_src, uint16_t x_dest, uint16_t y_dest, uint16_t width, uint16_t height); // Advanced buffer copy to screen
void G2D_CopyBufBlend(const void * src_addr, uint16_t offsline_src, uint16_t x_dest, uint16_t y_dest, uint16_t width, uint16_t height, uint8_t alpha); // Advanced buffer copy to screen with blending

void G2D_CacheFrame(void); // Save current state of the frame to cache memory
void G2D_RestoreFrame(void); // Restoring cached frame

#endif

#endif /* INC_GRAPH2D_H_ */
