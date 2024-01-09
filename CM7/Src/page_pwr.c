/*
 * page_pwr.c
 *
 *  Created on: Jan 2, 2024
 *      Author: marek
 */

#include "page_pwr.h"

static uint8_t pwr_state = 0;
static uint32_t pwr_counter = 0;
static uint32_t pwr_timestamp;

static GUI_Panel_TypeDef panel00;

static GUI_Button_TypeDef button00;
static GUI_Button_TypeDef button01;


static void button00_callback(void) {
	if (BSP_hlcdtp.gest_data.gest != LCD_TP_GEST_CLICK_DOWN) return;
	switch (pwr_state) {
	case 0:
		button00.state = GUI_STATE_ACTIVE;
		button01.state = GUI_STATE_DISABLED;
		button00.text = "Confirm";
		pwr_state = 1;
		pwr_timestamp = BSP_GetTick();
		break;
	case 1:
		pwr_state = 3;
		break;
	case 2:
		break;
	}
}

static void button01_callback(void) {
	if (BSP_hlcdtp.gest_data.gest != LCD_TP_GEST_CLICK_DOWN) return;
	switch (pwr_state) {
	case 0:
		button01.state = GUI_STATE_ACTIVE;
		button00.state = GUI_STATE_DISABLED;
		button01.text = "Confirm";
		pwr_state = 2;
		pwr_timestamp = BSP_GetTick();
		break;
	case 1:
		break;
	case 2:
		pwr_state = 4;
		break;
	}
}



uint8_t page_init_pwr(void) {
	BSP_LCD_TP_RemoveAreaRange(10, LCD_TP_AREA_NO - 1);

	pwr_state = 0;
	pwr_counter = 0;

	// Panel00 - Main Area
	panel00.x_pos = 220;
	panel00.y_pos = 10;
	panel00.width = 570;
	panel00.height = 460;

	// Button00 - Shut Down
	button00.x_pos = 405;
	button00.y_pos = 150;
	button00.width = 200;
	button00.height = 42;
	button00.icon = (uint32_t)ICON_32_Power;
	button00.text = "Shut Down";
	button00.state = GUI_STATE_ENABLED;
	BSP_LCD_TP_RegisterArea(10, 405, 150, 200, 42, button00_callback);

	// Button01 - Restart
	button01.x_pos = 405;
	button01.y_pos = 250;
	button01.width = 200;
	button01.height = 42;
	button01.icon = (uint32_t)ICON_32_Restart;
	button01.text = "Restart";
	button01.state = GUI_STATE_ENABLED;
	BSP_LCD_TP_RegisterArea(11, 405, 250, 200, 42, button01_callback);

	return GUI_OK;
}


uint8_t	page_render_pwr(void) {
	GUI_Panel(&panel00);
	GUI_Button(&button00);
	GUI_Button(&button01);

	if (pwr_state > 0) {
		pwr_counter++;
		if (!(pwr_counter % 10)) {
			if (pwr_state == 1) { if (button00.state == GUI_STATE_ENABLED) button00.state = GUI_STATE_ACTIVE; else button00.state = GUI_STATE_ENABLED; }
			if (pwr_state == 2) { if (button01.state == GUI_STATE_ENABLED) button01.state = GUI_STATE_ACTIVE; else button01.state = GUI_STATE_ENABLED; }
			if (pwr_state == 3) return 100;
			if (pwr_state == 4) return 200;
		}
		if ((pwr_timestamp + 5000) < BSP_GetTick()) {
			pwr_state = 0;
			button00.text = "Shut Down";
			button00.state = GUI_STATE_ENABLED;
			button01.text = "Restart";
			button01.state = GUI_STATE_ENABLED;
		}
	}

	return GUI_OK;
}
