/*
 * page_apps.c
 *
 *  Created on: Dec 13, 2023
 *      Author: marek
 */

#include "page_apps.h"

uint8_t page_init_apps(void) {

	return GUI_OK;
}


uint8_t	page_render_apps(void) {

	G2D_TextBlend(250, 40, FONT_28_verdana, "APPLICATIONS PAGE", BSP_LCD_Color(C_WHITE, 255));

	return GUI_OK;
}
