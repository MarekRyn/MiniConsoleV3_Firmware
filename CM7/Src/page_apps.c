/*
 * page_apps.c
 *
 *  Created on: Dec 13, 2023
 *      Author: marek
 */


#include "page_apps.h"
#include <stdlib.h>
#include <math.h>

#define APPS_LIST_SIZE	128

#define CAROUSEL_W				544		// [PX] Must be dividing by SEGMENT_W
#define CAROUSEL_H				200		// [PX]
#define CAROUSEL_SLIDE_W		320		// [PX] Must be dividing by SEGMENT_W
#define CAROUSEL_SPACING_W		16		// [PX] Must be dividing by SEGMENT_W
#define CAROUSEL_SEGMENT_W		4		// [PX]
#define CAROUSEL_COUNT 			(CAROUSEL_W / CAROUSEL_SEGMENT_W)		// Must be even number
#define CAROUSEL_SLIDE_COUNT	(CAROUSEL_SLIDE_W / CAROUSEL_SEGMENT_W)
#define CAROUSEL_SPACING_COUNT	(CAROUSEL_SPACING_W / CAROUSEL_SEGMENT_W)

static APP_TypeDef app_list[APPS_LIST_SIZE] = {0};
static uint8_t	carousel_alpha[CAROUSEL_COUNT];
static uint8_t	carousel_slides0[CAROUSEL_COUNT];
static uint8_t	carousel_slides1[CAROUSEL_COUNT];

static void * thumbnails[5];



static void _carousel_init(void) {
	uint8_t i = 0;

	// Calculating alpha blending factors
	for (i = 0; i < (CAROUSEL_COUNT >> 1); i++) {
		float x = (float)i / 18;
		carousel_alpha[i] = (x > (0.5 * M_PI)) ? 255 : (uint8_t)(sin(x) * 255);
		carousel_alpha[CAROUSEL_COUNT - i - 1] = carousel_alpha[i];
	}

	// Arranging slides (total 5 pcs)
	uint8_t s0 = 4;
	uint8_t s1 = 54;
	uint8_t t = 0;

	for (i = 0; i < CAROUSEL_COUNT; i++) {
		if ((s0 != 0xFF) && (s1 == CAROUSEL_SLIDE_COUNT)) {
			t = s0 + 1;
			s0 = 0xFF;
			s1 = 0;
			if (t > 4) t = 0;
		}
		if ((s0 == 0xFF) && (s1 == CAROUSEL_SPACING_COUNT)) {
			s0 = t;
			s1 = 0;
		}

		carousel_slides0[i] = s0;
		carousel_slides1[i] = s1;
		s1++;
	}

}

static void _carousel_scrol_left(uint8_t step) {
	if (step == 0) return;

	uint8_t i = 0;

	for (i = 0; i < (CAROUSEL_COUNT - step); i++) {
		carousel_slides0[i] = carousel_slides0[i+step];
		carousel_slides1[i] = carousel_slides1[i+step];
	}

	i = CAROUSEL_COUNT - step - 1;
	uint8_t s0 = carousel_slides0[CAROUSEL_COUNT - 1];
	uint8_t s1 = carousel_slides1[CAROUSEL_COUNT - 1];

	while (i < CAROUSEL_COUNT) {
		if ((s0 != 0xFF) && (s1 == CAROUSEL_SLIDE_COUNT)) {
			s0 = 0xFF;
			s1 = 0;
		}
		if ((s0 == 0xFF) && (s1 == CAROUSEL_SPACING_COUNT)) {
			s0 = carousel_slides0[CAROUSEL_COUNT - 1 - CAROUSEL_SPACING_COUNT - step] + 1;
			if (s0 > 4) s0 = 0;
			s1 = 0;
		}

		carousel_slides0[i] = s0;
		carousel_slides1[i] = s1;
		s1++;
		i++;
	}
}


static void _carousel_scrol_right(uint8_t step) {
	if (step == 0) return;

	uint8_t i = 0;

	for (i = (CAROUSEL_COUNT - 1); i > (step - 1); i--) {
		carousel_slides0[i] = carousel_slides0[i-step];
		carousel_slides1[i] = carousel_slides1[i-step];
	}

	i = step;
	uint8_t s0 = carousel_slides0[i];
	uint8_t s1 = carousel_slides1[i];

	while (i < CAROUSEL_COUNT) {
		if ((s0 != 0xFF) && (s1 >= CAROUSEL_SLIDE_COUNT)) {
			s0 = 0xFF;
			s1 = CAROUSEL_SPACING_COUNT;
		}
		if ((s0 == 0xFF) && (s1 >= CAROUSEL_SLIDE_COUNT)) {
			s0 = carousel_slides0[i + CAROUSEL_SPACING_COUNT + step] - 1;
			if (s0 > 4) s0 = 4;
			s1 = CAROUSEL_SLIDE_COUNT - 1;
		}
		carousel_slides0[i] = s0;
		carousel_slides1[i] = s1;
		s1--;
		i--;
	}

}

static void _carousel_drawsegments(void * thumbnail, uint16_t x, uint16_t y, uint8_t seg_start, uint8_t seg_stop, uint8_t alpha) {
	// BSP_LCD_CopyBuf(uint32_t src_addr, uint16_t offsline_src, uint16_t x_dest, uint16_t y_dest, uint16_t offsline_dest, uint16_t width, uint16_t height);
	// BSP_LCD_CopyBufBlend(uint32_t src_addr, uint16_t offsline_src, uint16_t x_dest, uint16_t y_dest, uint16_t offsline_dest, uint16_t width, uint16_t height, uint8_t alpha)

	uint32_t src_addr = (uint32_t)thumbnail + (seg_start * CAROUSEL_SEGMENT_W) * 3;
	uint16_t width = (seg_stop - seg_start) * CAROUSEL_SEGMENT_W;
	uint16_t height = CAROUSEL_H;
	uint16_t offsline_src = CAROUSEL_SLIDE_W - width;
	uint16_t offsline_dest = LCD_WIDTH - width;
	uint16_t x_dest = x;
	uint16_t y_dest = y;

	if (alpha == 255) BSP_LCD_CopyBuf(src_addr, offsline_src, x_dest, y_dest, offsline_dest, width, height);
		else BSP_LCD_CopyBufBlend(src_addr, offsline_src, x_dest, y_dest, offsline_dest, width, height, alpha);

}

static void _carousel_render(int16_t x, int16_t y) {
	uint8_t i = 0;
	while (i < CAROUSEL_COUNT) {
		if (carousel_slides0[i] == 0xFF) {i++; continue;}
		_carousel_drawsegments(thumbnails[carousel_slides0[i]], x + i * CAROUSEL_SEGMENT_W, y, carousel_slides1[i], carousel_slides1[i]+1, carousel_alpha[i]);
		i++;
	}
}


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


uint8_t page_loaddata_apps(void) {
	thumbnails[0] = BSP_Res_Load("/Kicus/thumbnail.bin");
	thumbnails[1] = BSP_Res_Load("/Game00/thumbnail.bin");
	thumbnails[2] = BSP_Res_Load("/Game01/thumbnail.bin");
	thumbnails[3] = BSP_Res_Load("/Game02/thumbnail.bin");
	thumbnails[4] = BSP_Res_Load("/Game03/thumbnail.bin");
	return GUI_OK;
}


uint8_t page_init_apps(void) {

	_carousel_init();
	_apps_updatelist();

	for (uint8_t i = 0; i< APPS_LIST_SIZE; i++) {
		if (app_list[i].status) printf("%s \n", app_list[i].title);
	}

	return GUI_OK;
}


uint8_t	page_render_apps(void) {

	if (BSP_hinputs.joy.joy_X > 0) _carousel_scrol_left(BSP_hinputs.joy.joy_X >> 4);
	if (BSP_hinputs.joy.joy_X < 0) _carousel_scrol_right((-BSP_hinputs.joy.joy_X) >> 4);

	G2D_TextBlend(250, 40, FONT_28_verdana, "APPLICATIONS PAGE", BSP_LCD_Color(C_WHITE, 255));

	_carousel_render(233, 80);


//	for (uint8_t i = 0; i < CAROUSEL_COUNT; i++) {
//		if (carousel_slides0[i] != 0xFF) G2D_DrawFillRectBlend(233 + (4 * i), 80, 4, 200, BSP_LCD_Color(C_RED + carousel_slides1[i], carousel_alpha[i]));
//	}

	//G2D_DrawBitmap(thumbnail, 345, 200, 320, 200);

	return GUI_OK;
}
