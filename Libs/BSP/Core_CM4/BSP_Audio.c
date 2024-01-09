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



#include "BSP_Audio.h"

// Codecs
#define DR_MP3_IMPLEMENTATION
#include "dr_mp3.h"
#include "hxcmod.h"


// Type definitions
typedef struct {
	drmp3		ctx;
	drmp3*		pctx;
	uint8_t		state;
	uint8_t		repeat;
} AUDIO_CTX_MP3_TypeDef;

typedef struct {
	modcontext	ctx;
	modcontext*	pctx;
	uint8_t		state;
	uint8_t		repeat;
} AUDIO_CTX_MOD_TypeDef;

typedef struct {
	// TODO: ctx definition
	uint8_t		state;
	uint8_t		repeat;
} AUDIO_CTX_RAW_TypeDef;

typedef struct {
	// TODO: ctx definition
	uint8_t		state;
	uint8_t		repeat;
} AUDIO_CTX_FM_TypeDef;

typedef struct {
	AUDIO_CTX_MP3_TypeDef	source_mp3[AUDIO_CFG_MP3_SOURCES];
	AUDIO_CTX_MOD_TypeDef	source_mod[AUDIO_CFG_MOD_SOURCES];
	AUDIO_CTX_RAW_TypeDef	source_raw[AUDIO_CFG_RAW_SOURCES];
	AUDIO_CTX_FM_TypeDef	source_fm[AUDIO_CFG_FM_SOURCES];
	uint8_t					buf_edit;
	uint8_t					buf_ready;
	uint8_t					buf_busy;
	int16_t *				pOutputBuf[2];
	int16_t	*				pChannelBuf[AUDIO_CFG_MIX_CHANNELS][2];
} AUDIO_CTX_TypeDef;


// Variables
__IO static	AUDIO_CTX_TypeDef		AUDIO_ctx = {0};
__IO static	TxRxContext_TypeDef		BSP_hdma_ctx = {0};

__IO static int16_t 			OutputBuf[AUDIO_CFG_BUF_SIZE] __attribute__ ((aligned (4)));	// Final output buffer
__IO static int16_t				ChannelBuf[AUDIO_CFG_MIX_CHANNELS][AUDIO_CFG_BUF_SIZE] __attribute__ ((aligned (4)));	// Channel buffers
__IO static int32_t				MixBuf[AUDIO_CFG_BUF_SIZE >> 1];


// ********** IRQ Handlers and Callbacks ****************

// I2S3 IRQ Handler
void SPI3_IRQHandler(void) {
	BSP_STM32_I2S_IRQHandler(AUDIO_CFG_I2S,(TxRxContext_TypeDef* )&BSP_hdma_ctx);
}

// IRQ from Core CM7 - activated when new command is available to process
void CM7_SEV_IRQHandler(void) {

}

// Callback when DMA reach end of buffer
static void _AudioCallbackTC(TxRxContext_TypeDef * ctx) {
	if (AUDIO_ctx.buf_ready != 0) return;
	AUDIO_ctx.buf_ready = 255;
	AUDIO_ctx.buf_edit = 1;
	AUDIO_ctx.buf_busy = 0;

	UNUSED(ctx);
}

// Callback when DMA reach half of buffer
static void _AudioCallbackHT(TxRxContext_TypeDef * ctx) {
	if (AUDIO_ctx.buf_ready != 1) return;
	AUDIO_ctx.buf_ready = 255;
	AUDIO_ctx.buf_edit = 0;
	AUDIO_ctx.buf_busy = 1;

	UNUSED(ctx);
}

// Callback on DMA error
static void _AudioCallbackTE(TxRxContext_TypeDef * ctx) {
	UNUSED(ctx);
}




// ********** Public functions ****************


uint8_t BSP_Audio_Init(void) {
	// Configuring I2S Interface
	BSP_STM32_I2S_Init(AUDIO_CFG_I2S, (TxRxContext_TypeDef* )&BSP_hdma_ctx, I2S_AUDIOFREQ_44K);

	// Linking DMA stream to I2C context
	BSP_STM32_DMA_Init(AUDIO_CFG_DMA, DMA_MEMORY_TO_PERIPH, DMA_PINC_DISABLE, DMA_MINC_ENABLE, DMA_PDATAALIGN_HALFWORD, DMA_MDATAALIGN_HALFWORD, DMA_CIRCULAR, DMA_PRIORITY_LOW);

	BSP_hdma_ctx.callback_HT = (uint32_t)_AudioCallbackHT;
	BSP_hdma_ctx.callback_TC = (uint32_t)_AudioCallbackTC;
	BSP_hdma_ctx.callback_TE = (uint32_t)_AudioCallbackTE;

	if (BSP_STM32_DMA_CtxLink(AUDIO_CFG_DMA, (TxRxContext_TypeDef* )&BSP_hdma_ctx, DMA_REQUEST_SPI3_TX)) return BSP_ERROR;

	// Enable Audio Amplifiers
	BSP_DRV_Audio_Enable();

	// Initialization of Audio Engine
	uint8_t i;
	for (i = 0; i < AUDIO_CFG_MP3_SOURCES; i++) AUDIO_ctx.source_mp3[i].pctx = (drmp3* )&AUDIO_ctx.source_mp3[i].ctx;
	for (i = 0; i < AUDIO_CFG_MOD_SOURCES; i++) AUDIO_ctx.source_mod[i].pctx = (modcontext* )&AUDIO_ctx.source_mod[i].ctx;

	AUDIO_ctx.buf_ready = 255;
	AUDIO_ctx.buf_edit = 1;
	AUDIO_ctx.buf_busy = 0;

	AUDIO_ctx.pOutputBuf[0] = (int16_t *)&OutputBuf[0];
	AUDIO_ctx.pOutputBuf[1] = (int16_t *)&OutputBuf[AUDIO_CFG_BUF_SIZE >> 1];
	for (i = 0; i < AUDIO_CFG_MIX_CHANNELS; i++) AUDIO_ctx.pChannelBuf[i][0] = (int16_t *)&ChannelBuf[i][0];
	for (i = 0; i < AUDIO_CFG_MIX_CHANNELS; i++) AUDIO_ctx.pChannelBuf[i][1] = (int16_t *)&ChannelBuf[i][AUDIO_CFG_BUF_SIZE >> 1];


	BSP_Delay(500);

	// Play Startup Sound
	drmp3_init_memory(AUDIO_ctx.source_mp3[0].pctx, (void *)SND_Logo, sizeof(SND_Logo), NULL);
	//drmp3_init_memory(AUDIO_ctx.source_mp3[1].pctx, (void *)0xC0000000, 4469294, NULL);
	hxcmod_init(AUDIO_ctx.source_mod[0].pctx);
	hxcmod_setcfg(AUDIO_ctx.source_mod[0].pctx, 44100, 0, 0);
	//hxcmod_load(AUDIO_ctx.source_mod[0].pctx, (void *)0xC0443230, 51090);

	// Start DMA
	BSP_STM32_I2S_TransmitDMA(AUDIO_CFG_I2S, (TxRxContext_TypeDef* )&BSP_hdma_ctx, (int16_t *)OutputBuf, AUDIO_CFG_BUF_SIZE);


	return BSP_OK;
}


uint8_t BSP_Audio_Start(void) {

	return BSP_OK;
}

uint8_t BSP_Audio_Loop(void) {

	while(1) {
		if (AUDIO_ctx.buf_edit == 255) continue;

		// Processing frames for each channel
		drmp3_read_pcm_frames_s16(AUDIO_ctx.source_mp3[0].pctx, AUDIO_CFG_BUF_SIZE >> 2, AUDIO_ctx.pChannelBuf[0][AUDIO_ctx.buf_edit]);
		//drmp3_read_pcm_frames_s16(AUDIO_ctx.source_mp3[1].pctx, AUDIO_CFG_BUF_SIZE >> 2, AUDIO_ctx.pChannelBuf[1][AUDIO_ctx.buf_edit]);
		//hxcmod_fillbuffer(AUDIO_ctx.source_mod[0].pctx, AUDIO_ctx.pChannelBuf[2][AUDIO_ctx.buf_edit], AUDIO_CFG_BUF_SIZE >> 2, NULL);

		// Mixing channels
		int32_t * mxsample;
		int16_t * outsample = AUDIO_ctx.pOutputBuf[AUDIO_ctx.buf_edit];
		int16_t * chsample;

		for (uint8_t ch = 0; ch < (AUDIO_CFG_MIX_CHANNELS); ch++) {
			chsample = AUDIO_ctx.pChannelBuf[ch][AUDIO_ctx.buf_edit];
			mxsample = (int32_t *)MixBuf;
			for (uint32_t i = 0; i < (AUDIO_CFG_BUF_SIZE >> 1); i++) {
				*mxsample += (int32_t)*chsample;
				mxsample++;
				chsample++;
			}
		}

		mxsample = (int32_t *)MixBuf;
		for (uint32_t i = 0; i < (AUDIO_CFG_BUF_SIZE >> 1); i++) {
			*outsample = (int16_t)(*mxsample >> 3);
			*mxsample = 0;
			outsample++;
			mxsample++;
		}

		// Switching buf flags
		AUDIO_ctx.buf_ready = AUDIO_ctx.buf_edit;
		AUDIO_ctx.buf_edit = 255;
	}

	return BSP_OK;
}


