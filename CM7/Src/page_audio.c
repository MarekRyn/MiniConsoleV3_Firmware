/*
 * page_audio.c
 *
 *  Created on: Dec 13, 2023
 *      Author: marek
 */

#include "page_audio.h"

static uint32_t					current_volume;
static uint32_t					default_volume;

static GUI_Panel_TypeDef		panel00;
static GUI_Button_TypeDef		button00;
static GUI_Button_TypeDef		button01;
static GUI_Button_TypeDef		button02;
static GUI_Button_TypeDef		button03;
static GUI_Slider_TypeDef		slider00;


static void button00_callback(void) {
	if (BSP_hlcdtp.gest_data.gest != LCD_TP_GEST_CLICK_DOWN) return;
	button00.state = GUI_STATE_ACTIVE;
	button01.state = GUI_STATE_ENABLED;
	button02.state = GUI_STATE_ENABLED;
	BSP_Audio_LinkTestSound(0);
	BSP_Audio_SetChannelVolumeLR(0, 128, 0);
	BSP_Audio_ChannelPLay(0, 1);
}

static void button01_callback(void) {
	if (BSP_hlcdtp.gest_data.gest != LCD_TP_GEST_CLICK_DOWN) return;
	button00.state = GUI_STATE_ENABLED;
	button01.state = GUI_STATE_ACTIVE;
	button02.state = GUI_STATE_ENABLED;
	BSP_Audio_LinkTestSound(0);
	BSP_Audio_SetChannelVolume(0, 128);
	BSP_Audio_ChannelPLay(0, 1);
}

static void button02_callback(void) {
	if (BSP_hlcdtp.gest_data.gest != LCD_TP_GEST_CLICK_DOWN) return;
	button00.state = GUI_STATE_ENABLED;
	button01.state = GUI_STATE_ENABLED;
	button02.state = GUI_STATE_ACTIVE;
	BSP_Audio_LinkTestSound(0);
	BSP_Audio_SetChannelVolumeLR(0, 0, 128);
	BSP_Audio_ChannelPLay(0, 1);
}

static void slider00_callback(void) {
	switch (BSP_hlcdtp.gest_data.gest) {
	case LCD_TP_GEST_CLICK_DOWN:

		if ((BSP_hlcdtp.gest_data.start_x > slider00.x_pos) && (BSP_hlcdtp.gest_data.start_x < slider00.x_pos + 42)) {
			current_volume -= slider00.step;
			if (current_volume > AUDIO_CFG_MAXMASTERVOL) current_volume = 0;
			BSP_Audio_SetMasterVolume(current_volume);
		}
		if ((BSP_hlcdtp.gest_data.start_x > (slider00.x_pos + slider00.width - 42)) && (BSP_hlcdtp.gest_data.start_x < (slider00.x_pos + slider00.width))) {
			current_volume += slider00.step;
			if (current_volume > AUDIO_CFG_MAXMASTERVOL) current_volume = AUDIO_CFG_MAXMASTERVOL;
			BSP_Audio_SetMasterVolume(current_volume);
		}
		if ((BSP_hlcdtp.gest_data.start_x > (slider00.ctx.slide_x - 5)) && (BSP_hlcdtp.gest_data.start_x < (slider00.ctx.slide_x + 32 + 5))) slider00.ctx.drag = 1;
			else slider00.ctx.drag = 0;
		break;

	case LCD_TP_GEST_DRAG:

		if (slider00.ctx.drag) {
			if (BSP_hlcdtp.gest_data.stop_x < slider00.ctx.slide_x0) {current_volume = slider00.min_value; break;}
			if (BSP_hlcdtp.gest_data.stop_x > (slider00.ctx.slide_x1 - 32)) {current_volume = slider00.max_value; break;}
			current_volume = ((BSP_hlcdtp.gest_data.stop_x - slider00.ctx.slide_x0) * (slider00.max_value - slider00.min_value) / (slider00.width - 120)) + slider00.min_value;
			current_volume -= (current_volume % slider00.step);
			BSP_Audio_SetMasterVolume(current_volume);
		}
		break;

	case LCD_TP_GEST_CLICK_UP:
		slider00.ctx.drag = 0;
		break;

	}
}

static void button03_callback(void) {
	if (BSP_hlcdtp.gest_data.gest != LCD_TP_GEST_CLICK_DOWN) return;
	if (button03.state != GUI_STATE_ENABLED) return;
	default_volume = current_volume;
	BSP_Audio_SaveMasterVolume();
}



uint8_t page_init_audio(void) {

	BSP_LCD_TP_RemoveAreaRange(10, LCD_TP_AREA_NO - 1);

	current_volume = BSP_Audio_GetMasterVolume();
	default_volume = (uint8_t)((BSP_STM32_RTC_GetBackupReg(RTC, 10) & 0x000000FF) >> 0);

	// Panel00 - Main Area
	panel00.x_pos = 220;
	panel00.y_pos = 10;
	panel00.width = 570;
	panel00.height = 460;

	// Button00 - Test Left Speaker
	button00.x_pos = 240;
	button00.y_pos = 70;
	button00.width = 160;
	button00.height = 42;
	button00.icon = (uint32_t)ICON_32_Audio;
	button00.text = "Left";
	button00.state = GUI_STATE_ENABLED;
	BSP_LCD_TP_RegisterArea(10, 240, 70, 160, 42, button00_callback);

	// Button01 - Test Both Speakers
	button01.x_pos = 425;
	button01.y_pos = 70;
	button01.width = 160;
	button01.height = 42;
	button01.icon = (uint32_t)ICON_32_Audio;
	button01.text = "Stereo";
	button01.state = GUI_STATE_ENABLED;
	BSP_LCD_TP_RegisterArea(11, 425, 70, 160, 42, button01_callback);

	// Button02 - Test Right Speaker
	button02.x_pos = 610;
	button02.y_pos = 70;
	button02.width = 160;
	button02.height = 42;
	button02.icon = (uint32_t)ICON_32_Audio;
	button02.text = "Right";
	button02.state = GUI_STATE_ENABLED;
	BSP_LCD_TP_RegisterArea(12, 610, 70, 160, 42, button02_callback);

	// Slider00 - Current Volume
	slider00.x_pos = 240;
	slider00.y_pos = 190;
	slider00.width = 530;
	slider00.min_value = 0;
	slider00.max_value = AUDIO_CFG_MAXMASTERVOL;
	slider00.step = 8;
	BSP_LCD_TP_RegisterArea(13, 240, 190, 530, 42, slider00_callback);

	// Button03 - Save level as default
	button03.x_pos = 590;
	button03.y_pos = 260;
	button03.width = 160;
	button03.height = 42;
	button03.icon = (uint32_t)ICON_32_Tools;
	button03.text = "Save";
	button03.state = GUI_STATE_ENABLED;
	BSP_LCD_TP_RegisterArea(14, 590, 260, 160, 42, button03_callback);

	return GUI_OK;
}


uint8_t	page_render_audio(void) {

	char str[48];

	current_volume = BSP_Audio_GetMasterVolume();

	GUI_Panel(&panel00);

	G2D_TextBlend(240, 30, FONT_24_verdana, "Audio test", BSP_LCD_Color(C_WHITE, 255));

	GUI_Button(&button00);
	GUI_Button(&button01);
	GUI_Button(&button02);

	G2D_TextBlend(240, 150, FONT_24_verdana, "Volume level", BSP_LCD_Color(C_WHITE, 255));
	sprintf(str, "%03d%%", (current_volume * 100) / 128);
	G2D_TextBlend(700, 150, FONT_24_verdana, str, BSP_LCD_Color(C_WHITE, 255));
	GUI_Slider(&slider00, current_volume);

	if (current_volume == default_volume) button03.state = GUI_STATE_DISABLED; else button03.state = GUI_STATE_ENABLED;

	sprintf(str, "Default volume: %03d%%", (default_volume * 100) / 128);
	G2D_TextBlend(270, 264, FONT_24_verdana, str, BSP_LCD_Color(C_WHITE, 255));

	GUI_Button(&button03);

	G2D_TextBlend(240, 400, FONT_16_verdana, "NOTE: Volume can be adjusted any time by pressing", BSP_LCD_Color(C_WHITE, 255));
	G2D_TextBlend(240, 430, FONT_16_verdana, "[Left] or [Right] key while holding [PWR] button", BSP_LCD_Color(C_WHITE, 255));

	return GUI_OK;
}


