/*
 * page_screen.c
 *
 *  Created on: Dec 13, 2023
 *      Author: marek
 */

#include "page_screen.h"

static uint32_t				current_bklight;
static uint32_t				default_bklight;
static uint8_t				screen_reduced;
static uint8_t				screen_pwroff;

static GUI_Panel_TypeDef	panel00;
static GUI_Button_TypeDef	button00;
static GUI_Slider_TypeDef	slider00;
static GUI_Spinner_TypeDef	spinner00;
static GUI_Spinner_TypeDef	spinner01;


static void slider00_callback(void) {
	switch (BSP_hlcdtp.gest_data.gest) {
	case LCD_TP_GEST_CLICK_DOWN:

		if ((BSP_hlcdtp.gest_data.start_x > slider00.x_pos) && (BSP_hlcdtp.gest_data.start_x < slider00.x_pos + 42)) {
			current_bklight -= slider00.step;
			if (current_bklight > 100) current_bklight = 0;
			BSP_LCD_SetBackLight(current_bklight, 128);
		}
		if ((BSP_hlcdtp.gest_data.start_x > (slider00.x_pos + slider00.width - 42)) && (BSP_hlcdtp.gest_data.start_x < (slider00.x_pos + slider00.width))) {
			current_bklight += slider00.step;
			if (current_bklight > 100) current_bklight = 100;
			BSP_LCD_SetBackLight(current_bklight, 128);
		}
		if ((BSP_hlcdtp.gest_data.start_x > (slider00.ctx.slide_x - 5)) && (BSP_hlcdtp.gest_data.start_x < (slider00.ctx.slide_x + 32 + 5))) slider00.ctx.drag = 1;
			else slider00.ctx.drag = 0;
		break;

	case LCD_TP_GEST_DRAG:

		if (slider00.ctx.drag) {
			if (BSP_hlcdtp.gest_data.stop_x < slider00.ctx.slide_x0) {current_bklight = slider00.min_value; break;}
			if (BSP_hlcdtp.gest_data.stop_x > (slider00.ctx.slide_x1 - 32)) {current_bklight = slider00.max_value; break;}
			current_bklight = ((BSP_hlcdtp.gest_data.stop_x - slider00.ctx.slide_x0) * (slider00.max_value - slider00.min_value) / (slider00.width - 120)) + slider00.min_value;
			current_bklight -= (current_bklight % slider00.step);
			BSP_LCD_SetBackLight(current_bklight, 128);
		}
		break;

	case LCD_TP_GEST_CLICK_UP:
		slider00.ctx.drag = 0;
		break;

	}
}


static void button00_callback(void) {
	if (BSP_hlcdtp.gest_data.gest != LCD_TP_GEST_CLICK_DOWN) return;
	if (button00.state != GUI_STATE_ENABLED) return;
	default_bklight = current_bklight;
	BSP_LCD_SaveBackLight();
}


static void spinner00_callback(void) {
	if (BSP_hlcdtp.gest_data.gest != LCD_TP_GEST_CLICK_DOWN) return;
	if (BSP_hlcdtp.gest_data.start_x > (spinner00.x_pos + 70)) {
		screen_reduced++;
		if (screen_reduced > 60) screen_reduced = 60;
		if ((screen_pwroff > 0) && (screen_pwroff < (screen_reduced + 1))) screen_pwroff = screen_reduced + 1;
	} else {
		screen_reduced--;
		if (screen_reduced > 60) screen_reduced = 0;
	}
	BSP_PWR_SaveConfig(screen_reduced, screen_pwroff);
}


static void spinner01_callback(void) {
	if (BSP_hlcdtp.gest_data.gest != LCD_TP_GEST_CLICK_DOWN) return;
	if (BSP_hlcdtp.gest_data.start_x > (spinner01.x_pos + 70)) {
		screen_pwroff++;
		if (screen_pwroff < (screen_reduced + 1)) screen_pwroff = screen_reduced + 1;
		if (screen_pwroff > 60) screen_pwroff = 60;
	} else {
		screen_pwroff--;
		if ((screen_pwroff < (screen_reduced + 1)) && (screen_reduced > 0)) screen_pwroff = 0;
		if (screen_pwroff > 60) screen_pwroff = 0;
	}
	BSP_PWR_SaveConfig(screen_reduced, screen_pwroff);
}


uint8_t page_init_screen(void) {

	BSP_LCD_TP_RemoveAreaRange(10, LCD_TP_AREA_NO - 1);

	current_bklight = BSP_LCD_GetBackLight();
	uint32_t rtcreg = BSP_STM32_RTC_GetBackupReg(RTC, 10);
	default_bklight = ((rtcreg & 0x0000FF00) >> 8);
	screen_reduced = ((rtcreg & 0x00FF0000) >> 16);
	screen_pwroff = ((rtcreg & 0xFF000000) >> 24);

	// Panel00 - Main Area
	panel00.x_pos = 220;
	panel00.y_pos = 10;
	panel00.width = 570;
	panel00.height = 460;

	// Button00 - Save as default
	button00.x_pos = 590;
	button00.y_pos = 140;
	button00.width = 160;
	button00.height = 42;
	button00.icon = (uint32_t)ICON_32_Tools;
	button00.text = "Save";
	button00.state = GUI_STATE_ENABLED;
	BSP_LCD_TP_RegisterArea(10, 590, 140, 160, 42, button00_callback);

	// Slider00 - Backlight
	slider00.x_pos = 240;
	slider00.y_pos = 70;
	slider00.width = 530;
	slider00.min_value = 0;
	slider00.max_value = 100;
	slider00.step = 5;
	BSP_LCD_TP_RegisterArea(11, 240, 70, 530, 42, slider00_callback);

	// Spinner00 - Low power timer
	spinner00.x_pos = 590;
	spinner00.y_pos = 220;
	BSP_LCD_TP_RegisterArea(12, 590, 220, 140, 42, spinner00_callback);

	// Spinner01 - Screen off timer
	spinner01.x_pos = 590;
	spinner01.y_pos = 280;
	BSP_LCD_TP_RegisterArea(13, 590, 280, 140, 42, spinner01_callback);

	return GUI_OK;
}


uint8_t	page_render_screen(void) {

	char str[48];

	current_bklight = BSP_LCD_GetBackLight();

	GUI_Panel(&panel00);

	G2D_TextBlend(240, 30, FONT_24_verdana, "LCD brightness", BSP_LCD_Color(C_WHITE, 255));
	sprintf(str, "%03d%%", current_bklight);
	G2D_TextBlend(700, 30, FONT_24_verdana, str, BSP_LCD_Color(C_WHITE, 255));

	GUI_Slider(&slider00, current_bklight);

	sprintf(str, "Default brightness: %03d%%", default_bklight);
	G2D_TextBlend(260, 144, FONT_24_verdana, str, BSP_LCD_Color(C_WHITE, 255));

	if (current_bklight != default_bklight) button00.state = GUI_STATE_ENABLED; else button00.state = GUI_STATE_DISABLED;
	GUI_Button(&button00);

	G2D_TextBlend(260, 224, FONT_24_verdana, "Screen reduced after:", BSP_LCD_Color(C_WHITE, 255));
	GUI_Spinner(&spinner00, screen_reduced);
	G2D_TextBlend(260, 284, FONT_24_verdana, "Screen turned off after:", BSP_LCD_Color(C_WHITE, 255));
	GUI_Spinner(&spinner01, screen_pwroff);


	G2D_TextBlend(240, 400, FONT_16_verdana, "NOTE: Brightness can be adjusted any time by pressing", BSP_LCD_Color(C_WHITE, 255));
	G2D_TextBlend(240, 430, FONT_16_verdana, "[Up] or [Down] key while holding [PWR] button", BSP_LCD_Color(C_WHITE, 255));

	return GUI_OK;
}


