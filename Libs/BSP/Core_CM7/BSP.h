/*******************************************************************
 * MiniConsole V3 - Board Support Package - CM7
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 *******************************************************************/

#ifndef INC_BSP_H
#define INC_BSP_H

#ifdef __cplusplus
extern "C" {
#endif

// Includes
#include "BSP_STM32.h"
#include "BSP_USB.h"
#include "BSP_IMU.h"
#include "BSP_LCD_TP.h"
#include "BSP_LCD.h"
#include "BSP_SDRAM.h"
#include "BSP_SDCARD.h"
#include "BSP_QSPI.h"
#include "BSP_Inputs.h"
#include "BSP_Resources.h"
#include "BSP_Audio.h"
#include "BSP_Serial.h"
#include "BSP_PWR.h"

#include "BSP_Icons.h"
#include "BSP_Fonts.h"
#include "BSP_Wallpaper.h"

#include "graph2d.h"


extern void * BSP_Driver[1024];	// Vector Table for external application

uint8_t BSP_BOARD_Init_0(void);
uint8_t BSP_BOARD_Init_1(void);
void BSP_Error_Handler(void);


#ifdef __cplusplus
}
#endif

#endif /* INC_BSP_H */
