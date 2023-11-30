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

#ifndef STM32H7_BSP_STM32_BDMA_H_
#define STM32H7_BSP_STM32_BDMA_H_

#include "BSP_Common.h"
#include "BSP_STM32.h"

/* DMAMUX2 requests */
#define BDMA_REQUEST_MEM2MEM          0U  /*!< memory to memory transfer   */
#define BDMA_REQUEST_GENERATOR0       1U  /*!< DMAMUX2 request generator 0 */
#define BDMA_REQUEST_GENERATOR1       2U  /*!< DMAMUX2 request generator 1 */
#define BDMA_REQUEST_GENERATOR2       3U  /*!< DMAMUX2 request generator 2 */
#define BDMA_REQUEST_GENERATOR3       4U  /*!< DMAMUX2 request generator 3 */
#define BDMA_REQUEST_GENERATOR4       5U  /*!< DMAMUX2 request generator 4 */
#define BDMA_REQUEST_GENERATOR5       6U  /*!< DMAMUX2 request generator 5 */
#define BDMA_REQUEST_GENERATOR6       7U  /*!< DMAMUX2 request generator 6 */
#define BDMA_REQUEST_GENERATOR7       8U  /*!< DMAMUX2 request generator 7 */
#define BDMA_REQUEST_LPUART1_RX       9U  /*!< DMAMUX2 LP_UART1_RX request */
#define BDMA_REQUEST_LPUART1_TX      10U  /*!< DMAMUX2 LP_UART1_TX request */
#define BDMA_REQUEST_SPI6_RX         11U  /*!< DMAMUX2 SPI6 RX request     */
#define BDMA_REQUEST_SPI6_TX         12U  /*!< DMAMUX2 SPI6 TX request     */
#define BDMA_REQUEST_I2C4_RX         13U  /*!< DMAMUX2 I2C4 RX request     */
#define BDMA_REQUEST_I2C4_TX         14U  /*!< DMAMUX2 I2C4 TX request     */
#if defined(SAI4)
#define BDMA_REQUEST_SAI4_A          15U  /*!< DMAMUX2 SAI4 A request      */
#define BDMA_REQUEST_SAI4_B          16U  /*!< DMAMUX2 SAI4 B request      */
#endif /* SAI4 */
#if defined(ADC3)
#define BDMA_REQUEST_ADC3            17U  /*!< DMAMUX2 ADC3 request        */
#endif /* ADC3 */
#if defined(DAC2)
#define BDMA_REQUEST_DAC2_CH1        17U  /*!< DMAMUX2 DAC2 CH1 request    */
#endif /* DAC2 */
#if defined(DFSDM2_Channel0)
#define BDMA_REQUEST_DFSDM2_FLT0     18U  /*!< DMAMUX2 DFSDM2 request      */
#endif /* DFSDM1_Channel0 */

#define BDMA_PERIPH_TO_MEMORY   	(0x0UL << 4)                	// Peripheral to memory direction
#define BDMA_MEMORY_TO_PERIPH       (0x1UL << 4)					// Memory to peripheral direction
#define BDMA_MEMORY_TO_MEMORY       (0x0UL << 4) | (0x1U << 14)		// Memory to memory direction

#define BDMA_PINC_ENABLE			(0x1UL << 6)					// Peripheral address increment enabled
#define BDMA_PINC_DISABLE			(0x0UL << 6)					// Peripheral address increment disabled

#define BDMA_MINC_ENABLE			(0x1UL << 7)					// Memory address increment enabled
#define BDMA_MINC_DISABLE			(0x0UL << 7)					// Memory address increment disabled

#define BDMA_PDATAALIGN_BYTE		(0x0UL << 8)					// Peripheral data size byte (8 bit)
#define BDMA_PDATAALIGN_HALFWORD	(0x1UL << 8)					// Peripheral data size half word (16 bit)
#define BDMA_PDATAALIGN_WORD		(0x2UL << 8)					// Peripheral data size word (32 bit)

#define BDMA_MDATAALIGN_BYTE		(0x0UL << 10)					// Memory data size byte (8 bit)
#define BDMA_MDATAALIGN_HALFWORD	(0x1UL << 10)					// Memory data size half word (16 bit)
#define BDMA_MDATAALIGN_WORD		(0x2UL << 10)					// Memory data size word (32 bit)

#define BDMA_NORMAL					(0x0UL << 5)					// BDMA normal mode
#define BDMA_CIRCULAR				(0x1UL << 5)					// BDMA circular mode

#define BDMA_PRIORITY_LOW			(0x0UL << 12)					// Priority LOW
#define BDMA_PRIORITY_MEDIUM		(0x1UL << 12)					// Priority MEDIUM
#define BDMA_PRIORITY_HIGH			(0x2UL << 12)					// Priority HIGH
#define BDMA_PRIORITY_VERY_HIGH		(0x3UL << 12)					// Priority VERY HIGH

#define BDMA_FLAG_GL0                      ((uint32_t)0x00000001)
#define BDMA_FLAG_TC0                      ((uint32_t)0x00000002)
#define BDMA_FLAG_HT0                      ((uint32_t)0x00000004)
#define BDMA_FLAG_TE0                      ((uint32_t)0x00000008)
#define BDMA_FLAG_GL1                      ((uint32_t)0x00000010)
#define BDMA_FLAG_TC1                      ((uint32_t)0x00000020)
#define BDMA_FLAG_HT1                      ((uint32_t)0x00000040)
#define BDMA_FLAG_TE1                      ((uint32_t)0x00000080)
#define BDMA_FLAG_GL2                      ((uint32_t)0x00000100)
#define BDMA_FLAG_TC2                      ((uint32_t)0x00000200)
#define BDMA_FLAG_HT2                      ((uint32_t)0x00000400)
#define BDMA_FLAG_TE2                      ((uint32_t)0x00000800)
#define BDMA_FLAG_GL3                      ((uint32_t)0x00001000)
#define BDMA_FLAG_TC3                      ((uint32_t)0x00002000)
#define BDMA_FLAG_HT3                      ((uint32_t)0x00004000)
#define BDMA_FLAG_TE3                      ((uint32_t)0x00008000)
#define BDMA_FLAG_GL4                      ((uint32_t)0x00010000)
#define BDMA_FLAG_TC4                      ((uint32_t)0x00020000)
#define BDMA_FLAG_HT4                      ((uint32_t)0x00040000)
#define BDMA_FLAG_TE4                      ((uint32_t)0x00080000)
#define BDMA_FLAG_GL5                      ((uint32_t)0x00100000)
#define BDMA_FLAG_TC5                      ((uint32_t)0x00200000)
#define BDMA_FLAG_HT5                      ((uint32_t)0x00400000)
#define BDMA_FLAG_TE5                      ((uint32_t)0x00800000)
#define BDMA_FLAG_GL6                      ((uint32_t)0x01000000)
#define BDMA_FLAG_TC6                      ((uint32_t)0x02000000)
#define BDMA_FLAG_HT6                      ((uint32_t)0x04000000)
#define BDMA_FLAG_TE6                      ((uint32_t)0x08000000)
#define BDMA_FLAG_GL7                      ((uint32_t)0x10000000)
#define BDMA_FLAG_TC7                      ((uint32_t)0x20000000)
#define BDMA_FLAG_HT7                      ((uint32_t)0x40000000)
#define BDMA_FLAG_TE7                      ((uint32_t)0x80000000)

uint8_t BSP_BDMA_Init(BDMA_Channel_TypeDef *hbdma, uint32_t dir, uint32_t pinc, uint32_t minc, uint32_t pdatalign, uint32_t mdatalign, uint32_t mode, uint32_t priority);
uint8_t BSP_BDMA_CtxLink(BDMA_Channel_TypeDef *hbdma, TxRxContext_TypeDef *ctx, uint32_t request);
uint8_t BSP_BDMA_StartIT(TxRxContext_TypeDef *ctx, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength);
void BDMA_Stream_IRQHandler(TxRxContext_TypeDef * ctx);

#endif /* STM32H7_BSP_STM32_BDMA_H_ */
