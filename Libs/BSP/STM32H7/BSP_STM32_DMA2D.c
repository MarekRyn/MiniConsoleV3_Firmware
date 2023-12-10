/*******************************************************************
 * MiniConsole V3 - Board Support Package - STM32 DMA2D
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 *******************************************************************/

#include "BSP_STM32_DMA2D.h"

uint8_t BSP_STM32_DMA2D_FillBuff(DMA2D_TypeDef * hdma2d, uint32_t colormode, uint16_t width, uint16_t height, uint16_t offsetline, uint32_t dest_addr, uint32_t color) {

	// Configuring DMA2D
	MODIFY_REG(hdma2d->CR, DMA2D_CR_MODE, DMA2D_R2M);
	MODIFY_REG(hdma2d->OPFCCR, DMA2D_OPFCCR_CM, colormode);
	MODIFY_REG(hdma2d->OOR, DMA2D_OOR_LO, (uint32_t)offsetline);
	MODIFY_REG(hdma2d->NLR, (DMA2D_NLR_NL | DMA2D_NLR_PL), ((uint32_t)height | ((uint32_t)width << DMA2D_NLR_PL_Pos)));
	WRITE_REG(hdma2d->OMAR, dest_addr);
	WRITE_REG(hdma2d->OCOLR, color);

	// Enable Interrupts
	hdma2d->CR |= DMA2D_CR_TCIE | DMA2D_CR_TEIE | DMA2D_CR_CEIE;

	// Enable DMA2D
	hdma2d->CR |= DMA2D_CR_START;

	return BSP_OK;
}


uint8_t BSP_STM32_DMA2D_FillBuffBlend(DMA2D_TypeDef * hdma2d, uint32_t colormode, uint16_t width, uint16_t height, uint16_t offsetline, uint32_t dest_addr, uint32_t color, uint8_t alpha) {
	// Configuring DMA2D
	MODIFY_REG(hdma2d->CR, DMA2D_CR_MODE, DMA2D_M2M_BLEND_FG);
	MODIFY_REG(hdma2d->OPFCCR, DMA2D_OPFCCR_CM, colormode);
	MODIFY_REG(hdma2d->OOR, DMA2D_OOR_LO, offsetline);

	uint32_t regValue1 = colormode | (DMA2D_COMBINE_ALPHA << DMA2D_FGPFCCR_AM_Pos) | (alpha << DMA2D_FGPFCCR_ALPHA_Pos);
	uint32_t regMask1  = (DMA2D_FGPFCCR_CM | DMA2D_FGPFCCR_AM | DMA2D_FGPFCCR_ALPHA);
	MODIFY_REG(hdma2d->FGPFCCR, regMask1, regValue1);
	WRITE_REG(hdma2d->FGCOLR, color);

	uint32_t regValue0 = colormode | (DMA2D_NO_MODIF_ALPHA << DMA2D_BGPFCCR_AM_Pos) | (0xFF << DMA2D_BGPFCCR_ALPHA_Pos);
	uint32_t regMask0  = (DMA2D_BGPFCCR_CM | DMA2D_BGPFCCR_AM | DMA2D_BGPFCCR_ALPHA);
	MODIFY_REG(hdma2d->BGPFCCR, regMask0, regValue0);
	WRITE_REG(hdma2d->BGOR, offsetline);

	WRITE_REG(hdma2d->BGMAR, dest_addr);
	MODIFY_REG(hdma2d->NLR, (DMA2D_NLR_NL | DMA2D_NLR_PL), (height | (width << DMA2D_NLR_PL_Pos)));
	WRITE_REG(hdma2d->OMAR, dest_addr);

	// Enable Interrupts
	hdma2d->CR |= DMA2D_CR_TCIE | DMA2D_CR_TEIE | DMA2D_CR_CEIE;
	// ENable DMA2D
	hdma2d->CR |= DMA2D_CR_START;

	return BSP_OK;
}


uint8_t BSP_STM32_DMA2D_CopyBuf(DMA2D_TypeDef * hdma2d, uint32_t colormode, uint16_t width, uint16_t height, uint16_t src_offsetline, uint32_t src_addr, uint16_t dest_offsetline, uint32_t dest_addr) {

	// Configuring DMA2D
	MODIFY_REG(hdma2d->CR, DMA2D_CR_MODE, DMA2D_M2M);
	MODIFY_REG(hdma2d->OPFCCR, DMA2D_OPFCCR_CM, colormode);
	MODIFY_REG(hdma2d->OOR, DMA2D_OOR_LO, dest_offsetline);
	uint32_t regValue = colormode | (DMA2D_NO_MODIF_ALPHA << DMA2D_FGPFCCR_AM_Pos) | (0xFF << DMA2D_FGPFCCR_ALPHA_Pos);
	uint32_t regMask  = (DMA2D_FGPFCCR_CM | DMA2D_FGPFCCR_AM | DMA2D_FGPFCCR_ALPHA);
	MODIFY_REG(hdma2d->FGPFCCR, regMask, regValue);
	WRITE_REG(hdma2d->FGOR, src_offsetline);

	MODIFY_REG(hdma2d->NLR, (DMA2D_NLR_NL | DMA2D_NLR_PL), (height | (width << DMA2D_NLR_PL_Pos)));
	WRITE_REG(hdma2d->OMAR, dest_addr);
	WRITE_REG(hdma2d->FGMAR, src_addr);

	// Enable Interrupts
	hdma2d->CR |= DMA2D_CR_TCIE | DMA2D_CR_TEIE | DMA2D_CR_CEIE;
	// ENable DMA2D
	hdma2d->CR |= DMA2D_CR_START;

	return BSP_OK;
}

uint8_t BSP_STM32_DMA2D_CopyBufJPEG(DMA2D_TypeDef * hdma2d, uint32_t colormode, uint16_t width, uint16_t height, uint16_t src_offsetline, uint32_t src_addr, uint16_t dest_offsetline, uint32_t dest_addr, uint32_t chroma) {

	// Configuring DMA2D
	MODIFY_REG(hdma2d->CR, DMA2D_CR_MODE, DMA2D_M2M_PFC);
	MODIFY_REG(hdma2d->OPFCCR, DMA2D_OPFCCR_CM, colormode);
	MODIFY_REG(hdma2d->OOR, DMA2D_OOR_LO, dest_offsetline);

	uint32_t regValue = DMA2D_YCBCR | (DMA2D_REPLACE_ALPHA << DMA2D_FGPFCCR_AM_Pos) | (0xFF << DMA2D_FGPFCCR_ALPHA_Pos) | (chroma << DMA2D_FGPFCCR_CSS_Pos);
	uint32_t regMask  = (DMA2D_FGPFCCR_CM | DMA2D_FGPFCCR_AM | DMA2D_FGPFCCR_ALPHA | DMA2D_FGPFCCR_CSS);
	MODIFY_REG(hdma2d->FGPFCCR, regMask, regValue);
	WRITE_REG(hdma2d->FGOR, src_offsetline);

	MODIFY_REG(hdma2d->NLR, (DMA2D_NLR_NL | DMA2D_NLR_PL), (height | (width << DMA2D_NLR_PL_Pos)));
	WRITE_REG(hdma2d->OMAR, dest_addr);
	WRITE_REG(hdma2d->FGMAR, src_addr);

	// Enable Interrupts
	hdma2d->CR |= DMA2D_CR_TCIE | DMA2D_CR_TEIE | DMA2D_CR_CEIE;
	// ENable DMA2D
	hdma2d->CR |= DMA2D_CR_START;

	return BSP_OK;
}

uint8_t BSP_STM32_DMA2D_CopyBufBlend(DMA2D_TypeDef * hdma2d, uint32_t colormode, uint16_t width, uint16_t height, uint16_t src_offsetline, uint32_t src_addr, uint16_t dest_offsetline, uint32_t dest_addr, uint8_t alpha) {

	// Configuring DMA2D
	MODIFY_REG(hdma2d->CR, DMA2D_CR_MODE, DMA2D_M2M_BLEND);
	MODIFY_REG(hdma2d->OPFCCR, DMA2D_OPFCCR_CM, colormode);
	MODIFY_REG(hdma2d->OOR, DMA2D_OOR_LO, dest_offsetline);

	uint32_t regValue1 = colormode | (DMA2D_COMBINE_ALPHA << DMA2D_FGPFCCR_AM_Pos) | (alpha << DMA2D_FGPFCCR_ALPHA_Pos);
	uint32_t regMask1  = (DMA2D_FGPFCCR_CM | DMA2D_FGPFCCR_AM | DMA2D_FGPFCCR_ALPHA);
	MODIFY_REG(hdma2d->FGPFCCR, regMask1, regValue1);
	WRITE_REG(hdma2d->FGOR, src_offsetline);

	uint32_t regValue0 = colormode | (DMA2D_NO_MODIF_ALPHA << DMA2D_BGPFCCR_AM_Pos) | (0xFF << DMA2D_BGPFCCR_ALPHA_Pos);
	uint32_t regMask0  = (DMA2D_BGPFCCR_CM | DMA2D_BGPFCCR_AM | DMA2D_BGPFCCR_ALPHA);
	MODIFY_REG(hdma2d->BGPFCCR, regMask0, regValue0);
	WRITE_REG(hdma2d->BGOR, dest_offsetline);

	WRITE_REG(hdma2d->BGMAR, dest_addr);
	MODIFY_REG(hdma2d->NLR, (DMA2D_NLR_NL | DMA2D_NLR_PL), (height | (width << DMA2D_NLR_PL_Pos)));
	WRITE_REG(hdma2d->OMAR, dest_addr);
	WRITE_REG(hdma2d->FGMAR, src_addr);

	// Enable Interrupts
	hdma2d->CR |= DMA2D_CR_TCIE | DMA2D_CR_TEIE | DMA2D_CR_CEIE;
	// ENable DMA2D
	hdma2d->CR |= DMA2D_CR_START;

	return BSP_OK;
}


uint8_t BSP_STM32_DMA2D_IRQHandler(DMA2D_TypeDef * hdma2d) {
	if (((hdma2d->ISR & DMA2D_ISR_TCIF) > 0) &&  ((hdma2d->CR & DMA2D_CR_TCIE) > 0)) {

		// Disable the transfer complete interrupt
		hdma2d->CR &= ~(DMA2D_CR_TCIE);

		// Clear the transfer complete flag
		hdma2d->IFCR = DMA2D_IFCR_CTCIF;

		return BSP_OK;
	}

	if (((hdma2d->ISR & DMA2D_ISR_TEIF) > 0) && ((hdma2d->CR & DMA2D_CR_TEIE) > 0)) {

		// Disable the transfer error interrupt
		hdma2d->CR &= ~(DMA2D_CR_TEIE);

		// Clear the transfer error flag
		hdma2d->IFCR = DMA2D_IFCR_CTEIF;

		return BSP_ERROR;
	}

	if (((hdma2d->ISR & DMA2D_ISR_CEIF) > 0) && ((hdma2d->CR & DMA2D_CR_CEIE) > 0)) {

		// Disable the configuration error interrupt
		hdma2d->CR &= ~(DMA2D_CR_CEIE);

		// Clear the configuration error flag
		hdma2d->IFCR = DMA2D_IFCR_CCEIF;

		return BSP_ERROR;
	}
	return BSP_ERROR;
}
