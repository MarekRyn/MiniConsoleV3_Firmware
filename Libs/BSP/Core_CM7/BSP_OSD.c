/*******************************************************************
 * MiniConsole V3 - Board Support Package - On Screen Menu
 *******************************************************************/

#include "BSP_OSD.h"

static uint8_t 	_osd_state = OSD_DISABLED;
static uint32_t	_osd_counter = 0;


static void _OSD_Background(uint32_t color) {
	G2D_OSD_FillFrame(G2D_OSD_Color(color, 255));
	uint8_t a = 255;
	for (uint16_t y = LCD_OSD_HEIGHT - 15; y < LCD_OSD_HEIGHT; y++) {
		a-=16;
		G2D_OSD_DrawHLine(0, y, LCD_WIDTH, G2D_OSD_Color(color, a));
	}
}

static void _OSD_Show(void) {
	BSP_LCD_OSD_SetAlpha(200);
	BSP_LCD_OSD_Show();
}

static void _OSD_Hide(void) {
	BSP_LCD_OSD_Hide();
}


uint8_t BSP_OSD_Init(void) {

	// Initialize memory with default background
	_OSD_Background(C_DARKGRAY);

	// Setting default Alpha for OSD layer
	BSP_LCD_OSD_SetAlpha(200);

	_osd_state = OSD_HIDDEN;

	return BSP_OK;
}


uint8_t	BSP_OSD_ShowInfo(void) {

	uint8_t month, day, hour, minute, second, weekday;
	uint16_t year;

	char dt[32];

	if (_osd_state == OSD_DISABLED) return BSP_ERROR;
	if (_osd_state == OSD_INFO) return BSP_OK;

	BSP_RTC_GetTime(&hour, &minute, &second);
	BSP_RTC_GetDate(&year, &month, &day, &weekday);
	sprintf(dt, "%02d/%02d/%04d  %02d:%02d", day, month, year, hour, minute);

	_OSD_Background(C_DARKGRAY);

	G2D_OSD_DrawIcon(ICON_32_DateTime, 18, 8, G2D_OSD_Color(C_WHITE, 255), G2D_OSD_Color(C_DARKGRAY, 255));

	G2D_OSD_Text(66, 8, FONT_28_verdana, dt, G2D_OSD_Color(C_WHITE, 255), G2D_OSD_Color(C_DARKGRAY, 255));

	if (_osd_state == OSD_HIDDEN) _OSD_Show();
	_osd_state = OSD_INFO;
	_osd_counter = 400;

	return BSP_OK;
}


uint8_t	BSP_OSD_ShowVolume(void) {

	if (_osd_state == OSD_DISABLED) return BSP_ERROR;
	if (_osd_state != OSD_VOLUME) _OSD_Background(C_DARKGRAY);

	uint16_t volume = (BSP_Audio_GetMasterVolume() * 100) / AUDIO_CFG_MAXMASTERVOL;
	volume *= 7;

	G2D_OSD_DrawIcon(ICON_32_Audio, 18, 8, G2D_OSD_Color(C_WHITE, 255), G2D_OSD_Color(C_DARKGRAY, 255));
	G2D_OSD_DrawRect(66, 8, 704, 32, G2D_OSD_Color(C_WHITE, 255));
	G2D_OSD_DrawFillRect(68, 10, volume, 28, G2D_OSD_Color(C_WHITE, 255));
	G2D_OSD_DrawFillRect(68 + volume, 10, 700 - volume, 28, G2D_OSD_Color(C_DARKGRAY, 255));

	if (_osd_state == OSD_HIDDEN) _OSD_Show();
	_osd_state = OSD_VOLUME;
	_osd_counter = 400;

	return BSP_OK;
}


uint8_t	BSP_OSD_ShowBrightness(void) {

	if (_osd_state == OSD_DISABLED) return BSP_ERROR;
	if (_osd_state != OSD_BRIGHTNESS) _OSD_Background(C_DARKGRAY);

	uint16_t brightness = BSP_LCD_GetBackLight() * 7;

	G2D_OSD_DrawIcon(ICON_32_Brightness, 18, 8, G2D_OSD_Color(C_WHITE, 255), G2D_OSD_Color(C_DARKGRAY, 255));
	G2D_OSD_DrawRect(66, 8, 704, 32, G2D_OSD_Color(C_WHITE, 255));
	G2D_OSD_DrawFillRect(68, 10, brightness, 28, G2D_OSD_Color(C_WHITE, 255));
	G2D_OSD_DrawFillRect(68 + brightness, 10, 700 - brightness, 28, G2D_OSD_Color(C_DARKGRAY, 255));

	if (_osd_state == OSD_HIDDEN) _OSD_Show();
	_osd_state = OSD_BRIGHTNESS;
	_osd_counter = 400;

	return BSP_OK;
}


uint8_t	BSP_OSD_ShowNotification(char * str) {

	if (_osd_state == OSD_DISABLED) return BSP_ERROR;
	if (_osd_state == OSD_NOTIFICATION) return BSP_OK;

	char nstr[64];
	uint8_t l = strlen(str);
	if (l > 55) {
		memcpy(nstr, str, 55);
		nstr[53] = '.';
		nstr[54] = '.';
		nstr[55] = '.';
		nstr[56] = 0;
	} else strcpy(nstr, str);

	_OSD_Background(C_DARKGRAY);
	G2D_OSD_DrawIcon(ICON_32_Apps, 18, 8, G2D_OSD_Color(C_WHITE, 255), G2D_OSD_Color(C_DARKGRAY, 255));
	G2D_OSD_Text(66, 8, FONT_28_verdana, nstr, G2D_OSD_Color(C_WHITE, 255), G2D_OSD_Color(C_DARKGRAY, 255));

	if (_osd_state == OSD_HIDDEN) _OSD_Show();
	_osd_state = OSD_NOTIFICATION;
	_osd_counter = 400;

	return BSP_OK;
}


uint8_t	BSP_OSD_ShowAlert(char * str) {

	if (_osd_state == OSD_DISABLED) return BSP_ERROR;
	if (_osd_state == OSD_ERROR) return BSP_OK;

	char nstr[64];
	uint8_t l = strlen(str);
	if (l > 55) {
		memcpy(nstr, str, 55);
		nstr[53] = '.';
		nstr[54] = '.';
		nstr[55] = '.';
		nstr[56] = 0;
	} else strcpy(nstr, str);

	_OSD_Background(C_RED);
	G2D_OSD_DrawIcon(ICON_32_Error, 18, 8, G2D_OSD_Color(C_BLACK, 255), G2D_OSD_Color(C_RED, 255));
	G2D_OSD_Text(66, 8, FONT_28_verdana, nstr, G2D_OSD_Color(C_BLACK, 255), G2D_OSD_Color(C_RED, 255));

	if (_osd_state == OSD_HIDDEN) _OSD_Show();
	_osd_state = OSD_ERROR;
	_osd_counter = 400;

	return BSP_OK;
}


uint8_t	BSP_OSD_ShowPwrOff(uint16_t value, uint16_t limit) {

	if (_osd_state == OSD_DISABLED) return BSP_ERROR;
	if (_osd_state != OSD_PWROFF) _OSD_Background(C_DARKGRAY);

	uint16_t v = 100 - ((value * 100) / limit);
	v *= 7;

	G2D_OSD_DrawIcon(ICON_32_Power, 18, 8, G2D_OSD_Color(C_WHITE, 255), G2D_OSD_Color(C_DARKGRAY, 255));
	G2D_OSD_DrawRect(66, 8, 704, 32, G2D_OSD_Color(C_WHITE, 255));
	G2D_OSD_DrawFillRect(68, 10, v, 28, G2D_OSD_Color(C_WHITE, 255));
	G2D_OSD_DrawFillRect(68 + v, 10, 700 - v, 28, G2D_OSD_Color(C_DARKGRAY, 255));

	if (_osd_state == OSD_HIDDEN) _OSD_Show();
	_osd_state = OSD_PWROFF;
	_osd_counter = 20;

	return BSP_OK;
}


uint8_t BSP_OSD_Hide(void) {

	if (_osd_state == OSD_DISABLED) return BSP_ERROR;
	_OSD_Hide();
	_osd_state = OSD_HIDDEN;
	_osd_counter = 0;
	return BSP_OK;
}

uint8_t BSP_OSD_Process(void) {

	switch (_osd_state) {

	case OSD_DISABLED:
	case OSD_HIDDEN:
		return BSP_OK;

	case OSD_INFO:
	case OSD_VOLUME:
	case OSD_BRIGHTNESS:
	case OSD_NOTIFICATION:
	case OSD_ERROR:
	case OSD_PWROFF:
		if (_osd_counter > 0) {
			_osd_counter--;
			if (_osd_counter < 20) BSP_LCD_OSD_SetAlpha(_osd_counter * 10);
		}
		if (_osd_counter == 0) _osd_state = OSD_HIDE;
		return BSP_OK;

	case OSD_HIDE:
		_OSD_Hide();
		_osd_state = OSD_HIDDEN;
		return BSP_OK;

	default:
		return BSP_OK;
	}

	return BSP_OK;
}

