/*******************************************************************
 * MiniConsole V3 - Board Support Package - Serial Connection
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 *******************************************************************/

#include "BSP_Serial.h"

BUF_RAM	SERIAL_HandleTypeDef	BSP_hserial = {0};
		TxRxContext_TypeDef		BSP_hserial_ctx = {0};


// Overriding syscall __io_putchar(int ch) for redirecting printf output to USART2
int __io_putchar(int ch) {
	uint8_t * pData = (uint8_t *)&ch;
	BSP_STM32_UART_Transmit(USART2, pData, 1);
	return ch;
}


uint8_t BSP_Serial_Init(void) {
	// Configure USART2 interface
	if (BSP_STM32_UART_Init(USART2, 115200, UART_WORDLENGTH_8B, UART_STOPBITS_1, UART_PARITY_NONE)) return BSP_ERROR;

	// Linking DMA Stream to USART2 RX context
	if (BSP_STM32_DMA_Init(DMA1_Stream3, DMA_PERIPH_TO_MEMORY, DMA_PINC_DISABLE, DMA_MINC_ENABLE, DMA_PDATAALIGN_BYTE, DMA_MDATAALIGN_BYTE, DMA_CIRCULAR, DMA_PRIORITY_LOW)) return BSP_ERROR;
	if (BSP_STM32_DMA_CtxLink(DMA1_Stream3, &BSP_hserial_ctx, DMA_REQUEST_USART2_RX)) return BSP_ERROR;

	// Start listening on USART2 port
	BSP_STM32_UART_StartListen(USART2, &BSP_hserial_ctx, BSP_hserial.data_rx, SERIAL_BUF_SIZE);

	return BSP_OK;
}


uint8_t BSP_Serial_Transmit(uint8_t * pData, uint32_t Size) {
	if (BSP_STM32_UART_Transmit(USART2, pData, Size)) return BSP_ERROR;
	return BSP_OK;
}


uint8_t BSP_Serial_Transmit_IT(uint8_t * pData, uint32_t Size) {
	if (BSP_STM32_UART_Transmit_IT(USART2, &BSP_hserial_ctx, pData, Size)) return BSP_ERROR;
	return BSP_OK;
}


void USART2_IRQHandler(void) {
	BSP_STM32_UART_IRQHandler(USART2, &BSP_hserial_ctx);
}

