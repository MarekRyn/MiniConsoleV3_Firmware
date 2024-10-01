/*
 * page_main.c
 *
 *  Created on: Dec 13, 2023
 *      Author: marek
 */


#include "page_main.h"

static uint32_t	prev_state = 0xFFFFFFFF;

static GUI_Button_TypeDef	button00;
static GUI_Button_TypeDef 	button01;
static GUI_Button_TypeDef 	button02;
static GUI_Button_TypeDef 	button03;
static GUI_Button_TypeDef 	button04;
static GUI_Button_TypeDef 	button05;
static GUI_Button_TypeDef	button06;
static GUI_Button_TypeDef	button07;
static GUI_Button_TypeDef	button08;
static GUI_Panel_TypeDef	panel00;

static void button00_callback(void){
	switch(BSP_hlcdtp.gest_data.gest) {
	case LCD_TP_GEST_CLICK_DOWN:
		button00.state = GUI_STATE_ACTIVE;
		button01.state = GUI_STATE_ENABLED;
		button02.state = GUI_STATE_ENABLED;
		button03.state = GUI_STATE_ENABLED;
		button04.state = GUI_STATE_ENABLED;
		button05.state = GUI_STATE_ENABLED;
		button06.state = GUI_STATE_ENABLED;
		button07.state = GUI_STATE_ENABLED;
		button08.state = GUI_STATE_ENABLED;
		state1 = STATE1_APPS;
		page_init_apps();
		break;
	}
}

static void button01_callback(void){
	switch(BSP_hlcdtp.gest_data.gest) {
	case LCD_TP_GEST_CLICK_DOWN:
		button00.state = GUI_STATE_ENABLED;
		button01.state = GUI_STATE_ACTIVE;
		button02.state = GUI_STATE_ENABLED;
		button03.state = GUI_STATE_ENABLED;
		button04.state = GUI_STATE_ENABLED;
		button05.state = GUI_STATE_ENABLED;
		button06.state = GUI_STATE_ENABLED;
		button07.state = GUI_STATE_ENABLED;
		button08.state = GUI_STATE_ENABLED;
		state1 = STATE1_INPUTS;
		page_init_inputs();
		break;
	}
}

static void button02_callback(void){
	switch(BSP_hlcdtp.gest_data.gest) {
	case LCD_TP_GEST_CLICK_DOWN:
		button00.state = GUI_STATE_ENABLED;
		button01.state = GUI_STATE_ENABLED;
		button02.state = GUI_STATE_ACTIVE;
		button03.state = GUI_STATE_ENABLED;
		button04.state = GUI_STATE_ENABLED;
		button05.state = GUI_STATE_ENABLED;
		button06.state = GUI_STATE_ENABLED;
		button07.state = GUI_STATE_ENABLED;
		button08.state = GUI_STATE_ENABLED;
		state1 = STATE1_IMU;
		page_init_imu();
		break;
	}
}

static void button03_callback(void){
	switch(BSP_hlcdtp.gest_data.gest) {
	case LCD_TP_GEST_CLICK_DOWN:
		button00.state = GUI_STATE_ENABLED;
		button01.state = GUI_STATE_ENABLED;
		button02.state = GUI_STATE_ENABLED;
		button03.state = GUI_STATE_ACTIVE;
		button04.state = GUI_STATE_ENABLED;
		button05.state = GUI_STATE_ENABLED;
		button06.state = GUI_STATE_ENABLED;
		button07.state = GUI_STATE_ENABLED;
		button08.state = GUI_STATE_ENABLED;
		state1 = STATE1_AUDIO;
		page_init_audio();
		break;
	}
}

static void button04_callback(void){
	switch(BSP_hlcdtp.gest_data.gest) {
	case LCD_TP_GEST_CLICK_DOWN:
		button00.state = GUI_STATE_ENABLED;
		button01.state = GUI_STATE_ENABLED;
		button02.state = GUI_STATE_ENABLED;
		button03.state = GUI_STATE_ENABLED;
		button04.state = GUI_STATE_ACTIVE;
		button05.state = GUI_STATE_ENABLED;
		button06.state = GUI_STATE_ENABLED;
		button07.state = GUI_STATE_ENABLED;
		button08.state = GUI_STATE_ENABLED;
		state1 = STATE1_SCREEN;
		page_init_screen();
		break;
	}
}

static void button05_callback(void){
	switch(BSP_hlcdtp.gest_data.gest) {
	case LCD_TP_GEST_CLICK_DOWN:
		button00.state = GUI_STATE_ENABLED;
		button01.state = GUI_STATE_ENABLED;
		button02.state = GUI_STATE_ENABLED;
		button03.state = GUI_STATE_ENABLED;
		button04.state = GUI_STATE_ENABLED;
		button05.state = GUI_STATE_ACTIVE;
		button06.state = GUI_STATE_ENABLED;
		button07.state = GUI_STATE_ENABLED;
		button08.state = GUI_STATE_ENABLED;
		state1 = STATE1_BT;
		page_init_bt();
		break;
	}
}

static void button06_callback(void){
	switch(BSP_hlcdtp.gest_data.gest) {
	case LCD_TP_GEST_CLICK_DOWN:
		button00.state = GUI_STATE_ENABLED;
		button01.state = GUI_STATE_ENABLED;
		button02.state = GUI_STATE_ENABLED;
		button03.state = GUI_STATE_ENABLED;
		button04.state = GUI_STATE_ENABLED;
		button05.state = GUI_STATE_ENABLED;
		button06.state = GUI_STATE_ACTIVE;
		button07.state = GUI_STATE_ENABLED;
		button08.state = GUI_STATE_ENABLED;
		state1 = STATE1_DATETIME;
		page_init_clock();
		break;
	}
}

static void button07_callback(void){
	switch(BSP_hlcdtp.gest_data.gest) {
	case LCD_TP_GEST_CLICK_DOWN:
		button00.state = GUI_STATE_ENABLED;
		button01.state = GUI_STATE_ENABLED;
		button02.state = GUI_STATE_ENABLED;
		button03.state = GUI_STATE_ENABLED;
		button04.state = GUI_STATE_ENABLED;
		button05.state = GUI_STATE_ENABLED;
		button06.state = GUI_STATE_ENABLED;
		button07.state = GUI_STATE_ACTIVE;
		button08.state = GUI_STATE_ENABLED;
		state1 = STATE1_POWER;
		page_init_pwr();
		break;
	}
}

static void button08_callback(void){
	switch(BSP_hlcdtp.gest_data.gest) {
	case LCD_TP_GEST_CLICK_DOWN:
		button00.state = GUI_STATE_ENABLED;
		button01.state = GUI_STATE_ENABLED;
		button02.state = GUI_STATE_ENABLED;
		button03.state = GUI_STATE_ENABLED;
		button04.state = GUI_STATE_ENABLED;
		button05.state = GUI_STATE_ENABLED;
		button06.state = GUI_STATE_ENABLED;
		button07.state = GUI_STATE_ENABLED;
		button08.state = GUI_STATE_ACTIVE;
		state1 = STATE1_INFO;
		page_init_info();
		break;
	}
}

uint8_t page_init_main() {
	BSP_LCD_TP_RemoveAllAreas();

	// Panel00 - Main Area
	panel00.x_pos = 220;
	panel00.y_pos = 10;
	panel00.width = 570;
	panel00.height = 460;

	// Button00 - Applications
	button00.x_pos = 10;
	button00.y_pos = 10;
	button00.width = 200;
	button00.height = 42;
	button00.icon = ICON_32_Apps;
	button00.text = "Applications";
	button00.state = GUI_STATE_ACTIVE;
	BSP_LCD_TP_RegisterArea(0, 10, 10, 200, 42, button00_callback);

	// Button01 - Inputs
	button01.x_pos = 10;
	button01.y_pos = 60;
	button01.width = 200;
	button01.height = 42;
	button01.icon = ICON_32_Inputs;
	button01.text = "Inputs";
	button01.state = GUI_STATE_ENABLED;
	BSP_LCD_TP_RegisterArea(1, 10, 60, 200, 42, button01_callback);

	// Button02 - IMU
	button02.x_pos = 10;
	button02.y_pos = 110;
	button02.width = 200;
	button02.height = 42;
	button02.icon = ICON_32_IMU;
	button02.text = "IMU";
	button02.state = GUI_STATE_ENABLED;
	BSP_LCD_TP_RegisterArea(2, 10, 110, 200, 42, button02_callback);

	// Button03 - Audio
	button03.x_pos = 10;
	button03.y_pos = 160;
	button03.width = 200;
	button03.height = 42;
	button03.icon = ICON_32_Audio;
	button03.text = "Audio";
	button03.state = GUI_STATE_ENABLED;
	BSP_LCD_TP_RegisterArea(3, 10, 160, 200, 42, button03_callback);

	// Button04 - Screen
	button04.x_pos = 10;
	button04.y_pos = 210;
	button04.width = 200;
	button04.height = 42;
	button04.icon = ICON_32_Screen;
	button04.text = "Screen";
	button04.state = GUI_STATE_ENABLED;
	BSP_LCD_TP_RegisterArea(4, 10, 210, 200, 42, button04_callback);

	// Button05 - Bluetooth
	button05.x_pos = 10;
	button05.y_pos = 260;
	button05.width = 200;
	button05.height = 42;
	button05.icon = ICON_32_BT;
	button05.text = "Bluetooth";
	button05.state = GUI_STATE_ENABLED;
	BSP_LCD_TP_RegisterArea(5, 10, 260, 200, 42, button05_callback);

	// Button06 - DateTime
	button06.x_pos = 10;
	button06.y_pos = 310;
	button06.width = 200;
	button06.height = 42;
	button06.icon = ICON_32_DateTime;
	button06.text = "Clock";
	button06.state = GUI_STATE_ENABLED;
	BSP_LCD_TP_RegisterArea(6, 10, 310, 200, 42, button06_callback);

	// Button07 - Power
	button07.x_pos = 10;
	button07.y_pos = 428;
	button07.width = 95;
	button07.height = 42;
	button07.icon = ICON_32_Power;
	button07.text = 0;
	button07.state = GUI_STATE_ENABLED;
	BSP_LCD_TP_RegisterArea(7, 10, 428, 95, 42, button07_callback);

	// Button08 - Info
	button08.x_pos = 115;
	button08.y_pos = 428;
	button08.width = 95;
	button08.height = 42;
	button08.icon = ICON_32_Info;
	button08.text = 0;
	button08.state = GUI_STATE_ENABLED;
	BSP_LCD_TP_RegisterArea(8, 115, 428, 95, 42, button08_callback);

	return GUI_OK;
}

uint8_t	page_render_main() {

	if (prev_state == state1) {

		// Restore from cache
		BSP_LCD_RestoreFrame();

	} else {

		// Background
		G2D_DrawLastJPEG(0, 0);

		// Main menu
		GUI_Button(&button00);
		GUI_Button(&button01);
		GUI_Button(&button02);
		GUI_Button(&button03);
		GUI_Button(&button04);
		GUI_Button(&button05);
		GUI_Button(&button06);
		GUI_Button(&button07);
		GUI_Button(&button08);

		// Side Panel
		GUI_Panel(&panel00);

		// Caching
		BSP_LCD_CacheFrame();
		prev_state = state1;

	}

	return GUI_OK;
}
