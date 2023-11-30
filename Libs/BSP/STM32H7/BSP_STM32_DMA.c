/*******************************************************************
 * MiniConsole V3 - Board Support Package - STM32 DMA
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 *******************************************************************/

#include "BSP_STM32_DMA.h"

typedef struct {
  __IO uint32_t ISR;   // DMA interrupt status register
  __IO uint32_t Reserved0;
  __IO uint32_t IFCR;  // DMA interrupt flag clear register
} DMA_Base_Registers;


uint32_t DMACTXvectors[16] = {0};


uint8_t BSP_STM32_DMA_Init(DMA_Stream_TypeDef *hdma, uint32_t dir, uint32_t pinc, uint32_t minc, uint32_t pdatalign, uint32_t mdatalign, uint32_t mode, uint32_t priority) {

	uint32_t tickstart = BSP_GetTick();

    // Disable the peripheral
    hdma->CR &= ~DMA_SxCR_EN;

    // Check if the DMA Stream is effectively disabled
    while ((hdma->CR & DMA_SxCR_EN) > 0) if ((BSP_GetTick() - tickstart) > 2) return BSP_ERROR;

    // Get the CR register value
    uint32_t registerValue = hdma->CR;

    // Clear CHSEL, MBURST, PBURST, PL, MSIZE, PSIZE, MINC, PINC, CIRC, DIR, CT and DBM bits
    registerValue &= ((uint32_t)~(DMA_SxCR_MBURST | DMA_SxCR_PBURST | DMA_SxCR_PL | DMA_SxCR_MSIZE | DMA_SxCR_PSIZE | DMA_SxCR_MINC | DMA_SxCR_PINC | DMA_SxCR_CIRC | DMA_SxCR_DIR | DMA_SxCR_CT | DMA_SxCR_DBM));

    // Prepare the DMA Stream configuration
    registerValue |=  dir | pinc | minc | pdatalign | mdatalign | mode | priority;

    // Write to DMA Stream CR register
    hdma->CR = registerValue;

    // Get the FCR register value
    registerValue = hdma->FCR;

    // Clear Direct mode and FIFO threshold bits
    registerValue &= (uint32_t)~(DMA_SxFCR_DMDIS | DMA_SxFCR_FTH);

    // Write to DMA Stream FCR
    hdma->FCR = registerValue;

	return BSP_OK;
}


uint8_t BSP_STM32_DMA_CtxLink(DMA_Stream_TypeDef *hdma, TxRxContext_TypeDef *ctx, uint32_t request) {

	ctx->dma_stream = (uint32_t)hdma;
	ctx->dma_mode = TXRX_CTX_MODE_DMA;

    // Initialize StreamBaseAddress and StreamIndex parameters
    uint32_t stream_number = (((uint32_t)((uint32_t*)hdma) & 0xFFU) - 16U) / 24U;
    static const uint8_t flagBitshiftOffset[8U] = {0U, 6U, 16U, 22U, 0U, 6U, 16U, 22U};
    ctx->dma_index = flagBitshiftOffset[stream_number & 0x7U];
    if (stream_number > 3U) ctx->dma_baseregs = (((uint32_t)((uint32_t*)hdma) & (uint32_t)(~0x3FFU)) + 4U);
    	else ctx->dma_baseregs = ((uint32_t)((uint32_t*)hdma) & (uint32_t)(~0x3FFU));

    //Clear all interrupt flags
    DMA_Base_Registers * regs_dma = (DMA_Base_Registers * )ctx->dma_baseregs;
    regs_dma->IFCR = 0x3FUL << (ctx->dma_index & 0x1FU);

    // Initialize parameters for DMAMUX channel
    if((ctx->dma_baseregs <= ((uint32_t)DMA2_Stream7)) && (ctx->dma_baseregs >= ((uint32_t)DMA2_Stream0))) stream_number += 8U;

    ctx->dma_muxchannel = ((uint32_t)(((uint32_t)DMAMUX1_Channel0) + (stream_number * 4U)));
    ctx->dma_muxchannelstatus = DMAMUX1_ChannelStatus_BASE;
    ctx->dma_muxchannelstatusmask = 1UL << (stream_number & 0x1FU);

    // Set peripheral request  to DMAMUX channel
    DMAMUX_Channel_TypeDef * dma_muxchannel = (DMAMUX_Channel_TypeDef *)ctx->dma_muxchannel;
    dma_muxchannel->CCR = (request & DMAMUX_CxCR_DMAREQ_ID);

    // Clear the DMAMUX synchro overrun flag
    DMAMUX_ChannelStatus_TypeDef * dma_muxchannelstatus = (DMAMUX_ChannelStatus_TypeDef *)ctx->dma_muxchannelstatus;
    dma_muxchannelstatus->CFR = ctx->dma_muxchannelstatusmask;

    // Assigning TxRx Context to DMA stream
    DMACTXvectors[stream_number] = (uint32_t)ctx;

	return BSP_OK;
}


uint8_t BSP_STM32_DMA_StartIT(TxRxContext_TypeDef *ctx, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength) {

	DMA_Stream_TypeDef * hdma = (DMA_Stream_TypeDef *)ctx->dma_stream;
	DMA_Base_Registers * regs_dma  = (DMA_Base_Registers *)ctx->dma_baseregs;

	// Disable the peripheral
	hdma->CR &= ~DMA_SxCR_EN;

	// Clear all interrupt flags at correct offset within the register
	regs_dma->IFCR = 0x3FUL << (ctx->dma_index & 0x1FU);

	// Clear DBM bit
	hdma->CR &= (uint32_t)(~DMA_SxCR_DBM);

	// Configure DMA Stream data length
	hdma->NDTR = DataLength;

	if((hdma->CR & DMA_SxCR_DIR) == DMA_MEMORY_TO_PERIPH) {
		// Memory to Peripheral
		// Configure DMA Stream source address
		hdma->PAR = DstAddress;

		// Configure DMA Stream destination address
		hdma->M0AR = SrcAddress;
	} else {
		// Peripheral to Memory
		// Configure DMA Stream destination address
	    hdma->PAR = SrcAddress;

	    // Configure DMA Stream source address
	    hdma->M0AR = DstAddress;
	}

	// Enable Common interrupts
	MODIFY_REG(hdma->CR, (DMA_IT_TC | DMA_IT_TE | DMA_IT_DME | DMA_IT_HT), (DMA_IT_TC | DMA_IT_TE | DMA_IT_DME | DMA_IT_HT));

	// Enable DMA
	hdma->CR |=  DMA_SxCR_EN;

	return BSP_OK;
}


uint8_t BSP_STM32_DMA_Abort(TxRxContext_TypeDef *ctx) {

	DMA_Stream_TypeDef * hdma = (DMA_Stream_TypeDef *)ctx->dma_stream;
	DMA_Base_Registers * regs_dma = (DMA_Base_Registers *)ctx->dma_baseregs;
	DMAMUX_ChannelStatus_TypeDef * dma_muxchannelstatus = (DMAMUX_ChannelStatus_TypeDef *)ctx->dma_muxchannelstatus;

	// Disable DMA All Interrupts
    hdma->CR  &= ~(DMA_IT_TC | DMA_IT_TE | DMA_IT_DME | DMA_IT_HT);
    hdma->FCR &= ~(DMA_IT_FE);

    // Disable the stream
    hdma->CR &=  ~DMA_SxCR_EN;

    // Wait for stream disable
    while (hdma->CR & DMA_SxCR_EN) {};

    // Clear all interrupt flags at correct offset within the register
    regs_dma->IFCR = 0x3FUL << (ctx->dma_index & 0x1FU);

    // Clear the DMAMUX synchro overrun flag
    dma_muxchannelstatus->CFR = ctx->dma_muxchannelstatusmask;

	return BSP_OK;
}

uint32_t BSP_STM32_DMA_GetCounter(DMA_Stream_TypeDef *hdma) {
	return hdma->NDTR;
}




// ************* DMA INT handlers *************

void DMA_Stream_IRQHandler(TxRxContext_TypeDef * ctx) {

	// Callback management
	void (* pCallback_TE)(TxRxContext_TypeDef * ctx);
	void (* pCallback_HT)(TxRxContext_TypeDef * ctx);
	void (* pCallback_TC)(TxRxContext_TypeDef * ctx);

	pCallback_TE = (void *)ctx->callback_TE;
	pCallback_HT = (void *)ctx->callback_HT;
	pCallback_TC = (void *)ctx->callback_TC;

	// Accessing DMA registers
	DMA_Base_Registers  *regs_dma  = (DMA_Base_Registers *)ctx->dma_baseregs;
	DMA_Stream_TypeDef	*regs_dma_stream = (DMA_Stream_TypeDef *)ctx->dma_stream;

	// Transfer Error Interrupt management
	if (((regs_dma_stream->CR & DMA_IT_TE) != 0) && ((regs_dma->ISR & (DMA_FLAG_TEIF0_4 << (ctx->dma_index & 0x1FU))) != 0)) {
        // Disable the transfer error interrupt
        regs_dma_stream->CR  &= ~(DMA_IT_TE);
        // Clear the transfer error flag
        regs_dma->IFCR = DMA_FLAG_TEIF0_4 << (ctx->dma_index & 0x1FU);
	    // Disable DMA
	    regs_dma_stream->CR &= ~DMA_SxCR_EN;

	    // Execute error callback if set
	    if (ctx->callback_TE > 0) pCallback_TE(ctx);
	}

	// FIFO Error Interrupt management
	if (((regs_dma_stream->FCR & DMA_IT_FE) != 0) && ((regs_dma->ISR & (DMA_FLAG_FEIF0_4 << (ctx->dma_index & 0x1FU))) != 0)) {
        // Disable the FIFO error interrupt
        regs_dma_stream->FCR &= ~(DMA_IT_FE);
		// Clear the FIFO error flag
	    regs_dma->IFCR = DMA_FLAG_FEIF0_4 << (ctx->dma_index & 0x1FU);
	    // Disable DMA
	    regs_dma_stream->CR &= ~DMA_SxCR_EN;

	    // Execute error callback if set
	    if (ctx->callback_TE > 0) pCallback_TE(ctx);
	}

	// Direct Mode Error Interrupt management
	if (((regs_dma_stream->CR & DMA_IT_DME) != 0) && ((regs_dma->ISR & (DMA_FLAG_DMEIF0_4 << (ctx->dma_index & 0x1FU))) != 0)) {
        // Clear the direct mode error flag
        regs_dma->IFCR = DMA_FLAG_DMEIF0_4 << (ctx->dma_index & 0x1FU);

        // Execute error callback if set
        if (ctx->callback_TE > 0) pCallback_TE(ctx);
	}

	// Half Transfer Complete Interrupt management
	if (((regs_dma_stream->CR & DMA_IT_HT) != 0) && ((regs_dma->ISR & (DMA_FLAG_HTIF0_4 << (ctx->dma_index & 0x1FU))) != 0)) {
        // Clear the half transfer complete flag
        regs_dma->IFCR = DMA_FLAG_HTIF0_4 << (ctx->dma_index & 0x1FU);
        // Disable the half transfer interrupt if the DMA mode is not CIRCULAR
        if ((regs_dma_stream->CR & DMA_SxCR_CIRC) == 0U) regs_dma_stream->CR  &= ~(DMA_IT_HT);

        // Execute half transfer callback if set
        if (ctx->callback_HT > 0) pCallback_HT(ctx);

	}

	// Transfer Complete Interrupt management
	if (((regs_dma_stream->CR & DMA_IT_TC) != 0) && ((regs_dma->ISR & (DMA_FLAG_TCIF0_4 << (ctx->dma_index & 0x1FU))) != 0)) {
		// Clear the transfer complete flag
		regs_dma->IFCR = DMA_FLAG_TCIF0_4 << (ctx->dma_index & 0x1FU);
		// Disable the transfer complete interrupt if the DMA mode is not CIRCULAR
		if ((regs_dma_stream->CR & DMA_SxCR_CIRC) == 0U) regs_dma_stream->CR  &= ~(DMA_IT_TC);

        // Execute transfer completed callback if set
        if (ctx->callback_TC > 0) pCallback_TC(ctx);
	}

}

void DMA1_Stream0_IRQHandler() { if (DMACTXvectors[0x00]>0) DMA_Stream_IRQHandler((TxRxContext_TypeDef *)(DMACTXvectors[0x00])); }
void DMA1_Stream1_IRQHandler() { if (DMACTXvectors[0x01]>0) DMA_Stream_IRQHandler((TxRxContext_TypeDef *)(DMACTXvectors[0x01])); }
void DMA1_Stream2_IRQHandler() { if (DMACTXvectors[0x02]>0) DMA_Stream_IRQHandler((TxRxContext_TypeDef *)(DMACTXvectors[0x02])); }
void DMA1_Stream3_IRQHandler() { if (DMACTXvectors[0x03]>0) DMA_Stream_IRQHandler((TxRxContext_TypeDef *)(DMACTXvectors[0x03])); }
void DMA1_Stream4_IRQHandler() { if (DMACTXvectors[0x04]>0) DMA_Stream_IRQHandler((TxRxContext_TypeDef *)(DMACTXvectors[0x04])); }
void DMA1_Stream5_IRQHandler() { if (DMACTXvectors[0x05]>0) DMA_Stream_IRQHandler((TxRxContext_TypeDef *)(DMACTXvectors[0x05])); }
void DMA1_Stream6_IRQHandler() { if (DMACTXvectors[0x06]>0) DMA_Stream_IRQHandler((TxRxContext_TypeDef *)(DMACTXvectors[0x06])); }
void DMA1_Stream7_IRQHandler() { if (DMACTXvectors[0x07]>0) DMA_Stream_IRQHandler((TxRxContext_TypeDef *)(DMACTXvectors[0x07])); }

void DMA2_Stream0_IRQHandler() { if (DMACTXvectors[0x08]>0) DMA_Stream_IRQHandler((TxRxContext_TypeDef *)(DMACTXvectors[0x08])); }
void DMA2_Stream1_IRQHandler() { if (DMACTXvectors[0x09]>0) DMA_Stream_IRQHandler((TxRxContext_TypeDef *)(DMACTXvectors[0x09])); }
void DMA2_Stream2_IRQHandler() { if (DMACTXvectors[0x0A]>0) DMA_Stream_IRQHandler((TxRxContext_TypeDef *)(DMACTXvectors[0x0A])); }
void DMA2_Stream3_IRQHandler() { if (DMACTXvectors[0x0B]>0) DMA_Stream_IRQHandler((TxRxContext_TypeDef *)(DMACTXvectors[0x0B])); }
void DMA2_Stream4_IRQHandler() { if (DMACTXvectors[0x0C]>0) DMA_Stream_IRQHandler((TxRxContext_TypeDef *)(DMACTXvectors[0x0C])); }
void DMA2_Stream5_IRQHandler() { if (DMACTXvectors[0x0D]>0) DMA_Stream_IRQHandler((TxRxContext_TypeDef *)(DMACTXvectors[0x0D])); }
void DMA2_Stream6_IRQHandler() { if (DMACTXvectors[0x0E]>0) DMA_Stream_IRQHandler((TxRxContext_TypeDef *)(DMACTXvectors[0x0E])); }
void DMA2_Stream7_IRQHandler() { if (DMACTXvectors[0x0F]>0) DMA_Stream_IRQHandler((TxRxContext_TypeDef *)(DMACTXvectors[0x0F])); }






