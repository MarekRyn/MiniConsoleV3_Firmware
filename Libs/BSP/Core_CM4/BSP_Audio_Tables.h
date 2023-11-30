/*******************************************************************
 * MiniConsole V3 - Board Support Package - Audio Lookup Tables
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 *******************************************************************/

#ifndef CORE_CM4_BSP_AUDIO_TABLES_H_
#define CORE_CM4_BSP_AUDIO_TABLES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "BSP_STM32.h"

extern int16_t AUDIO_TAB_VOL32[32];
extern int16_t AUDIO_TAB_SIN512[512];


#ifdef __cplusplus
}
#endif

#endif /* CORE_CM4_BSP_AUDIO_TABLES_H_ */
