/*******************************************************************
 * MiniConsole V3 - Simple GUI Library
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 *******************************************************************/

#include "gui.h"

uint8_t	GUI_Button(GUI_Button_TypeDef * btn) {
	uint32_t col_bg = GUI_BUTTON_DISABLED_BG_COLOR;
	uint32_t col_border = GUI_BUTTON_DISABLED_BORDER_COLOR;
	uint32_t col_text = GUI_BUTTON_DISABLED_TEXT_COLOR;
	uint32_t col_icon = GUI_BUTTON_DISABLED_ICON_COLOR;

	switch (btn->state) {
	case GUI_STATE_ENABLED:
		col_bg = GUI_BUTTON_ENABLED_BG_COLOR;
		col_border = GUI_BUTTON_ENABLED_BORDER_COLOR;
		col_text = GUI_BUTTON_ENABLED_TEXT_COLOR;
		col_icon = GUI_BUTTON_ENABLED_ICON_COLOR;
		break;
	case GUI_STATE_CLICKED:
		col_bg = GUI_BUTTON_CLICKED_BG_COLOR;
		col_border = GUI_BUTTON_CLICKED_BORDER_COLOR;
		col_text = GUI_BUTTON_CLICKED_TEXT_COLOR;
		col_icon = GUI_BUTTON_CLICKED_ICON_COLOR;
		break;
	case GUI_STATE_ACTIVE:
		col_bg = GUI_BUTTON_ACTIVE_BG_COLOR;
		col_border = GUI_BUTTON_ACTIVE_BORDER_COLOR;
		col_text = GUI_BUTTON_ACTIVE_TEXT_COLOR;
		col_icon = GUI_BUTTON_ACTIVE_ICON_COLOR;
		break;
	}

	G2D_DrawFillRoundRectBlend(btn->x_pos, btn->y_pos, btn->width, btn->height, GUI_BUTTON_RADIUS, col_bg);
	G2D_DrawRoundRect(btn->x_pos, btn->y_pos, btn->width, btn->height, GUI_BUTTON_RADIUS, col_border);
	uint16_t toffset_x = GUI_BUTTON_MARGIN;
	uint16_t toffset_y = ((btn->height - G2D_GetTextHeight(GUI_BUTTON_TEXT_FONT)) >> 1);
	if (btn->icon != 0) {
		uint16_t ih = G2D_GetIconHeight(btn->icon);
		uint16_t iw = G2D_GetIconWidth(btn->icon);
		uint16_t ioffset = GUI_BUTTON_MARGIN;
		if (btn->text == 0) ioffset = (btn->width - iw) >> 1;
		G2D_DrawIconBlend(btn->icon, btn->x_pos + ioffset, btn->y_pos + ((btn->height - ih) >> 1), col_icon);
		toffset_x += (iw + GUI_BUTTON_MARGIN);
	}

	if (btn->text) G2D_TextBlend(btn->x_pos + toffset_x, btn->y_pos + toffset_y, GUI_BUTTON_TEXT_FONT, btn->text, col_text);

	if ((btn->state == GUI_STATE_CLICKED) && ((btn->ctx.click_timestamp + 200) < BSP_GetTick())) {
		btn->ctx.click_timestamp = 0;
		btn->state = GUI_STATE_ENABLED;
	}

	return GUI_OK;
}


uint8_t GUI_Button_Click(GUI_Button_TypeDef * btn) {
	btn->ctx.click_timestamp = BSP_GetTick();
	btn->state = GUI_STATE_CLICKED;
	return GUI_OK;
}


uint8_t GUI_Panel(GUI_Panel_TypeDef * pnl) {
	G2D_DrawFillRoundRectBlend(pnl->x_pos, pnl->y_pos, pnl->width, pnl->height, GUI_PANEL_RADIUS, GUI_PANEL_BG_COLOR);
	G2D_DrawRoundRect(pnl->x_pos, pnl->y_pos, pnl->width, pnl->height, GUI_PANEL_RADIUS, GUI_PANEL_BORDER_COLOR);
	return GUI_OK;
}


uint8_t GUI_ProgressBar(GUI_ProgressBar_TypeDef * pg, uint32_t value) {

	uint16_t progress = 0;

	G2D_DrawFillRoundRectBlend(pg->x_pos, pg->y_pos, pg->width, GUI_PROGRESSBAR_RADIUS * 2 + 2, GUI_PROGRESSBAR_RADIUS, GUI_PROGRESSBAR_BG_COLOR);
	G2D_DrawRoundRect(pg->x_pos, pg->y_pos, pg->width, GUI_PROGRESSBAR_RADIUS * 2 + 2, GUI_PROGRESSBAR_RADIUS, GUI_PROGRESSBAR_BORDER_COLOR);

	if (value <= pg->min_value) {

		value = pg->min_value;

	} else if (value >= pg->max_value) {

		value = pg->max_value;

	}

	progress = ((value - pg->min_value) * (pg->width - 2)) / (pg->max_value - pg->min_value);

	G2D_DrawFillRoundRectBlend(pg->x_pos + 2, pg->y_pos + 2, progress, (GUI_PROGRESSBAR_RADIUS * 2) - 2, GUI_PROGRESSBAR_RADIUS - 2, GUI_PROGRESSBAR_COLOR);

	return GUI_OK;
}


uint8_t GUI_Slider(GUI_Slider_TypeDef * sl, uint32_t value) {

	G2D_DrawFillRoundRectBlend(sl->x_pos, sl->y_pos, sl->width, 42, GUI_SLIDER_RADIUS, GUI_SLIDER_BG_COLOR);
	G2D_DrawRoundRect(sl->x_pos, sl->y_pos, sl->width, 42, GUI_SLIDER_RADIUS, GUI_SLIDER_BORDER_COLOR);
	G2D_DrawVLine(sl->x_pos + 42, sl->y_pos, 42, GUI_SLIDER_BORDER_COLOR);
	G2D_DrawVLine(sl->x_pos + sl->width - 42, sl->y_pos, 42, GUI_SLIDER_BORDER_COLOR);
	G2D_DrawIconBlendC((uint32_t)ICON_32_Left, sl->x_pos + 21, sl->y_pos + 21, GUI_SLIDER_BORDER_COLOR);
	G2D_DrawIconBlendC((uint32_t)ICON_32_Right, sl->x_pos + sl->width - 21, sl->y_pos + 21, GUI_SLIDER_BORDER_COLOR);

	if (value < sl->min_value) value = sl->min_value;
	if (value > sl->max_value) value = sl->max_value;

	sl->ctx.slide_x0 = sl->x_pos + 44;
	sl->ctx.slide_x1 = sl->x_pos + sl->width - 44;
	sl->ctx.slide_x = (((sl->width - 120) * (value - sl->min_value)) / (sl->max_value - sl->min_value)) + sl->x_pos + 44;

	G2D_DrawFillRoundRectBlend(sl->ctx.slide_x, sl->y_pos + 2, 32, 38, GUI_SLIDER_RADIUS, GUI_SLIDER_HANDLE_COLOR);
	G2D_DrawRoundRect(sl->ctx.slide_x, sl->y_pos + 2, 32, 38, GUI_SLIDER_RADIUS, GUI_SLIDER_BORDER_COLOR);

	return GUI_OK;
}

uint8_t GUI_Spinner(GUI_Spinner_TypeDef * sp, uint32_t value) {

	G2D_DrawFillRoundRectBlend(sp->x_pos, sp->y_pos, 140, 42, GUI_SLIDER_RADIUS, GUI_SLIDER_BG_COLOR);
	G2D_DrawRoundRect(sp->x_pos, sp->y_pos, 140, 42, GUI_SLIDER_RADIUS, GUI_SLIDER_BORDER_COLOR);
	G2D_DrawIconBlendC((uint32_t)ICON_32_Left, sp->x_pos + 21, sp->y_pos + 21, GUI_SLIDER_BORDER_COLOR);
	G2D_DrawIconBlendC((uint32_t)ICON_32_Right, sp->x_pos + 140 - 21, sp->y_pos + 21, GUI_SLIDER_BORDER_COLOR);

	char str[12];
	if (value > 0) {
		sprintf(str, "%02d", value);
		G2D_TextBlend(sp->x_pos + 55, sp->y_pos + 6, GUI_SPINNER_TEXT_FONT, str, GUI_SPINNER_TEXT_COLOR);
	} else G2D_TextBlend(sp->x_pos + 50, sp->y_pos + 6, GUI_SPINNER_TEXT_FONT, "Off", GUI_SPINNER_TEXT_COLOR);


	return GUI_OK;
}
