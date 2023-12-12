/*******************************************************************
 * MiniConsole V3 - Board Support Package - STM32 USART
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 *******************************************************************/

#ifndef STM32H7_BSP_STM32_UART_H_
#define STM32H7_BSP_STM32_UART_H_

#include "BSP_Common.h"
#include "BSP_STM32.h"


#define USART_CR1_FIELDS  				((uint32_t)(USART_CR1_M | USART_CR1_PCE | USART_CR1_PS | USART_CR1_TE | USART_CR1_RE | USART_CR1_OVER8 | USART_CR1_FIFOEN)) // UART or USART CR1 fields of parameters set by UART_SetConfig API
#define USART_CR3_FIELDS  				((uint32_t)(USART_CR3_RTSE | USART_CR3_CTSE | USART_CR3_ONEBIT | USART_CR3_TXFTCFG | USART_CR3_RXFTCFG)) // UART or USART CR3 fields of parameters set by UART_SetConfig API
#define UART_BRR_MIN    				0x10U        							// UART BRR minimum authorized value
#define UART_BRR_MAX    				0x0000FFFFU  							// UART BRR maximum authorized value

#define UART_FIFOMODE_DISABLE  			0x00000000U       						// FIFO mode disable
#define UART_FIFOMODE_ENABLE    		USART_CR1_FIFOEN  						// FIFO mode enable

#define UART_WORDLENGTH_7B          USART_CR1_M1   /*!< 7-bit long UART frame */
#define UART_WORDLENGTH_8B          0x00000000U    /*!< 8-bit long UART frame */
#define UART_WORDLENGTH_9B          USART_CR1_M0   /*!< 9-bit long UART frame */

#define UART_STOPBITS_0_5   			USART_CR2_STOP_0                    	// UART frame with 0.5 stop bit
#define UART_STOPBITS_1         		0x00000000U                         	// UART frame with 1 stop bit
#define UART_STOPBITS_1_5       		(USART_CR2_STOP_0 | USART_CR2_STOP_1) 	// UART frame with 1.5 stop bits
#define UART_STOPBITS_2         		USART_CR2_STOP_1                     	// UART frame with 2 stop bits

#define UART_PARITY_NONE       			0x00000000U                        		// No parity
#define UART_PARITY_EVEN        		USART_CR1_PCE                      		// Even parity
#define UART_PARITY_ODD         		(USART_CR1_PCE | USART_CR1_PS)     		// Odd parity

#define UART_HWCONTROL_NONE     		0x00000000U                          	// No hardware control
#define UART_HWCONTROL_RTS      		USART_CR3_RTSE                       	// Request To Send
#define UART_HWCONTROL_CTS      		USART_CR3_CTSE                       	// Clear To Send
#define UART_HWCONTROL_RTS_CTS  		(USART_CR3_RTSE | USART_CR3_CTSE)    	// Request and Clear To Send

#define UART_MODE_RX            		USART_CR1_RE                    		// RX mode
#define UART_MODE_TX            		USART_CR1_TE                    		// TX mode
#define UART_MODE_TX_RX         		(USART_CR1_TE |USART_CR1_RE)    		// RX and TX mode

#define UART_OVERSAMPLING_16    		0x00000000U         					// Oversampling by 16
#define UART_OVERSAMPLING_8     		USART_CR1_OVER8     					// Oversampling by 8

#define UART_ONE_BIT_SAMPLE_DISABLE		0x00000000U         					// One-bit sampling disable
#define UART_ONE_BIT_SAMPLE_ENABLE      USART_CR3_ONEBIT    					// One-bit sampling enable

#define UART_PRESCALER_DIV1    			0x00000000U  							// fclk_pres = fclk
#define UART_PRESCALER_DIV2    			0x00000001U 							// fclk_pres = fclk/2
#define UART_PRESCALER_DIV4    			0x00000002U  							// fclk_pres = fclk/4
#define UART_PRESCALER_DIV6    			0x00000003U  							// fclk_pres = fclk/6
#define UART_PRESCALER_DIV8    			0x00000004U  							// fclk_pres = fclk/8
#define UART_PRESCALER_DIV10   			0x00000005U  							// fclk_pres = fclk/10
#define UART_PRESCALER_DIV12   			0x00000006U  							// fclk_pres = fclk/12
#define UART_PRESCALER_DIV16   			0x00000007U  							// fclk_pres = fclk/16
#define UART_PRESCALER_DIV32   			0x00000008U  							// fclk_pres = fclk/32
#define UART_PRESCALER_DIV64   			0x00000009U  							// fclk_pres = fclk/64
#define UART_PRESCALER_DIV128  			0x0000000AU  							// fclk_pres = fclk/128
#define UART_PRESCALER_DIV256  			0x0000000BU  							// fclk_pres = fclk/256

#define UART_TXFIFO_THRESHOLD_1_8   	0x00000000U                             // TX FIFO reaches 1/8 of its depth
#define UART_TXFIFO_THRESHOLD_1_4   	USART_CR3_TXFTCFG_0                     // TX FIFO reaches 1/4 of its depth
#define UART_TXFIFO_THRESHOLD_1_2   	USART_CR3_TXFTCFG_1                     // TX FIFO reaches 1/2 of its depth
#define UART_TXFIFO_THRESHOLD_3_4   	(USART_CR3_TXFTCFG_0|USART_CR3_TXFTCFG_1) // TX FIFO reaches 3/4 of its depth
#define UART_TXFIFO_THRESHOLD_7_8   	USART_CR3_TXFTCFG_2                     // TX FIFO reaches 7/8 of its depth
#define UART_TXFIFO_THRESHOLD_8_8   	(USART_CR3_TXFTCFG_2|USART_CR3_TXFTCFG_0) // TX FIFO becomes empty

#define UART_RXFIFO_THRESHOLD_1_8   	0x00000000U                             // RX FIFO reaches 1/8 of its depth
#define UART_RXFIFO_THRESHOLD_1_4   	USART_CR3_RXFTCFG_0                     // RX FIFO reaches 1/4 of its depth
#define UART_RXFIFO_THRESHOLD_1_2   	USART_CR3_RXFTCFG_1                     // RX FIFO reaches 1/2 of its depth
#define UART_RXFIFO_THRESHOLD_3_4   	(USART_CR3_RXFTCFG_0|USART_CR3_RXFTCFG_1) // RX FIFO reaches 3/4 of its depth
#define UART_RXFIFO_THRESHOLD_7_8   	USART_CR3_RXFTCFG_2                     // RX FIFO reaches 7/8 of its depth
#define UART_RXFIFO_THRESHOLD_8_8   	(USART_CR3_RXFTCFG_2|USART_CR3_RXFTCFG_0) // RX FIFO becomes full


uint8_t BSP_STM32_UART_Init(USART_TypeDef *huart, uint32_t BaudRate, uint32_t WordLength, uint32_t StopBits, uint32_t Parity);
uint8_t BSP_STM32_UART_StartListen(USART_TypeDef *huart, TxRxContext_TypeDef * ctx, uint8_t * pBuf, uint32_t bufSize);
uint8_t BSP_STM32_UART_Transmit(USART_TypeDef *huart, uint8_t * pData, uint32_t Size);
uint8_t BSP_STM32_UART_Transmit_IT(USART_TypeDef *huart, TxRxContext_TypeDef * ctx, uint8_t * pData, uint32_t Size);
uint8_t BSP_STM32_UART_IRQHandler(USART_TypeDef *huart, TxRxContext_TypeDef *ctx);

#endif /* STM32H7_BSP_STM32_UART_H_ */
