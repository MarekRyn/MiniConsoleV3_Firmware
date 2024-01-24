/*******************************************************************
 * MiniConsole V3 - Simple GUI Library
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 *******************************************************************/

#ifndef GUI_H_
#define GUI_H_

#include <stdint.h>
#include <stdio.h>
#include "BSP_Fonts.h"
#include "BSP_Icons.h"
#include "graph2d.h"


#define GUI_OK		0
#define	GUI_ERROR	1

#define GUI_STATE_DISABLED	0
#define GUI_STATE_ENABLED	1
#define GUI_STATE_CLICKED	2
#define GUI_STATE_ACTIVE	3


// GUI setup

// Button appearance - general
#define GUI_BUTTON_TEXT_FONT					FONT_24_verdana
#define GUI_BUTTON_RADIUS						8
#define GUI_BUTTON_MARGIN						10

// Button appearance - enabled state
#define GUI_BUTTON_ENABLED_BORDER_COLOR			BSP_LCD_Color(0x00E0E0E0, 255)
#define GUI_BUTTON_ENABLED_BG_COLOR				BSP_LCD_Color(C_GRAY, 150)
#define GUI_BUTTON_ENABLED_TEXT_COLOR			BSP_LCD_Color(0x00E0E0E0, 255)
#define GUI_BUTTON_ENABLED_ICON_COLOR			BSP_LCD_Color(0x00E0E0E0, 255)

// Button appearance - disabled state
#define GUI_BUTTON_DISABLED_BORDER_COLOR		BSP_LCD_Color(C_GRAY, 100)
#define GUI_BUTTON_DISABLED_BG_COLOR			BSP_LCD_Color(C_GRAY, 80)
#define GUI_BUTTON_DISABLED_TEXT_COLOR			BSP_LCD_Color(C_LIGHTGRAY, 80)
#define GUI_BUTTON_DISABLED_ICON_COLOR			BSP_LCD_Color(C_LIGHTGRAY, 80)

// Button appearance - clicked state
#define GUI_BUTTON_CLICKED_BORDER_COLOR			BSP_LCD_Color(C_WHITE, 255)
#define GUI_BUTTON_CLICKED_BG_COLOR				BSP_LCD_Color(C_GRAY, 255)
#define GUI_BUTTON_CLICKED_TEXT_COLOR			BSP_LCD_Color(C_WHITE, 255)
#define GUI_BUTTON_CLICKED_ICON_COLOR			BSP_LCD_Color(C_WHITE, 255)

// Button appearance - active state
#define GUI_BUTTON_ACTIVE_BORDER_COLOR			BSP_LCD_Color(C_WHITE, 255)
#define GUI_BUTTON_ACTIVE_BG_COLOR				BSP_LCD_Color(C_GRAY, 255)
#define GUI_BUTTON_ACTIVE_TEXT_COLOR			BSP_LCD_Color(C_WHITE, 255)
#define GUI_BUTTON_ACTIVE_ICON_COLOR			BSP_LCD_Color(C_WHITE, 255)

// Panel appearance - general
#define GUI_PANEL_BORDER_COLOR					BSP_LCD_Color(C_WHITE, 255)
#define GUI_PANEL_BG_COLOR						BSP_LCD_Color(C_GRAY, 150)
#define GUI_PANEL_RADIUS						8

// Progress Bar appearance
#define GUI_PROGRESSBAR_BORDER_COLOR			BSP_LCD_Color(C_WHITE, 255)
#define GUI_PROGRESSBAR_BG_COLOR				BSP_LCD_Color(C_GRAY, 255)
#define GUI_PROGRESSBAR_COLOR					BSP_LCD_Color(C_DARKBLUE, 255)
#define GUI_PROGRESSBAR_RADIUS					8

// Slider appearance
#define GUI_SLIDER_BORDER_COLOR					BSP_LCD_Color(C_WHITE, 255)
#define GUI_SLIDER_BG_COLOR						BSP_LCD_Color(C_GRAY, 150)
#define GUI_SLIDER_HANDLE_COLOR					BSP_LCD_Color(C_LIGHTGRAY, 255)
#define GUI_SLIDER_RADIUS						8

// Spinner appearance
#define GUI_SPINNER_BORDER_COLOR				BSP_LCD_Color(C_WHITE, 255)
#define GUI_SPINNER_BG_COLOR					BSP_LCD_Color(C_GRAY, 150)
#define GUI_SPINNER_TEXT_COLOR					BSP_LCD_Color(C_WHITE, 255)
#define GUI_SPINNER_TEXT_FONT					FONT_24_verdana
#define GUI_SPINNER_RADIUS						8

// GUI structure definitions

typedef struct {
	uint32_t	click_timestamp;
} GUI_Button_ctx;

typedef struct {
	GUI_Button_ctx	ctx;
	uint16_t		x_pos;
	uint16_t		y_pos;
	uint16_t		width;
	uint16_t		height;
	uint32_t		icon;
	char *			text;
	uint8_t			state;
} GUI_Button_TypeDef;


typedef struct {

} GUI_Panel_ctx;

typedef struct {
	GUI_Panel_ctx	ctx;
	uint16_t		x_pos;
	uint16_t		y_pos;
	uint16_t		width;
	uint16_t		height;
} GUI_Panel_TypeDef;


typedef struct {

} GUI_ProgressBar_ctx;

typedef struct {
	GUI_ProgressBar_ctx	ctx;
	uint16_t			x_pos;
	uint16_t			y_pos;
	uint16_t			width;
	uint32_t			max_value;
	uint32_t			min_value;
} GUI_ProgressBar_TypeDef;


typedef struct {
	uint16_t			slide_x0;
	uint16_t			slide_x1;
	uint16_t			slide_x;
	uint8_t				drag;
} GUI_Slider_ctx;

typedef struct {
	GUI_Slider_ctx		ctx;
	uint16_t			x_pos;
	uint16_t			y_pos;
	uint16_t			width;
	uint32_t			max_value;
	uint32_t			min_value;
	uint32_t			step;
} GUI_Slider_TypeDef;


typedef struct {

} GUI_Spinner_ctx;

typedef struct {
	GUI_Spinner_ctx		ctx;
	uint16_t			x_pos;
	uint16_t			y_pos;
	uint16_t			width;
} GUI_Spinner_TypeDef;


uint8_t	GUI_Button(GUI_Button_TypeDef * btn);
uint8_t GUI_Button_Click(GUI_Button_TypeDef * btn);
uint8_t GUI_Panel(GUI_Panel_TypeDef * pnl);
uint8_t GUI_ProgressBar(GUI_ProgressBar_TypeDef * pg, uint32_t value);
uint8_t GUI_Slider(GUI_Slider_TypeDef * sl, uint32_t value);
uint8_t GUI_Spinner(GUI_Spinner_TypeDef * sp, uint32_t value);

#endif /* GUI_H_ */
