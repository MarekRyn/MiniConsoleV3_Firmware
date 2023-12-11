/*******************************************************************
 * MiniConsole V3 - Board Support Package - Graph 2D Library
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

void G2D_DrawBitmapBlend(uint32_t sourcedata, int16_t x, int16_t y, int16_t width, int16_t height, uint8_t alpha);
void G2D_DrawBitmapBlendC(uint32_t sourcedata, int16_t x, int16_t y, int16_t width, int16_t height, uint8_t alpha);
void G2D_DrawBitmap(uint32_t sourcedata, int16_t x, int16_t y, int16_t width, int16_t height);
void G2D_DrawBitmapC(uint32_t sourcedata, int16_t x, int16_t y, int16_t width, int16_t height);
void G2D_DrawBitmapRotate(uint32_t sourcedata, int16_t x, int16_t y, int16_t width, int16_t height, float angle);
void G2D_DrawBitmapRotateC(uint32_t sourcedata, int16_t x, int16_t y, int16_t width, int16_t height, float angle);

void G2D_DrawIcon(uint32_t iconsource, int16_t x, int16_t y, uint32_t color, uint32_t bgcolor);
void G2D_DrawIconC(uint32_t iconsource, int16_t x, int16_t y, uint32_t color, uint32_t bgcolor);
void G2D_DrawIconBlend(uint32_t iconsource, int16_t x, int16_t y, uint32_t color);
void G2D_DrawIconBlendC(uint32_t iconsource, int16_t x, int16_t y, uint32_t color);

void G2D_DrawJPEG(uint32_t jpeg_addr, uint32_t jpeg_size, int16_t x, int16_t y);
void G2D_DrawJPEGC(uint32_t jpeg_addr, uint32_t jpeg_size, int16_t x, int16_t y);
void G2D_DrawLastJPEG(int16_t x, int16_t y);
void G2D_DrawLastJPEGC(int16_t x, int16_t y);
void G2D_DecodeJPEG(LCD_HandleTypeDef *lcd, uint32_t jpeg_addr, uint32_t jpeg_size);

void G2D_DrawTile(uint32_t tileset_addr, uint32_t tileset_cols, uint32_t tile_width, uint32_t tile_height, uint32_t tile_col, uint32_t tile_row, int16_t x, int16_t y);
void G2D_DrawTileC(uint32_t tileset_addr, uint32_t tileset_cols, uint32_t tile_width, uint32_t tile_height, uint32_t tile_col, uint32_t tile_row, int16_t x, int16_t y);
void G2D_DrawTileBlend(uint32_t tileset_addr, uint32_t tileset_cols, uint32_t tile_width, uint32_t tile_height, uint32_t tile_col, uint32_t tile_row, int16_t x, int16_t y);
void G2D_DrawTileBlendC(uint32_t tileset_addr, uint32_t tileset_cols, uint32_t tile_width, uint32_t tile_height, uint32_t tile_col, uint32_t tile_row, int16_t x, int16_t y);

#endif

#endif /* INC_GRAPH2D_H_ */
