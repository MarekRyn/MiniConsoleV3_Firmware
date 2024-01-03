/*
 * page_clock.c
 *
 *  Created on: Dec 31, 2023
 *      Author: marek
 */

#include "page_clock.h"

static uint8_t clock_state = 0;

static uint8_t hour, minute, second;
static uint16_t year;
static uint8_t	month, day, wday;

static GUI_Panel_TypeDef panel00;
static GUI_Panel_TypeDef panel01;

static GUI_Button_TypeDef button00;
static GUI_Button_TypeDef button01;
static GUI_Button_TypeDef button02;
static GUI_Button_TypeDef button03;
static GUI_Button_TypeDef button04;
static GUI_Button_TypeDef button05;

static GUI_Button_TypeDef button06;
static GUI_Button_TypeDef button07;
static GUI_Button_TypeDef button08;
static GUI_Button_TypeDef button09;
static GUI_Button_TypeDef button10;
static GUI_Button_TypeDef button11;

static GUI_Button_TypeDef button12;
static GUI_Button_TypeDef button13;


static uint8_t isLeap(uint16_t y) {
	uint8_t leapyear = 0;
	if (!(y % 4)) leapyear = 1;
	if (!(y % 100)) leapyear = 0;
	if (!(y % 400)) leapyear = 1;
	return leapyear;
}

static uint8_t getDaysNo(uint16_t y, uint8_t m) {
	uint8_t leapyear = isLeap(y);
	uint8_t monthsdays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	uint8_t daysno = monthsdays[m - 1];
	if (m == 2) if (leapyear) daysno = 29;
	return daysno;
}

static void button00_callback(void) {
	if (BSP_hlcdtp.gest_data.gest != LCD_TP_GEST_CLICK_DOWN) return;
	if (clock_state != 1) return;
	hour++;
	if (hour == 24) hour = 0;
}


static void button01_callback(void) {
	if (BSP_hlcdtp.gest_data.gest != LCD_TP_GEST_CLICK_DOWN) return;
	if (clock_state != 1) return;
	minute++;
	if (minute == 60) minute = 0;
}


static void button02_callback(void) {
	if (BSP_hlcdtp.gest_data.gest != LCD_TP_GEST_CLICK_DOWN) return;
	if (clock_state != 1) return;
	second++;
	if (second == 60) second = 0;
}


static void button03_callback(void) {
	if (BSP_hlcdtp.gest_data.gest != LCD_TP_GEST_CLICK_DOWN) return;
	if (clock_state != 1) return;
	hour--;
	if (hour >= 24) hour = 23;
}


static void button04_callback(void) {
	if (BSP_hlcdtp.gest_data.gest != LCD_TP_GEST_CLICK_DOWN) return;
	if (clock_state != 1) return;
	minute--;
	if (minute >= 60) minute = 59;
}


static void button05_callback(void) {
	if (BSP_hlcdtp.gest_data.gest != LCD_TP_GEST_CLICK_DOWN) return;
	if (clock_state != 1) return;
	second--;
	if (second >= 60) second = 59;
}


static void button06_callback(void) {
	if (BSP_hlcdtp.gest_data.gest != LCD_TP_GEST_CLICK_DOWN) return;
	if (clock_state != 2) return;
	year++;
	if (year > 2099) year = 2000;
	if (day > getDaysNo(year, month)) day = getDaysNo(year, month);
}


static void button07_callback(void) {
	if (BSP_hlcdtp.gest_data.gest != LCD_TP_GEST_CLICK_DOWN) return;
	if (clock_state != 2) return;
	month++;
	if (month > 12) month = 1;
	if (day > getDaysNo(year, month)) day = getDaysNo(year, month);
}


static void button08_callback(void) {
	if (BSP_hlcdtp.gest_data.gest != LCD_TP_GEST_CLICK_DOWN) return;
	if (clock_state != 2) return;
	day++;
	if (day > getDaysNo(year, month)) day = 1;
}


static void button09_callback(void) {
	if (BSP_hlcdtp.gest_data.gest != LCD_TP_GEST_CLICK_DOWN) return;
	if (clock_state != 2) return;
	year--;
	if (year < 2000) year = 2099;
	if (day > getDaysNo(year, month)) day = getDaysNo(year, month);
}


static void button10_callback(void) {
	if (BSP_hlcdtp.gest_data.gest != LCD_TP_GEST_CLICK_DOWN) return;
	if (clock_state != 2) return;
	month--;
	if (month < 1) month = 12;
	if (day > getDaysNo(year, month)) day = getDaysNo(year, month);
}


static void button11_callback(void) {
	if (BSP_hlcdtp.gest_data.gest != LCD_TP_GEST_CLICK_DOWN) return;
	if (clock_state != 2) return;
	day--;
	if (day < 1) day = getDaysNo(year, month);
}


static void button12_callback(void) {
	if (BSP_hlcdtp.gest_data.gest != LCD_TP_GEST_CLICK_DOWN) return;
	switch (clock_state) {
	case 0:
		clock_state = 1;
		button13.state = GUI_STATE_DISABLED;
		button12.text = "Save";
		button00.state = GUI_STATE_ENABLED;
		button01.state = GUI_STATE_ENABLED;
		button02.state = GUI_STATE_ENABLED;
		button03.state = GUI_STATE_ENABLED;
		button04.state = GUI_STATE_ENABLED;
		button05.state = GUI_STATE_ENABLED;
		break;
	case 1:
		BSP_STM32_RTC_SetTime(RTC, hour, minute, second);
		clock_state = 0;
		button13.state = GUI_STATE_ENABLED;
		button12.text = "Set";
		button00.state = GUI_STATE_DISABLED;
		button01.state = GUI_STATE_DISABLED;
		button02.state = GUI_STATE_DISABLED;
		button03.state = GUI_STATE_DISABLED;
		button04.state = GUI_STATE_DISABLED;
		button05.state = GUI_STATE_DISABLED;
		break;
	case 2:
		break;
	}
}

static void button13_callback(void) {
	if (BSP_hlcdtp.gest_data.gest != LCD_TP_GEST_CLICK_DOWN) return;
	switch (clock_state) {
	case 0:
		clock_state = 2;
		button12.state = GUI_STATE_DISABLED;
		button13.text = "Save";
		button06.state = GUI_STATE_ENABLED;
		button07.state = GUI_STATE_ENABLED;
		button08.state = GUI_STATE_ENABLED;
		button09.state = GUI_STATE_ENABLED;
		button10.state = GUI_STATE_ENABLED;
		button11.state = GUI_STATE_ENABLED;
		break;
	case 1:
		break;
	case 2:
		BSP_STM32_RTC_SetDate(RTC, year, month, day);
		clock_state = 0;
		button12.state = GUI_STATE_ENABLED;
		button13.text = "Set";
		button06.state = GUI_STATE_DISABLED;
		button07.state = GUI_STATE_DISABLED;
		button08.state = GUI_STATE_DISABLED;
		button09.state = GUI_STATE_DISABLED;
		button10.state = GUI_STATE_DISABLED;
		button11.state = GUI_STATE_DISABLED;
		break;
	}
}



uint8_t page_init_clock() {

	BSP_LCD_TP_RemoveAreaRange(10, LCD_TP_AREA_NO - 1);

	clock_state = 0;

	// Panel00 - Main Area
	panel00.x_pos = 220;
	panel00.y_pos = 10;
	panel00.width = 570;
	panel00.height = 220;

	// Panel01 - Main Area
	panel01.x_pos = 220;
	panel01.y_pos = 250;
	panel01.width = 570;
	panel01.height = 220;

	// Button00 - Hour Inc
	button00.x_pos = 250;
	button00.y_pos = 40;
	button00.height = 40;
	button00.width = 80;
	button00.icon = ICON_32_Up;
	button00.text = NULL;
	button00.state = GUI_STATE_DISABLED;
	BSP_LCD_TP_RegisterArea(10, 250, 40, 80, 40, button00_callback);


	// Button01 - Minute Inc
	button01.x_pos = 360;
	button01.y_pos = 40;
	button01.height = 40;
	button01.width = 80;
	button01.icon = ICON_32_Up;
	button01.text = NULL;
	button01.state = GUI_STATE_DISABLED;
	BSP_LCD_TP_RegisterArea(11, 360, 40, 80, 40, button01_callback);

	// Button02 - Second Inc
	button02.x_pos = 470;
	button02.y_pos = 40;
	button02.height = 40;
	button02.width = 80;
	button02.icon = ICON_32_Up;
	button02.text = NULL;
	button02.state = GUI_STATE_DISABLED;
	BSP_LCD_TP_RegisterArea(12, 470, 40, 80, 40, button02_callback);

	// Button03 - Hour Dec
	button03.x_pos = 250;
	button03.y_pos = 160;
	button03.height = 40;
	button03.width = 80;
	button03.icon = ICON_32_Down;
	button03.text = NULL;
	button03.state = GUI_STATE_DISABLED;
	BSP_LCD_TP_RegisterArea(13, 250, 160, 80, 40, button03_callback);

	// Button04 - Minute Dec
	button04.x_pos = 360;
	button04.y_pos = 160;
	button04.height = 40;
	button04.width = 80;
	button04.icon = ICON_32_Down;
	button04.text = NULL;
	button04.state = GUI_STATE_DISABLED;
	BSP_LCD_TP_RegisterArea(14, 360, 160, 80, 40, button04_callback);

	// Button05 - Second Dec
	button05.x_pos = 470;
	button05.y_pos = 160;
	button05.height = 40;
	button05.width = 80;
	button05.icon = ICON_32_Down;
	button05.text = NULL;
	button05.state = GUI_STATE_DISABLED;
	BSP_LCD_TP_RegisterArea(15, 470, 160, 80, 40, button05_callback);

	// Button06 - Year Inc
	button06.x_pos = 250;
	button06.y_pos = 280;
	button06.height = 40;
	button06.width = 160;
	button06.icon = ICON_32_Up;
	button06.text = NULL;
	button06.state = GUI_STATE_DISABLED;
	BSP_LCD_TP_RegisterArea(16, 250, 280, 160, 40, button06_callback);

	// Button07 - Month Inc
	button07.x_pos = 440;
	button07.y_pos = 280;
	button07.height = 40;
	button07.width = 80;
	button07.icon = ICON_32_Up;
	button07.text = NULL;
	button07.state = GUI_STATE_DISABLED;
	BSP_LCD_TP_RegisterArea(17, 440, 280, 80, 40, button07_callback);

	// Button08 - Day Inc
	button08.x_pos = 550;
	button08.y_pos = 280;
	button08.height = 40;
	button08.width = 80;
	button08.icon = ICON_32_Up;
	button08.text = NULL;
	button08.state = GUI_STATE_DISABLED;
	BSP_LCD_TP_RegisterArea(18, 550, 280, 80, 40, button08_callback);

	// Button09 - Year Dec
	button09.x_pos = 250;
	button09.y_pos = 400;
	button09.height = 40;
	button09.width = 160;
	button09.icon = ICON_32_Down;
	button09.text = NULL;
	button09.state = GUI_STATE_DISABLED;
	BSP_LCD_TP_RegisterArea(19, 250, 400, 160, 40, button09_callback);

	// Button10 - Month Dec
	button10.x_pos = 440;
	button10.y_pos = 400;
	button10.height = 40;
	button10.width = 80;
	button10.icon = ICON_32_Down;
	button10.text = NULL;
	button10.state = GUI_STATE_DISABLED;
	BSP_LCD_TP_RegisterArea(20, 440, 400, 80, 40, button10_callback);

	// Button11 - Day Dec
	button11.x_pos = 550;
	button11.y_pos = 400;
	button11.height = 40;
	button11.width = 80;
	button11.icon = ICON_32_Down;
	button11.text = NULL;
	button11.state = GUI_STATE_DISABLED;
	BSP_LCD_TP_RegisterArea(21, 550, 400, 80, 40, button11_callback);

	// Button12 - Set Clock
	button12.x_pos = 645;
	button12.y_pos = 100;
	button12.height = 50;
	button12.width = 120;
	button12.icon = ICON_32_Clock;
	button12.text = "Set";
	button12.state = GUI_STATE_ENABLED;
	BSP_LCD_TP_RegisterArea(22, 645, 100, 120, 50, button12_callback);

	// Button13 - Set Clock
	button13.x_pos = 645;
	button13.y_pos = 340;
	button13.height = 50;
	button13.width = 120;
	button13.icon = ICON_32_Calendar;
	button13.text = "Set";
	button13.state = GUI_STATE_ENABLED;
	BSP_LCD_TP_RegisterArea(23, 645, 340, 120, 50, button13_callback);

	return GUI_OK;
}


uint8_t	page_render_clock() {

	int16_t xx = 250;
	int16_t yy = 80;
	char str[6];


	if (clock_state != 1) BSP_STM32_RTC_GetTime(RTC, &hour, &minute, &second);
	if (clock_state != 2) BSP_STM32_RTC_GetDate(RTC, &year, &month, &day, &wday);

	GUI_Panel(&panel00);
	GUI_Panel(&panel01);

	GUI_Button(&button00);
	GUI_Button(&button01);
	GUI_Button(&button02);
	GUI_Button(&button03);
	GUI_Button(&button04);
	GUI_Button(&button05);
	GUI_Button(&button06);
	GUI_Button(&button07);
	GUI_Button(&button08);
	GUI_Button(&button09);
	GUI_Button(&button10);
	GUI_Button(&button11);
	GUI_Button(&button12);
	GUI_Button(&button13);


	sprintf(str, "%02d", hour);
	G2D_TextBlend(xx, yy, FONT_64_verdana, str, BSP_LCD_Color(C_WHITE, 255));
	xx+=80;
	G2D_TextBlend(xx, yy, FONT_64_verdana, ":", BSP_LCD_Color(C_WHITE, 255));
	xx+=30;

	sprintf(str, "%02d", minute);
	G2D_TextBlend(xx, yy, FONT_64_verdana, str, BSP_LCD_Color(C_WHITE, 255));
	xx+=80;
	G2D_TextBlend(xx, yy, FONT_64_verdana, ":", BSP_LCD_Color(C_WHITE, 255));
	xx+=30;

	sprintf(str, "%02d", second);
	G2D_TextBlend(xx, yy, FONT_64_verdana, str, BSP_LCD_Color(C_WHITE, 255));

	xx = 250;
	yy = 320;

	sprintf(str, "%04d", year);
	G2D_TextBlend(xx, yy, FONT_64_verdana, str, BSP_LCD_Color(C_WHITE, 255));
	xx+=160;
	G2D_TextBlend(xx, yy, FONT_64_verdana, "/", BSP_LCD_Color(C_WHITE, 255));
	xx+=30;

	sprintf(str, "%02d", month);
	G2D_TextBlend(xx, yy, FONT_64_verdana, str, BSP_LCD_Color(C_WHITE, 255));
	xx+=80;
	G2D_TextBlend(xx, yy, FONT_64_verdana, "/", BSP_LCD_Color(C_WHITE, 255));
	xx+=30;

	sprintf(str, "%02d", day);
	G2D_TextBlend(xx, yy, FONT_64_verdana, str, BSP_LCD_Color(C_WHITE, 255));

	return GUI_OK;
}
