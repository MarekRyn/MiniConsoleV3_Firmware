/*******************************************************************
 * MiniConsole V3 - Board Support Package - STM32 LTDC
 *
 * Author: Marek Ryn
 * Version: 1.0
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 * - 1.0	- Initial version
 *******************************************************************/

#ifndef STM32H7_BSP_STM32_LTDC_H_
#define STM32H7_BSP_STM32_LTDC_H_

#include "BSP_Common.h"

#define LTDC_BLENDING_FACTOR1_CA          0x00000400U   // Blending factor : Cte Alpha
#define LTDC_BLENDING_FACTOR1_PAxCA       0x00000600U   // Blending factor : Cte Alpha x Pixel Alpha
#define LTDC_BLENDING_FACTOR2_CA          0x00000005U   // Blending factor : Cte Alpha
#define LTDC_BLENDING_FACTOR2_PAxCA       0x00000007U   // Blending factor : Cte Alpha x Pixel Alpha

#define LTDC_PIXEL_FORMAT_ARGB8888        0x00000000U   // ARGB8888 LTDC pixel format
#define LTDC_PIXEL_FORMAT_RGB888          0x00000001U   // RGB888 LTDC pixel format
#define LTDC_PIXEL_FORMAT_RGB565          0x00000002U   // RGB565 LTDC pixel format
#define LTDC_PIXEL_FORMAT_ARGB1555        0x00000003U   // ARGB1555 LTDC pixel format
#define LTDC_PIXEL_FORMAT_ARGB4444        0x00000004U   // ARGB4444 LTDC pixel format
#define LTDC_PIXEL_FORMAT_L8              0x00000005U   // L8 LTDC pixel format
#define LTDC_PIXEL_FORMAT_AL44            0x00000006U   // AL44 LTDC pixel format
#define LTDC_PIXEL_FORMAT_AL88            0x00000007U   // AL88 LTDC pixel format

#define LTDC_PCPOLARITY_IPC               0x00000000U   	// input pixel clock.
#define LTDC_PCPOLARITY_IIPC              LTDC_GCR_PCPOL 	// inverted input pixel clock

#define LTDC_VSPOLARITY_AL                0x00000000U   	// Vertical Synchronization is active low
#define LTDC_VSPOLARITY_AH                LTDC_GCR_VSPOL    // Vertical Synchronization is active high

#define LTDC_DEPOLARITY_AL                0x00000000U   	// Data Enable, is active low
#define LTDC_DEPOLARITY_AH                LTDC_GCR_DEPOL    // Data Enable, is active high

#define LTDC_HSPOLARITY_AL                0x00000000U   	// Horizontal Synchronization is active low
#define LTDC_HSPOLARITY_AH                LTDC_GCR_HSPOL    // Horizontal Synchronization is active high

uint8_t BSP_STM32_LTDC_IsEnabled(LTDC_TypeDef *hltdc);

uint8_t BSP_STM32_LTDC_Init(LTDC_TypeDef *hltdc, uint32_t lcd_h_sync, uint32_t lcd_v_sync,
		uint32_t lcd_acc_h_back_porch_width, uint32_t lcd_acc_v_back_porch_height,
		uint32_t lcd_acc_active_width, uint32_t lcd_acc_active_height,
		uint32_t lcd_total_width, uint32_t lcd_total_height, uint32_t bgcolor);

uint8_t BSP_STM32_LTDC_DisableLayer(LTDC_TypeDef *hltdc, uint32_t layer);

uint8_t BSP_STM32_LTDC_EnableLayer(LTDC_TypeDef *hltdc, uint32_t layer);

uint8_t BSP_STM32_LTDC_ConfigLayer(LTDC_TypeDef *hltdc, uint32_t layer, uint32_t alpha, uint32_t alpha0, uint32_t bgcolor,
		uint32_t blendingfactor1, uint32_t blendingfactor2, uint32_t fbstartaddress, uint32_t imgheight, uint32_t imgwidth,
		uint32_t pixelformat, uint32_t x0, uint32_t x1, uint32_t y0, uint32_t y1);

uint8_t BSP_STM32_LTDC_UpdateFrameBufAddr(LTDC_TypeDef *hltdc, uint32_t layer, uint32_t fbstartaddress);

uint8_t BSP_STM32_LTDC_ConfigCLUT(LTDC_TypeDef *hltdc, uint32_t layer, uint32_t *clut);

uint8_t BSP_STM32_LTDC_SetTransparentColor(LTDC_TypeDef *hltdc, uint32_t layer, uint32_t color);

uint8_t BSP_STM32_LTDC_SetLineInt(LTDC_TypeDef *hltdc, uint32_t line);

uint8_t BSP_STM32_LTDC_IRQHandler(LTDC_TypeDef *hltdc);

#endif /* STM32H7_BSP_STM32_LTDC_H_ */
