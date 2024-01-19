/*******************************************************************
 * MiniConsole V3 - Board Support Package - STM32 USART
 *
 * Author: Marek Ryn
 * Version: 1.0
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 * - 1.0	- Initial version
 *******************************************************************/

#include "BSP_STM32_UART.h"

const uint16_t UARTPrescTable[12] = {1U, 2U, 4U, 6U, 8U, 10U, 12U, 16U, 32U, 64U, 128U, 256U};

static void _UART_DMA_Callback_TC(void) {

}



uint8_t BSP_STM32_UART_Init(USART_TypeDef *huart, uint32_t BaudRate, uint32_t WordLength, uint32_t StopBits, uint32_t Parity) {
	uint32_t tmpreg = 0;

	// Disable UART
	huart->CR1 &= ~USART_CR1_UE;

	// USART CR1 Configuration
	tmpreg = (uint32_t)WordLength | Parity | UART_MODE_TX_RX | UART_OVERSAMPLING_16 | UART_FIFOMODE_ENABLE;
	MODIFY_REG(huart->CR1, USART_CR1_FIELDS, tmpreg);

	// USART CR2 Configuration
	MODIFY_REG(huart->CR2, USART_CR2_STOP, StopBits);

	// USART CR3 Configuration
	tmpreg = UART_HWCONTROL_NONE | UART_ONE_BIT_SAMPLE_DISABLE | UART_RXFIFO_THRESHOLD_1_2 | UART_TXFIFO_THRESHOLD_1_2;
	MODIFY_REG(huart->CR3, USART_CR3_FIELDS, tmpreg);

	// Configuring prescaler
	MODIFY_REG(huart->PRESC, USART_PRESC_PRESCALER, UART_PRESCALER_DIV1);

	// USART BRR Configuration
	uint32_t pclk = 120000000; // 120MHz from PCLK1
	uint32_t usartdiv = ((pclk/UARTPrescTable[UART_PRESCALER_DIV1]) + (BaudRate/2U)) / (BaudRate);

	if (usartdiv < UART_BRR_MIN) return BSP_ERROR;
	if (usartdiv > UART_BRR_MAX) return BSP_ERROR;

	huart->BRR = (uint16_t)usartdiv;

	// Clear bits required for asynchronous mode
	CLEAR_BIT(huart->CR2, (USART_CR2_LINEN | USART_CR2_CLKEN));
	CLEAR_BIT(huart->CR3, (USART_CR3_SCEN | USART_CR3_HDSEL | USART_CR3_IREN));

	// Enable UART
	huart->CR1 |= USART_CR1_UE;

	return BSP_OK;
}

uint8_t BSP_STM32_UART_StartListen(USART_TypeDef *huart, TxRxContext_TypeDef * ctx, uint8_t * pBuf, uint32_t bufSize) {

	UARTContext_TypeDef * uart_ctx = (UARTContext_TypeDef *)ctx->ctxmem;

	// Configuring context
	uart_ctx->indexRX0 = 0;
	uart_ctx->pDataRX = pBuf;
	uart_ctx->sizeRX = bufSize;

	// Start DMA
	ctx->callback_TC = (uint32_t)_UART_DMA_Callback_TC;
	BSP_STM32_DMA_StartIT(ctx, (uint32_t)&huart->RDR, (uint32_t)pBuf, bufSize);

	// Enabling IDLE interrupt
	SET_BIT(huart->ICR, USART_ICR_IDLECF);
	SET_BIT(huart->CR1, USART_CR1_IDLEIE);

	// Start DMA receiving
	SET_BIT(huart->CR3, USART_CR3_DMAR);

	return BSP_OK;
}


uint8_t BSP_STM32_UART_Transmit(USART_TypeDef *huart, uint8_t * pData, uint32_t Size) {

	uint8_t * pCurrentData = pData;

	// Disabling transmit interrupts (TXFTIE, TCIE)
	CLEAR_BIT(huart->CR3, USART_CR3_TXFTIE);
	CLEAR_BIT(huart->CR1, USART_CR1_TCIE);

	// Data transfer
	while (Size > 0) {
		if (huart->ISR & USART_ISR_TXE_TXFNF) {
			huart->TDR = *pCurrentData;
			pCurrentData++;
			Size--;
		}
	}

	return BSP_OK;
}

uint8_t BSP_STM32_UART_Transmit_IT(USART_TypeDef *huart, TxRxContext_TypeDef * ctx, uint8_t * pData, uint32_t Size) {

	UARTContext_TypeDef * uart_ctx = (UARTContext_TypeDef *)ctx->ctxmem;

	// Configuring context
	uart_ctx->pDataTX = pData;
	uart_ctx->sizeTX = Size;
	uart_ctx->indexTX = 0;

	// Enabling interrupt (TXFTIE)
	SET_BIT(huart->CR3, USART_CR3_TXFTIE);

	return BSP_OK;
}


uint8_t BSP_STM32_UART_IRQHandler(USART_TypeDef *huart, TxRxContext_TypeDef *ctx) {

	UARTContext_TypeDef * uart_ctx = (UARTContext_TypeDef *)ctx->ctxmem;

	// Handling TXFTIE interrupt (FIFO buffer not full)
	if (huart->CR3 & USART_CR3_TXFTIE) {

		while ((huart->ISR & USART_ISR_TXE_TXFNF) && (uart_ctx->indexTX < uart_ctx->sizeTX)) {
			huart->TDR = *uart_ctx->pDataTX;
			uart_ctx->pDataTX++;
			uart_ctx->indexTX++;
		}

		// Switch off TXFTIE interrupt when no more data is available
		if (uart_ctx->indexTX == uart_ctx->sizeTX) {
			CLEAR_BIT(huart->CR3, USART_CR3_TXFTIE);
			SET_BIT(huart->CR1, USART_CR1_TCIE); // Enabling TCIE interrupt
		}

	}

	// Handling TCIE interrupt
	if ((huart->CR1 & USART_CR1_TCIE) && (huart->ISR & USART_ISR_TC)) {
		CLEAR_BIT(huart->CR1, USART_CR1_TCIE);
		SET_BIT(huart->ICR, USART_ICR_TCCF);
	}

	// Handling IDLE interrupt
	if ((huart->CR1 & USART_CR1_IDLEIE) && (huart->ISR & USART_ISR_IDLE)) {
		SET_BIT(huart->ICR, USART_ICR_IDLECF);
		/* #define __HAL_DMA_GET_COUNTER(__HANDLE__) ((IS_DMA_STREAM_INSTANCE((__HANDLE__)->Instance))? \
                                           (((DMA_Stream_TypeDef *)(__HANDLE__)->Instance)->NDTR) :\
                                           (((BDMA_Channel_TypeDef *)(__HANDLE__)->Instance)->CNDTR)) */
		uart_ctx->indexRX0 = uart_ctx->indexRX1;
		uart_ctx->indexRX1 = (SERIAL_BUF_SIZE - BSP_STM32_DMA_GetCounter((DMA_Stream_TypeDef *)ctx->dma_stream));
		if (uart_ctx->indexRX1 >= uart_ctx->indexRX0) {
			uart_ctx->sizeRX = uart_ctx->indexRX1 - uart_ctx->indexRX0;
		} else {
			uart_ctx->sizeRX = (uart_ctx->indexRX1 + SERIAL_BUF_SIZE) - uart_ctx->indexRX0;
		}
	}

	return BSP_OK;
}
