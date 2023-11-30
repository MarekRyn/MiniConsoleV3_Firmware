/*******************************************************************
 * MiniConsole V3 - Board Support Package - LCD Touch Panel
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 *******************************************************************/

#include "BSP_LCD_TP.h"

BUF_RAM	LCD_TP_HandleTypeDef	BSP_hlcdtp = {0};
		TxRxContext_TypeDef		BSP_hlcdtp_ctx = {0};


uint8_t BSP_LCD_TP_Init() {
	// Configuring I2C interface
	if (BSP_STM32_I2C_Init(I2C1)) return BSP_ERROR;

	// Linking DMA stream to I2C context
	BSP_STM32_DMA_Init(DMA1_Stream2, DMA_PERIPH_TO_MEMORY, DMA_PINC_DISABLE, DMA_MINC_ENABLE, DMA_PDATAALIGN_BYTE, DMA_MDATAALIGN_BYTE, DMA_NORMAL, DMA_PRIORITY_LOW);
	if (BSP_STM32_DMA_CtxLink(DMA1_Stream2, &BSP_hlcdtp_ctx, DMA_REQUEST_I2C1_RX)) return BSP_ERROR;

	// Reset Panel Controller
	BSP_STM32_GPIO_WritePin(BSP_LCD_TP_RST_PORT, BSP_LCD_TP_RST_PIN, GPIO_PIN_RESET);
	BSP_Delay(5);
	BSP_STM32_GPIO_WritePin(BSP_LCD_TP_RST_PORT, BSP_LCD_TP_RST_PIN, GPIO_PIN_SET);
	BSP_Delay(5);

	// Testing connection
	if (BSP_STM32_I2C_IsDeviceReady(I2C1, (LCD_TP_I2C_ADDR) << 1, 4, 1000)) return BSP_ERROR;

	// Initialization of Touch Panel
	if (BSP_DRV_LCD_TP_Init(I2C1)) return BSP_ERROR;

	return BSP_OK;
}



uint8_t BSP_LCD_TP_Reset() {
	// Reset Panel Controller
	BSP_STM32_GPIO_WritePin(BSP_LCD_TP_RST_PORT, BSP_LCD_TP_RST_PIN, GPIO_PIN_RESET);
	BSP_Delay(5);
	BSP_STM32_GPIO_WritePin(BSP_LCD_TP_RST_PORT, BSP_LCD_TP_RST_PIN, GPIO_PIN_SET);
	BSP_Delay(5);

	// Testing connection
	if (BSP_STM32_I2C_IsDeviceReady(I2C1, (LCD_TP_I2C_ADDR) << 1, 4, 1000)) return BSP_ERROR;

	return BSP_OK;
}


void BSP_LCD_TP_INTHandler() {
	// New data available from touch panel, start receiving
	//BSP_STM32_I2C_MemReadIT(I2C1, &BSP_hlcdtp_ctx, LCD_TP_I2C_ADDR << 1, 0, BSP_hlcdtp.raw_data, LCD_TP_REG_TOTAL_LENGTH);
	BSP_STM32_I2C_MemRead(I2C1, &BSP_hlcdtp_ctx, LCD_TP_I2C_ADDR << 1, 0, BSP_hlcdtp.raw_data, LCD_TP_REG_TOTAL_LENGTH);
}


void I2C1_EV_IRQHandler(void) {
	// Receiving and parsing data from touch panel
	if (BSP_STM32_I2C_IRQHandler(I2C1, &BSP_hlcdtp_ctx) == BSP_OK) BSP_DRV_LCD_TP_Parse(&BSP_hlcdtp);
}


