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

#include "BSP_STM32_LTDC.h"


uint8_t BSP_STM32_LTDC_IsEnabled(LTDC_TypeDef *hltdc) {
	return (hltdc->GCR & LTDC_GCR_LTDCEN);
}


uint8_t BSP_STM32_LTDC_Init(LTDC_TypeDef *hltdc, uint32_t lcd_h_sync, uint32_t lcd_v_sync,
		uint32_t lcd_acc_h_back_porch_width, uint32_t lcd_acc_v_back_porch_height,
		uint32_t lcd_acc_active_width, uint32_t lcd_acc_active_height,
		uint32_t lcd_total_width, uint32_t lcd_total_height, uint32_t bgcolor) {

	// Checking if LTDC already initialized
	if (hltdc->GCR & LTDC_GCR_LTDCEN) return BSP_OK;

	uint32_t tmp;

	//Reseting LTDC
	//	RCC->APB3RSTR |= RCC_APB3RSTR_LTDCRST;
	//	BSP_Delay(1);
	//	RCC->APB3RSTR &= ~RCC_APB3RSTR_LTDCRST;
	//	BSP_Delay(1);

	// Setting AXI QOS to higher priority for LTDC peripheral
	GPV->AXI_INI6_READ_QOS = 1UL;
	GPV->AXI_INI6_WRITE_QOS = 1UL;

	// LTDC Init

	// Configure the HS, VS, DE and PC polarity
	hltdc->GCR &= ~(LTDC_GCR_HSPOL | LTDC_GCR_VSPOL | LTDC_GCR_DEPOL | LTDC_GCR_PCPOL);
	hltdc->GCR |= (uint32_t)(LTDC_HSPOLARITY_AL | LTDC_VSPOLARITY_AL | LTDC_DEPOLARITY_AL | LTDC_PCPOLARITY_IPC);

	// Set Synchronization size
	hltdc->SSCR &= ~(LTDC_SSCR_VSH | LTDC_SSCR_HSW);
	tmp = ((lcd_h_sync) << 16U);
	hltdc->SSCR |= (tmp | (lcd_v_sync));

	// Set Accumulated Back porch
	hltdc->BPCR &= ~(LTDC_BPCR_AVBP | LTDC_BPCR_AHBP);
	tmp = ((lcd_acc_h_back_porch_width) << 16U);
	hltdc->BPCR |= (tmp | (lcd_acc_v_back_porch_height));

	// Set Accumulated Active Width
	hltdc->AWCR &= ~(LTDC_AWCR_AAH | LTDC_AWCR_AAW);
	tmp = ((lcd_acc_active_width) << 16U);
	hltdc->AWCR |= (tmp | (lcd_acc_active_height));

	// Set Total Width
	hltdc->TWCR &= ~(LTDC_TWCR_TOTALH | LTDC_TWCR_TOTALW);
	tmp = ((lcd_total_width) << 16U);
	hltdc->TWCR |= (tmp | (lcd_total_height));

	// Set the background color value
	hltdc->BCCR &= ~(LTDC_BCCR_BCBLUE | LTDC_BCCR_BCGREEN | LTDC_BCCR_BCRED);
	hltdc->BCCR |= (bgcolor & 0x00FFFFFF);

	// Enable the Transfer Error and FIFO underrun interrupts */
	hltdc->IER |= LTDC_IER_TERRIE | LTDC_IER_FUIE;

	// Enable LTDC by setting LTDCEN bit
	hltdc->GCR |= LTDC_GCR_LTDCEN;

	return BSP_OK;
}

uint8_t BSP_STM32_LTDC_DisableLayer(LTDC_TypeDef *hltdc, uint32_t layer) {
	LTDC_Layer_TypeDef *l = (LTDC_Layer_TypeDef *)(((uint32_t)hltdc) + 0x84U + (0x80U*(layer)));

	// Disable Layer
	l->CR &= ~(LTDC_LxCR_LEN | LTDC_LxCR_CLUTEN | LTDC_LxCR_COLKEN);
	l->CKCR = 0x00000000;

	// Set the Reload on Vertical Blank
	hltdc->SRCR |= LTDC_SRCR_VBR;

	// Wait for layer to disable
	while (l->CR & LTDC_LxCR_LEN) {};

	return BSP_OK;
}

uint8_t BSP_STM32_LTDC_EnableLayer(LTDC_TypeDef *hltdc, uint32_t layer) {
	LTDC_Layer_TypeDef *l = (LTDC_Layer_TypeDef *)(((uint32_t)hltdc) + 0x84U + (0x80U*(layer)));

	// Enable layer
	l->CR |= LTDC_LxCR_LEN;

	// Set the Reload on Vertical Blank
	hltdc->SRCR |= LTDC_SRCR_VBR;

	// Wait for layer to enable
	while (!(l->CR & LTDC_LxCR_LEN)) {};

	return BSP_OK;
}

uint8_t BSP_STM32_LTDC_ConfigLayer(LTDC_TypeDef *hltdc, uint32_t layer, uint32_t alpha, uint32_t alpha0, uint32_t bgcolor,
		uint32_t blendingfactor1, uint32_t blendingfactor2, uint32_t fbstartaddress, uint32_t imgheight, uint32_t imgwidth,
		uint32_t pixelformat, uint32_t x0, uint32_t x1, uint32_t y0, uint32_t y1) {

	LTDC_Layer_TypeDef *l = (LTDC_Layer_TypeDef *)(((uint32_t)hltdc) + 0x84U + (0x80U*(layer)));
	uint32_t tmp;

	// Configure the horizontal start and stop position
	tmp = ((x1 + ((hltdc->BPCR & LTDC_BPCR_AHBP) >> 16U)) << 16U);
	l->WHPCR &= ~(LTDC_LxWHPCR_WHSTPOS | LTDC_LxWHPCR_WHSPPOS);
	l->WHPCR = ((x0 + ((hltdc->BPCR & LTDC_BPCR_AHBP) >> 16U) + 1U) | tmp);

	// Configure the vertical start and stop position
	tmp = ((y1 + (hltdc->BPCR & LTDC_BPCR_AVBP)) << 16U);
	l->WVPCR &= ~(LTDC_LxWVPCR_WVSTPOS | LTDC_LxWVPCR_WVSPPOS);
	l->WVPCR  = ((y0 + (hltdc->BPCR & LTDC_BPCR_AVBP) + 1U) | tmp);

	// Specifies the pixel format
	l->PFCR &= ~(LTDC_LxPFCR_PF);
	l->PFCR = pixelformat;

	// Configure the default color values
	l->DCCR &= ~(LTDC_LxDCCR_DCBLUE | LTDC_LxDCCR_DCGREEN | LTDC_LxDCCR_DCRED | LTDC_LxDCCR_DCALPHA);
	l->DCCR = bgcolor;

	// Specifies the constant alpha value
	l->CACR &= ~(LTDC_LxCACR_CONSTA);
	l->CACR = alpha;

	// Specifies the blending factors
	l->BFCR &= ~(LTDC_LxBFCR_BF2 | LTDC_LxBFCR_BF1);
	l->BFCR = (blendingfactor1 | blendingfactor2);

	// Configure the color frame buffer start address
	l->CFBAR &= ~(LTDC_LxCFBAR_CFBADD);
	l->CFBAR = fbstartaddress;

	if (pixelformat == LTDC_PIXEL_FORMAT_ARGB8888) { tmp = 4U; }
	else if (pixelformat == LTDC_PIXEL_FORMAT_RGB888) { tmp = 3U; }
	else if ((pixelformat == LTDC_PIXEL_FORMAT_ARGB4444) || (pixelformat == LTDC_PIXEL_FORMAT_RGB565) || (pixelformat == LTDC_PIXEL_FORMAT_ARGB1555) || (pixelformat == LTDC_PIXEL_FORMAT_AL88)) { tmp = 2U; }
	else { tmp = 1U; }

	// Configure the color frame buffer pitch in byte
	l->CFBLR  &= ~(LTDC_LxCFBLR_CFBLL | LTDC_LxCFBLR_CFBP);
	l->CFBLR  = (((imgwidth * tmp) << 16U) | (((x1 - x0) * tmp)  + 7U));

	// Configure the frame buffer line number
	l->CFBLNR  &= ~(LTDC_LxCFBLNR_CFBLNBR);
	l->CFBLNR  = (imgheight);

	return BSP_OK;
}


uint8_t BSP_STM32_LTDC_UpdateFrameBufAddr(LTDC_TypeDef *hltdc, uint32_t layer, uint32_t fbstartaddress) {

	// Updating Framebuffer address
	LTDC_Layer_TypeDef *l = ((LTDC_Layer_TypeDef *)((uint32_t)(((uint32_t)LTDC) + 0x84U + (0x80U*(layer)))));
	l->CFBAR &= ~(LTDC_LxCFBAR_CFBADD);
	l->CFBAR = fbstartaddress;
	hltdc->SRCR |= LTDC_SRCR_VBR;

	return BSP_OK;
}


uint8_t BSP_STM32_LTDC_ConfigCLUT(LTDC_TypeDef *hltdc, uint32_t layer, uint32_t *clut) {

	LTDC_Layer_TypeDef *l = (LTDC_Layer_TypeDef *)(((uint32_t)LTDC) + 0x84U + (0x80U*(layer)));
	uint32_t *pcolorlut = clut;
	uint32_t counter;
	uint32_t tmp;

	// Configure CLUT
	for (counter = 0U; (counter < 256); counter++) {
		tmp  = (counter << 24U) | ((uint32_t)(*pcolorlut) & 0x00FFFFFF);
	    pcolorlut++;
	    l->CLUTWR  = tmp;
	}
	// Enable CLUT
	l->CR |= (uint32_t)LTDC_LxCR_CLUTEN;
	hltdc->SRCR = LTDC_SRCR_IMR;

	// Configure color keying
	l->CKCR &=  ~(LTDC_LxCKCR_CKBLUE | LTDC_LxCKCR_CKGREEN | LTDC_LxCKCR_CKRED);
	l->CKCR  = 0x00000000;
	hltdc->SRCR = LTDC_SRCR_IMR;
	// Enable color keying
	l->CR |= (uint32_t)LTDC_LxCR_COLKEN;
	hltdc->SRCR = LTDC_SRCR_IMR;

	return BSP_OK;
}

uint8_t BSP_STM32_LTDC_SetTransparentColor(LTDC_TypeDef *hltdc, uint32_t layer, uint32_t color) {

	LTDC_Layer_TypeDef *l = (LTDC_Layer_TypeDef *)(((uint32_t)LTDC) + 0x84U + (0x80U*(layer)));

	// Setting transparent color
	l->CKCR  = color;
	hltdc->SRCR = LTDC_SRCR_IMR;
	// Enable color keying
	l->CR |= (uint32_t)LTDC_LxCR_COLKEN;
	hltdc->SRCR = LTDC_SRCR_IMR;

	return BSP_OK;
}

uint8_t BSP_STM32_LTDC_SetLineInt(LTDC_TypeDef *hltdc, uint32_t line) {
	// Programming LTDC line interrupt
	hltdc->IER &= ~(LTDC_IER_LIE);
	hltdc->LIPCR = line;
	hltdc->IER |= LTDC_IER_LIE;
	hltdc->SRCR = LTDC_SRCR_IMR;

	return BSP_OK;
}

uint8_t BSP_STM32_LTDC_IRQHandler(LTDC_TypeDef *hltdc) {
	if (((hltdc->ISR & LTDC_ISR_LIF) != 0U) && ((hltdc->IER & LTDC_IER_LIE) != 0U)) {
	    // Disable the Line interrupt
		hltdc->IER &= ~(LTDC_IER_LIE);
	    // Clear the Line interrupt flag
		hltdc->ICR = LTDC_ICR_CLIF;
	    // Enable the Line interrupt
		hltdc->IER |= (LTDC_IER_LIE);
		return BSP_OK;
	}

	if (((hltdc->ISR & LTDC_ISR_FUIF) != 0U) && ((hltdc->IER & LTDC_IER_FUIE) != 0U)) {
		// Disable the interrupt FIFO Underrun
		hltdc->IER &= ~(LTDC_IER_FUIE);
		// Clear the interrupt flag
		hltdc->ICR = LTDC_ICR_CFUIF;
		// Enable the interrupt
		hltdc->IER |= (LTDC_IER_FUIE);
	}

	if (((hltdc->ISR & LTDC_ISR_RRIF) != 0U) && ((hltdc->IER & LTDC_IER_RRIE) != 0U)) {
		// Disable the interrupt Register Reload
		hltdc->IER &= ~(LTDC_IER_RRIE);
		// Clear the interrupt flag
		hltdc->ICR = LTDC_ICR_CRRIF;
		// Enable the interrupt
		hltdc->IER |= (LTDC_IER_RRIE);
	}

	if (((hltdc->ISR & LTDC_ISR_TERRIF) != 0U) && ((hltdc->IER & LTDC_IER_TERRIE) != 0U)) {
		// Disable the interrupt Transfer Error
		hltdc->IER &= ~(LTDC_IER_TERRIE);
		// Clear the interrupt flag
		hltdc->ICR = LTDC_ICR_CTERRIF;
		// Enable the interrupt
		hltdc->IER |= (LTDC_IER_TERRIE);
	}

	return BSP_ERROR;
}

