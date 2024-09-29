/*******************************************************************
 * MiniConsole V3 - Board Support Package - Keyboard and Joystick
 *
 * Author: Marek Ryn
 * Version: 1.1
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 * - 1.0	- Added calibration functions
 * - 1.1	- Shortcuts for control of backlight, volume and pwr off
 *******************************************************************/

#ifndef BSP_INPUTS_H_
#define BSP_INPUTS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "BSP_STM32.h"
#include "BSP_LCD.h"	// For controlling brightness
#include "BSP_PWR.h"	// For controlling power
#include "BSP_Audio.h"	// For controlling audio volume

// BSP structures - Inputs (Buttons and Joystick)

typedef struct _INPUTS_JOY_CAL {
	uint16_t		x_center_min;
	uint16_t		x_center_max;
	uint16_t		y_center_min;
	uint16_t		y_center_max;
	uint16_t		x_min;
	uint16_t		x_max;
	uint16_t		y_min;
	uint16_t		y_max;
} INPUTS_JOY_CAL;

typedef struct _INPUTS_JOY {
	int16_t			joy_X;		// range from -512 to +512 (0 means center)
	int16_t			joy_Y;		// range from -512 to +512 (0 means center)
} INPUTS_JOY;

typedef struct _INPUTS_BTNS {
	uint8_t			btn_A;
	uint8_t			btn_B;
	uint8_t			btn_C;
	uint8_t			btn_D;
	uint8_t			btn_X_U;
	uint8_t			btn_X_D;
	uint8_t			btn_X_L;
	uint8_t			btn_X_R;
	uint8_t			btn_JOY;
	uint8_t			btn_MENU;
	uint8_t			btn_PWR;
} INPUTS_BTNS;

typedef struct _INPUTS {
	uint16_t		raw_data_joy[2];
	INPUTS_JOY_CAL	joy_cal;
	INPUTS_JOY		joy;
	INPUTS_BTNS		buttons;
	uint8_t			mode;
	uint32_t		timestamp;
} INPUTS_HandleTypeDef;


extern BUF_RAM	INPUTS_HandleTypeDef	BSP_hinputs;

uint8_t BSP_Inputs_LoadCalData(void);
uint8_t BSP_Inputs_SaveCalData(void);
uint8_t BSP_Inputs_Init(void);
uint8_t BSP_Inputs_ParseData(void);
uint8_t BSP_Inputs_CalibrateJoyInit(void);
uint8_t BSP_Inputs_CalibrateJoyDeadZone(void);
uint8_t BSP_Inputs_CalibrateJoyRange(void);
uint8_t BSP_Inputs_CancelCallibration(void);


#ifdef __cplusplus
}
#endif

#endif /* BSP_INPUTS_H_ */
