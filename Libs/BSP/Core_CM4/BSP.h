/*******************************************************************
 * MiniConsole V3 - Board Support Package - CM4
 *
 * Author: Marek Ryn
 * Version: 1.0
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 * - 1.0	- First stable release
 *******************************************************************/

#ifndef INC_BSP_H
#define INC_BSP_H

#ifdef __cplusplus
extern "C" {
#endif

// Includes
#include "BSP_STM32.h"
//#include "BSP_USB.h"
//#include "BSP_IMU.h"
//#include "BSP_LCD_TP.h"
//#include "BSP_LCD.h"
//#include "BSP_SDRAM.h"
//#include "BSP_SDCARD.h"
//#include "BSP_QSPI.h"
//#include "BSP_Inputs.h"
//#include "BSP_Resources.h"
#include "BSP_Audio.h"


uint8_t BSP_BOARD_Init();


#ifdef __cplusplus
}
#endif

#endif /* INC_BSP_H */
