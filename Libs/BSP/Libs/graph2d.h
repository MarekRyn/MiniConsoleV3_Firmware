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

void G2D_ClearFrame(LCD_HandleTypeDef *lcd, uint8_t layer);
void G2D_FillFrame(LCD_HandleTypeDef *lcd, uint8_t layer, uint32_t color);
void G2D_CopyPrevFrame(LCD_HandleTypeDef *lcd, uint8_t layer);
void G2D_CopyScrollPrevFrame(LCD_HandleTypeDef *lcd, uint8_t layer, int16_t dx, int16_t dy);
void G2D_CopyLayer0to1(LCD_HandleTypeDef *lcd);
void G2D_CopyLayer1to0(LCD_HandleTypeDef *lcd);
void G2D_DrawPixel(LCD_HandleTypeDef *lcd, uint8_t layer, int16_t x, int16_t y, uint32_t color);
void G2D_DrawHLine(LCD_HandleTypeDef *lcd, uint8_t layer, int16_t x, int16_t y, int16_t length, uint32_t color);
void G2D_DrawVLine(LCD_HandleTypeDef *lcd, uint8_t layer, int16_t x, int16_t y, int16_t length, uint32_t color);
void G2D_DrawLine(LCD_HandleTypeDef *lcd, uint8_t layer, int16_t X1, int16_t Y1, int16_t X2, int16_t Y2, uint32_t color);
void G2D_DrawRect(LCD_HandleTypeDef *lcd, uint8_t layer, int16_t x, int16_t y, uint16_t width, uint16_t height, uint32_t color);
void G2D_DrawFillRect(LCD_HandleTypeDef *lcd, uint8_t layer, int16_t x, int16_t y, uint16_t width, uint16_t height, uint32_t color);
void G2D_DrawCircle(LCD_HandleTypeDef *lcd, uint8_t layer, int16_t x, int16_t y, uint16_t r, uint32_t color);
void G2D_DrawFillCircle(LCD_HandleTypeDef *lcd, uint8_t layer, int16_t x, int16_t y, uint16_t r, uint32_t color);
uint16_t G2D_Text(LCD_HandleTypeDef *lcd, uint8_t layer, int16_t x, int16_t y, uint8_t *font, char *str, uint32_t color, uint32_t bgcolor);
void G2D_DrawBitmapBlend(LCD_HandleTypeDef *lcd, uint8_t layer, uint32_t sourcedata, int16_t x, int16_t y, int16_t width, int16_t height, uint8_t alpha);
void G2D_DrawBitmapBlendC(LCD_HandleTypeDef *lcd, uint8_t layer, uint32_t sourcedata, int16_t x, int16_t y, int16_t width, int16_t height, uint8_t alpha);
void G2D_DrawBitmap(LCD_HandleTypeDef *lcd, uint8_t layer, uint32_t sourcedata, int16_t x, int16_t y, int16_t width, int16_t height);
void G2D_DrawBitmapC(LCD_HandleTypeDef *lcd, uint8_t layer, uint32_t sourcedata, int16_t x, int16_t y, int16_t width, int16_t height);
void G2D_DrawBitmapRotate(LCD_HandleTypeDef *lcd, uint8_t layer, uint32_t sourcedata, int16_t x, int16_t y, int16_t width, int16_t height, float angle);
void G2D_DrawBitmapRotateC(LCD_HandleTypeDef *lcd, uint8_t layer, uint32_t sourcedata, int16_t x, int16_t y, int16_t width, int16_t height, float angle);
void G2D_DrawIcon(LCD_HandleTypeDef *lcd, uint8_t layer, uint32_t iconsource, int16_t x, int16_t y, uint32_t color, uint32_t bgcolor);
void G2D_DrawIconC(LCD_HandleTypeDef *lcd, uint8_t layer, uint32_t iconsource, int16_t x, int16_t y, uint32_t color, uint32_t bgcolor);
void G2D_DrawJPEG(LCD_HandleTypeDef *lcd, uint8_t layer, uint32_t jpeg_addr, uint32_t jpeg_size, int16_t x, int16_t y);
void G2D_DrawJPEGC(LCD_HandleTypeDef *lcd, uint8_t layer, uint32_t jpeg_addr, uint32_t jpeg_size, int16_t x, int16_t y);
void G2D_DrawLastJPEG(LCD_HandleTypeDef *lcd, uint8_t layer, int16_t x, int16_t y);
void G2D_DrawLastJPEGC(LCD_HandleTypeDef *lcd, uint8_t layer, int16_t x, int16_t y);
void G2D_DecodeJPEG(LCD_HandleTypeDef *lcd, uint32_t jpeg_addr, uint32_t jpeg_size);
void G2D_DrawTile(LCD_HandleTypeDef *lcd, uint8_t layer, uint32_t tileset_addr, uint32_t tileset_cols, uint32_t tile_width, uint32_t tile_height, uint32_t tile_col, uint32_t tile_row, int16_t x, int16_t y);
void G2D_DrawTileC(LCD_HandleTypeDef *lcd, uint8_t layer, uint32_t tileset_addr, uint32_t tileset_cols, uint32_t tile_width, uint32_t tile_height, uint32_t tile_col, uint32_t tile_row, int16_t x, int16_t y);
void G2D_DrawTileBlend(LCD_HandleTypeDef *lcd, uint8_t layer, uint32_t tileset_addr, uint32_t tileset_cols, uint32_t tile_width, uint32_t tile_height, uint32_t tile_col, uint32_t tile_row, int16_t x, int16_t y);
void G2D_DrawTileBlendC(LCD_HandleTypeDef *lcd, uint8_t layer, uint32_t tileset_addr, uint32_t tileset_cols, uint32_t tile_width, uint32_t tile_height, uint32_t tile_col, uint32_t tile_row, int16_t x, int16_t y);

#endif

#endif /* INC_GRAPH2D_H_ */
