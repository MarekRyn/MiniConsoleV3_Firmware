/*******************************************************************
 * MiniConsole V3 - Board Support Package - Audio Libs
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 *******************************************************************/

#ifndef BSP_AUDIO_H_
#define BSP_AUDIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "BSP_STM32.h"
#include "../Components/MAX98357A.h"


uint8_t BSP_Audio_Init();
uint8_t BSP_Audio_Transmit(int16_t * pData, uint32_t size);
uint8_t BSP_Audio_Pause();
uint8_t BSP_Audio_Resume();

#ifdef __cplusplus
}
#endif

#endif /* BSP_AUDIO_H_ */
