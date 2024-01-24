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
#include "MAX98357A.h"
#include "BSP_Sounds.h"

// Buffer size 8192 equals to DMA IRQ executed every ~ 45ms (for stereo audio @ 44100Hz)

#define AUDIO_CFG_I2S			SPI3
#define AUDIO_CFG_DMA			DMA1_Stream0
#define AUDIO_CFG_BUF_SIZE		8192		// Buffer size in samples (int16_t)
#define AUDIO_CFG_CHANNELS		8
#define AUDIO_CFG_MIXDIV		3			// must be log2(AUDIO_CFG_CHANNELS)


#define AUDIO_FREQ_11025	I2S_AUDIOFREQ_11K
#define AUDIO_FREQ_22050	I2S_AUDIOFREQ_22K
#define AUDIO_FREQ_44100	I2S_AUDIOFREQ_44K

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
	AUDIO_STATUS_CH_STOP
};

enum AUDIO_CH_STATE {
	AUDIO_CH_STATE_DISABLED,
	AUDIO_CH_STATE_PLAY,
	AUDIO_CH_STATE_STOP,
	AUDIO_CH_STATE_PAUSE
};

enum AUDIO_CH_SOURCE {
	AUDIO_CH_SOURCE_NONE,
	AUDIO_CH_SOURCE_MP3,
	AUDIO_CH_SOURCE_MOD,
	AUDIO_CH_SOURCE_RAW,
	AUDIO_CH_SOURCE_FM
};

uint8_t	BSP_Audio_SetChannelVolume(uint8_t chno, uint8_t volume);
uint8_t	BSP_Audio_SetMasterVolume(uint8_t volume);
uint8_t BSP_Audio_ChannelFree(uint8_t chno);
uint8_t BSP_Audio_ChannelLinkSource(uint8_t chno, uint32_t source);
uint8_t BSP_Audio_Init(void);
uint8_t BSP_Audio_Loop(void);


#ifdef __cplusplus
}
#endif

#endif /* BSP_AUDIO_H_ */
