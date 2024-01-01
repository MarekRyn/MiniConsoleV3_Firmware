/*
 * page_screen.c
 *
 *  Created on: Dec 13, 2023
 *      Author: marek
 */

#include "page_screen.h"

static GUI_Panel_TypeDef	panel00;

uint8_t page_init_screen(void) {

	// Panel00 - Main Area
	panel00.x_pos = 220;
	panel00.y_pos = 10;
	panel00.width = 570;
	panel00.height = 460;
}


uint8_t	page_render_screen(void) {

	GUI_Panel(&panel00);

	G2D_TextBlend(250, 40, FONT_28_verdana, "SCREEN PAGE", BSP_LCD_Color(C_WHITE, 255));

}


