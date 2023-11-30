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


// Variables
TxRxContext_TypeDef				BSP_haudio_ctx = {0};
static msample					AudioBuf[2 * AUDIO_BUF_SIZE] __attribute__ ((aligned (4)));
static modcontext 				modctx;



// ********** IRQ Handlers and Callbacks ****************

// I2S3 IRQ Handler
void SPI3_IRQHandler(void) {
	BSP_STM32_I2S_IRQHandler(AUDIO_I2S, &BSP_haudio_ctx);
}

// IRQ from Core CM7 - activated when new command is available to process
void CM7_SEV_IRQHandler(void) {
	return;
}

// Callback when DMA reach end of buffer
void _AudioCallbackTC(TxRxContext_TypeDef * ctx) {
	hxcmod_fillbuffer(&modctx, (msample *)&AudioBuf[AUDIO_BUF_SIZE], AUDIO_BUF_SIZE / 2, NULL);
	for (uint32_t i = 0; i < AUDIO_BUF_SIZE; i++) AudioBuf[i + AUDIO_BUF_SIZE] /= 4;
	UNUSED(ctx);
}

// Callback when DMA reach half of buffer
void _AudioCallbackHT(TxRxContext_TypeDef * ctx) {
	hxcmod_fillbuffer(&modctx, (msample *)&AudioBuf[0], AUDIO_BUF_SIZE / 2, NULL);
	for (uint32_t i = 0; i < AUDIO_BUF_SIZE; i++) AudioBuf[i] /= 4;
	UNUSED(ctx);
}

// Callback on DMA error
void _AudioCallbackTE(TxRxContext_TypeDef * ctx) {
	UNUSED(ctx);
}


// ********** Public functions ****************


uint8_t BSP_Audio_Init() {
	// Configuring I2S Interface
	BSP_STM32_I2S_Init(AUDIO_I2S, &BSP_haudio_ctx, I2S_AUDIOFREQ_44K);

	// Linking DMA stream to I2C context
	BSP_STM32_DMA_Init(AUDIO_DMA, DMA_MEMORY_TO_PERIPH, DMA_PINC_DISABLE, DMA_MINC_ENABLE, DMA_PDATAALIGN_HALFWORD, DMA_MDATAALIGN_HALFWORD, DMA_CIRCULAR, DMA_PRIORITY_LOW);

	BSP_haudio_ctx.callback_HT = (uint32_t)_AudioCallbackHT;
	BSP_haudio_ctx.callback_TC = (uint32_t)_AudioCallbackTC;
	BSP_haudio_ctx.callback_TE = (uint32_t)_AudioCallbackTE;

	if (BSP_STM32_DMA_CtxLink(AUDIO_DMA, &BSP_haudio_ctx, DMA_REQUEST_SPI3_TX)) return BSP_ERROR;

	// Enable Audio Amplifiers
	BSP_DRV_Audio_Enable();

	// Initialization of Audio Engine


	return BSP_OK;
}


uint8_t BSP_Audio_Task() {

	return BSP_OK;
}




uint8_t BSP_Audio_Transmit(int16_t * pData, uint32_t size) {
	//return BSP_STM32_I2S_TransmitIT(AUDIO_I2S, &BSP_haudio_ctx, pData, size);
	return BSP_STM32_I2S_TransmitDMA(AUDIO_I2S, &BSP_haudio_ctx, pData, size);
}


uint8_t BSP_Audio_Start() {
	  hxcmod_init(&modctx);
	  hxcmod_setcfg(&modctx, 44100, 0, 0);
	  hxcmod_load(&modctx, (void *)0xC08C42BC, 51090);

	return BSP_STM32_I2S_TransmitDMA(AUDIO_I2S, &BSP_haudio_ctx, (int16_t *)AudioBuf, AUDIO_BUF_SIZE * 2);
}

uint8_t BSP_Audio_Pause() {
	return BSP_STM32_I2S_Pause(AUDIO_I2S, &BSP_haudio_ctx);
}

uint8_t BSP_Audio_Resume() {
	return BSP_STM32_I2S_Resume(AUDIO_I2S, &BSP_haudio_ctx);
}



