/*******************************************************************
 * MiniConsole V3 - Board Support Package - LCD Touch Panel
 *
 * Author: Marek Ryn
 * Version: 1.0
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 * - 1.0	- Added support for areas
 *******************************************************************/

#include "BSP_LCD_TP.h"

BUF_RAM	LCD_TP_HandleTypeDef	BSP_hlcdtp = {0};
static	TxRxContext_TypeDef		BSP_hlcdtp_ctx = {0};
static 	uint8_t					active_flag = 0;


uint8_t BSP_LCD_TP_Init(void) {
	active_flag = 0;

	// Configuring I2C interface
	if (BSP_STM32_I2C_Init(I2C1)) return BSP_ERROR;

	// Linking DMA stream to I2C context
	BSP_STM32_DMA_Init(DMA1_Stream2, DMA_PERIPH_TO_MEMORY, DMA_PINC_DISABLE, DMA_MINC_ENABLE, DMA_PDATAALIGN_BYTE, DMA_MDATAALIGN_BYTE, DMA_NORMAL, DMA_PRIORITY_LOW);
	if (BSP_STM32_DMA_CtxLink(DMA1_Stream2, &BSP_hlcdtp_ctx, DMA_REQUEST_I2C1_RX)) return BSP_ERROR;

	// Reset Panel Controller
	BSP_DRV_LCD_TP_Reset();

	// Testing connection
	if (BSP_STM32_I2C_IsDeviceReady(I2C1, (LCD_TP_I2C_ADDR) << 1, 4, 1000)) return BSP_ERROR;

	// Initialization of Touch Panel
	if (BSP_DRV_LCD_TP_Init(I2C1)) return BSP_ERROR;

	// Activating interrupts
	active_flag = 1;

	return BSP_OK;
}


uint8_t BSP_LCD_TP_Reset(void) {
	active_flag = 0;

	// Reset Panel Controller
	BSP_DRV_LCD_TP_Reset();

	// Testing connection
	if (BSP_STM32_I2C_IsDeviceReady(I2C1, (LCD_TP_I2C_ADDR) << 1, 4, 1000)) return BSP_ERROR;

	active_flag = 1;
	return BSP_OK;
}


uint8_t BSP_LCD_TP_RegisterArea(uint8_t areaid, uint16_t x, uint16_t y, uint16_t width, uint16_t height, void* callback) {
	active_flag = 0;
	if (areaid >= LCD_TP_AREA_NO) return BSP_ERROR;
	BSP_hlcdtp.touch_areas[areaid].x = x;
	BSP_hlcdtp.touch_areas[areaid].y = y;
	BSP_hlcdtp.touch_areas[areaid].w = width;
	BSP_hlcdtp.touch_areas[areaid].h = height;
	BSP_hlcdtp.touch_areas[areaid].callback = callback;
	BSP_hlcdtp.touch_areas[areaid].active = 1;
	active_flag = 1;
	return BSP_OK;
}


uint8_t BSP_LCD_TP_RemoveArea(uint8_t areaid) {
	if (areaid >= LCD_TP_AREA_NO) return BSP_ERROR;
	active_flag = 0;
	BSP_hlcdtp.touch_areas[areaid].active = 0;
	BSP_hlcdtp.touch_areas[areaid].callback = NULL;
	active_flag = 1;
	return BSP_OK;
}


uint8_t BSP_CLD_TP_RemoveAreaRange(uint8_t aid_start, uint8_t aid_stop) {
	if (aid_start >= LCD_TP_AREA_NO) return BSP_ERROR;
	if (aid_stop >= LCD_TP_AREA_NO) return BSP_ERROR;
	if (aid_start > aid_stop) return BSP_ERROR;

	active_flag = 0;
	for (uint8_t i = aid_start; i<= aid_stop; i++) {
		BSP_hlcdtp.touch_areas[i].active = 0;
		BSP_hlcdtp.touch_areas[i].callback = NULL;
	}
	active_flag = 1;
	return BSP_OK;
}


uint8_t BSP_LCD_TP_RemoveAllAreas(void) {
	active_flag = 0;
	for (uint8_t i = 0; i<LCD_TP_AREA_NO; i++) {
		BSP_hlcdtp.touch_areas[i].active = 0;
		BSP_hlcdtp.touch_areas[i].callback = NULL;
	}
	active_flag = 1;
	return BSP_OK;
}

// ************ IRQ Handlers ***************

void EXTI13_IRQHandler(void) {
	if (!active_flag) return;
	// New data available in touch panel, start receiving by I2C
	BSP_STM32_I2C_MemRead(I2C1, &BSP_hlcdtp_ctx, LCD_TP_I2C_ADDR << 1, 0, BSP_hlcdtp.raw_data, LCD_TP_REG_TOTAL_LENGTH);
}


void I2C1_EV_IRQHandler(void) {
	if (!active_flag) return;
	// Receiving from I2C completed.
	if (BSP_STM32_I2C_IRQHandler(I2C1, &BSP_hlcdtp_ctx) == BSP_OK) {
		// Parsing data from touch panel
		BSP_DRV_LCD_TP_Parse(&BSP_hlcdtp);
		// Calling callback for active touch area (if defined)
		if (BSP_hlcdtp.touch_areas[BSP_hlcdtp.gest_data.area].callback != NULL) ((void(*)())BSP_hlcdtp.touch_areas[BSP_hlcdtp.gest_data.area].callback)();
	}
}


