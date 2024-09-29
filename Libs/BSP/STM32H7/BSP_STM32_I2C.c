/*******************************************************************
 * MiniConsole V3 - Board Support Package - STM32 I2C
 *******************************************************************/


#include "BSP_STM32_I2C.h"


/*******************************************************************
* Private Functions
 *******************************************************************/

static uint8_t _I2C_MemReadIT(I2C_TypeDef *hi2c, TxRxContext_TypeDef *ctx, uint16_t DevAddress, uint8_t MemAddress, uint8_t *pData, uint16_t Size) {

	#define I2C_NO_OPTION_FRAME     (0xFFFF0000U)

	I2CContext_TypeDef * i2c_ctx = (I2CContext_TypeDef *)ctx->ctxmem;

	i2c_ctx->pData = pData;
	i2c_ctx->size = Size;
	i2c_ctx->index = 0xFFFFFFFFU;
	i2c_ctx->option = I2C_NO_OPTION_FRAME;
	i2c_ctx->devaddr = DevAddress;

	// Assuming that memory address size is 8Bit (only use case in this project)
	hi2c->TXDR = MemAddress;

	// Declaration of tmp to prevent undefined behavior of volatile usage
	uint32_t tmp = ((uint32_t)(((uint32_t)DevAddress & I2C_CR2_SADD) | ((1 << I2C_CR2_NBYTES_Pos) & I2C_CR2_NBYTES) | I2C_SOFTEND_MODE | I2C_GENERATE_START_WRITE) & (~0x80000000U));

	// Update CR2 register
	MODIFY_REG(hi2c->CR2, ((I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_AUTOEND | (I2C_CR2_RD_WRN & (uint32_t)(I2C_GENERATE_START_WRITE >> (31U - I2C_CR2_RD_WRN_Pos))) | I2C_CR2_START | I2C_CR2_STOP)), tmp);

	// Enable I2C Interrupts
	hi2c->CR1 |= I2C_CR1_ERRIE | I2C_CR1_TCIE | I2C_CR1_STOPIE | I2C_CR1_NACKIE | I2C_CR1_TXIE | I2C_CR1_RXIE;

	return BSP_OK;
}


static uint8_t _I2C_MemReadDMA(I2C_TypeDef *hi2c, TxRxContext_TypeDef *ctx, uint16_t DevAddress, uint8_t MemAddress, uint8_t *pData, uint16_t Size) {

	#define I2C_NO_OPTION_FRAME     (0xFFFF0000U)

	I2CContext_TypeDef * i2c_ctx = (I2CContext_TypeDef *)ctx->ctxmem;

	i2c_ctx->pData = pData;
	i2c_ctx->size = Size;
	i2c_ctx->index = 0xFFFFFFFFU;
	i2c_ctx->option = I2C_NO_OPTION_FRAME;
	i2c_ctx->devaddr = DevAddress;

	// Assuming that memory address size is 8Bit (only use case in this project)
	hi2c->TXDR = MemAddress;

	// Enable the DMA stream or channel depends on Instance
	switch (ctx->dma_mode) {
	case TXRX_CTX_MODE_DMA:
		BSP_STM32_DMA_StartIT(ctx, (uint32_t)&hi2c->RXDR, (uint32_t)pData, Size);
		break;
	case TXRX_CTX_MODE_BDMA:
		BSP_BDMA_StartIT(ctx, (uint32_t)&hi2c->RXDR, (uint32_t)pData, Size);
		break;
	}


	// Declaration of tmp to prevent undefined behavior of volatile usage
	uint32_t tmp = ((uint32_t)(((uint32_t)DevAddress & I2C_CR2_SADD) | ((1 << I2C_CR2_NBYTES_Pos) & I2C_CR2_NBYTES) | I2C_SOFTEND_MODE | I2C_GENERATE_START_WRITE) & (~0x80000000U));

	// Update CR2 register
	MODIFY_REG(hi2c->CR2, ((I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_AUTOEND | (I2C_CR2_RD_WRN & (uint32_t)(I2C_GENERATE_START_WRITE >> (31U - I2C_CR2_RD_WRN_Pos))) | I2C_CR2_START | I2C_CR2_STOP)), tmp);

	// Enable I2C Interrupts
	hi2c->CR1 |= I2C_CR1_ERRIE | I2C_CR1_TCIE | I2C_CR1_STOPIE | I2C_CR1_NACKIE | I2C_CR1_TXIE;

	return BSP_OK;
}


static uint8_t _I2C_IRQHandler_IT(I2C_TypeDef *hi2c, TxRxContext_TypeDef *ctx) {

	I2CContext_TypeDef * i2c_ctx = (I2CContext_TypeDef *)ctx->ctxmem;

	// Get current IT Flags and IT sources value
	uint32_t itflags   = hi2c->ISR;
	uint32_t itsources = hi2c->CR1;

	uint32_t direction = (uint32_t)(0x80000000U | I2C_CR2_START); //Generate start write

	// Handling errors
	if (((itflags & I2C_ISR_NACKF) > 0) && ((itsources & I2C_CR1_NACKIE) > 0)) {
	    // Clear NACK Flag
	    hi2c->ICR = I2C_ICR_NACKCF;

	    // Flush TX register
	    // If a pending TXIS flag is set
	    // Write a dummy data in TXDR to clear it
	    if ((hi2c->ISR & I2C_ISR_TXIS) > 0) hi2c->TXDR = 0x00U;

	    // Flush TX register if not empty
	    if ((hi2c->ISR & I2C_ISR_TXE) == 0) hi2c->ISR |= I2C_ISR_TXE;

	    return BSP_ERROR;
	}

	// Initiate reading from memory
	if (((itflags & I2C_ISR_TC) > 0) && ((itsources & I2C_CR1_TCIE) > 0)) {

		direction = (uint32_t)(0x80000000U | I2C_CR2_START | I2C_CR2_RD_WRN);

		// Set NBYTES to write and generate RESTART
	    uint32_t tmp = ((uint32_t)(((uint32_t)i2c_ctx->devaddr & I2C_CR2_SADD) | (((uint32_t)i2c_ctx->size << I2C_CR2_NBYTES_Pos) & I2C_CR2_NBYTES) | (uint32_t)I2C_AUTOEND_MODE | (uint32_t)direction) & (~0x80000000U));
	    MODIFY_REG(hi2c->CR2, ((I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_AUTOEND | (I2C_CR2_RD_WRN & (uint32_t)(direction >> (31U - I2C_CR2_RD_WRN_Pos))) | I2C_CR2_START | I2C_CR2_STOP)), tmp);
	}

	// Read one byte from memory
	if (((itflags & I2C_ISR_RXNE) > 0) && ((itsources & I2C_CR1_RXIE) > 0)) {

		// Clear the flag
	    // Read data from RXDR
	    *i2c_ctx->pData = (uint8_t)hi2c->RXDR;

	    // Increment Buffer pointer
	    i2c_ctx->pData++;
	    i2c_ctx->size--;
	}

	// Completion of reading
	if (((itflags & I2C_ISR_STOPF) > 0) && ((itsources & I2C_CR1_STOPIE) > 0)) {

		// Clear the flag
		hi2c->ICR = I2C_ICR_STOPCF;

		return BSP_OK;
	}

	return BSP_BUSY;
}


static uint8_t _I2C_IRQHandler_DMA(I2C_TypeDef *hi2c, TxRxContext_TypeDef *ctx) {

	I2CContext_TypeDef * i2c_ctx = (I2CContext_TypeDef *)ctx->ctxmem;

	// Get current IT Flags and IT sources value
	uint32_t itflags   = hi2c->ISR;
	uint32_t itsources = hi2c->CR1;

	uint32_t direction = (uint32_t)(0x80000000U | I2C_CR2_START); //Generate start write

	// Handling errors
	if (((itflags & I2C_ISR_NACKF) > 0) && ((itsources & I2C_CR1_NACKIE) > 0)) {
	    // Clear NACK Flag
	    hi2c->ICR = I2C_ICR_NACKCF;

	    // Flush TX register
	    // If a pending TXIS flag is set
	    // Write a dummy data in TXDR to clear it
	    if ((hi2c->ISR & I2C_ISR_TXIS) > 0) hi2c->TXDR = 0x00U;

	    // Flush TX register if not empty
	    if ((hi2c->ISR & I2C_ISR_TXE) == 0) hi2c->ISR |= I2C_ISR_TXE;

	    return BSP_ERROR;
	}

	// Initiate reading from memory
	if (((itflags & I2C_ISR_TC) > 0) && ((itsources & I2C_CR1_TCIE) > 0)) {

		direction = I2C_GENERATE_START_READ;

		uint32_t tmp = ((uint32_t)(((uint32_t)i2c_ctx->devaddr & I2C_CR2_SADD) | (((uint32_t)i2c_ctx->size << I2C_CR2_NBYTES_Pos) & I2C_CR2_NBYTES) | (uint32_t)I2C_AUTOEND_MODE | (uint32_t)direction) & (~0x80000000U));
		MODIFY_REG(hi2c->CR2, ((I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_AUTOEND | (I2C_CR2_RD_WRN & (uint32_t)(direction >> (31U - I2C_CR2_RD_WRN_Pos))) | I2C_CR2_START | I2C_CR2_STOP)), tmp);

		/* Enable DMA Request */
		hi2c->CR1 |= I2C_CR1_RXDMAEN;
	}


	// Completion of reading
	if (((itflags & I2C_ISR_STOPF) > 0) && ((itsources & I2C_CR1_STOPIE) > 0)) {

		// Clear the flag
		hi2c->ICR = I2C_ICR_STOPCF;

		return BSP_OK;
	}

	return BSP_BUSY;
}


/*******************************************************************
* I2C Functions
 *******************************************************************/

uint8_t BSP_STM32_I2C_Init(I2C_TypeDef *hi2c) {
	#define TIMING_CLEAR_MASK   (0xF0FFFFFFU)

	// Disable I2C periph
	CLEAR_BIT(hi2c->CR1, I2C_CR1_PE);

	// Configure I2Cx: Frequency range
	hi2c->TIMINGR = 0x009034B6 & TIMING_CLEAR_MASK; // 400kHz - High Speed I2C

	// Disable Own Address1
	hi2c->OAR1 &= ~I2C_OAR1_OA1EN;

	// Disable Own Address2
	hi2c->OAR2 &= ~I2C_OAR2_OA2EN;

	// Enable the AUTOEND and NACK
	hi2c->CR2 |= (I2C_CR2_AUTOEND | I2C_CR2_NACK);

	// Configure I2C Generalcall and NoStretch mode
	hi2c->CR1 = (I2C_GENERALCALL_DISABLE | I2C_NOSTRETCH_DISABLE);

	// Configure analog filter
	hi2c->CR1 &= ~(I2C_CR1_ANFOFF);
	hi2c->CR1 |= I2C_ANALOGFILTER_ENABLE;

	// Enable I2C periph
	SET_BIT(hi2c->CR1, I2C_CR1_PE);

	return BSP_OK;
}


uint8_t BSP_STM32_I2C_IsDeviceReady(I2C_TypeDef *hi2c, uint32_t device_address, uint32_t maxtrials, uint32_t timeout) {

	uint32_t tickstart = 0;

	__IO uint32_t trials = 0UL;

	uint8_t tmp1 = 0;
	uint8_t tmp2 = 0;

	// No need to Check TC flag, with AUTOEND mode the stop is automatically generated. Wait until STOPF flag is set or a NACK flag is set.
	tickstart = BSP_GetTick();

    while (trials < maxtrials) {
		// Generate Start
		hi2c->CR2 = ((((uint32_t)(device_address) & (I2C_CR2_SADD)) | (I2C_CR2_START) | (I2C_CR2_AUTOEND)) & (~I2C_CR2_RD_WRN));

		tmp1 = hi2c->ISR & I2C_ISR_STOPF;
		tmp2 = hi2c->ISR & I2C_ISR_NACKF;

		while ((tmp1 == 0) && (tmp2 == 0)) {

			if ((BSP_GetTick() - tickstart) > timeout) return BSP_ERROR;
			tmp1 = hi2c->ISR & I2C_ISR_STOPF;
			tmp2 = hi2c->ISR & I2C_ISR_NACKF;
		}

	    // Check if the NACKF flag has not been set
	    if ((hi2c->ISR & I2C_ISR_NACKF) == 0) {

	    	// Wait until STOPF flag is reset
	    	while ((hi2c->ISR & I2C_ISR_STOPF) == 0) if ((BSP_GetTick() - tickstart) > timeout) return BSP_ERROR;

	        // Clear STOP Flag
	        hi2c->ICR = I2C_ICR_STOPCF;

	        return BSP_OK;

	    } else {

	    	// Wait until STOPF flag is reset
	    	while ((hi2c->ISR & I2C_ISR_STOPF) == 0) if ((BSP_GetTick() - tickstart) > timeout) return BSP_ERROR;

	        // Clear NACK Flag
	    	hi2c->ICR = I2C_ICR_NACKCF;

	        // Clear STOP Flag, auto generated with autoend
	    	hi2c->ICR = I2C_ICR_STOPCF;
	    }

	    // Increment Trials
	    trials++;
    }

    // Generate Stop
    hi2c->CR2 |= I2C_CR2_STOP;

    // Wait until STOPF flag is reset
    while ((hi2c->ISR & I2C_ISR_STOPF) == 0) if ((BSP_GetTick() - tickstart) > timeout) return BSP_ERROR;

    // Clear STOP Flag
    hi2c->ICR = I2C_ICR_STOPCF;

    return BSP_ERROR;
}


uint8_t BSP_STM32_I2C_RegWrite(I2C_TypeDef *hi2c, uint16_t DevAddress, uint8_t RegAddress, uint8_t RegValue, uint32_t Timeout) {

	uint32_t tickstart = BSP_GetTick();
	uint32_t tmp = 0;

	// Wait for BUSY flag to reset
	while ((hi2c->ISR & I2C_ISR_BUSY) > 0) if ((BSP_GetTick() - tickstart) > Timeout) return BSP_ERROR;

	// Config transfer
	tmp = ((uint32_t)(((uint32_t)DevAddress & I2C_CR2_SADD) | (((uint32_t)2 << I2C_CR2_NBYTES_Pos) & I2C_CR2_NBYTES) | (uint32_t)I2C_AUTOEND_MODE | (uint32_t)I2C_GENERATE_START_WRITE) & (~0x80000000U));
	MODIFY_REG(hi2c->CR2, ((I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_AUTOEND | (I2C_CR2_RD_WRN & (uint32_t)(I2C_GENERATE_START_WRITE >> (31U - I2C_CR2_RD_WRN_Pos))) | I2C_CR2_START | I2C_CR2_STOP)), tmp);

	while ((hi2c->ISR & I2C_ISR_TXIS) == 0) if ((BSP_GetTick() - tickstart) > Timeout) return BSP_ERROR;

	hi2c->TXDR = RegAddress;

	while ((hi2c->ISR & I2C_ISR_TXIS) == 0) if ((BSP_GetTick() - tickstart) > Timeout) return BSP_ERROR;

	hi2c->TXDR = RegValue;

	//while ((hi2c->ISR & I2C_FLAG_TCR) == 0) if ((BSP_GetTick() - tickstart) > Timeout) return BSP_ERROR;

	// Wait for Stop flag
	while ((hi2c->ISR & I2C_ISR_STOPF) == 0) if ((BSP_GetTick() - tickstart) > Timeout) return BSP_ERROR;

	// Clear Stop Flag
	hi2c->ICR = I2C_ICR_STOPCF;

	hi2c->CR2 &= (uint32_t)~((uint32_t)(I2C_CR2_SADD | I2C_CR2_HEAD10R | I2C_CR2_NBYTES | I2C_CR2_RELOAD  | I2C_CR2_RD_WRN));


	return BSP_OK;
}


uint8_t BSP_STM32_I2C_MemRead(I2C_TypeDef *hi2c, TxRxContext_TypeDef *ctx, uint16_t DevAddress, uint8_t MemAddress, uint8_t *pData, uint16_t Size) {
	switch (ctx->dma_mode) {
	case TXRX_CTX_MODE_IT:
		return _I2C_MemReadIT(hi2c, ctx, DevAddress, MemAddress, pData, Size);
	case TXRX_CTX_MODE_DMA:
	case TXRX_CTX_MODE_BDMA:
		return _I2C_MemReadDMA(hi2c, ctx, DevAddress, MemAddress, pData, Size);
	}
	return _I2C_MemReadIT(hi2c, ctx, DevAddress, MemAddress, pData, Size);
}


uint8_t BSP_STM32_I2C_IRQHandler(I2C_TypeDef *hi2c, TxRxContext_TypeDef *ctx) {
	switch (ctx->dma_mode) {
	case TXRX_CTX_MODE_IT:
		return _I2C_IRQHandler_IT(hi2c, ctx);
	case TXRX_CTX_MODE_DMA:
	case TXRX_CTX_MODE_BDMA:
		return _I2C_IRQHandler_DMA(hi2c, ctx);
	}
	return _I2C_IRQHandler_IT(hi2c, ctx);
}


