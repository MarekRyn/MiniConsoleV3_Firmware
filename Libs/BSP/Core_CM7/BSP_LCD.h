/*******************************************************************
 * MiniConsole V3 - Board Support Package - LCD
 *
 * Author: Marek Ryn
 * Version: 1.0
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 * - 0.2b 	- Added hardware JPEG decoding
 * - 0.3b	- Added ARGB1555 and ARGB4444 modes
 * - 1.0	- Major refactoring.
 *******************************************************************/

#ifndef INC_BSP_LCD_H_
#define INC_BSP_LCD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "BSP_STM32.h"
#include "../Components/RVT50AQTNWC00.h"	// LCD Screen driver - model RVT50AQTNWC00


// Backlight PWM configuration
#define LCD_BKL_TIM			TIM3					/* PWM Timer							*/
#define LCD_BKL_MIN_PWM		200						/* PWM Duty for 0% backlight			*/
#define LCD_BKL_MAX_PWM		500						/* PWM Duty for 100% backlight			*/

// Frame buffer address configuration
#define LCD_FRAMEBUFFER_END_ADDR	0xC0000000 + 0x04000000		/* Frame buffer always sticks to the end of SDRAM	*/

// Mode definitions
#define LCD_COLOR_MODE_ARGB8888			6
#define LCD_COLOR_MODE_ARGB4444			5
#define LCD_COLOR_MODE_ARGB1555			4
#define LCD_COLOR_MODE_RGB888			3
#define LCD_COLOR_MODE_AL88				2
#define LCD_COLOR_MODE_L8				1

#define LCD_BUFFER_MODE_DOUBLE		2
#define LCD_BUFFER_MODE_TRIPLE		3

// Color Definitions - Valid only for ARGB and RGB modes
#define C_BLUE          0x0000FF
#define C_GREEN         0x00FF00
#define C_RED           0xFF0000
#define C_CYAN          0x00FFFF
#define C_MAGENTA       0xFF00FF
#define C_YELLOW        0xFFFF00
#define C_LIGHTBLUE     0x8080FF
#define C_LIGHTGREEN    0x80FF80
#define C_LIGHTRED      0xFF8080
#define C_LIGHTCYAN     0x80FFFF
#define C_LIGHTMAGENTA  0xFF80FF
#define C_LIGHTYELLOW   0xFFFF80
#define C_DARKBLUE      0x000080
#define C_DARKGREEN     0x008000
#define C_DARKRED       0x800000
#define C_DARKCYAN      0x008080
#define C_DARKMAGENTA   0x800080
#define C_DARKYELLOW    0x808000
#define C_WHITE         0xFFFFFF
#define C_LIGHTGRAY     0xD3D3D3
#define C_GRAY          0x808080
#define C_DARKGRAY      0x404040
#define C_BLACK         0x000000
#define C_BROWN         0xA52A2A
#define C_ORANGE        0xFFA500

// Status code definitions
#define LCD_FRAME_IDLE				0
#define LCD_FRAME_EDIT				1
#define LCD_FRAME_READY				2
#define LCD_FRAME_ACTIVE			3
#define LCD_FRAME_NOUSE				4

#define LCD_DMA2D_READY				0
#define LCD_DMA2D_BUSY				1

#define LCD_JPEGBUF_SIZE			2*1024*1024	// 2MB is sufficient buffer for JPEG image of size 800x480 at CSS 4:4:4

// Pre-defined LUT color palettes
extern const uint32_t C_LUT_COLOR[256];
extern const uint32_t C_LUT_GRAY[256];

// Functions
void BSP_LCD_Init(uint8_t color_mode, uint8_t buffer_mode, uint32_t bgcolor, uint32_t *clut);
void BSP_LCD_FrameReady(void);
uint8_t BSP_LCD_GetEditPermission(void);
void BSP_LCD_SetLayerAlpha(uint8_t alpha);
uint8_t BSP_LCD_GetLayerAlpha(void);
void BSP_LCD_InitBackLight(uint8_t value);
void BSP_LCD_SetBackLight(uint8_t value, uint8_t dimspeed);
uint8_t BSP_LCD_GetBackLight(void);
void BSP_LCD_DecodeJPEG(uint32_t jpeg_addr, uint32_t jpeg_size);
uint32_t BSP_LCD_GetEditFrameAddr(void);
uint32_t BSP_LCD_GetPrevFrameAddr(void);
uint32_t BSP_LCD_GetColorMode(void);
uint8_t	BSP_LCD_GetBytesPerPixel(void);

// Functions for overloading (content depends on color mode)
extern uint32_t (*BSP_LCD_Color)(uint32_t color, uint8_t alpha); // Calculating color value and include alpha in modes with alpha channel
extern uint32_t (*BSP_LCD_Alpha)(uint32_t color, uint8_t alpha); // Updating alpha channel in pre-calculated color value
extern void (*BSP_LCD_DMA2D_Wait)(void);
extern void (*BSP_LCD_UpdatePixel)(uint32_t offset, int16_t x, int16_t y, uint32_t value);
extern void (*BSP_LCD_UpdatePixelBlend)(uint32_t offset, int16_t x, int16_t y, uint32_t value);
extern uint32_t (*BSP_LCD_GetPixel)(uint32_t offset,  int16_t x, int16_t y);
extern void (*BSP_LCD_FillBuf)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t offsetline, uint32_t color);
extern void (*BSP_LCD_FillBufBlend)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t offsetline, uint32_t color);
extern void (*BSP_LCD_CopyBuf)(uint32_t src_addr, uint16_t offsline_src, uint16_t x_dest, uint16_t y_dest, uint16_t offsline_dest, uint16_t width, uint16_t height);
extern void (*BSP_LCD_CopyBufBlend)(uint32_t src_addr, uint16_t offsline_src, uint16_t x_dest, uint16_t y_dest, uint16_t offsline_dest, uint16_t width, uint16_t height, uint8_t alpha);
extern void (*BSP_LCD_CopyBufJPEG)(uint16_t x_dest, uint16_t y_dest);


#ifdef __cplusplus
}
#endif

#endif /* INC_BSP_LCD_H_ */

