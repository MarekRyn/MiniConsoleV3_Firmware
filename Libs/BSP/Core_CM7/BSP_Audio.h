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

#define AUDIO_CFG_CHANNELS		8
#define AUDIO_CFG_MAXMASTERVOL	192

uint8_t BSP_Audio_LoadMasterVolume(void);
uint8_t BSP_Audio_SaveMasterVolume(void);
uint8_t BSP_Audio_SetMasterVolume(uint8_t volume);
uint8_t BSP_Audio_GetMasterVolume(void);
uint8_t BSP_Audio_GetMasterVolumeL(void);
uint8_t BSP_Audio_GetMasterVolumeR(void);
uint8_t BSP_Audio_SetMasterVolumeLR(uint8_t volume_L, uint8_t volume_R);
uint8_t BSP_Audio_IncMasterVolume(uint8_t delta);
uint8_t BSP_Audio_DecMasterVolume(uint8_t delta);
uint8_t BSP_Audio_SetChannelVolume(uint8_t chno, uint8_t volume);
uint8_t BSP_Audio_SetChannelVolumeLR(uint8_t chno, uint8_t volume_L, uint8_t volume_R);
uint8_t BSP_Audio_IncChannelVolume(uint8_t chno, uint8_t delta);
uint8_t BSP_Audio_DecChannelVolume(uint8_t chno, uint8_t delta);
uint8_t BSP_Audio_Init(void);
uint8_t BSP_Audio_LinkStartupSound(uint8_t chno);
uint8_t BSP_Audio_LinkTestSound(uint8_t chno);
uint8_t BSP_Audio_LinkSourceMP3(uint8_t chno, uint32_t addr, uint32_t size);
uint8_t BSP_Audio_LinkSourceMOD(uint8_t chno, uint32_t addr, uint32_t size);
uint8_t BSP_Audio_LinkSourceRAW(uint8_t chno, uint32_t addr, uint32_t size);
uint8_t BSP_Audio_ChannelPLay(uint8_t chno, uint8_t repeat);
uint8_t BSP_Audio_ChannelStop(uint8_t chno);
uint8_t BPS_Audio_ChannelPause(uint8_t chno);

// uint8_t BSP_Audio_LinkSourceFM(uint8_t chno, ....);


#ifdef __cplusplus
}
#endif

#endif /* BSP_AUDIO_H_ */
