/*
 * page_imu.c
 *
 *  Created on: Dec 13, 2023
 *      Author: marek
 */

#include "page_imu.h"

static uint8_t imu_state = 0;
static uint8_t cal_state = 0;
static int16_t graph_buf[3][490] = {0};
static uint8_t graph_flag[490] = {0};
static uint16_t graph_i = 0;

static GUI_Button_TypeDef		button00;
static GUI_Button_TypeDef		button01;
static GUI_Button_TypeDef		button02;
static GUI_Button_TypeDef		button03;
static GUI_ProgressBar_TypeDef 	pbar00;


static void button00_callback(void) {
	if (BSP_hlcdtp.gest_data.gest != LCD_TP_GEST_CLICK_DOWN) return;
	button00.state = GUI_STATE_ACTIVE;
	button01.state = GUI_STATE_ENABLED;
	button02.state = GUI_STATE_ENABLED;
	memset(graph_buf, 0, sizeof(graph_buf));
	memset(graph_flag, 0, sizeof(graph_flag));
	imu_state = 0;
}

static void button01_callback(void) {
	if (BSP_hlcdtp.gest_data.gest != LCD_TP_GEST_CLICK_DOWN) return;
	button00.state = GUI_STATE_ENABLED;
	button01.state = GUI_STATE_ACTIVE;
	button02.state = GUI_STATE_ENABLED;
	memset(graph_buf, 0, sizeof(graph_buf));
	memset(graph_flag, 0, sizeof(graph_flag));
	imu_state = 1;
}

static void button02_callback(void) {
	if (BSP_hlcdtp.gest_data.gest != LCD_TP_GEST_CLICK_DOWN) return;
	button00.state = GUI_STATE_ENABLED;
	button01.state = GUI_STATE_ENABLED;
	button02.state = GUI_STATE_ACTIVE;
	memset(graph_buf, 0, sizeof(graph_buf));
	memset(graph_flag, 0, sizeof(graph_flag));
	imu_state = 2;
}

static void button03_callback(void) {
	if (BSP_hlcdtp.gest_data.gest != LCD_TP_GEST_CLICK_DOWN) return;

	switch (cal_state) {
	case 0:
		BSP_IMU_Calibrate();
		cal_state = 1;
		break;
	case 1:
		break;
	case 2:
		BSP_IMU_SaveCalData();
		cal_state = 0;
	}
}



uint8_t page_init_imu(void) {

	BSP_LCD_TP_RemoveAreaRange(10, LCD_TP_AREA_NO - 1);

	imu_state = 0;
	cal_state = 0;

	// Button00 - Test Orientation
	button00.x_pos = 240;
	button00.y_pos = 30;
	button00.width = 160;
	button00.height = 42;
	button00.icon = NULL;
	button00.text = "Orientation";
	button00.state = GUI_STATE_ACTIVE;
	BSP_LCD_TP_RegisterArea(10, 240, 30, 160, 42, button00_callback);

	// Button01 - Test Acceleration
	button01.x_pos = 425;
	button01.y_pos = 30;
	button01.width = 160;
	button01.height = 42;
	button01.icon = NULL;
	button01.text = "Acceleration";
	button01.state = GUI_STATE_ENABLED;
	BSP_LCD_TP_RegisterArea(11, 425, 30, 160, 42, button01_callback);

	// Button02 - Test Gyro
	button02.x_pos = 610;
	button02.y_pos = 30;
	button02.width = 160;
	button02.height = 42;
	button02.icon = NULL;
	button02.text = "Gyro";
	button02.state = GUI_STATE_ENABLED;
	BSP_LCD_TP_RegisterArea(12, 610, 30, 160, 42, button02_callback);

	// Button03 - IMU Calibration
	button03.x_pos = 590;
	button03.y_pos = 350;
	button03.width = 160;
	button03.height = 42;
	button03.icon = ICON_32_Tools;
	button03.text = "Calibrate";
	button03.state = GUI_STATE_ENABLED;
	BSP_LCD_TP_RegisterArea(13, 590, 350, 160, 42, button03_callback);

	// ProgressBar00 - IMU Calibration
	pbar00.max_value = 128;
	pbar00.min_value = 0;
	pbar00.width = 490;
	pbar00.x_pos = 260;
	pbar00.y_pos = 415;

	return GUI_OK;

}


uint8_t	page_render_imu(void) {

	GUI_Button(&button00);
	GUI_Button(&button01);
	GUI_Button(&button02);

	// Graph
	G2D_DrawFillRoundRect(240, 90, 530, 240, 8, BSP_LCD_Color(C_BLACK, 255));
	G2D_DrawRoundRect(240, 90, 530, 240, 8, BSP_LCD_Color(C_WHITE, 255));

	switch (imu_state) {
	case 0:
		graph_buf[0][graph_i] = ((int16_t)BSP_himu.pos.pitch) >> 1;
		graph_buf[1][graph_i] = ((int16_t)BSP_himu.pos.roll) >> 1;
		graph_buf[2][graph_i] = ((int16_t)BSP_himu.pos.yaw) >> 1;
		break;
	case 1:
		graph_buf[0][graph_i] = (int16_t)(BSP_himu.data.x * 50);
		graph_buf[1][graph_i] = (int16_t)(BSP_himu.data.y * 50);
		graph_buf[2][graph_i] = (int16_t)(BSP_himu.data.z * 50);
		break;
	case 2:
		graph_buf[0][graph_i] = (int16_t)(BSP_himu.data.ox / 5);
		graph_buf[1][graph_i] = (int16_t)(BSP_himu.data.oy / 5);
		graph_buf[2][graph_i] = (int16_t)(BSP_himu.data.oz / 5);
		break;
	}
	graph_flag[graph_i] = 1;


	graph_i++;
	graph_i = graph_i % 490;

	for(uint16_t i = 0; i<490; i++) {
		uint16_t ii = (graph_i + i) % 490;
		if (!graph_flag[ii]) continue;
		uint16_t x = i + 260;
		G2D_DrawPixel(x, graph_buf[0][ii] + 210, BSP_LCD_Color(C_RED, 255));
		G2D_DrawPixel(x, graph_buf[1][ii] + 210, BSP_LCD_Color(C_GREEN, 255));
		G2D_DrawPixel(x, graph_buf[2][ii] + 210, BSP_LCD_Color(C_BLUE, 255));
	}

	G2D_TextBlend(260, 348, FONT_16_verdana, "To calibrate IMU unit, place console on", BSP_LCD_Color(C_WHITE, 255));
	G2D_TextBlend(260, 374, FONT_16_verdana, "flat surface and press [Calibrate].", BSP_LCD_Color(C_WHITE, 255));

	switch (cal_state) {
	case 0:
		button03.state = GUI_STATE_ENABLED;
		button03.text = "Calibrate";
		break;
	case 1:
		button03.state = GUI_STATE_DISABLED;
		GUI_ProgressBar(&pbar00, 128 - BSP_himu.cal.progress);
		if (BSP_himu.cal.progress == 0) cal_state = 2;
		break;
	case 2:
		button03.state = GUI_STATE_ENABLED;
		button03.text = "Save";
		GUI_ProgressBar(&pbar00, 128);
		break;
	}

	GUI_Button(&button03);

	return GUI_OK;
}


