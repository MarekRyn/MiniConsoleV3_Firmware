/*******************************************************************
 * MiniConsole V3 - Board Support Package - Accelerometer & Gyro IC
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 *******************************************************************/

#include "BSP_IMU.h"

BUF_RAM	IMU_HandleTypeDef		BSP_himu = {0};
		TxRxContext_TypeDef		BSP_himu_ctx = {0};

uint8_t BSP_IMU_Init() {
	// Configuring I2C interface
	if (BSP_STM32_I2C_Init(I2C4)) return BSP_ERROR;

	// Linking BDMA stream to I2C context
	BSP_BDMA_Init(BDMA_Channel0, BDMA_PERIPH_TO_MEMORY, BDMA_PINC_DISABLE, BDMA_MINC_ENABLE, BDMA_PDATAALIGN_BYTE, BDMA_MDATAALIGN_BYTE, BDMA_NORMAL, BDMA_PRIORITY_LOW);
	BSP_BDMA_CtxLink(BDMA_Channel0, &BSP_himu_ctx, BDMA_REQUEST_I2C4_RX);

	// Testing connection
	if (BSP_STM32_I2C_IsDeviceReady(I2C4, (IMU_I2C_ADDR) << 1, 4, 1000)) return BSP_ERROR;

	// Initialization of IMU IC
	if (BSP_DRV_IMU_Init(&BSP_himu, I2C4)) return BSP_ERROR;

	return BSP_OK;
}


uint8_t	BSP_IMU_Calibrate() {
	if (BSP_DRV_IMU_Calibrate(&BSP_himu)) return BSP_ERROR;
	return BSP_OK;
}


uint8_t BSP_IMU_Reset() {

	return BSP_OK;
}

void BSP_IMU_INTHandler() {
	// New data available from IMU, start receiving
	BSP_STM32_I2C_MemRead(I2C4, &BSP_himu_ctx, IMU_I2C_ADDR << 1, IMU_DATA_ADDR, BSP_himu.raw_data, IMU_DATA_LEN);
}

void I2C4_EV_IRQHandler(void) {
	// Receiving and parsing data from IMU
	if (BSP_STM32_I2C_IRQHandler(I2C4, &BSP_himu_ctx) == BSP_OK) BSP_DRV_IMU_Parse(&BSP_himu);
}
