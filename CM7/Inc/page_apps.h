/*
 * page_apps.h
 *
 *  Created on: Dec 13, 2023
 *      Author: marek
 */

#ifndef PAGE_APPS_H_
#define PAGE_APPS_H_

#include "BSP.h"
#include "gui.h"

enum APP_STATUS {
	APP_EMPTY,
	APP_LISTED,
	APP_UPLOADED
};

typedef struct {
	uint8_t	status;
	char path[32];
	char title[32];
} APP_TypeDef;

uint8_t page_init_apps(void);
uint8_t	page_render_apps(void);

#endif /* PAGE_APPS_H_ */
