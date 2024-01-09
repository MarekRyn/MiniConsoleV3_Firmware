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
#include "BSP_Audio_Tables.h"
#include "MAX98357A.h"
#include "BSP_Sounds.h"


#define AUDIO_CFG_I2S			SPI3
#define AUDIO_CFG_DMA			DMA1_Stream0
#define AUDIO_CFG_BUF_SIZE		4096		// Buffer size in samples (int16_t)
#define AUDIO_CFG_MP3_SOURCES	2
#define AUDIO_CFG_MOD_SOURCES	2
#define AUDIO_CFG_RAW_SOURCES	4
#define AUDIO_CFG_FM_SOURCES	8
#define AUDIO_CFG_MIX_CHANNELS	8


#define AUDIO_FREQ_11025	I2S_AUDIOFREQ_11K
#define AUDIO_FREQ_22050	I2S_AUDIOFREQ_22K
#define AUDIO_FREQ_44100	I2S_AUDIOFREQ_44K

#define AUDIO_BUF_FLAG_EDIT		0
#define AUDIO_BUF_FLAG_READY	1
#define AUDIO_BUF_FLAG_BUSY		2

#define AUDIO_STATE_DISABLED	0
#define AUDIO_STATE_PLAY		1
#define AUDIO_STATE_STOP		2
#define AUDIO_STATE_PAUSE		3

uint8_t BSP_Audio_Init(void);
uint8_t BSP_Audio_Loop(void);

uint8_t BSP_Audio_Start(void);


#ifdef __cplusplus
}
#endif

#endif /* BSP_AUDIO_H_ */
