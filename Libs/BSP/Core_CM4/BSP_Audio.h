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
#include "../Components/MAX98357A.h"
#include "../Libs/hxcmod.h"

#define AUDIO_I2S			SPI3
#define AUDIO_DMA			DMA1_Stream0
#define AUDIO_BUF_SIZE		2048	// Buffer size in samples (int16_t)
#define AUDIO_CTX_SIZE		256		// Max number of blocks in audio engine

#define AUDIO_FREQ_11025	I2S_AUDIOFREQ_11K
#define AUDIO_FREQ_22050	I2S_AUDIOFREQ_22K
#define AUDIO_FREQ_44100	I2S_AUDIOFREQ_44K

#define AUDIO_FLAG_NODATA		0
#define AUDIO_FLAG_DATAREADY	1

#define AUDIO_CMD_NOP		0x0000
#define AUDIO_CMD_INIT		0x0001
#define AUDIO_CMD_ADDBLOCK	0x0002
#define AUDIO_CMD_CLEARCTX	0x0003
#define AUDIO_CMD_SETPARAM	0x0004
#define AUDIO_CMD_GETPARAM	0x0005

#define AUDIO_CMD_START		0x0100
#define AUDIO_CMD_STOP		0x0101
#define AUDIO_CMD_PAUSE		0x0102
#define AUDIO_CMD_RESUME	0x0103



// Data channel type definitions

typedef struct _DATA_int8 {
	int8_t		data;
	uint8_t		flag;
} DATA_int8;

typedef struct _DATA_int16 {
	int16_t		data;
	uint8_t 	flag;
} DATA_int16;

typedef struct _DATA_int32 {
	int32_t		data;
	uint8_t		flag;
} DATA_int32;

typedef struct _DATA_uint8 {
	uint8_t		data;
	uint8_t		flag;
} DATA_uint8;

typedef struct _DATA_uint16 {
	uint16_t	data;
	uint8_t		flag;
} DATA_uint16;

typedef struct _DATA_uint32 {
	uint32_t	data;
	uint8_t		flag;
} DATA_uint32;



// Memory placeholder for Audio Block
typedef struct _AB {
	uint16_t		type;
	uint8_t			blockdata[256];
} AB;


// Speakers block (final block)
#define AUDIO_BLOCK_NONE		0;
#define AUDIO_BLOCK_SPEAKERS	1;

typedef struct _AB_Speakers_priv {
	int16_t*		pBuf;
	uint32_t		BufSize;
	uint32_t		BufHalfSize;
	uint8_t			BufFLag1;
	uint8_t			BufFlag2;
} AB_Speakers_priv;


typedef struct _AB_Speakers_input {
	DATA_int16*		pChannelL;
	DATA_int16*		pChannelR;
} AB_Speakers_input;


typedef struct _AB_Speakers_params {
	DATA_uint8*		pVolumeL;
	DATA_uint8*		pVolumeR;
} AB_Speakers_params;


typedef struct _AB_Speakers {
	AB_Speakers_priv	_priv;
	AB_Speakers_input	input;
	AB_Speakers_params	params;
} AB_Speakers_TypeDef;




uint8_t BSP_Audio_Init();
uint8_t BSP_Audio_Task();
uint8_t BSP_Audio_Transmit(int16_t * pData, uint32_t size);
uint8_t BSP_Audio_Start();
uint8_t BSP_Audio_Pause();
uint8_t BSP_Audio_Resume();


#ifdef __cplusplus
}
#endif

#endif /* BSP_AUDIO_H_ */
