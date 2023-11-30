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

TxRxContext_TypeDef		BSP_haudio_ctx = {0};


uint8_t BSP_Audio_Init() {
	// Configuring I2S Interface
	BSP_STM32_I2S_Init(SPI3, &BSP_haudio_ctx, I2S_AUDIOFREQ_44K);

	// Linking DMA stream to I2C context
	BSP_STM32_DMA_Init(DMA1_Stream0, DMA_MEMORY_TO_PERIPH, DMA_PINC_DISABLE, DMA_MINC_ENABLE, DMA_PDATAALIGN_HALFWORD, DMA_MDATAALIGN_HALFWORD, DMA_CIRCULAR, DMA_PRIORITY_LOW);
	if (BSP_STM32_DMA_CtxLink(DMA1_Stream0, &BSP_haudio_ctx, DMA_REQUEST_SPI3_TX)) return BSP_ERROR;

	// Enable Audio Amplifiers
	BSP_DRV_Audio_Enable();

	// Initialization of Audio Engine


	return BSP_OK;
}


uint8_t BSP_Audio_Transmit(int16_t * pData, uint32_t size) {
	return BSP_STM32_I2S_TransmitIT(SPI3, &BSP_haudio_ctx, pData, size);
	//return BSP_STM32_I2S_TransmitDMA(SPI3, &BSP_haudio_ctx, pData, size);
}


uint8_t BSP_Audio_Pause() {
	return BSP_STM32_I2S_Pause(SPI3, &BSP_haudio_ctx);
}

uint8_t BSP_Audio_Resume() {
	return BSP_STM32_I2S_Resume(SPI3, &BSP_haudio_ctx);
}

void SPI3_IRQHandler(void) {
	BSP_STM32_I2S_IRQHandler(SPI3, &BSP_haudio_ctx);
}
