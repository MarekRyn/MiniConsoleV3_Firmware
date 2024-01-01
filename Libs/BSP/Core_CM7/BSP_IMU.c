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

uint8_t BSP_IMU_LoadCalData(void) {
	BSP_himu.cal.c_ox = BSP_STM32_RTC_GetBackupReg(RTC, 4);
	BSP_himu.cal.c_oy = BSP_STM32_RTC_GetBackupReg(RTC, 5);
	BSP_himu.cal.c_oz = BSP_STM32_RTC_GetBackupReg(RTC, 6);
	BSP_himu.cal.c_x = BSP_STM32_RTC_GetBackupReg(RTC, 7);
	BSP_himu.cal.c_y = BSP_STM32_RTC_GetBackupReg(RTC, 8);
	BSP_himu.cal.c_z = BSP_STM32_RTC_GetBackupReg(RTC, 9);
	return BSP_OK;
}

uint8_t BSP_IMU_SaveCalData(void) {
	BSP_STM32_RTC_SetBackupReg(RTC, 4, BSP_himu.cal.c_ox);
	BSP_STM32_RTC_SetBackupReg(RTC, 5, BSP_himu.cal.c_oy);
	BSP_STM32_RTC_SetBackupReg(RTC, 6, BSP_himu.cal.c_oz);
	BSP_STM32_RTC_SetBackupReg(RTC, 7, BSP_himu.cal.c_x);
	BSP_STM32_RTC_SetBackupReg(RTC, 8, BSP_himu.cal.c_y);
	BSP_STM32_RTC_SetBackupReg(RTC, 9, BSP_himu.cal.c_z);
	return BSP_OK;
}

uint8_t BSP_IMU_Init(void) {
	// Configuring I2C interface
	if (BSP_STM32_I2C_Init(I2C4)) return BSP_ERROR;

	// Linking BDMA stream to I2C context
	BSP_BDMA_Init(BDMA_Channel0, BDMA_PERIPH_TO_MEMORY, BDMA_PINC_DISABLE, BDMA_MINC_ENABLE, BDMA_PDATAALIGN_BYTE, BDMA_MDATAALIGN_BYTE, BDMA_NORMAL, BDMA_PRIORITY_LOW);
	BSP_BDMA_CtxLink(BDMA_Channel0, &BSP_himu_ctx, BDMA_REQUEST_I2C4_RX);

	// Testing connection
	if (BSP_STM32_I2C_IsDeviceReady(I2C4, (IMU_I2C_ADDR) << 1, 4, 1000)) return BSP_ERROR;

	// Initialization of IMU IC
	if (BSP_DRV_IMU_Init(&BSP_himu, I2C4)) return BSP_ERROR;

	// Load calibration data
	BSP_IMU_LoadCalData();

	return BSP_OK;
}


uint8_t	BSP_IMU_Calibrate(void) {
	if (BSP_DRV_IMU_Calibrate(&BSP_himu)) return BSP_ERROR;
	return BSP_OK;
}


uint8_t BSP_IMU_Reset(void) {

	return BSP_OK;
}


// ******* IRQ Handlers **********

void EXTI11_IRQHandler(void) {
	// New data available from IMU, start receiving
	BSP_STM32_I2C_MemRead(I2C4, &BSP_himu_ctx, IMU_I2C_ADDR << 1, IMU_DATA_ADDR, BSP_himu.raw_data, IMU_DATA_LEN);
}

void I2C4_EV_IRQHandler(void) {
	// Receiving and parsing data from IMU
	if (BSP_STM32_I2C_IRQHandler(I2C4, &BSP_himu_ctx) == BSP_OK) BSP_DRV_IMU_Parse(&BSP_himu);
}
