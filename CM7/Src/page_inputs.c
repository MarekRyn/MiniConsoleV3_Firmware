/*
 * page_inputs.c
 *
 *  Created on: Dec 13, 2023
 *      Author: marek
 */

#include "page_inputs.h"

static uint8_t cal_state = 0;
static uint32_t cal_progress, cal1_progress, cal2_progress;
static uint32_t	cal1_area, cal1_parea;
static uint32_t	cal2_area, cal2_parea;

static int32_t obj_x = 0;
static int32_t obj_y = 0;

static GUI_Button_TypeDef		button00;
static GUI_ProgressBar_TypeDef	pbar00;

static void button00_callback(void){
	switch(BSP_hlcdtp.gest_data.gest) {
	case LCD_TP_GEST_CLICK_DOWN:
		GUI_Button_Click(&button00);

		if (cal_state == 0) {
			cal_state = 1;
			break;
		}

		if ((cal_state > 0) && (cal_state < 4)) {
			BSP_Inputs_CancelCallibration();
			BSP_Delay(500);
			button00.text = "Calibrate";
			cal_state = 0;
			break;
		}

		if (cal_state == 4) {
			BSP_Inputs_SaveCalData();
			BSP_Delay(500);
			button00.text = "Calibrate";
			cal_state = 0;
			break;
		}
		break;
	}
}

uint8_t page_init_inputs(void) {

	BSP_LCD_TP_RemoveAreaRange(10, LCD_TP_AREA_NO - 1);

	cal_state = 0;

	// Button00 - Joystick Calibration
	button00.x_pos = 590;
	button00.y_pos = 350;
	button00.width = 160;
	button00.height = 42;
	button00.icon = (uint32_t)ICON_32_Tools;
	button00.text = "Calibrate";
	button00.state = GUI_STATE_ENABLED;
	BSP_LCD_TP_RegisterArea(10, 590, 350, 160, 42, button00_callback);


	// ProgressBar00 - Joystick Calibration
	pbar00.max_value = 100;
	pbar00.min_value = 0;
	pbar00.width = 490;
	pbar00.x_pos = 260;
	pbar00.y_pos = 415;

	return GUI_OK;
}


uint8_t	page_render_inputs(void) {

	uint32_t color;

	G2D_DrawFillRoundRect(240, 50, 530, 250, 20, BSP_LCD_Color(C_DARKGRAY, 255));
	G2D_DrawRoundRect(240, 50, 530, 250, 20, BSP_LCD_Color(C_WHITE, 255));
	G2D_DrawFillRoundRect(355, 70, 300, 210, 5, BSP_LCD_Color(0x00202020, 255));
	G2D_DrawRoundRect(355, 70, 300, 210, 5, BSP_LCD_Color(C_WHITE, 255));

	// BTN-PWR
	color = (BSP_hinputs.buttons.btn_PWR)?BSP_LCD_Color(C_RED, 255):BSP_LCD_Color(0x00202020, 255);
	G2D_DrawFillRect(270, 42, 40, 8, color);
	G2D_DrawRect(270, 42, 40, 9, BSP_LCD_Color(C_WHITE, 255));

	//BTN-MENU
	color = (BSP_hinputs.buttons.btn_MENU)?BSP_LCD_Color(C_RED, 255):BSP_LCD_Color(0x00202020, 255);
	G2D_DrawFillRect(700, 42, 40, 8, color);
	G2D_DrawRect(700, 42, 40, 9, BSP_LCD_Color(C_WHITE, 255));

	// BTN-A
	color = (BSP_hinputs.buttons.btn_A)?BSP_LCD_Color(C_RED, 255):BSP_LCD_Color(0x00202020, 255);
	G2D_DrawFillCircle(285, 90, 15, color);
	G2D_DrawCircle(285, 90, 15, BSP_LCD_Color(C_WHITE, 255));

	// BTN-B
	color = (BSP_hinputs.buttons.btn_B)?BSP_LCD_Color(C_RED, 255):BSP_LCD_Color(0x00202020, 255);
	G2D_DrawFillCircle(295, 140, 15, color);
	G2D_DrawCircle(295, 140, 15, BSP_LCD_Color(C_WHITE, 255));

	// BTN-C
	color = (BSP_hinputs.buttons.btn_C)?BSP_LCD_Color(C_RED, 255):BSP_LCD_Color(0x00202020, 255);
	G2D_DrawFillCircle(725, 90, 15, color);
	G2D_DrawCircle(725, 90, 15, BSP_LCD_Color(C_WHITE, 255));

	// BTN-D
	color = (BSP_hinputs.buttons.btn_D)?BSP_LCD_Color(C_RED, 255):BSP_LCD_Color(0x00202020, 255);
	G2D_DrawFillCircle(715, 140, 15, color);
	G2D_DrawCircle(715, 140, 15, BSP_LCD_Color(C_WHITE, 255));

	// BTN-UP
	color = (BSP_hinputs.buttons.btn_X_U)?BSP_LCD_Color(C_RED, 255):BSP_LCD_Color(0x00202020, 255);
	G2D_DrawFillRoundRect(285, 180, 20, 30, 3, color);
	G2D_DrawRoundRect(285, 180, 20, 30, 3, BSP_LCD_Color(C_WHITE, 255));

	// BTN-DOWN
	color = (BSP_hinputs.buttons.btn_X_D)?BSP_LCD_Color(C_RED, 255):BSP_LCD_Color(0x00202020, 255);
	G2D_DrawFillRoundRect(285, 240, 20, 30, 3, color);
	G2D_DrawRoundRect(285, 240, 20, 30, 3, BSP_LCD_Color(C_WHITE, 255));

	// BTN-LEFT
	color = (BSP_hinputs.buttons.btn_X_L)?BSP_LCD_Color(C_RED, 255):BSP_LCD_Color(0x00202020, 255);
	G2D_DrawFillRoundRect(250, 215, 30, 20, 2, color);
	G2D_DrawRoundRect(250, 215, 30, 20, 2, BSP_LCD_Color(C_WHITE, 255));

	// BTN-RIGHT
	color = (BSP_hinputs.buttons.btn_X_R)?BSP_LCD_Color(C_RED, 255):BSP_LCD_Color(0x00202020, 255);
	G2D_DrawFillRoundRect(310, 215, 30, 20, 2, color);
	G2D_DrawRoundRect(310, 215, 30, 20, 2, BSP_LCD_Color(C_WHITE, 255));

	// Joystick
	int16_t dx = BSP_hinputs.joy.joy_X / 8;
	int16_t dy = BSP_hinputs.joy.joy_Y / 8;

	G2D_DrawCircle(715 ,225, 30, BSP_LCD_Color(C_WHITE, 255));
	color = (BSP_hinputs.buttons.btn_JOY)?BSP_LCD_Color(C_RED, 255):BSP_LCD_Color(0x00202020, 255);
	G2D_DrawFillCircle(715 + dx, 225 + dy, 25, color);
	G2D_DrawCircle(715 + dx, 225 + dy, 25, BSP_LCD_Color(C_WHITE, 255));

	// Demo object on screen
	obj_x += BSP_hinputs.joy.joy_X;
	obj_y += BSP_hinputs.joy.joy_Y;

	if (obj_x > 1400) obj_x = 1400;
	if (obj_x < -1400) obj_x = -1400;
	if (obj_y > 950) obj_y = 950;
	if (obj_y < -950) obj_y = -950;
	G2D_DrawFillCircle(505 + (obj_x / 10), 175 + (obj_y / 10), 5, BSP_LCD_Color(C_BLUE, 255));

	// Calibration Area
	switch (cal_state) {
	case 0:
		G2D_TextBlend(260, 348, FONT_16_verdana, "To calibrate joystick, set it to center", BSP_LCD_Color(C_WHITE, 255));
		G2D_TextBlend(260, 374, FONT_16_verdana, "position and press [Calibrate].", BSP_LCD_Color(C_WHITE, 255));
		GUI_Button(&button00);
		break;
	case 1:
		BSP_Inputs_CalibrateJoyInit();
		button00.text = "Abort";
		cal_progress = 0;
		cal1_progress = 0;
		cal1_area = 0;
		cal1_parea = 0;
		cal2_progress = 0;
		cal2_area = 0;
		cal2_parea = 0;

		BSP_Delay(500);
		cal_state = 2;
		BSP_Inputs_CalibrateJoyDeadZone();
	case 2:
		cal1_area = (BSP_hinputs.joy_cal.x_center_max - BSP_hinputs.joy_cal.x_center_min) * (BSP_hinputs.joy_cal.y_center_max - BSP_hinputs.joy_cal.y_center_min);
		if (BSP_hinputs.joy_cal.x_center_max < BSP_hinputs.joy_cal.x_center_min) cal1_area = 0;
		if (BSP_hinputs.joy_cal.y_center_max < BSP_hinputs.joy_cal.y_center_min) cal1_area = 0;

		if (cal1_area > cal1_parea) {
			cal1_progress = 0;
			cal1_parea = cal1_area;
		} else cal1_progress++;

		if (cal1_progress > cal_progress) cal_progress = cal1_progress;

		G2D_TextBlend(260, 348, FONT_16_verdana, "STEP 1/2: Calibrating dead zone.", BSP_LCD_Color(C_WHITE, 255));
		G2D_TextBlend(260, 374, FONT_16_verdana, "Do not touch Joystick.", BSP_LCD_Color(C_RED, 255));
		GUI_Button(&button00);
		GUI_ProgressBar(&pbar00, cal_progress);

		if (cal1_progress >= 100) {
			cal_state = 3;
			cal_progress = 0;
			BSP_Inputs_CalibrateJoyRange();
		}

		break;

	case 3:
		cal2_area = (BSP_hinputs.joy_cal.x_max - BSP_hinputs.joy_cal.x_min) * (BSP_hinputs.joy_cal.y_max - BSP_hinputs.joy_cal.y_min);
		if (BSP_hinputs.joy_cal.x_max < BSP_hinputs.joy_cal.x_min) cal2_area = 0;
		if (BSP_hinputs.joy_cal.y_max < BSP_hinputs.joy_cal.y_min) cal2_area = 0;
		if (cal2_area < cal1_area) cal2_progress = 0;
		if (cal2_area > cal2_parea) {
			cal2_progress = 0;
			cal2_parea = cal2_area;
		} else cal2_progress++;

		if (cal2_progress > cal_progress) cal_progress = cal2_progress;

		G2D_TextBlend(260, 348, FONT_16_verdana, "STEP 2/2: Calibrating max range.", BSP_LCD_Color(C_WHITE, 255));
		G2D_TextBlend(260, 374, FONT_16_verdana, "Rotate Joystick in all directions.", BSP_LCD_Color(C_RED, 255));
		GUI_Button(&button00);
		GUI_ProgressBar(&pbar00, cal_progress);

		if (cal2_progress >= 100) {
			cal_state = 4;
			button00.text = "Save";
		}

		break;

	case 4:
		G2D_TextBlend(260, 348, FONT_16_verdana, "Calibration completed.", BSP_LCD_Color(C_WHITE, 255));
		G2D_TextBlend(260, 374, FONT_16_verdana, "Press SAVE to store calibration data.", BSP_LCD_Color(C_WHITE, 255));
		GUI_Button(&button00);

		break;
	}




	return GUI_OK;
}


