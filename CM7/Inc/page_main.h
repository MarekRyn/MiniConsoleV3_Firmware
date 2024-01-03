/*
 * page_main.h
 *
 *  Created on: Dec 13, 2023
 *      Author: marek
 */

#ifndef PAGE_MAIN_H_
#define PAGE_MAIN_H_

#include "BSP.h"
#include "gui.h"
#include "common.h"

#include "page_apps.h"
#include "page_audio.h"
#include "page_bt.h"
#include "page_clock.h"
#include "page_imu.h"
#include "page_inputs.h"
#include "page_pwr.h"
#include "page_screen.h"


uint8_t page_init_main();
uint8_t	page_render_main();

#endif /* PAGE_MAIN_H_ */
