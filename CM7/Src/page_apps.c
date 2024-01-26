/*
 * page_apps.c
 *
 *  Created on: Dec 13, 2023
 *      Author: marek
 */


#include "page_apps.h"
#include <stdlib.h>

#define APPS_LIST_SIZE	128

static APP_TypeDef app_list[APPS_LIST_SIZE] = {0};


static int _pstrcmp( const void* a, const void* b ) {

	APP_TypeDef * element_a = (APP_TypeDef *)a;
	APP_TypeDef * element_b = (APP_TypeDef *)b;

	return stricmp(element_a->title, element_b->title);
}


static uint8_t _apps_updatelist(void) {

	DIR dir;
	FILINFO finfo;

	uint32_t index = 0;

	f_opendir(&dir, "");

	while (index < APPS_LIST_SIZE) {

		uint8_t res = f_readdir(&dir, &finfo);
		if ((res != FR_OK) || (finfo.fname[0] == 0)) break;
		if ((finfo.fattrib & AM_DIR) && (!(finfo.fattrib & AM_SYS))) {
			strcpy(app_list[index].title, finfo.fname);
			app_list[index].status = APP_LISTED;
			index++;
		}
	}

	qsort(app_list, index, sizeof(app_list[0]), _pstrcmp);

	return GUI_OK;
}



uint8_t page_init_apps(void) {

	_apps_updatelist();

	for (uint8_t i = 0; i< APPS_LIST_SIZE; i++) {
		if (app_list[i].status) printf("%s \n", app_list[i].title);
	}

	return GUI_OK;
}


uint8_t	page_render_apps(void) {

	G2D_TextBlend(250, 40, FONT_28_verdana, "APPLICATIONS PAGE", BSP_LCD_Color(C_WHITE, 255));

	return GUI_OK;
}
