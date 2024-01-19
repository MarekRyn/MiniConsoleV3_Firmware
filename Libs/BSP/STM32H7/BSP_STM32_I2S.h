/*******************************************************************
 * MiniConsole V3 - Board Support Package - STM32 I2S
 *
 * Author: Marek Ryn
 * Version: 1.0
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 * - 1.0	- Initial version
 *******************************************************************/

#ifndef STM32H7_BSP_STM32_I2S_H_
#define STM32H7_BSP_STM32_I2S_H_

#include "BSP_Common.h"
#include "BSP_STM32_DMA.h"

#define I2S_AUDIOFREQ_44K				44100
#define I2S_AUDIOFREQ_22K       		22050
#define I2S_AUDIOFREQ_11K       		11025

#define I2S_STATUS_READY				0
#define I2S_STATUS_TRANSMIT_IT			1
#define I2S_STATUS_TRANSMIT_DMA			2
#define I2S_STATUS_PAUSE_IT				3
#define I2S_STATUS_PAUSE_DMA			4

#define I2S_MODE_SLAVE_TX                (0x00000000UL)
#define I2S_MODE_SLAVE_RX                (SPI_I2SCFGR_I2SCFG_0)
#define I2S_MODE_MASTER_TX               (SPI_I2SCFGR_I2SCFG_1)
#define I2S_MODE_MASTER_RX               (SPI_I2SCFGR_I2SCFG_0 | SPI_I2SCFGR_I2SCFG_1)
#define I2S_MODE_SLAVE_FULLDUPLEX        (SPI_I2SCFGR_I2SCFG_2)
#define I2S_MODE_MASTER_FULLDUPLEX       (SPI_I2SCFGR_I2SCFG_2 | SPI_I2SCFGR_I2SCFG_0)

#define I2S_STANDARD_PHILIPS             (0x00000000UL)
#define I2S_STANDARD_MSB                 (SPI_I2SCFGR_I2SSTD_0)
#define I2S_STANDARD_LSB                 (SPI_I2SCFGR_I2SSTD_1)
#define I2S_STANDARD_PCM_SHORT           (SPI_I2SCFGR_I2SSTD_0 | SPI_I2SCFGR_I2SSTD_1)
#define I2S_STANDARD_PCM_LONG            (SPI_I2SCFGR_I2SSTD_0 | SPI_I2SCFGR_I2SSTD_1 | SPI_I2SCFGR_PCMSYNC)

#define I2S_MCLKOUTPUT_ENABLE            (SPI_I2SCFGR_MCKOE)
#define I2S_MCLKOUTPUT_DISABLE           (0x00000000UL)

#define I2S_DATA_24BIT_ALIGNMENT_RIGHT   (0x00000000UL)
#define I2S_DATA_24BIT_ALIGNMENT_LEFT    SPI_I2SCFGR_DATFMT

#define I2S_WS_INVERSION_DISABLE         (0x00000000UL)
#define I2S_WS_INVERSION_ENABLE          SPI_I2SCFGR_WSINV

#define I2S_CPOL_LOW                     (0x00000000UL)
#define I2S_CPOL_HIGH                    (SPI_I2SCFGR_CKPOL)

#define I2S_DATAFORMAT_16B               (0x00000000UL)
#define I2S_DATAFORMAT_16B_EXTENDED      (SPI_I2SCFGR_CHLEN)
#define I2S_DATAFORMAT_24B               (SPI_I2SCFGR_DATLEN_0)
#define I2S_DATAFORMAT_32B               (SPI_I2SCFGR_DATLEN_1)

#define I2S_FIRSTBIT_MSB                 (0x00000000UL)
#define I2S_FIRSTBIT_LSB                 SPI_CFG2_LSBFRST

#define I2S_MASTER_KEEP_IO_STATE_DISABLE (0x00000000U)
#define I2S_MASTER_KEEP_IO_STATE_ENABLE  SPI_CFG2_AFCNTR

#define I2S_IT_RXP                       SPI_IER_RXPIE
#define I2S_IT_TXP                       SPI_IER_TXPIE
#define I2S_IT_DXP                       SPI_IER_DXPIE
#define I2S_IT_UDR                       SPI_IER_UDRIE
#define I2S_IT_OVR                       SPI_IER_OVRIE
#define I2S_IT_FRE                       SPI_IER_TIFREIE
#define I2S_IT_ERR                       (SPI_IER_UDRIE | SPI_IER_OVRIE | SPI_IER_TIFREIE)

#define I2S_FLAG_RXP                     SPI_SR_RXP       // I2S status flag : Rx-Packet available flag
#define I2S_FLAG_TXP                     SPI_SR_TXP       // I2S status flag : Tx-Packet space available flag
#define I2S_FLAG_DXP                     SPI_SR_DXP       // I2S status flag : Dx-Packet space available flag
#define I2S_FLAG_UDR                     SPI_SR_UDR       // I2S Error flag  : Underrun flag
#define I2S_FLAG_OVR                     SPI_SR_OVR       // I2S Error flag  : Overrun flag
#define I2S_FLAG_FRE                     SPI_SR_TIFRE     // I2S Error flag  : TI mode frame format error flag
#define I2S_FLAG_MASK                    (SPI_SR_RXP | SPI_SR_TXP | SPI_SR_DXP |SPI_SR_UDR | SPI_SR_OVR | SPI_SR_TIFRE)


typedef struct _I2SContext {
	int16_t		*pData; 		// 4 bytes pointer to 16 bit data buffer
	uint32_t	size;
	uint32_t	index;
	uint8_t		status;
} I2SContext_TypeDef;


uint8_t BSP_STM32_I2S_Init(SPI_TypeDef * hi2s, TxRxContext_TypeDef * ctx, uint32_t AudioFreq);
uint8_t BSP_STM32_I2S_RegisterCallbacks(TxRxContext_TypeDef * ctx, uint32_t CallbackTC, uint32_t CallbackHT, uint32_t CallbackTE);
uint8_t BSP_STM32_I2S_TransmitIT(SPI_TypeDef * hi2s, TxRxContext_TypeDef * ctx, int16_t *pData, uint32_t Size);
uint8_t BSP_STM32_I2S_TransmitDMA(SPI_TypeDef * hi2s, TxRxContext_TypeDef * ctx, int16_t *pData, uint32_t Size);
uint8_t BSP_STM32_I2S_Pause(SPI_TypeDef * hi2s, TxRxContext_TypeDef * ctx);
uint8_t BSP_STM32_I2S_Resume(SPI_TypeDef * hi2s, TxRxContext_TypeDef * ctx);
uint8_t BSP_STM32_I2S_Stop(SPI_TypeDef * hi2s, TxRxContext_TypeDef * ctx);


uint8_t BSP_STM32_I2S_IRQHandler(SPI_TypeDef *hi2s, TxRxContext_TypeDef *ctx);

#endif /* STM32H7_BSP_STM32_I2S_H_ */
