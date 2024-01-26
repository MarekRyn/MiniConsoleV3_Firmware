/*
 * page_info.c
 *
 *  Created on: Jan 20, 2024
 *      Author: marek
 */

#include "page_info.h"

static void * tracks[6] = {0};

static uint32_t mix = 0;

GUI_Slider_TypeDef 	slider00;
GUI_Button_TypeDef	button00;
GUI_Button_TypeDef	button01;
GUI_Button_TypeDef	button02;
GUI_Button_TypeDef	button03;
GUI_Button_TypeDef	button04;
GUI_Button_TypeDef	button05;

static void audio_callback_ch_stop(void) {
	if (BSP_Audio_GetStatusParam(0) == 0) {
		button00.state = GUI_STATE_ENABLED;
		button01.state = GUI_STATE_ENABLED;
		button02.state = GUI_STATE_ENABLED;
	}
	if (BSP_Audio_GetStatusParam(0) == 1) {
		button03.state = GUI_STATE_ENABLED;
		button04.state = GUI_STATE_ENABLED;
		button05.state = GUI_STATE_ENABLED;
	}
}

static void slider00_callback(void) {
	switch (BSP_hlcdtp.gest_data.gest) {
	case LCD_TP_GEST_CLICK_DOWN:

		if ((BSP_hlcdtp.gest_data.start_x > slider00.x_pos) && (BSP_hlcdtp.gest_data.start_x < slider00.x_pos + 42)) {
			mix -= slider00.step;
			if (mix > slider00.max_value) mix = 0;
			BSP_Audio_SetChannelVolume(0, 196 - mix);
			BSP_Audio_SetChannelVolume(1, mix);
		}
		if ((BSP_hlcdtp.gest_data.start_x > (slider00.x_pos + slider00.width - 42)) && (BSP_hlcdtp.gest_data.start_x < (slider00.x_pos + slider00.width))) {
			mix += slider00.step;
			if (mix > slider00.max_value) mix = slider00.max_value;
			BSP_Audio_SetChannelVolume(0, 196 - mix);
			BSP_Audio_SetChannelVolume(1, mix);
		}
		if ((BSP_hlcdtp.gest_data.start_x > (slider00.ctx.slide_x - 5)) && (BSP_hlcdtp.gest_data.start_x < (slider00.ctx.slide_x + 32 + 5))) slider00.ctx.drag = 1;
			else slider00.ctx.drag = 0;
		break;

	case LCD_TP_GEST_DRAG:

		if (slider00.ctx.drag) {
			if (BSP_hlcdtp.gest_data.stop_x < slider00.ctx.slide_x0) {mix = slider00.min_value; break;}
			if (BSP_hlcdtp.gest_data.stop_x > (slider00.ctx.slide_x1 - 32)) {mix = slider00.max_value; break;}
			mix = ((BSP_hlcdtp.gest_data.stop_x - slider00.ctx.slide_x0) * (slider00.max_value - slider00.min_value) / (slider00.width - 120)) + slider00.min_value;
			mix -= (mix % slider00.step);
			BSP_Audio_SetChannelVolume(0, 196 - mix);
			BSP_Audio_SetChannelVolume(1, mix);
		}
		break;

	case LCD_TP_GEST_CLICK_UP:
		slider00.ctx.drag = 0;
		break;

	}
}


static void button00_callback(void) {
	if (BSP_hlcdtp.gest_data.gest != LCD_TP_GEST_CLICK_DOWN) return;

	BSP_Audio_ChannelStop(0);

	if (button00.state != GUI_STATE_ACTIVE) {

		button00.state = GUI_STATE_ACTIVE;
		button01.state = GUI_STATE_ENABLED;
		button02.state = GUI_STATE_ENABLED;
		BSP_Audio_LinkSourceMP3(0, tracks[0], BSP_Res_GetSize(tracks[0]));
		BSP_Audio_ChannelPLay(0, 1);

	} else button00.state = GUI_STATE_ENABLED;
}

static void button01_callback(void) {
	if (BSP_hlcdtp.gest_data.gest != LCD_TP_GEST_CLICK_DOWN) return;

	BSP_Audio_ChannelStop(0);

	if (button01.state != GUI_STATE_ACTIVE) {

		button00.state = GUI_STATE_ENABLED;
		button01.state = GUI_STATE_ACTIVE;
		button02.state = GUI_STATE_ENABLED;

		BSP_Audio_LinkSourceMP3(0, tracks[1], BSP_Res_GetSize(tracks[1]));
		BSP_Audio_ChannelPLay(0, 1);

	} else button01.state = GUI_STATE_ENABLED;
}

static void button02_callback(void) {
	if (BSP_hlcdtp.gest_data.gest != LCD_TP_GEST_CLICK_DOWN) return;

	BSP_Audio_ChannelStop(0);

	if (button02.state != GUI_STATE_ACTIVE) {

		button00.state = GUI_STATE_ENABLED;
		button01.state = GUI_STATE_ENABLED;
		button02.state = GUI_STATE_ACTIVE;

		BSP_Audio_LinkSourceMP3(0, tracks[2], BSP_Res_GetSize(tracks[2]));
		BSP_Audio_ChannelPLay(0, 1);

	} else button02.state = GUI_STATE_ENABLED;
}

static void button03_callback(void) {
	if (BSP_hlcdtp.gest_data.gest != LCD_TP_GEST_CLICK_DOWN) return;

	BSP_Audio_ChannelStop(1);

	if (button03.state != GUI_STATE_ACTIVE) {

		button03.state = GUI_STATE_ACTIVE;
		button04.state = GUI_STATE_ENABLED;
		button05.state = GUI_STATE_ENABLED;

		BSP_Audio_LinkSourceMP3(1, tracks[3], BSP_Res_GetSize(tracks[3]));
		BSP_Audio_ChannelPLay(1, 1);

	} else button03.state = GUI_STATE_ENABLED;
}

static void button04_callback(void) {
	if (BSP_hlcdtp.gest_data.gest != LCD_TP_GEST_CLICK_DOWN) return;

	BSP_Audio_ChannelStop(1);

	if (button04.state != GUI_STATE_ACTIVE) {

		button03.state = GUI_STATE_ENABLED;
		button04.state = GUI_STATE_ACTIVE;
		button05.state = GUI_STATE_ENABLED;

		BSP_Audio_LinkSourceMP3(1, tracks[4], BSP_Res_GetSize(tracks[4]));
		BSP_Audio_ChannelPLay(1, 1);

	} else button04.state = GUI_STATE_ENABLED;
}

static void button05_callback(void) {
	if (BSP_hlcdtp.gest_data.gest != LCD_TP_GEST_CLICK_DOWN) return;

	BSP_Audio_ChannelStop(1);

	if (button05.state != GUI_STATE_ACTIVE) {

		button03.state = GUI_STATE_ENABLED;
		button04.state = GUI_STATE_ENABLED;
		button05.state = GUI_STATE_ACTIVE;

		BSP_Audio_LinkSourceMP3(1, tracks[5], BSP_Res_GetSize(tracks[5]));
		BSP_Audio_ChannelPLay(1, 1);

	} else button05.state = GUI_STATE_ENABLED;
}


uint8_t page_loaddata_info(void) {
	tracks[0] = BSP_Res_Load("alex-productions-revolution.mp3");
	tracks[1] = BSP_Res_Load("corporate-music-zone-rise.mp3");
	tracks[2] = BSP_Res_Load("luke-bergs-agusalvarez-heaven.mp3");
	tracks[3] = BSP_Res_Load("mixaund-dreamers.mp3");
	tracks[4] = BSP_Res_Load("mixaund-hope.mp3");
	tracks[5] = BSP_Res_Load("alex-productions-efficsounds-energetic-rock-hiking-free-music.mp3");
	return BSP_OK;
}

uint8_t page_init_info(void) {

	BSP_LCD_TP_RemoveAreaRange(10, LCD_TP_AREA_NO - 1);

	// Slider00 - Backlight
	slider00.x_pos = 240;
	slider00.y_pos = 70;
	slider00.width = 530;
	slider00.min_value = 0;
	slider00.max_value = 196;
	slider00.step = 2;
	BSP_LCD_TP_RegisterArea(11, 240, 70, 530, 42, slider00_callback);

	// Button00 - Track01
	button00.x_pos = 240;
	button00.y_pos = 140;
	button00.width = 160;
	button00.height = 42;
	button00.icon = (uint32_t)ICON_32_Audio;
	button00.text = "Track 1";
	button00.state = GUI_STATE_ENABLED;
	BSP_LCD_TP_RegisterArea(20, 240, 140, 160, 42, button00_callback);

	// Button01 - Track02
	button01.x_pos = 240;
	button01.y_pos = 200;
	button01.width = 160;
	button01.height = 42;
	button01.icon = (uint32_t)ICON_32_Audio;
	button01.text = "Track 2";
	button01.state = GUI_STATE_ENABLED;
	BSP_LCD_TP_RegisterArea(21, 240, 200, 160, 42, button01_callback);

	// Button02 - Track03
	button02.x_pos = 240;
	button02.y_pos = 260;
	button02.width = 160;
	button02.height = 42;
	button02.icon = (uint32_t)ICON_32_Audio;
	button02.text = "Track 3";
	button02.state = GUI_STATE_ENABLED;
	BSP_LCD_TP_RegisterArea(22, 240, 260, 160, 42, button02_callback);

	// Button03 - Track04
	button03.x_pos = 610;
	button03.y_pos = 140;
	button03.width = 160;
	button03.height = 42;
	button03.icon = (uint32_t)ICON_32_Audio;
	button03.text = "Track 4";
	button03.state = GUI_STATE_ENABLED;
	BSP_LCD_TP_RegisterArea(23, 610, 140, 160, 42, button03_callback);

	// Button04 - Track05
	button04.x_pos = 610;
	button04.y_pos = 200;
	button04.width = 160;
	button04.height = 42;
	button04.icon = (uint32_t)ICON_32_Audio;
	button04.text = "Track 5";
	button04.state = GUI_STATE_ENABLED;
	BSP_LCD_TP_RegisterArea(24, 610, 200, 160, 42, button04_callback);

	// Button05 - Track06
	button05.x_pos = 610;
	button05.y_pos = 260;
	button05.width = 160;
	button05.height = 42;
	button05.icon = (uint32_t)ICON_32_Audio;
	button05.text = "Track 6";
	button05.state = GUI_STATE_ENABLED;
	BSP_LCD_TP_RegisterArea(25, 610, 260, 160, 42, button05_callback);

	// Setting default volumes for channels;
	mix = 0;
	BSP_Audio_SetChannelVolume(0, 196 - mix);
	BSP_Audio_SetChannelVolume(1, mix);
	BSP_Audio_RegisterStatusCallback(AUDIO_STATUS_CH_STOP, audio_callback_ch_stop);

	return GUI_OK;
}


uint8_t	page_render_info(void) {

	G2D_TextBlend(240, 30, FONT_24_verdana, "Audio Demo", BSP_LCD_Color(C_WHITE, 255));
	GUI_Slider(&slider00, mix);

	GUI_Button(&button00);
	GUI_Button(&button01);
	GUI_Button(&button02);
	GUI_Button(&button03);
	GUI_Button(&button04);
	GUI_Button(&button05);

	return GUI_OK;
}

