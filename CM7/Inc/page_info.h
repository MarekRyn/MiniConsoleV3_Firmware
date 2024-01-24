/*
 * page_info.h
 *
 *  Created on: Jan 20, 2024
 *      Author: marek
 */

#ifndef PAGE_INFO_H_
#define PAGE_INFO_H_

#include "BSP.h"
#include "gui.h"

extern ResCtx_TypeDef resctx;

uint8_t page_init_info(void);
uint8_t	page_render_info(void);

#endif /* PAGE_INFO_H_ */
