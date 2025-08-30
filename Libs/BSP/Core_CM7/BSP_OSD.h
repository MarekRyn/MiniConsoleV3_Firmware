/*******************************************************************
 * MiniConsole V3 - Board Support Package - On Screen Menu
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 *******************************************************************/

#ifndef BSP_CORE_CM7_BSP_OSD_H_
#define BSP_CORE_CM7_BSP_OSD_H_

#define OSD_DISABLED			0
#define OSD_HIDDEN				1
#define OSD_HIDE				2
#define OSD_INFO				3
#define	OSD_VOLUME				4
#define OSD_BRIGHTNESS			5
#define OSD_NOTIFICATION		6
#define OSD_ERROR				7
#define OSD_PWROFF				8

#include <stdio.h>

#include "BSP_STM32.h"
#include "BSP_RTC.h"
#include "BSP_Audio.h"
#include "BSP_Fonts.h"
#include "BSP_Icons.h"
#include "graph2d.h"

uint8_t BSP_OSD_Init(void);
uint8_t	BSP_OSD_ShowInfo(void);
uint8_t	BSP_OSD_ShowVolume(void);
uint8_t	BSP_OSD_ShowBrightness(void);
uint8_t	BSP_OSD_ShowNotification(char * str);
uint8_t	BSP_OSD_ShowAlert(char * str);
uint8_t	BSP_OSD_ShowPwrOff(uint16_t value, uint16_t limit);
uint8_t BSP_OSD_Hide(void);
uint8_t BSP_OSD_Process(void);


#endif /* BSP_CORE_CM7_BSP_OSD_H_ */
