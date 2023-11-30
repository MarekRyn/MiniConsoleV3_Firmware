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

#ifndef STM32H7_BSP_STM32_DMA_H_
#define STM32H7_BSP_STM32_DMA_H_

#include "BSP_Common.h"
#include "BSP_STM32.h"

/* DMAMUX1 requests */
#define DMA_REQUEST_MEM2MEM          0U  /*!< memory to memory transfer   */

#define DMA_REQUEST_GENERATOR0       1U  /*!< DMAMUX1 request generator 0 */
#define DMA_REQUEST_GENERATOR1       2U  /*!< DMAMUX1 request generator 1 */
#define DMA_REQUEST_GENERATOR2       3U  /*!< DMAMUX1 request generator 2 */
#define DMA_REQUEST_GENERATOR3       4U  /*!< DMAMUX1 request generator 3 */
#define DMA_REQUEST_GENERATOR4       5U  /*!< DMAMUX1 request generator 4 */
#define DMA_REQUEST_GENERATOR5       6U  /*!< DMAMUX1 request generator 5 */
#define DMA_REQUEST_GENERATOR6       7U  /*!< DMAMUX1 request generator 6 */
#define DMA_REQUEST_GENERATOR7       8U  /*!< DMAMUX1 request generator 7 */

#define DMA_REQUEST_ADC1             9U  /*!< DMAMUX1 ADC1 request */
#define DMA_REQUEST_ADC2             10U /*!< DMAMUX1 ADC2 request */

#define DMA_REQUEST_TIM1_CH1         11U  /*!< DMAMUX1 TIM1 CH1 request  */
#define DMA_REQUEST_TIM1_CH2         12U  /*!< DMAMUX1 TIM1 CH2 request  */
#define DMA_REQUEST_TIM1_CH3         13U  /*!< DMAMUX1 TIM1 CH3 request  */
#define DMA_REQUEST_TIM1_CH4         14U  /*!< DMAMUX1 TIM1 CH4 request  */
#define DMA_REQUEST_TIM1_UP          15U  /*!< DMAMUX1 TIM1 UP request   */
#define DMA_REQUEST_TIM1_TRIG        16U  /*!< DMAMUX1 TIM1 TRIG request */
#define DMA_REQUEST_TIM1_COM         17U  /*!< DMAMUX1 TIM1 COM request  */

#define DMA_REQUEST_TIM2_CH1         18U  /*!< DMAMUX1 TIM2 CH1 request  */
#define DMA_REQUEST_TIM2_CH2         19U  /*!< DMAMUX1 TIM2 CH2 request  */
#define DMA_REQUEST_TIM2_CH3         20U  /*!< DMAMUX1 TIM2 CH3 request  */
#define DMA_REQUEST_TIM2_CH4         21U  /*!< DMAMUX1 TIM2 CH4 request  */
#define DMA_REQUEST_TIM2_UP          22U  /*!< DMAMUX1 TIM2 UP request   */

#define DMA_REQUEST_TIM3_CH1         23U  /*!< DMAMUX1 TIM3 CH1 request  */
#define DMA_REQUEST_TIM3_CH2         24U  /*!< DMAMUX1 TIM3 CH2 request  */
#define DMA_REQUEST_TIM3_CH3         25U  /*!< DMAMUX1 TIM3 CH3 request  */
#define DMA_REQUEST_TIM3_CH4         26U  /*!< DMAMUX1 TIM3 CH4 request  */
#define DMA_REQUEST_TIM3_UP          27U  /*!< DMAMUX1 TIM3 UP request   */
#define DMA_REQUEST_TIM3_TRIG        28U  /*!< DMAMUX1 TIM3 TRIG request */

#define DMA_REQUEST_TIM4_CH1         29U  /*!< DMAMUX1 TIM4 CH1 request  */
#define DMA_REQUEST_TIM4_CH2         30U  /*!< DMAMUX1 TIM4 CH2 request  */
#define DMA_REQUEST_TIM4_CH3         31U  /*!< DMAMUX1 TIM4 CH3 request  */
#define DMA_REQUEST_TIM4_UP          32U  /*!< DMAMUX1 TIM4 UP request   */

#define DMA_REQUEST_I2C1_RX          33U  /*!< DMAMUX1 I2C1 RX request   */
#define DMA_REQUEST_I2C1_TX          34U  /*!< DMAMUX1 I2C1 TX request   */
#define DMA_REQUEST_I2C2_RX          35U  /*!< DMAMUX1 I2C2 RX request   */
#define DMA_REQUEST_I2C2_TX          36U  /*!< DMAMUX1 I2C2 TX request   */

#define DMA_REQUEST_SPI1_RX          37U  /*!< DMAMUX1 SPI1 RX request   */
#define DMA_REQUEST_SPI1_TX          38U  /*!< DMAMUX1 SPI1 TX request   */
#define DMA_REQUEST_SPI2_RX          39U  /*!< DMAMUX1 SPI2 RX request   */
#define DMA_REQUEST_SPI2_TX          40U  /*!< DMAMUX1 SPI2 TX request   */

#define DMA_REQUEST_USART1_RX        41U  /*!< DMAMUX1 USART1 RX request */
#define DMA_REQUEST_USART1_TX        42U  /*!< DMAMUX1 USART1 TX request */
#define DMA_REQUEST_USART2_RX        43U  /*!< DMAMUX1 USART2 RX request */
#define DMA_REQUEST_USART2_TX        44U  /*!< DMAMUX1 USART2 TX request */
#define DMA_REQUEST_USART3_RX        45U  /*!< DMAMUX1 USART3 RX request */
#define DMA_REQUEST_USART3_TX        46U  /*!< DMAMUX1 USART3 TX request */

#define DMA_REQUEST_TIM8_CH1         47U  /*!< DMAMUX1 TIM8 CH1 request  */
#define DMA_REQUEST_TIM8_CH2         48U  /*!< DMAMUX1 TIM8 CH2 request  */
#define DMA_REQUEST_TIM8_CH3         49U  /*!< DMAMUX1 TIM8 CH3 request  */
#define DMA_REQUEST_TIM8_CH4         50U  /*!< DMAMUX1 TIM8 CH4 request  */
#define DMA_REQUEST_TIM8_UP          51U  /*!< DMAMUX1 TIM8 UP request   */
#define DMA_REQUEST_TIM8_TRIG        52U  /*!< DMAMUX1 TIM8 TRIG request */
#define DMA_REQUEST_TIM8_COM         53U  /*!< DMAMUX1 TIM8 COM request  */

#define DMA_REQUEST_TIM5_CH1         55U  /*!< DMAMUX1 TIM5 CH1 request  */
#define DMA_REQUEST_TIM5_CH2         56U  /*!< DMAMUX1 TIM5 CH2 request  */
#define DMA_REQUEST_TIM5_CH3         57U  /*!< DMAMUX1 TIM5 CH3 request  */
#define DMA_REQUEST_TIM5_CH4         58U  /*!< DMAMUX1 TIM5 CH4 request  */
#define DMA_REQUEST_TIM5_UP          59U  /*!< DMAMUX1 TIM5 UP request   */
#define DMA_REQUEST_TIM5_TRIG        60U  /*!< DMAMUX1 TIM5 TRIG request */

#define DMA_REQUEST_SPI3_RX          61U  /*!< DMAMUX1 SPI3 RX request   */
#define DMA_REQUEST_SPI3_TX          62U  /*!< DMAMUX1 SPI3 TX request   */

#define DMA_REQUEST_UART4_RX         63U  /*!< DMAMUX1 UART4 RX request */
#define DMA_REQUEST_UART4_TX         64U  /*!< DMAMUX1 UART4 TX request */
#define DMA_REQUEST_UART5_RX         65U  /*!< DMAMUX1 UART5 RX request */
#define DMA_REQUEST_UART5_TX         66U  /*!< DMAMUX1 UART5 TX request */

#define DMA_REQUEST_DAC1_CH1         67U  /*!< DMAMUX1 DAC1 Channel 1 request */
#define DMA_REQUEST_DAC1_CH2         68U  /*!< DMAMUX1 DAC1 Channel 2 request */

#define DMA_REQUEST_TIM6_UP          69U  /*!< DMAMUX1 TIM6 UP request   */
#define DMA_REQUEST_TIM7_UP          70U  /*!< DMAMUX1 TIM7 UP request   */

#define DMA_REQUEST_USART6_RX        71U  /*!< DMAMUX1 USART6 RX request */
#define DMA_REQUEST_USART6_TX        72U  /*!< DMAMUX1 USART6 TX request */

#define DMA_REQUEST_I2C3_RX          73U  /*!< DMAMUX1 I2C3 RX request   */
#define DMA_REQUEST_I2C3_TX          74U  /*!< DMAMUX1 I2C3 TX request   */

#if defined (PSSI)
#define DMA_REQUEST_DCMI_PSSI        75U  /*!< DMAMUX1 DCMI/PSSI request    */
#define DMA_REQUEST_DCMI             DMA_REQUEST_DCMI_PSSI /* Legacy define */
#else
#define DMA_REQUEST_DCMI             75U  /*!< DMAMUX1 DCMI request         */
#endif /* PSSI */

#define DMA_REQUEST_CRYP_IN          76U  /*!< DMAMUX1 CRYP IN request   */
#define DMA_REQUEST_CRYP_OUT         77U  /*!< DMAMUX1 CRYP OUT request  */

#define DMA_REQUEST_HASH_IN          78U  /*!< DMAMUX1 HASH IN request   */

#define DMA_REQUEST_UART7_RX         79U  /*!< DMAMUX1 UART7 RX request  */
#define DMA_REQUEST_UART7_TX         80U  /*!< DMAMUX1 UART7 TX request  */
#define DMA_REQUEST_UART8_RX         81U  /*!< DMAMUX1 UART8 RX request  */
#define DMA_REQUEST_UART8_TX         82U  /*!< DMAMUX1 UART8 TX request  */

#define DMA_REQUEST_SPI4_RX          83U  /*!< DMAMUX1 SPI4 RX request   */
#define DMA_REQUEST_SPI4_TX          84U  /*!< DMAMUX1 SPI4 TX request   */
#define DMA_REQUEST_SPI5_RX          85U  /*!< DMAMUX1 SPI5 RX request   */
#define DMA_REQUEST_SPI5_TX          86U  /*!< DMAMUX1 SPI5 TX request   */

#define DMA_REQUEST_SAI1_A           87U  /*!< DMAMUX1 SAI1 A request    */
#define DMA_REQUEST_SAI1_B           88U  /*!< DMAMUX1 SAI1 B request    */

#if defined(SAI2)
#define DMA_REQUEST_SAI2_A           89U  /*!< DMAMUX1 SAI2 A request    */
#define DMA_REQUEST_SAI2_B           90U  /*!< DMAMUX1 SAI2 B request    */
#endif /* SAI2 */

#define DMA_REQUEST_SWPMI_RX         91U  /*!< DMAMUX1 SWPMI RX request  */
#define DMA_REQUEST_SWPMI_TX         92U  /*!< DMAMUX1 SWPMI TX request  */

#define DMA_REQUEST_SPDIF_RX_DT      93U  /*!< DMAMUX1 SPDIF RXDT request*/
#define DMA_REQUEST_SPDIF_RX_CS      94U  /*!< DMAMUX1 SPDIF RXCS request*/

#if defined(HRTIM1)
#define DMA_REQUEST_HRTIM_MASTER     95U  /*!< DMAMUX1 HRTIM1 Master request 1 */
#define DMA_REQUEST_HRTIM_TIMER_A    96U  /*!< DMAMUX1 HRTIM1 Timer A request 2 */
#define DMA_REQUEST_HRTIM_TIMER_B    97U  /*!< DMAMUX1 HRTIM1 Timer B request 3 */
#define DMA_REQUEST_HRTIM_TIMER_C    98U  /*!< DMAMUX1 HRTIM1 Timer C request 4 */
#define DMA_REQUEST_HRTIM_TIMER_D    99U  /*!< DMAMUX1 HRTIM1 Timer D request 5 */
#define DMA_REQUEST_HRTIM_TIMER_E   100U  /*!< DMAMUX1 HRTIM1 Timer E request 6*/
#endif /* HRTIM1 */

#define DMA_REQUEST_DFSDM1_FLT0     101U  /*!< DMAMUX1 DFSDM Filter0 request */
#define DMA_REQUEST_DFSDM1_FLT1     102U  /*!< DMAMUX1 DFSDM Filter1 request */
#define DMA_REQUEST_DFSDM1_FLT2     103U  /*!< DMAMUX1 DFSDM Filter2 request */
#define DMA_REQUEST_DFSDM1_FLT3     104U  /*!< DMAMUX1 DFSDM Filter3 request */

#define DMA_REQUEST_TIM15_CH1       105U  /*!< DMAMUX1 TIM15 CH1 request  */
#define DMA_REQUEST_TIM15_UP        106U  /*!< DMAMUX1 TIM15 UP request   */
#define DMA_REQUEST_TIM15_TRIG      107U  /*!< DMAMUX1 TIM15 TRIG request */
#define DMA_REQUEST_TIM15_COM       108U  /*!< DMAMUX1 TIM15 COM request  */

#define DMA_REQUEST_TIM16_CH1       109U  /*!< DMAMUX1 TIM16 CH1 request  */
#define DMA_REQUEST_TIM16_UP        110U  /*!< DMAMUX1 TIM16 UP request   */

#define DMA_REQUEST_TIM17_CH1       111U  /*!< DMAMUX1 TIM17 CH1 request  */
#define DMA_REQUEST_TIM17_UP        112U  /*!< DMAMUX1 TIM17 UP request   */

#if defined(SAI3)
#define DMA_REQUEST_SAI3_A          113U  /*!< DMAMUX1 SAI3 A request  */
#define DMA_REQUEST_SAI3_B          114U  /*!< DMAMUX1 SAI3 B request  */
#endif /* SAI3 */

#if defined(ADC3)
#define DMA_REQUEST_ADC3            115U  /*!< DMAMUX1 ADC3 request  */
#endif /* ADC3 */

#if defined(UART9)
#define DMA_REQUEST_UART9_RX        116U  /*!< DMAMUX1 UART9 request  */
#define DMA_REQUEST_UART9_TX        117U  /*!< DMAMUX1 UART9 request  */
#endif /* UART9 */

#if defined(USART10)
#define DMA_REQUEST_USART10_RX      118U  /*!< DMAMUX1 USART10 request  */
#define DMA_REQUEST_USART10_TX      119U  /*!< DMAMUX1 USART10 request  */
#endif /* USART10 */

#if defined(FMAC)
#define DMA_REQUEST_FMAC_READ       120U  /*!< DMAMUX1 FMAC Read request  */
#define DMA_REQUEST_FMAC_WRITE      121U  /*!< DMAMUX1 FMAC Write request */
#endif /* FMAC */

#if defined(CORDIC)
#define DMA_REQUEST_CORDIC_READ     122U  /*!< DMAMUX1 CORDIC Read request  */
#define DMA_REQUEST_CORDIC_WRITE    123U  /*!< DMAMUX1 CORDIC Write request */
#endif /* CORDIC */

#if defined(I2C5)
#define DMA_REQUEST_I2C5_RX         124U  /*!< DMAMUX1 I2C5 RX request   */
#define DMA_REQUEST_I2C5_TX         125U  /*!< DMAMUX1 I2C5 TX request   */
#endif /* I2C5 */

#if defined(TIM23)
#define DMA_REQUEST_TIM23_CH1        126U  /*!< DMAMUX1 TIM23 CH1 request  */
#define DMA_REQUEST_TIM23_CH2        127U  /*!< DMAMUX1 TIM23 CH2 request  */
#define DMA_REQUEST_TIM23_CH3        128U  /*!< DMAMUX1 TIM23 CH3 request  */
#define DMA_REQUEST_TIM23_CH4        129U  /*!< DMAMUX1 TIM23 CH4 request  */
#define DMA_REQUEST_TIM23_UP         130U  /*!< DMAMUX1 TIM23 UP request   */
#define DMA_REQUEST_TIM23_TRIG       131U  /*!< DMAMUX1 TIM23 TRIG request */
#endif /* TIM23 */

#if defined(TIM24)
#define DMA_REQUEST_TIM24_CH1        132U  /*!< DMAMUX1 TIM24 CH1 request  */
#define DMA_REQUEST_TIM24_CH2        133U  /*!< DMAMUX1 TIM24 CH2 request  */
#define DMA_REQUEST_TIM24_CH3        134U  /*!< DMAMUX1 TIM24 CH3 request  */
#define DMA_REQUEST_TIM24_CH4        135U  /*!< DMAMUX1 TIM24 CH4 request  */
#define DMA_REQUEST_TIM24_UP         136U  /*!< DMAMUX1 TIM24 UP request   */
#define DMA_REQUEST_TIM24_TRIG       137U  /*!< DMAMUX1 TIM24 TRIG request */
#endif /* TIM24 */


#define DMA_PERIPH_TO_MEMORY         ((uint32_t)0x00000000U)      /*!< Peripheral to memory direction */
#define DMA_MEMORY_TO_PERIPH         ((uint32_t)DMA_SxCR_DIR_0)  /*!< Memory to peripheral direction */
#define DMA_MEMORY_TO_MEMORY         ((uint32_t)DMA_SxCR_DIR_1)  /*!< Memory to memory direction     */

#define DMA_PDATAALIGN_BYTE          ((uint32_t)0x00000000U)       /*!< Peripheral data alignment: Byte      */
#define DMA_PDATAALIGN_HALFWORD      ((uint32_t)DMA_SxCR_PSIZE_0)  /*!< Peripheral data alignment: HalfWord  */
#define DMA_PDATAALIGN_WORD          ((uint32_t)DMA_SxCR_PSIZE_1)  /*!< Peripheral data alignment: Word      */

#define DMA_MDATAALIGN_BYTE          ((uint32_t)0x00000000U)       /*!< Memory data alignment: Byte     */
#define DMA_MDATAALIGN_HALFWORD      ((uint32_t)DMA_SxCR_MSIZE_0)  /*!< Memory data alignment: HalfWord */
#define DMA_MDATAALIGN_WORD          ((uint32_t)DMA_SxCR_MSIZE_1)  /*!< Memory data alignment: Word     */

#define DMA_PINC_ENABLE        ((uint32_t)DMA_SxCR_PINC)  /*!< Peripheral increment mode enable  */
#define DMA_PINC_DISABLE       ((uint32_t)0x00000000U)     /*!< Peripheral increment mode disable */

#define DMA_MINC_ENABLE         ((uint32_t)DMA_SxCR_MINC)  /*!< Memory increment mode enable  */
#define DMA_MINC_DISABLE        ((uint32_t)0x00000000U)     /*!< Memory increment mode disable */

#define DMA_PRIORITY_LOW             ((uint32_t)0x00000000U)    /*!< Priority level: Low       */
#define DMA_PRIORITY_MEDIUM          ((uint32_t)DMA_SxCR_PL_0)  /*!< Priority level: Medium    */
#define DMA_PRIORITY_HIGH            ((uint32_t)DMA_SxCR_PL_1)  /*!< Priority level: High      */
#define DMA_PRIORITY_VERY_HIGH       ((uint32_t)DMA_SxCR_PL)    /*!< Priority level: Very High */

#define DMA_NORMAL              ((uint32_t)0x00000000U)                  /*!< Normal mode                                    */
#define DMA_CIRCULAR            ((uint32_t)DMA_SxCR_CIRC)                /*!< Circular mode                                  */
#define DMA_PFCTRL              ((uint32_t)DMA_SxCR_PFCTRL)              /*!< Peripheral flow control mode                   */
#define DMA_DOUBLE_BUFFER_M0    ((uint32_t)DMA_SxCR_DBM)                 /*!< Double buffer mode with first target memory M0 */
#define DMA_DOUBLE_BUFFER_M1    ((uint32_t)(DMA_SxCR_DBM | DMA_SxCR_CT)) /*!< Double buffer mode with first target memory M1 */

#define DMA_IT_TC                         ((uint32_t)DMA_SxCR_TCIE)
#define DMA_IT_HT                         ((uint32_t)DMA_SxCR_HTIE)
#define DMA_IT_TE                         ((uint32_t)DMA_SxCR_TEIE)
#define DMA_IT_DME                        ((uint32_t)DMA_SxCR_DMEIE)
#define DMA_IT_FE                         ((uint32_t)0x00000080U)

#define DMA_FLAG_FEIF0_4                    ((uint32_t)0x00000001U)
#define DMA_FLAG_DMEIF0_4                   ((uint32_t)0x00000004U)
#define DMA_FLAG_TEIF0_4                    ((uint32_t)0x00000008U)
#define DMA_FLAG_HTIF0_4                    ((uint32_t)0x00000010U)
#define DMA_FLAG_TCIF0_4                    ((uint32_t)0x00000020U)
#define DMA_FLAG_FEIF1_5                    ((uint32_t)0x00000040U)
#define DMA_FLAG_DMEIF1_5                   ((uint32_t)0x00000100U)
#define DMA_FLAG_TEIF1_5                    ((uint32_t)0x00000200U)
#define DMA_FLAG_HTIF1_5                    ((uint32_t)0x00000400U)
#define DMA_FLAG_TCIF1_5                    ((uint32_t)0x00000800U)
#define DMA_FLAG_FEIF2_6                    ((uint32_t)0x00010000U)
#define DMA_FLAG_DMEIF2_6                   ((uint32_t)0x00040000U)
#define DMA_FLAG_TEIF2_6                    ((uint32_t)0x00080000U)
#define DMA_FLAG_HTIF2_6                    ((uint32_t)0x00100000U)
#define DMA_FLAG_TCIF2_6                    ((uint32_t)0x00200000U)
#define DMA_FLAG_FEIF3_7                    ((uint32_t)0x00400000U)
#define DMA_FLAG_DMEIF3_7                   ((uint32_t)0x01000000U)
#define DMA_FLAG_TEIF3_7                    ((uint32_t)0x02000000U)
#define DMA_FLAG_HTIF3_7                    ((uint32_t)0x04000000U)
#define DMA_FLAG_TCIF3_7                    ((uint32_t)0x08000000U)

uint8_t BSP_STM32_DMA_Init(DMA_Stream_TypeDef *hdma, uint32_t dir, uint32_t pinc, uint32_t minc, uint32_t pdatalign, uint32_t mdatalign, uint32_t mode, uint32_t priority);
uint8_t BSP_STM32_DMA_CtxLink(DMA_Stream_TypeDef *hdma, TxRxContext_TypeDef *ctx, uint32_t request);
uint8_t BSP_STM32_DMA_StartIT(TxRxContext_TypeDef *ctx, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength);
uint8_t BSP_STM32_DMA_Abort(TxRxContext_TypeDef *ctx);
uint32_t BSP_STM32_DMA_GetCounter(DMA_Stream_TypeDef *hdma);
void DMA_Stream_IRQHandler(TxRxContext_TypeDef * ctx);

#endif /* STM32H7_BSP_STM32_DMA_H_ */
