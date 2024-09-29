/*******************************************************************
 * MiniConsole V3 - Board Support Package - STM32 I2S
 *******************************************************************/

#include "BSP_STM32_I2S.h"

uint8_t BSP_STM32_I2S_Init(SPI_TypeDef * hi2s, TxRxContext_TypeDef * ctx, uint32_t AudioFreq) {

	I2SContext_TypeDef * i2s_ctx = (I2SContext_TypeDef *)ctx->ctxmem;

	// Data format: 16 bit / No channels: 2 (stereo) / Standard: PHILIPS
	uint32_t packetlength = 1;
	uint32_t ispcm = 0;
	uint32_t tmp, i2sodd, i2sdiv;

	// Getting PLL2P parameters and calculating I2S CLK
	uint32_t PLL2M = (RCC->PLLCKSELR & RCC_PLLCKSELR_DIVM2_Msk) >> RCC_PLLCKSELR_DIVM2_Pos;
	uint32_t PLL2N = ((RCC->PLL2DIVR & RCC_PLL2DIVR_N2_Msk) >> RCC_PLL2DIVR_N2_Pos) + 1;
	uint32_t PLL2P = ((RCC->PLL2DIVR & RCC_PLL2DIVR_P2_Msk) >> RCC_PLL2DIVR_P2_Pos) + 1;
	uint32_t PLLfrac = (RCC->PLL2FRACR & RCC_PLL2FRACR_FRACN2_Msk) >> RCC_PLL2FRACR_FRACN2_Pos;
	uint32_t i2sclk = (uint32_t)(((HSE_VALUE / (float)PLL2M) * ((float)PLL2N + ((float)PLLfrac / 0x2000))) / (float)PLL2P);

	// Disable the selected I2S peripheral
	if ((hi2s->CR1 & SPI_CR1_SPE) == SPI_CR1_SPE) CLEAR_BIT(hi2s->CR1, SPI_CR1_SPE);

	// Clear I2S configuration register
	CLEAR_REG(hi2s->I2SCFGR);

	// Compute the Real divider with a floating point
	tmp = (uint32_t)((((i2sclk / ((32UL >> ispcm) * packetlength)) * 10UL) / AudioFreq) + 5UL);
	tmp = tmp / 10UL;

    // Check the parity of the divider
	i2sodd = (uint32_t)(tmp & (uint32_t)1UL);

    // Compute the i2sdiv prescaler
	i2sdiv = (uint32_t)((tmp - i2sodd) / 2UL);

	// Test if the obtain values are forbidden or out of range
	if (((i2sodd == 1UL) && (i2sdiv == 1UL)) || (i2sdiv > 0xFFUL)) return BSP_ERROR;

    // Force i2smod to 1 just to be sure that (2xi2sdiv + i2sodd) is always higher than 0
    if (i2sdiv == 0UL) i2sodd = 1UL;

    MODIFY_REG(hi2s->I2SCFGR, (SPI_I2SCFGR_I2SDIV | SPI_I2SCFGR_ODD), ((i2sdiv << SPI_I2SCFGR_I2SDIV_Pos) | (i2sodd << SPI_I2SCFGR_ODD_Pos)));

    // I2Sx I2SCFGR Configuration
    MODIFY_REG(hi2s->I2SCFGR, (SPI_I2SCFGR_I2SMOD | SPI_I2SCFGR_I2SCFG | SPI_I2SCFGR_I2SSTD | SPI_I2SCFGR_PCMSYNC | SPI_I2SCFGR_DATLEN | SPI_I2SCFGR_CHLEN | SPI_I2SCFGR_CKPOL | SPI_I2SCFGR_WSINV | SPI_I2SCFGR_DATFMT | SPI_I2SCFGR_MCKOE),
    		(SPI_I2SCFGR_I2SMOD | I2S_MODE_MASTER_TX | I2S_STANDARD_PHILIPS | I2S_DATAFORMAT_16B  | I2S_CPOL_LOW | I2S_WS_INVERSION_DISABLE | I2S_DATA_24BIT_ALIGNMENT_RIGHT | I2S_MCLKOUTPUT_DISABLE));

    // Clear status register
    WRITE_REG(hi2s->IFCR, 0x0FF8);

    // I2Sx CFG2 Configuration

    // Unlock the AF configuration to configure CFG2 register*/
    CLEAR_BIT(hi2s->CR1, SPI_CR1_IOLOCK);

    MODIFY_REG(hi2s->CFG2, SPI_CFG2_LSBFRST, I2S_FIRSTBIT_MSB);

    // Insure that AFCNTR is managed only by Master
    // Alternate function GPIOs control
    MODIFY_REG(hi2s->CFG2, SPI_CFG2_AFCNTR, I2S_MASTER_KEEP_IO_STATE_DISABLE);

    // Update status
    i2s_ctx->status = I2S_STATUS_READY;
    i2s_ctx->size = 0;
    i2s_ctx->index = 0;
    i2s_ctx->pData = 0;

    return BSP_OK;

}

uint8_t BSP_STM32_I2S_RegisterCallbacks(TxRxContext_TypeDef * ctx, uint32_t CallbackTC, uint32_t CallbackHT, uint32_t CallbackTE) {
	ctx->callback_TC = CallbackTC;
	ctx->callback_HT = CallbackHT;
	ctx->callback_TE = CallbackTE;
	return BSP_OK;
}


uint8_t BSP_STM32_I2S_TransmitIT(SPI_TypeDef * hi2s, TxRxContext_TypeDef * ctx, int16_t *pData, uint32_t Size) {

	I2SContext_TypeDef * i2s_ctx = (I2SContext_TypeDef *)ctx->ctxmem;

	if (i2s_ctx->status != I2S_STATUS_READY) return BSP_ERROR;

	i2s_ctx->index = 0;
	i2s_ctx->pData = pData;
	i2s_ctx->size = Size;

	// Check if the I2S is already enabled. If not than enable.
	if ((hi2s->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE) hi2s->CR1 |= SPI_CR1_SPE;

	// Enable TXP and UDR interrupt
	hi2s->IER |= (I2S_IT_TXP | I2S_IT_UDR);

	// Start the transfer
	hi2s->CR1 |= SPI_CR1_CSTART;

	// Update status
	i2s_ctx->status = I2S_STATUS_TRANSMIT_IT;

	return BSP_OK;
}


uint8_t BSP_STM32_I2S_TransmitDMA(SPI_TypeDef * hi2s, TxRxContext_TypeDef * ctx, int16_t *pData, uint32_t Size) {

	I2SContext_TypeDef * i2s_ctx = (I2SContext_TypeDef *)ctx->ctxmem;

	if (i2s_ctx->status != I2S_STATUS_READY) return BSP_ERROR;

	i2s_ctx->index = 0;
	i2s_ctx->pData = pData;
	i2s_ctx->size = Size;

	// Enable the Tx DMA Stream
	BSP_STM32_DMA_StartIT(ctx, (uint32_t)pData, (uint32_t)&hi2s->TXDR, Size);

	// Check if the I2S Tx request is already enabled
	if ((hi2s->CFG1 & SPI_CFG1_TXDMAEN) == 0) {
		// Enable Tx DMA Request
		hi2s->CFG1 |= SPI_CFG1_TXDMAEN;
	}

	// Check if the I2S is already enabled
	if ((hi2s->CR1 &SPI_CR1_SPE) == 0) {
		// Enable I2S peripheral
		hi2s->CR1 |= SPI_CR1_SPE;
	}

	// Start the transfer
	hi2s->CR1 |= SPI_CR1_CSTART;

	// Update status
	i2s_ctx->status = I2S_STATUS_TRANSMIT_DMA;

	return BSP_OK;
}

uint8_t BSP_STM32_I2S_Pause(SPI_TypeDef * hi2s, TxRxContext_TypeDef * ctx) {

	I2SContext_TypeDef * i2s_ctx = (I2SContext_TypeDef *)ctx->ctxmem;

	// Update status
	switch (i2s_ctx->status) {
	case I2S_STATUS_TRANSMIT_IT:
		i2s_ctx->status = I2S_STATUS_PAUSE_IT;
		break;
	case I2S_STATUS_TRANSMIT_DMA:
		i2s_ctx->status = I2S_STATUS_PAUSE_DMA;
		break;
	default:
		return BSP_ERROR;
	}

	// Pause transmission
	hi2s->CR1 |= SPI_CR1_CSUSP;

	// Wait until last frame transmit completed
	while (hi2s->CR1 & SPI_CR1_CSTART) {};

	return BSP_OK;
}

uint8_t BSP_STM32_I2S_Resume(SPI_TypeDef * hi2s, TxRxContext_TypeDef * ctx) {

	I2SContext_TypeDef * i2s_ctx = (I2SContext_TypeDef *)ctx->ctxmem;

	// Update status
	switch (i2s_ctx->status) {
	case I2S_STATUS_PAUSE_IT:
		i2s_ctx->status = I2S_STATUS_TRANSMIT_IT;
		break;
	case I2S_STATUS_PAUSE_DMA:
		i2s_ctx->status = I2S_STATUS_TRANSMIT_DMA;
		break;
	default:
		return BSP_ERROR;
	}

	// Resume transition
	hi2s->CR1 |= SPI_CR1_CSTART;

	return BSP_OK;
}

uint8_t BSP_STM32_I2S_Stop(SPI_TypeDef * hi2s, TxRxContext_TypeDef * ctx) {

	I2SContext_TypeDef * i2s_ctx = (I2SContext_TypeDef *)ctx->ctxmem;

	switch (i2s_ctx->status) {
	case I2S_STATUS_TRANSMIT_DMA:
	case I2S_STATUS_PAUSE_DMA:
		 // Disable the I2S Tx DMA request
		hi2s->CFG1 &= ~SPI_CFG1_TXDMAEN;
		// Abort DMA
		BSP_STM32_DMA_Abort(ctx);
		// Disable I2S
		hi2s->CR1 &= ~SPI_CR1_SPE;
		break;
	case I2S_STATUS_TRANSMIT_IT:
	case I2S_STATUS_PAUSE_IT:
		// Disable interrupts
		hi2s->IER &= ~(I2S_IT_TXP | I2S_IT_UDR);
		// Disable I2S
		hi2s->CR1 &= ~SPI_CR1_SPE;
		break;
	default:
		return BSP_ERROR;
	}

	// Update status
	i2s_ctx->status = I2S_STATUS_READY;
	return BSP_OK;
}



uint8_t BSP_STM32_I2S_IRQHandler(SPI_TypeDef *hi2s, TxRxContext_TypeDef *ctx) {

	I2SContext_TypeDef * i2s_ctx = (I2SContext_TypeDef *)ctx->ctxmem;

	uint32_t	reg_ier = hi2s->IER;
	uint32_t	reg_sr = hi2s->SR;
	uint32_t	trigger = reg_ier & reg_sr;

	// Callback management
	void (* pCallback_TE)(TxRxContext_TypeDef * ctx);
	void (* pCallback_HT)(TxRxContext_TypeDef * ctx);
	void (* pCallback_TC)(TxRxContext_TypeDef * ctx);

	pCallback_TE = (void *)ctx->callback_TE;
	pCallback_HT = (void *)ctx->callback_HT;
	pCallback_TC = (void *)ctx->callback_TC;

	__IO int16_t *ptxdr = (__IO int16_t *)(&(hi2s->TXDR));

	// TX Mode
	if (((trigger & I2S_FLAG_TXP) == I2S_FLAG_TXP) && ((trigger & I2S_FLAG_UDR)== 0)) {
		// Writing 16 bits to TX data register
		*ptxdr = *i2s_ctx->pData;
		// Increasing index by 2 bytes (16 bits)
		i2s_ctx->index+=2;
		i2s_ctx->pData++;

		// Half buffer transmited??
		if (i2s_ctx->index == (i2s_ctx->size >> 1)) {
			 // Execute half transfer callback if set
			 if (ctx->callback_HT > 0) pCallback_HT(ctx);
		}

		// Transmission completed??
		if (i2s_ctx->index >= i2s_ctx->size) {
			// Disable TXP and ERR interrupt
			hi2s->IER &= ~(I2S_FLAG_TXP | I2S_FLAG_UDR);
			// Update status
			i2s_ctx->status = I2S_STATUS_READY;
			// Execute transfer completed callback if set
			if (ctx->callback_TC > 0) pCallback_TC(ctx);
		}
	}

	// Underrun error
	if ((trigger & I2S_FLAG_UDR) == I2S_FLAG_UDR) {
		// Disable TXP and ERR interrupts
		hi2s->IER &= ~(I2S_FLAG_TXP | I2S_FLAG_UDR);
		// Clear underrun flag
		hi2s->IFCR |= I2S_FLAG_UDR;
		// Update status
		i2s_ctx->status = I2S_STATUS_READY;
		// Execute error callback if set
	    if (ctx->callback_TE > 0) pCallback_TE(ctx);
		return BSP_ERROR;
	}

	return BSP_OK;
}


