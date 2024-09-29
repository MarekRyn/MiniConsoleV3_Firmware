/*******************************************************************
 * MiniConsole V3 - Board Support Package - Video Library
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 * - 1.0	- First stable release
 *******************************************************************/

#ifndef BSP_CORE_CM7_BSP_VIDEO_H_
#define BSP_CORE_CM7_BSP_VIDEO_H_

#include "BSP_STM32.h"
#include "BSP_LCD.h"
#include "BSP_fatfs.h"

uint8_t BSP_Video_Init(char * filename, uint8_t * pVideoBuffer, uint32_t VideoBufferSize);
uint8_t BSP_Video_GetFrame(void);
uint8_t BSP_Video_DeInit(void);
uint8_t BSP_Video_Seek(uint32_t frame);
uint8_t BSP_Video_Rev(uint32_t delta);
uint8_t BSP_Video_Fwd(uint32_t delta);
uint8_t BSP_Video_Play(void);
uint8_t BSP_Video_Stop(void);
uint8_t BSP_Video_Pause(void);
uint8_t BSP_Video_SetVolume(uint8_t vol);
uint8_t BSP_Video_DrawFrame(int16_t x, int16_t y);
uint8_t BSP_Video_DrawFrameC(int16_t x, int16_t y);
uint32_t BSP_Video_GetTotalFrames(void);
uint32_t BSP_Video_GetCurrentFrame(void);
uint32_t BSP_Video_GetWidth(void);
uint32_t BSP_Video_GetHeight(void);
uint32_t BSP_Video_GetFrameRate(void);

#endif /* BSP_CORE_CM7_BSP_VIDEO_H_ */
