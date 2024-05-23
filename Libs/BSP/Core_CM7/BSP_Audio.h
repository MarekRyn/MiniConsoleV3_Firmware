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

enum AUDIO_CMD {
	AUDIO_CMD_NONE,
	AUDIO_CMD_LINK_SND_LOGO,
	AUDIO_CMD_LINK_SND_TEST,
	AUDIO_CMD_LINK_MP3,
	AUDIO_CMD_LINK_MOD,
	AUDIO_CMD_LINK_RAW,
	AUDIO_CMD_PLAY,
	AUDIO_CMD_STOP,
	AUDIO_CMD_PAUSE
};

enum AUDIO_STATUS {
	AUDIO_STATUS_NONE,
	AUDIO_STATUS_READY,
	AUDIO_STATUS_BUF_UNDERRUN,
	AUDIO_STATUS_CH_REPEAT,
	AUDIO_STATUS_CH_STOP,
	AUDIO_STATUS_COUNT				// Not a real status, just counter how many status values are defined
};

enum AUDIO_CH_STATE {
	AUDIO_CH_STATE_DISABLED,
	AUDIO_CH_STATE_PLAY,
	AUDIO_CH_STATE_STOP,
	AUDIO_CH_STATE_PAUSE
};


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
uint8_t BSP_Audio_LinkSourceMP3(uint8_t chno, void * addr, uint32_t size);
uint8_t BSP_Audio_LinkSourceMOD(uint8_t chno, void * addr, uint32_t size);
uint8_t BSP_Audio_LinkSourceRAW(uint8_t chno, void * addr, uint32_t size);
uint8_t BSP_Audio_ChannelPLay(uint8_t chno, uint8_t repeat);
uint8_t BSP_Audio_ChannelStop(uint8_t chno);
uint8_t BSP_Audio_ChannelPause(uint8_t chno);
uint8_t BSP_Audio_RegisterStatusCallback(uint8_t status, void* callback);
uint32_t BSP_Audio_GetStatusParam(uint8_t index);

// uint8_t BSP_Audio_LinkSourceFM(uint8_t chno, ....);


#ifdef __cplusplus
}
#endif

#endif /* BSP_AUDIO_H_ */
