/*******************************************************************
 * MiniConsole V3 - Driver - RVT50AQTNWC00 - LCD Screen
 *
 * Author: Marek Ryn
 * Version: 1.0
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 * - 1.0	- Refactoring + bugs correction
 *******************************************************************/

#include "RVT50AQTNWC00.h"

uint8_t BSP_DRV_LCD_TP_Init(I2C_TypeDef *hi2c) {
	return BSP_OK;
}

void BSP_DRV_LCD_TP_Parse(LCD_TP_HandleTypeDef *hlcdtp) {
	// Parsing General Data
	hlcdtp->touch_count = hlcdtp->raw_data[2] & 0b00001111;

	// Parsing Individual Touch Points
	for (uint8_t i=0; i<5; i++) {
		uint8_t ii = 3 + i * 6;
		hlcdtp->touch_data[i].status = (hlcdtp->raw_data[ii] & 0b11000000) >> 6;
		hlcdtp->touch_data[i].x = (uint16_t)((hlcdtp->raw_data[ii] & 0b00001111) << 8) | (hlcdtp->raw_data[ii+1]);
		hlcdtp->touch_data[i].id = (hlcdtp->raw_data[ii+2] & 0b11110000) >> 4;
		hlcdtp->touch_data[i].y = (uint16_t)((hlcdtp->raw_data[ii+2] & 0b00001111) << 8) | (hlcdtp->raw_data[ii+3]);
	}

	// Detecting gestures
	switch (hlcdtp->touch_count) {
		case 0: // no fingers touching screen / finger just release screen
			hlcdtp->gest_data.gest = LCD_TP_GEST_NONE;
			hlcdtp->gest_data.speed_x = 1000.0 * hlcdtp->gest_data.delta_x / hlcdtp->gest_data.delta_t;
			hlcdtp->gest_data.speed_y = 1000.0 * hlcdtp->gest_data.delta_y / hlcdtp->gest_data.delta_t;

			// Gest: Click
			if ((hlcdtp->gest_data.delta_x > -5) && (hlcdtp->gest_data.delta_x < 5)
					&& (hlcdtp->gest_data.delta_y > -5) && (hlcdtp->gest_data.delta_y < 5)) {
				hlcdtp->gest_data.gest = LCD_TP_GEST_CLICK_UP;
				break;
			}

			// Gest: Swipe Left
			if ((hlcdtp->gest_data.delta_x < -100) && (hlcdtp->gest_data.speed_x < -800)
					&& (hlcdtp->gest_data.speed_y > -400) && (hlcdtp->gest_data.speed_y < 400)) {
				hlcdtp->gest_data.gest = LCD_TP_GEST_SWIPE_LEFT;
				break;
			}

			// Gest: Swipe Right
			if ((hlcdtp->gest_data.delta_x > 100) && (hlcdtp->gest_data.speed_x > 800)
					&& (hlcdtp->gest_data.speed_y > -400) && (hlcdtp->gest_data.speed_y < 400)) {
				hlcdtp->gest_data.gest = LCD_TP_GEST_SWIPE_RIGHT;
				break;
			}

			// Gest: Swipe Up
			if ((hlcdtp->gest_data.delta_y < -100) && (hlcdtp->gest_data.speed_y < -800)
					&& (hlcdtp->gest_data.speed_x > -400) && (hlcdtp->gest_data.speed_x < 400)) {
				hlcdtp->gest_data.gest = LCD_TP_GEST_SWIPE_UP;
				break;
			}

			// Gest: Swipe Up
			if ((hlcdtp->gest_data.delta_y > 100) && (hlcdtp->gest_data.speed_y > 800)
					&& (hlcdtp->gest_data.speed_x > -400) && (hlcdtp->gest_data.speed_x < 400)) {
				hlcdtp->gest_data.gest = LCD_TP_GEST_SWIPE_DOWN;
				break;
			}

			break;

		case 1:	// 1 finger

			if ((hlcdtp->gest_data.gest != LCD_TP_GEST_CLICK_DOWN) && (hlcdtp->gest_data.gest != LCD_TP_GEST_DRAG)) {
				hlcdtp->gest_data.start_x = hlcdtp->touch_data[0].x;
				hlcdtp->gest_data.start_y = hlcdtp->touch_data[0].y;
				hlcdtp->gest_data.start_t = BSP_GetTick();
				hlcdtp->gest_data.gest = LCD_TP_GEST_CLICK_DOWN;

				// Checking active area;
				hlcdtp->gest_data.area = 255;
				for (uint8_t i=0;i<LCD_TP_AREA_NO;i++) {
					if (hlcdtp->touch_areas[i].active == 0) continue;
					if (hlcdtp->gest_data.start_x < hlcdtp->touch_areas[i].x) continue;
					if (hlcdtp->gest_data.start_x > (hlcdtp->touch_areas[i].x + hlcdtp->touch_areas[i].w)) continue;
					if (hlcdtp->gest_data.start_y < hlcdtp->touch_areas[i].y) continue;
					if (hlcdtp->gest_data.start_y > (hlcdtp->touch_areas[i].y + hlcdtp->touch_areas[i].h)) continue;
					hlcdtp->gest_data.area = i;
					break;
				}

			} else {
				hlcdtp->gest_data.stop_x = hlcdtp->touch_data[0].x;
				hlcdtp->gest_data.stop_y = hlcdtp->touch_data[0].y;
				hlcdtp->gest_data.stop_t = BSP_GetTick();
				hlcdtp->gest_data.delta_x = hlcdtp->gest_data.stop_x - hlcdtp->gest_data.start_x;
				hlcdtp->gest_data.delta_y = hlcdtp->gest_data.stop_y - hlcdtp->gest_data.start_y;
				hlcdtp->gest_data.delta_t = hlcdtp->gest_data.stop_t - hlcdtp->gest_data.start_t;
				hlcdtp->gest_data.gest = LCD_TP_GEST_DRAG;
			}

			break;

		case 2:	// 2 fingers

			if ((hlcdtp->touch_data[0].status == 2) && (hlcdtp->touch_data[1].status == 2)) {

				hlcdtp->gest_data.start_x = hlcdtp->touch_data[0].x;
				hlcdtp->gest_data.start_y = hlcdtp->touch_data[0].y;
				hlcdtp->gest_data.stop_x = hlcdtp->touch_data[1].x;
				hlcdtp->gest_data.stop_y = hlcdtp->touch_data[1].y;
				hlcdtp->gest_data.delta_x = hlcdtp->gest_data.stop_x - hlcdtp->gest_data.start_x;
				hlcdtp->gest_data.delta_y = hlcdtp->gest_data.stop_y - hlcdtp->gest_data.start_y;
				hlcdtp->gest_data.stop_t = BSP_GetTick();

				if (hlcdtp->gest_data.gest != LCD_TP_GEST_ZOOM) {
					hlcdtp->gest_data.start_t = BSP_GetTick();

					// Checking active area;
					hlcdtp->gest_data.area = 255;
					for (uint8_t i=0;i<LCD_TP_AREA_NO;i++) {
						if (hlcdtp->touch_areas[i].active == 0) continue;
						if (hlcdtp->gest_data.start_x < hlcdtp->touch_areas[i].x) continue;
						if (hlcdtp->gest_data.start_x > (hlcdtp->touch_areas[i].x + hlcdtp->touch_areas[i].w)) continue;
						if (hlcdtp->gest_data.stop_x < hlcdtp->touch_areas[i].x) continue;
						if (hlcdtp->gest_data.stop_x > (hlcdtp->touch_areas[i].x + hlcdtp->touch_areas[i].w)) continue;
						if (hlcdtp->gest_data.start_y < hlcdtp->touch_areas[i].y) continue;
						if (hlcdtp->gest_data.start_y > (hlcdtp->touch_areas[i].y + hlcdtp->touch_areas[i].h)) continue;
						if (hlcdtp->gest_data.stop_y < hlcdtp->touch_areas[i].y) continue;
						if (hlcdtp->gest_data.stop_y > (hlcdtp->touch_areas[i].y + hlcdtp->touch_areas[i].h)) continue;

						hlcdtp->gest_data.area = i;
					}

					hlcdtp->gest_data.gest = LCD_TP_GEST_ZOOM;
				}
			}
			break;

		default:
			hlcdtp->gest_data.gest = LCD_TP_GEST_NONE;
		}
}


void BSP_DRV_LCD_TP_Reset(void) {
	BSP_STM32_GPIO_WritePin(LCD_TP_RESET_PORT, LCD_TP_RESET_PIN, GPIO_PIN_RESET);
	BSP_Delay(5);
	BSP_STM32_GPIO_WritePin(LCD_TP_RESET_PORT, LCD_TP_RESET_PIN, GPIO_PIN_SET);
	BSP_Delay(5);
}
