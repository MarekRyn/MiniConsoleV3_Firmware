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


#ifndef INC_RVT50AQTNWC00_H_
#define INC_RVT50AQTNWC00_H_

#ifdef __cplusplus
extern "C" {
#endif

// Includes

#include "BSP_Common.h"
#include "BSP_STM32.h"


// Driver constants
#define LCD_TP_GEST_NONE			0x00
#define LCD_TP_GEST_CLICK_DOWN		0x01
#define LCD_TP_GEST_CLICK_UP		0x02
#define LCD_TP_GEST_SWIPE_UP		0x04
#define LCD_TP_GEST_SWIPE_DOWN		0x08
#define LCD_TP_GEST_SWIPE_LEFT		0x10
#define LCD_TP_GEST_SWIPE_RIGHT		0x20
#define LCD_TP_GEST_DRAG			0x40
#define LCD_TP_GEST_ZOOM			0x80


// LCD Parameter definitions (For 30Hz Refresh Rate)

#define LCD_WIDTH					800 																		// Width in pixels
#define LCD_HEIGHT					480																			// Height in pixels

#define LCD_H_SYNC_WIDTH			26																			// Horizontal Synchronization Width
#define LCD_H_BACK_PORCH			20																			// Horizontal Back Porch
#define LCD_H_FRONT_PORCH			16																			// Horizontal Front Porch
#define LCD_V_SYNC_HEIGHT			3																			// Vertical Synchronization Height
#define LCD_V_BACK_PORCH			20																			// Vertical Back Porch
#define LCD_V_FRONT_PORCH			77																			// Vertical Front Porch


// For clock 15MHz:
// Total width x Total height = (800 + 26 + 20 + 16) * (480 + 3 + 20 + 77) = 862 x 580 = 499960
// Refresh Rate = 15000000 / 499960 = 30.002 Hz


#define LCD_H_SYNC					LCD_H_SYNC_WIDTH - 1														// Horizontal Synchronization Width Calculated
#define LCD_ACC_H_BACK_PORCH_WIDTH	LCD_H_SYNC_WIDTH + LCD_H_BACK_PORCH - 1										// Accumulated Horizontal Back Porch Width
#define LCD_ACC_ACTIVE_WIDTH		LCD_H_SYNC_WIDTH + LCD_H_BACK_PORCH + LCD_WIDTH - 1							// Accumulated Active Width
#define LCD_TOTAL_WIDTH				LCD_H_SYNC_WIDTH + LCD_H_BACK_PORCH + LCD_WIDTH + LCD_H_FRONT_PORCH - 1 	// Total Width
#define LCD_V_SYNC					LCD_V_SYNC_HEIGHT - 1														// Vertical Synchronization Height Calculated
#define LCD_ACC_V_BACK_PORCH_HEIGHT	LCD_V_SYNC_HEIGHT + LCD_V_BACK_PORCH - 1									// Accumulated Vertical Back Porch Height
#define LCD_ACC_ACTIVE_HEIGHT		LCD_V_SYNC_HEIGHT + LCD_V_BACK_PORCH + LCD_HEIGHT - 1						// Accumulated Active Height
#define LCD_TOTAL_HEIGHT			LCD_V_SYNC_HEIGHT + LCD_V_BACK_PORCH + LCD_HEIGHT + LCD_V_FRONT_PORCH - 1	// Total Height

#define LCD_MAX_CLK_FREQ			50																			// Maximum Clock Frequency in MHz
#define LCD_MAX_REFRESH_RATE		LCD_MAX_CLK_FREQ * 1000000 / LCD_TOTAL_WIDTH / LCD_TOTAL_HEIGHT				// Maximum Refresh Rate in Hz

#define LCD_TP_INTERFACE			LCD_TP_INTERFACE_I2C


// LCD I2C Touch Panel definitions

#define LCD_TP_I2C_SPEED			400						// Interface Speed in kHz
#define LCD_TP_I2C_ADDR				0x38					// Slave I2C Address (7-bit)

#define LCD_TP_REG_TOTAL_LENGTH		0x1F					// Total length of registers in bytes

#define LCD_TP_REG_DEVICE_MODE		0x00					// Device mode
#define LCD_TP_REG_GEST_ID			0x01					// Gest ID calculated by TP controller (NOT WORKING AS EXPACTED)
#define LCD_TP_REG_TD_STATUS		0x02					// Number of touch points

#define LCD_TP_REG_TOUCH_01_XH		0x03					// Event flag and 4 major bits of touch X coordinate
#define LCD_TP_REG_TOUCH_01_XL		0x04					// 8 minor bits of X coordinate
#define LCD_TP_REG_TOUCH_01_YH		0x05					// Event flag and 4 major bits of touch Y coordinate
#define LCD_TP_REG_TOUCH_01_YL		0x06					// 8 minor bits of Y coordinate

#define LCD_TP_REG_TOUCH_02_XH		0x09					// Event flag and 4 major bits of touch X coordinate
#define LCD_TP_REG_TOUCH_02_XL		0x0A					// 8 minor bits of X coordinate
#define LCD_TP_REG_TOUCH_02_YH		0x0B					// Event flag and 4 major bits of touch Y coordinate
#define LCD_TP_REG_TOUCH_02_YL		0x0C					// 8 minor bits of Y coordinate

#define LCD_TP_REG_TOUCH_03_XH		0x0F					// Event flag and 4 major bits of touch X coordinate
#define LCD_TP_REG_TOUCH_03_XL		0x10					// 8 minor bits of X coordinate
#define LCD_TP_REG_TOUCH_03_YH		0x11					// Event flag and 4 major bits of touch Y coordinate
#define LCD_TP_REG_TOUCH_03_YL		0x12					// 8 minor bits of Y coordinate

#define LCD_TP_REG_TOUCH_04_XH		0x15					// Event flag and 4 major bits of touch X coordinate
#define LCD_TP_REG_TOUCH_04_XL		0x16					// 8 minor bits of X coordinate
#define LCD_TP_REG_TOUCH_04_YH		0x17					// Event flag and 4 major bits of touch Y coordinate
#define LCD_TP_REG_TOUCH_04_YL		0x18					// 8 minor bits of Y coordinate

#define LCD_TP_REG_TOUCH_05_XH		0x1B					// Event flag and 4 major bits of touch X coordinate
#define LCD_TP_REG_TOUCH_05_XL		0x1C					// 8 minor bits of X coordinate
#define LCD_TP_REG_TOUCH_05_YH		0x1D					// Event flag and 4 major bits of touch Y coordinate
#define LCD_TP_REG_TOUCH_05_YL		0x1E					// 8 minor bits of Y coordinate

#define LCD_TP_RESET_PIN			GPIO_PIN_14
#define LCD_TP_RESET_PORT			GPIOG
#define LCD_TP_AREA_NO				32
#define LCD_TP_DATA_NO				5

// BSP structures - LCD Touch Panel

typedef struct {
	uint16_t	x;
	uint16_t	y;
	uint8_t		status;
	uint8_t		pstatus;
	uint8_t		id;
} TP_DATA;

typedef struct {
	int16_t		x;
	int16_t		y;
	uint16_t	w;
	uint16_t	h;
	void *		callback;
	uint8_t		active;
} TP_AREA;

typedef struct {
	uint8_t		area;
	uint32_t	gest;
	uint16_t	start_x;
	uint16_t	start_y;
	uint32_t	start_t;
	uint16_t	stop_x;
	uint16_t	stop_y;
	uint32_t	stop_t;
	int16_t		delta_x;
	int16_t		delta_y;
	uint32_t	delta_t;
	float		speed_x;
	float		speed_y;
} TP_GEST;

typedef struct {
	uint8_t		enabled;
	uint8_t		raw_data[LCD_TP_REG_TOTAL_LENGTH];
	uint8_t		touch_count;
	TP_DATA		touch_data[LCD_TP_DATA_NO];
	TP_AREA		touch_areas[LCD_TP_AREA_NO];
	TP_GEST		gest_data;
} LCD_TP_HandleTypeDef;



uint8_t BSP_DRV_LCD_TP_Init(I2C_TypeDef *hi2c);
void BSP_DRV_LCD_TP_Parse(LCD_TP_HandleTypeDef *hlcdtp);
void BSP_DRV_LCD_TP_Reset(void);

#ifdef __cplusplus
}
#endif

#endif /* INC_RVT50AQTNWC00_H_ */
