/*
 * page_apps.c
 *
 *  Created on: Dec 13, 2023
 *      Author: marek
 */


#include "page_apps.h"
#include "flasher.h"
#include <stdlib.h>
#include <math.h>

#define APPS_LIST_SIZE	128

#define CAROUSEL_W				544														// [PX] Must be dividing by SEGMENT_W
#define CAROUSEL_H				200														// [PX]
#define CAROUSEL_SLIDE_W		320														// [PX] Must be dividing by SEGMENT_W
#define CAROUSEL_SPACING_W		16														// [PX] Must be dividing by SEGMENT_W
#define CAROUSEL_SEGMENT_W		4														// [PX]
#define CAROUSEL_COUNT 			(CAROUSEL_W / CAROUSEL_SEGMENT_W)						// Number of segments per visible part of carousel
#define CAROUSEL_SLIDE_COUNT	(CAROUSEL_SLIDE_W / CAROUSEL_SEGMENT_W)					// Number of segments per slide
#define CAROUSEL_SPACING_COUNT	(CAROUSEL_SPACING_W / CAROUSEL_SEGMENT_W)				// Number of segments per spacing
#define CAROUSEL_SEG_COUNT		(5 * CAROUSEL_SLIDE_COUNT + 5 * CAROUSEL_SPACING_COUNT)	// Segment array size

static GUI_Panel_TypeDef			panel00;
static GUI_Button_TypeDef			button00;
static GUI_ProgressBar_TypeDef		pbar00;

static APP_TypeDef 	app_list[APPS_LIST_SIZE] = {0};
static uint32_t		app_count = 0;

typedef struct {
	uint8_t		alpha_mask[CAROUSEL_COUNT];	// alpha mask for visible segments
	uint8_t		segments0[CAROUSEL_SEG_COUNT]; // indexes slide images
	uint8_t		segments1[CAROUSEL_SEG_COUNT]; // segment numbers for each slide
	void * 		slides[5];	// pointers to slides' graphics
	uint32_t	apps[5];	// app indexes assigned to slides
	uint32_t	pos;		// position offset
	uint32_t	dist;		// distance to scroll
	uint8_t		state;		// current state
	uint8_t		slide_i;	// slide index
	uint8_t		slide_i_;	// previous slide index
	uint32_t	app_i;		// selected app index
	uint32_t	timestamp;	// timestamp when carousel stops
} Carousel_TypeDef;

static Carousel_TypeDef carouselctx = {0};


// Functions for handling carousel with slides

static void _carousel_init(Carousel_TypeDef * ctx) {

	uint32_t i = 0;
	uint32_t j = 0;
	uint32_t k = 0;

	// Calculating alpha mask
	for (i = 0; i < (CAROUSEL_COUNT >> 1); i++) {
		float x = (float)i / 18;
		ctx->alpha_mask[i] = (x > (0.5 * M_PI)) ? 255 : (uint8_t)(sin(x) * 255);
		ctx->alpha_mask[CAROUSEL_COUNT - i - 1] = ctx->alpha_mask[i];
	}

	// Initiating segment arrays
	k = 0;
	for (i = 0; i < 5; i++) {
		for (j = 0; j < (CAROUSEL_SPACING_COUNT >> 1); j++) {
			ctx->segments0[k] = 0xFF;
			ctx->segments1[k] = 0xFF;
			k++;
		}
		for (j = 0; j < (CAROUSEL_SLIDE_COUNT); j++) {
			ctx->segments0[k] = i;
			ctx->segments1[k] = j;
			k++;
		}
		for (j = 0; j < (CAROUSEL_SPACING_COUNT >> 1); j++) {
			ctx->segments0[k] = 0xFF;
			ctx->segments1[k] = 0xFF;
			k++;
		}

		ctx->apps[i] = i;
	}

	// Initiating variables
	ctx->slide_i = ctx->segments0[CAROUSEL_SEG_COUNT >> 1];	// Index of selected slide
	ctx->slide_i_ = ctx->slide_i_;
	ctx->app_i = ctx->apps[ctx->slide_i];					// Index of selected app
	ctx->state = 0;
	ctx->timestamp = BSP_GetTick();
}


static void _carousel_scrol_left(Carousel_TypeDef * ctx) {
	if (ctx->state == 2) return;
	ctx->state = 1;
	ctx->dist += CAROUSEL_SLIDE_COUNT + CAROUSEL_SPACING_COUNT;
}


static void _carousel_scrol_right(Carousel_TypeDef * ctx) {
	if (ctx->state == 1) return;
	ctx->state = 2;
	ctx->dist += CAROUSEL_SLIDE_COUNT + CAROUSEL_SPACING_COUNT;
}


static void _carousel_drawsegments(void * thumbnail, uint16_t x, uint16_t y, uint8_t seg_start, uint8_t seg_stop, uint8_t alpha) {
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

static void _carousel_loadthumb(Carousel_TypeDef * ctx, uint8_t slidepos, uint32_t app_index) {
	char fname[256] = {0};

	memset(fname, 0x00, 256);
	strcpy(fname, app_list[app_index].path);
	strcat(fname, "thumbnail.bin");
	BSP_Res_Free(carouselctx.slides[slidepos]);
	ctx->slides[slidepos] = BSP_Res_Load(fname);
	ctx->apps[slidepos] = app_index;
}


static void _carousel_render(Carousel_TypeDef * ctx, int16_t x, int16_t y) {
	uint8_t si0;
	uint8_t si1;
	uint32_t ai;

	// Scrolling slides
	uint8_t step = 0;
	if (ctx->dist > 0) step = 1;
	if (ctx->dist > 4) step = ctx->dist >> 1;
	if (step > 24) step = 16;

	switch (ctx->state) {
	case 0:
		ctx->app_i = ctx->apps[ctx->slide_i];
		break;
	case 1:
		ctx->pos += step;
		ctx->pos %= CAROUSEL_SEG_COUNT;
		ctx->dist -= step;

		ctx->slide_i = ctx->segments0[((CAROUSEL_SEG_COUNT >> 1) + ctx->pos) % CAROUSEL_SEG_COUNT];
		ctx->app_i = ctx->apps[ctx->slide_i];
		si0 = (ctx->slide_i + 1) % 5;
		si1 = (ctx->slide_i + 2) % 5;
		ai = (ctx->apps[si0] + 1) % app_count;
		if (ctx->slide_i_ != ctx->slide_i) {
			_carousel_loadthumb(ctx, si1, ai);
			ctx->slide_i_ = ctx->slide_i;
		}
		if (ctx->dist == 0) { ctx->state = 0; ctx->timestamp = BSP_GetTick(); }


		break;
	case 2:
		ctx->pos += (CAROUSEL_SEG_COUNT - step);
		ctx->pos %= CAROUSEL_SEG_COUNT;
		ctx->dist -= step;

		ctx->slide_i = ctx->segments0[((CAROUSEL_SEG_COUNT >> 1) + ctx->pos) % CAROUSEL_SEG_COUNT];
		ctx->app_i = ctx->apps[ctx->slide_i];
		si0 = (ctx->slide_i + 5 - 1) % 5;
		si1 = (ctx->slide_i + 5 - 2) % 5;
		ai = (ctx->apps[si0] + app_count - 1) % app_count;
		if (ctx->slide_i_ != ctx->slide_i) {
			_carousel_loadthumb(ctx, si1, ai);
			ctx->slide_i_ = ctx->slide_i;
		}
		if (ctx->dist == 0) { ctx->state = 0; ctx->timestamp = BSP_GetTick(); }
		break;
	}

	// Displaying slides
	uint32_t i = ctx->pos + (CAROUSEL_SEG_COUNT >> 1) - (CAROUSEL_COUNT >> 1);
	for (uint32_t j = 0; j < CAROUSEL_COUNT; j++) {
		if (ctx->segments0[(i+j) % CAROUSEL_SEG_COUNT] < 0xFF)
			_carousel_drawsegments(ctx->slides[ctx->segments0[(i+j) % CAROUSEL_SEG_COUNT]],
					x + j * CAROUSEL_SEGMENT_W, y, ctx->segments1[(i+j) % CAROUSEL_SEG_COUNT],
					ctx->segments1[(i+j) % CAROUSEL_SEG_COUNT]+1,
					ctx->alpha_mask[j]);
	}
}


// Other private functions

static void carousel_callback(void) {
	switch(BSP_hlcdtp.gest_data.gest) {
	case LCD_TP_GEST_SWIPE_RIGHT:
		FLASHER_Reset();
		button00.state = GUI_STATE_ENABLED;
		_carousel_scrol_right(&carouselctx);
		break;
	case LCD_TP_GEST_SWIPE_LEFT:
		FLASHER_Reset();
		button00.state = GUI_STATE_ENABLED;
		_carousel_scrol_left(&carouselctx);
		break;
	}
}


static void button00_callback(void){

	if (carouselctx.state != 0) return;

	switch(BSP_hlcdtp.gest_data.gest) {
	case LCD_TP_GEST_CLICK_DOWN:
		FLASHER_Init(app_list[carouselctx.app_i].path);
		BSP_LCD_TP_Disable();
		break;
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
	FIL file;
	FRESULT res;

	uint32_t read = 0;

	uint32_t index = 0;
	char	path[128];

	f_opendir(&dir, "");

	while (index < APPS_LIST_SIZE) {

		res = f_readdir(&dir, &finfo);
		if ((res != FR_OK) || (finfo.fname[0] == 0)) break;
		if ((finfo.fattrib & AM_DIR) && (!(finfo.fattrib & AM_SYS))) {

			memset(path, 0x00, 128);
			strcpy(path, "0:/");
			strcat(path, finfo.fname);
			strcat(path, "/");

			strcpy(app_list[index].path, path);

			strcat(path, "title.txt");

			res = f_open(&file, path, FA_READ);
			f_read(&file, app_list[index].title, 32, &read);
			f_close(&file);

			app_list[index].status = APP_LISTED;
			index++;
		}
	}

	app_count = index;
	qsort(app_list, index, sizeof(app_list[0]), _pstrcmp);

	return GUI_OK;
}




// Public functions

uint8_t page_init_apps(void) {

	panel00.x_pos = 233;
	panel00.y_pos = 310;
	panel00.width = 544;
	panel00.height = 140;

	button00.x_pos = 620;
	button00.y_pos = 327;
	button00.width = 140;
	button00.height = 42;
	button00.text = "Install";
	button00.icon = (uint32_t)ICON_32_Download;
	button00.state = GUI_STATE_ENABLED;

	pbar00.x_pos = 250;
	pbar00.y_pos = 390;
	pbar00.width = 510;
	pbar00.min_value = 0;
	pbar00.max_value = 100;


	_carousel_init(&carouselctx);
	_apps_updatelist();

	// Updating initial slides
	uint8_t si = 0;
	uint8_t ai = (app_count - 2) % app_count;
	for (uint8_t i = 0; i < 5; i++) {
		_carousel_loadthumb(&carouselctx, si, ai);
		si++;
		ai++;
		si %= 5;
		ai %= app_count;
	}

	// Register touch areas
	BSP_LCD_TP_RemoveAreaRange(10, LCD_TP_AREA_NO - 1);
	BSP_LCD_TP_RegisterArea(10, 233, 80, 544, 200, carousel_callback);
	BSP_LCD_TP_RegisterArea(11, 620, 327, 140, 42, button00_callback);

	return GUI_OK;
}


uint8_t	page_render_apps(void) {

	uint32_t fl_progress = FLASHER_GetProgress();

	G2D_TextBlend(250, 30, FONT_28_verdana, "APPLICATIONS", BSP_LCD_Color(C_WHITE, 255));

	_carousel_render(&carouselctx, 233, 80);

	if ((carouselctx.state == 0) && ((BSP_GetTick() - carouselctx.timestamp) > 300)) {
		GUI_Panel(&panel00);
		G2D_TextBlend(250, 330, FONT_28_verdana, app_list[carouselctx.app_i].title, BSP_LCD_Color(C_WHITE, 255));
		GUI_Button(&button00);
		GUI_ProgressBar(&pbar00, fl_progress);

		if (!FLASHER_IsError()) {

			if ((fl_progress > 0) && (fl_progress <= 20)) G2D_TextBlend(250, 410, FONT_22_verdana, "Erasing memory...", BSP_LCD_Color(C_WHITE, 255));
			if ((fl_progress > 20) && (fl_progress <= 70)) G2D_TextBlend(250, 410, FONT_22_verdana, "Writing memory...", BSP_LCD_Color(C_WHITE, 255));
			if ((fl_progress > 70) && (fl_progress < 100)) G2D_TextBlend(250, 410, FONT_22_verdana, "Verifying memory...", BSP_LCD_Color(C_WHITE, 255));

			if (fl_progress == 100) {
				G2D_TextBlend(250, 410, FONT_22_verdana, "Installation completed.", BSP_LCD_Color(C_WHITE, 255));
				BSP_LCD_TP_Enable();
			}

		} else {
			G2D_TextBlend(250, 410, FONT_22_verdana, "Installation failed.", BSP_LCD_Color(C_RED, 255));
			BSP_LCD_TP_Enable();
		}

	}

	return GUI_OK;
}
