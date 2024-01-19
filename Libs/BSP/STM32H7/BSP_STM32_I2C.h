/*******************************************************************
 * MiniConsole V3 - Board Support Package - STM32 I2C
 *
 * Author: Marek Ryn
 * Version: 1.0
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 * - 1.0	- Initial version
 *******************************************************************/

#ifndef STM32H7_BSP_STM32_I2C_H_
#define STM32H7_BSP_STM32_I2C_H_

#include "BSP_Common.h"
#include "BSP_STM32_DMA.h"
#include "BSP_STM32_BDMA.h"

#define I2C_NO_STARTSTOP                (0x00000000U)
#define I2C_GENERATE_STOP               (uint32_t)(0x80000000U | I2C_CR2_STOP)
#define I2C_GENERATE_START_READ         (uint32_t)(0x80000000U | I2C_CR2_START | I2C_CR2_RD_WRN)
#define I2C_GENERATE_START_WRITE         (uint32_t)(0x80000000U | I2C_CR2_START)
#define I2C_RELOAD_MODE                 I2C_CR2_RELOAD
#define I2C_AUTOEND_MODE                I2C_CR2_AUTOEND
#define I2C_SOFTEND_MODE                (0x00000000U)
#define I2C_GENERALCALL_DISABLE         (0x00000000U)
#define I2C_GENERALCALL_ENABLE          I2C_CR1_GCEN
#define I2C_NOSTRETCH_DISABLE           (0x00000000U)
#define I2C_NOSTRETCH_ENABLE            I2C_CR1_NOSTRETCH
#define I2C_ANALOGFILTER_ENABLE         0x00000000U
#define I2C_ANALOGFILTER_DISABLE        I2C_CR1_ANFOFF

typedef struct _I2CContext {
	uint8_t		*pData; // 4 bytes pointer to 8 bit data buffer
	uint32_t	size;
	uint32_t	index;
	uint32_t	option;
	uint32_t	devaddr;
} I2CContext_TypeDef;

uint8_t BSP_STM32_I2C_Init(I2C_TypeDef *hi2c);
uint8_t BSP_STM32_I2C_IsDeviceReady(I2C_TypeDef *hi2c, uint32_t device_address, uint32_t maxtrials, uint32_t timeout);
uint8_t BSP_STM32_I2C_RegWrite(I2C_TypeDef *hi2c, uint16_t DevAddress, uint8_t RegAddress, uint8_t RegValue, uint32_t Timeout);
uint8_t BSP_STM32_I2C_MemRead(I2C_TypeDef *hi2c, TxRxContext_TypeDef *ctx, uint16_t DevAddress, uint8_t MemAddress, uint8_t *pData, uint16_t Size);
uint8_t BSP_STM32_I2C_IRQHandler(I2C_TypeDef *hi2c, TxRxContext_TypeDef *ctx);


#endif /* STM32H7_BSP_STM32_I2C_H_ */
