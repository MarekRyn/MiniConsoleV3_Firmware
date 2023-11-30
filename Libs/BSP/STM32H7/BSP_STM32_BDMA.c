/*******************************************************************
 * MiniConsole V3 - Board Support Package - STM32 BDMA
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 *******************************************************************/

#include "BSP_STM32_BDMA.h"

typedef struct
{
  __IO uint32_t ISR;   // BDMA interrupt status register
  __IO uint32_t IFCR;  // BDMA interrupt flag clear register
} BDMA_Base_Registers;

uint32_t BDMACTXvectors[8] = {0};

uint8_t BSP_BDMA_Init(BDMA_Channel_TypeDef *hbdma, uint32_t dir, uint32_t pinc, uint32_t minc, uint32_t pdatalign, uint32_t mdatalign, uint32_t mode, uint32_t priority) {

	uint32_t tickstart = BSP_GetTick();

    // Disable the peripheral
    hbdma->CCR &= ~BDMA_CCR_EN;

    // Check if the BDMA Channel is effectively disabled
    while ((hbdma->CCR & BDMA_CCR_EN) > 0) if ((BSP_GetTick() - tickstart) > 2) return BSP_ERROR;

	// Get the CR register value
	uint32_t registerValue = hbdma->CCR;

	// Clear PL, MSIZE, PSIZE, MINC, PINC, CIRC, DIR, MEM2MEM, DBM and CT bits
	registerValue &= ((uint32_t)~(BDMA_CCR_PL | BDMA_CCR_MSIZE | BDMA_CCR_PSIZE | BDMA_CCR_MINC | BDMA_CCR_PINC | BDMA_CCR_CIRC | BDMA_CCR_DIR | BDMA_CCR_MEM2MEM | BDMA_CCR_DBM | BDMA_CCR_CT));

	// Prepare the BDMA Channel configuration */
	    registerValue |=  dir | pinc | minc | pdatalign | mdatalign | mode | priority;

	// Write to BDMA Channel CR register
	hbdma->CCR = registerValue;

	return BSP_OK;
}


uint8_t BSP_BDMA_CtxLink(BDMA_Channel_TypeDef *hbdma, TxRxContext_TypeDef *ctx, uint32_t request) {

	ctx->dma_stream = (uint32_t)hbdma;
	ctx->dma_mode = TXRX_CTX_MODE_BDMA;

	// Initialize BaseAddress and Index parameters
	uint32_t channel_number = (((uint32_t)(hbdma) - (uint32_t)BDMA_Channel0) / ((uint32_t)BDMA_Channel1 - (uint32_t)BDMA_Channel0));
	ctx->dma_index = channel_number << 2U;
	BDMA_Base_Registers * dma_baseregs = (BDMA_Base_Registers *)((uint32_t)(hbdma) & (uint32_t)(~0xFFU));
	ctx->dma_baseregs = (uint32_t)dma_baseregs;

	// Clear all interrupt flags
	dma_baseregs->IFCR = ((BDMA_IFCR_CGIF0) << (ctx->dma_index & 0x1FU));

	// Initialize parameters for DMAMUX channel
    // BDMA Channels are connected to DMAMUX2 channels
	ctx->dma_muxchannel = ((uint32_t)(((uint32_t)DMAMUX2_Channel0) + (channel_number * 4U)));
	ctx->dma_muxchannelstatus = DMAMUX2_ChannelStatus_BASE;
	ctx->dma_muxchannelstatusmask = 1UL << (channel_number & 0x1FU);

	DMAMUX_Channel_TypeDef * dma_muxchannel = (DMAMUX_Channel_TypeDef *)ctx->dma_muxchannel;
	dma_muxchannel->CCR = (request & DMAMUX_CxCR_DMAREQ_ID);

	// Clear the DMAMUX synchro overrun flag
	DMAMUX_ChannelStatus_TypeDef * dma_muxchannelstatus = (DMAMUX_ChannelStatus_TypeDef *)ctx->dma_muxchannelstatus;
	dma_muxchannelstatus->CFR = ctx->dma_muxchannelstatusmask;

	// Assigning TxRx Context to BDMA channel
	BDMACTXvectors[channel_number] = (uint32_t)ctx;

	return BSP_OK;
}


uint8_t BSP_BDMA_StartIT(TxRxContext_TypeDef *ctx, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength) {

	BDMA_Channel_TypeDef * hbdma = (BDMA_Channel_TypeDef *)ctx->dma_stream;
	BDMA_Base_Registers * regs_bdma  = (BDMA_Base_Registers *)ctx->dma_baseregs;

	// Disable the peripheral
	hbdma->CCR &= ~BDMA_CCR_EN;

	// Clear all flags
	regs_bdma->IFCR = (BDMA_ISR_GIF0) << (ctx->dma_index & 0x1FU);

	// Configure DMA Stream data length
	hbdma->CNDTR = DataLength;

	if((hbdma->CCR & BDMA_CCR_DIR) == BDMA_MEMORY_TO_PERIPH) {
			// Memory to Peripheral
			// Configure DMA Stream source address
			hbdma->CPAR = DstAddress;

			// Configure DMA Stream destination address
			hbdma->CM0AR = SrcAddress;
		} else {
			// Peripheral to Memory
			// Configure DMA Stream destination address
		    hbdma->CPAR = SrcAddress;

		    // Configure DMA Stream source address
		    hbdma->CM0AR = DstAddress;
		}

	// Enable Common interrupts
	MODIFY_REG(hbdma->CCR, (BDMA_CCR_TCIE | BDMA_CCR_HTIE | BDMA_CCR_TEIE), (BDMA_CCR_TCIE | BDMA_CCR_HTIE | BDMA_CCR_TEIE));

	// Enable BDMA
	hbdma->CCR |=  BDMA_CCR_EN;

	return BSP_OK;
}


void BDMA_Stream_IRQHandler(TxRxContext_TypeDef * ctx) {
	BDMA_Base_Registers  *regs_bdma  = (BDMA_Base_Registers *)ctx->dma_baseregs;
	BDMA_Channel_TypeDef *regs_bdma_channel = (BDMA_Channel_TypeDef *)ctx->dma_stream;
	uint32_t ccr_reg = regs_bdma_channel->CCR;

	// Half Transfer Complete Interrupt management ******************************
    if (((regs_bdma->ISR & (BDMA_FLAG_HT0 << (ctx->dma_index & 0x1FU))) != 0U) && ((ccr_reg & BDMA_CCR_HTIE) != 0U)) {

      // Clear the half transfer complete flag
      regs_bdma->IFCR = (BDMA_ISR_HTIF0 << (ctx->dma_index & 0x1FU));

      // Disable the half transfer interrupt if the BDMA mode is not CIRCULAR
      if((ccr_reg & BDMA_CCR_CIRC) == 0U) regs_bdma_channel->CCR &= ~(BDMA_CCR_HTIE);

      // TODO: HALF TRANSFER CALLBACK

    }

	// Transfer Complete Interrupt management ***********************************
    if (((regs_bdma->ISR & (BDMA_FLAG_TC0 << (ctx->dma_index & 0x1FU))) != 0U) && ((ccr_reg & BDMA_CCR_TCIE) != 0U)) {

      // Clear the transfer complete flag
      regs_bdma->IFCR = (BDMA_ISR_TCIF0 << (ctx->dma_index & 0x1FU));

      // Disable the transfer complete interrupt if the BDMA mode is not CIRCULAR
      if((ccr_reg & BDMA_CCR_CIRC) == 0U) regs_bdma_channel->CCR &= ~(BDMA_CCR_TCIE);

      // TODO: HALF TRANSFER CALLBACK

    }

	// Transfer Error Interrupt management **************************************
    if (((regs_bdma->ISR & (BDMA_FLAG_TE0 << (ctx->dma_index & 0x1FU))) != 0U) && ((ccr_reg & BDMA_CCR_TEIE) != 0U)) {

    	// Disable all interrupts
    	regs_bdma_channel->CCR &= ~(BDMA_CCR_TCIE | BDMA_CCR_TEIE | BDMA_CCR_HTIE );

    	// Clear all flags
    	regs_bdma->IFCR = (BDMA_ISR_GIF0) << (ctx->dma_index & 0x1FU);
    }


}


void BDMA_Channel0_IRQHandler() {
	if (BDMACTXvectors[0x00]>0) BDMA_Stream_IRQHandler((TxRxContext_TypeDef *)(BDMACTXvectors[0x00]));
}
void BDMA_Channel1_IRQHandler() { if (BDMACTXvectors[0x01]>0) BDMA_Stream_IRQHandler((TxRxContext_TypeDef *)(BDMACTXvectors[0x01])); }
void BDMA_Channel2_IRQHandler() { if (BDMACTXvectors[0x02]>0) BDMA_Stream_IRQHandler((TxRxContext_TypeDef *)(BDMACTXvectors[0x02])); }
void BDMA_Channel3_IRQHandler() { if (BDMACTXvectors[0x03]>0) BDMA_Stream_IRQHandler((TxRxContext_TypeDef *)(BDMACTXvectors[0x03])); }
void BDMA_Channel4_IRQHandler() { if (BDMACTXvectors[0x04]>0) BDMA_Stream_IRQHandler((TxRxContext_TypeDef *)(BDMACTXvectors[0x04])); }
void BDMA_Channel5_IRQHandler() { if (BDMACTXvectors[0x05]>0) BDMA_Stream_IRQHandler((TxRxContext_TypeDef *)(BDMACTXvectors[0x05])); }
void BDMA_Channel6_IRQHandler() { if (BDMACTXvectors[0x06]>0) BDMA_Stream_IRQHandler((TxRxContext_TypeDef *)(BDMACTXvectors[0x06])); }
void BDMA_Channel7_IRQHandler() { if (BDMACTXvectors[0x07]>0) BDMA_Stream_IRQHandler((TxRxContext_TypeDef *)(BDMACTXvectors[0x07])); }

