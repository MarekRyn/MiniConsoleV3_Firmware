/*
 * page_bt.c
 *
 *  Created on: Dec 13, 2023
 *      Author: marek
 */

#include "page_bt.h"

static GUI_Panel_TypeDef	panel00;

uint8_t page_init_bt(void) {

	// Panel00 - Main Area
	panel00.x_pos = 220;
	panel00.y_pos = 10;
	panel00.width = 570;
	panel00.height = 460;

	return GUI_OK;
}


uint8_t	page_render_bt(void) {

	GUI_Panel(&panel00);

	G2D_TextBlend(250, 40, FONT_28_verdana, "BLUETOOTH PAGE", BSP_LCD_Color(C_WHITE, 255));

	return GUI_OK;
}


