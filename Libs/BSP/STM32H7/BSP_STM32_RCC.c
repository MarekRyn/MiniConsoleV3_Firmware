/*******************************************************************
 * MiniConsole V3 - Board Support Package - STM32 RCC
 *
 * Author: Marek Ryn
 * Version: 1.0
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 * - 1.0	- Initial version
 *******************************************************************/

#include "BSP_STM32_RCC.h"

// Setting RTC clock source to external LSE
uint8_t BSP_STM32_RCC_CLKConfig_RTC(void) {

	uint32_t tickstart = 0;
	uint32_t tmpreg = 0;

	// Enable write access to Backup domain
	SET_BIT(PWR->CR1, PWR_CR1_DBP);

	// Wait for Backup domain Write protection disable
	tickstart = BSP_GetTick();

	while ((PWR->CR1 & PWR_CR1_DBP) == 0) if ((BSP_GetTick() - tickstart) > 100) return BSP_ERROR;

	// Reset the Backup domain only if the RTC Clock source selection is modified
	if ((RCC->BDCR & RCC_BDCR_RTCSEL) != RCC_RTCCLKSOURCE_LSE) {

		// Store the content of BDCR register before the reset of Backup Domain
	    tmpreg = (RCC->BDCR & ~(RCC_BDCR_RTCSEL));

	    // RTC Clock selection can be changed only if the Backup Domain is reset
	    __BSP_RCC_BACKUPRESET_FORCE();
	    __BSP_RCC_BACKUPRESET_RELEASE();

	    // Restore the Content of BDCR register
	    RCC->BDCR = tmpreg;
	}

	// Wait for LSE reactivation
    tickstart = BSP_GetTick();

	while ((RCC->BDCR & RCC_BDCR_LSERDY) == 0U) if ((BSP_GetTick() - tickstart) > 5000) return BSP_ERROR;

	__BSP_RCC_RTC_CONFIG(RCC_RTCCLKSOURCE_LSE);

	return BSP_OK;
}


// Setting FMC clock source to HCLK
uint8_t BSP_STM32_RCC_CLKConfig_FMC(void) {
	// This is default configuration. Nothing to change.
	return BSP_OK;
}

// Setting I2C1 clock source to PCLK1
uint8_t BSP_STM32_RCC_CLKConfig_I2C1(void) {
	MODIFY_REG(RCC->D2CCIP2R, RCC_D2CCIP2R_I2C123SEL, RCC_I2C123CLKSOURCE_D2PCLK1);
	return BSP_OK;
}

// Setting I2C4 clock source to PCLK1
uint8_t BSP_STM32_RCC_CLKConfig_I2C4(void) {
	MODIFY_REG(RCC->D3CCIPR, RCC_D3CCIPR_I2C4SEL, RCC_I2C4CLKSOURCE_D3PCLK1);
	return BSP_OK;
}

// Setting I2S3 clock source to PLL2
uint8_t BSP_STM32_RCC_CLKConfig_I2S3(void) {

	uint32_t tickstart = 0;

	// Output Freq = ((25MHz / M) * (N + fracN/8192)) / P
	// Setup audio frequency: ((25000000 / 5) * (151 + 2299/8192)) / 67 = 11.2896 MHz
	// 11289600 / 256  = 44100 Hz
	// 11289600 / 512  = 22050 Hz
	// 11289600 / 1024 = 11025 Hz

	uint32_t PLL2M = 5;
	uint32_t PLL2N = 151;
	uint32_t PLL2P = 67;
	uint32_t PLL2Q = 2; // Not used
	uint32_t PLL2R = 2; // Not used
	uint32_t PLL2RGE = RCC_PLL2VCIRANGE_2;
	uint32_t PLL2VCOSEL = RCC_PLL2VCOWIDE;
	uint32_t PLL2FRACN = 2299;

	// Check that PLL2 OSC clock source is already set
	if ((RCC->PLLCKSELR & RCC_PLLCKSELR_PLLSRC) == RCC_PLLSOURCE_NONE) return BSP_ERROR;

	// Disable  PLL2
	CLEAR_BIT(RCC->CR, RCC_CR_PLL2ON);

	// Wait till PLL is disabled
	tickstart = BSP_GetTick();

	while ((RCC->CR & RCC_CR_PLL2RDY) != 0U) if ((BSP_GetTick() - tickstart) > 2) return BSP_ERROR;

    // Configure PLL2 multiplication and division factors. */
	MODIFY_REG(RCC->PLLCKSELR, ( RCC_PLLCKSELR_DIVM2) , ( (PLL2M) <<12U));
	WRITE_REG(RCC->PLL2DIVR , ( (((PLL2N) - 1U ) & RCC_PLL2DIVR_N2) | ((((PLL2P) -1U ) << 9U) & RCC_PLL2DIVR_P2) | ((((PLL2Q) -1U) << 16U) & RCC_PLL2DIVR_Q2) | ((((PLL2R)- 1U) << 24U) & RCC_PLL2DIVR_R2)));

	// Select PLL2 input reference frequency range: VCI
	MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLL2RGE, (PLL2RGE));

	// Select PLL2 output frequency range : VCO
	MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLL2VCOSEL, (PLL2VCOSEL));

	// Disable PLL2FRACN
	CLEAR_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLL2FRACEN);

	// Configures PLL2 clock Fractional Part Of The Multiplication Factor
	MODIFY_REG(RCC->PLL2FRACR, RCC_PLL2FRACR_FRACN2,(PLL2FRACN << RCC_PLL2FRACR_FRACN2_Pos));

	// Enable PLL2FRACN
	SET_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLL2FRACEN);

	// Enable the PLL2 clock output
    RCC->PLLCFGR |= (0x1UL << RCC_PLLCFGR_DIVP2EN_Pos);

    // Enable  PLL2
    SET_BIT(RCC->CR, RCC_CR_PLL2ON);

    // Wait till PLL2 is ready
    tickstart = BSP_GetTick();
	while ((RCC->CR & RCC_CR_PLL2RDY) == 0U) if ((BSP_GetTick() - tickstart) > 2) return BSP_ERROR;

	// Set the source of SPI3 peripheral
	MODIFY_REG(RCC->D2CCIP1R, RCC_D2CCIP1R_SPI123SEL, RCC_SPI123CLKSOURCE_PLL2);

	return BSP_OK;
}


// Setting QSPI clock source to D1HCLK
uint8_t BSP_STM32_RCC_CLKConfig_QSPI(void) {
	MODIFY_REG(RCC->D1CCIPR, RCC_D1CCIPR_QSPISEL, RCC_QSPICLKSOURCE_D1HCLK);
	return BSP_OK;
}


// Setting SDMMC2 clock source to PLL
uint8_t BSP_STM32_RCC_CLKConfig_SDMMC2(void) {
	RCC->PLLCFGR |= RCC_PLLCFGR_DIVQ1EN;
	MODIFY_REG(RCC->D1CCIPR, RCC_D1CCIPR_SDMMCSEL, RCC_SDMMCCLKSOURCE_PLL);
	return BSP_OK;
}


// Setting UART5 clock source to D2PCLK1
uint8_t BSP_STM32_RCC_CLKConfig_UART5(void) {
	MODIFY_REG(RCC->D2CCIP2R, RCC_D2CCIP2R_USART28SEL, RCC_USART234578CLKSOURCE_D2PCLK1);
	return BSP_OK;
}


// Setting UART5 clock source to D2PCLK1
uint8_t BSP_STM32_RCC_CLKConfig_USART2(void) {
	MODIFY_REG(RCC->D2CCIP2R, RCC_D2CCIP2R_USART28SEL, RCC_USART234578CLKSOURCE_D2PCLK1);
	return BSP_OK;
}

// Setting USBFS clock source to_HSI48
uint8_t BSP_STM32_RCC_CLKConfig_USBFS(void) {
	MODIFY_REG(RCC->D2CCIP2R, RCC_D2CCIP2R_USBSEL, RCC_USBCLKSOURCE_HSI48);
	return BSP_OK;
}

// Setting ADC clock source to PLL3
uint8_t BSP_STM32_RCC_CLKConfig_ADC(void) {
	MODIFY_REG(RCC->D3CCIPR, RCC_D3CCIPR_ADCSEL, RCC_ADCCLKSOURCE_PLL3);
	return BSP_OK;
}

// Setting LTDC clock source to PLL3
uint8_t	BSP_STM32_RCC_CLKConfig_LTDC(void) {
	// Nothing to do. PLL3 is default and only clock source for LTDC.
	return BSP_OK;
}

// Setting the PLL clock source
uint8_t BSP_STM32_RCC_PLLClockSource(uint32_t pllsource) {
	MODIFY_REG(RCC->PLLCKSELR, RCC_PLLCKSELR_PLLSRC, pllsource);
	return BSP_OK;
}

// Setting LSE drive capability
uint8_t BSP_STM32_RCC_LSEConfigDrive(uint32_t lsedrive) {
	MODIFY_REG(RCC->BDCR, RCC_BDCR_LSEDRV, lsedrive);
	return BSP_OK;
}

// Configure HSE Oscillator
uint8_t BSP_STM32_RCC_ConfigHSE(uint32_t hsestate) {

	uint32_t tickstart = 0;

	// Getting system clock source
    uint32_t temp_sysclksrc = RCC->CFGR & RCC_CFGR_SWS;
    uint32_t temp_pllckselr = RCC->PLLCKSELR;

    // When the HSE is used as system clock or clock source for PLL in these cases HSE will not be disabled
    if ((temp_sysclksrc == RCC_CFGR_SWS_HSE) || ((temp_sysclksrc == RCC_CFGR_SWS_PLL1) && ((temp_pllckselr & RCC_PLLCKSELR_PLLSRC) == RCC_PLLCKSELR_PLLSRC_HSE))) {

    	if (((RCC->CR & RCC_CR_HSERDY) != 0U) && (hsestate == RCC_HSE_OFF)) return BSP_ERROR;

    } else {

    	// Set the new HSE configuration
    	switch (hsestate) {
    	case RCC_HSE_ON:
    		SET_BIT(RCC->CR, RCC_CR_HSEON);
    		break;
    	case RCC_HSE_OFF:
    		CLEAR_BIT(RCC->CR, RCC_CR_HSEON);
    		CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP);
    		break;
    	case RCC_HSE_BYPASS:
    		SET_BIT(RCC->CR, RCC_CR_HSEBYP);
    		SET_BIT(RCC->CR, RCC_CR_HSEON);
    		break;
    	default:
    		CLEAR_BIT(RCC->CR, RCC_CR_HSEON);
    		CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP);
    	}

    	// Check the HSE State

    	tickstart = BSP_GetTick();
    	if (hsestate != RCC_HSE_OFF) {

    		// Wait till HSE is ready
    		while ((RCC->CR & RCC_CR_HSERDY) == 0U) if ((BSP_GetTick() - tickstart) > 100) return BSP_TIMEOUT;

    	} else {

    		// Wait till HSE is disabled
    		while ((RCC->CR & RCC_CR_HSERDY) != 0U) if ((BSP_GetTick() - tickstart) > 100) return BSP_TIMEOUT;

        }
    }
    return BSP_OK;
}


// Configure HSI Oscillator
uint8_t BSP_STM32_RCC_ConfigHSI(uint32_t hsistate, uint32_t hsicalvalue) {

	uint32_t tickstart = 0;

    // When the HSI is used as system clock it will not be disabled
    const uint32_t temp_sysclksrc = RCC->CFGR & RCC_CFGR_SWS;
    const uint32_t temp_pllckselr = RCC->PLLCKSELR;
    if ((temp_sysclksrc == RCC_CFGR_SWS_HSI) || ((temp_sysclksrc == RCC_CFGR_SWS_PLL1) && ((temp_pllckselr & RCC_PLLCKSELR_PLLSRC) == RCC_PLLCKSELR_PLLSRC_HSI))) {

    	// When HSI is used as system clock it will not be disabled */
    	if (((RCC->CR & RCC_CR_HSIRDY) != 0U) && (hsistate == RCC_HSI_OFF)) {

    		return BSP_ERROR;

    	} else {

    		// Enable the Internal High Speed oscillator (HSI, HSIDIV2, HSIDIV4, or HSIDIV8)
    		MODIFY_REG(RCC->CR, RCC_CR_HSION | RCC_CR_HSIDIV , hsistate);

    		// Wait till HSI is ready
    		tickstart = BSP_GetTick();
    		while ((RCC->CR & RCC_CR_HSIRDY) == 0U) if ((uint32_t)(BSP_GetTick() - tickstart) > 2) return BSP_TIMEOUT;

    	}

        // Adjusts the Internal High Speed oscillator (HSI) calibration value (REV_V only)
    	MODIFY_REG(RCC->HSICFGR, RCC_HSICFGR_HSITRIM, hsicalvalue << RCC_HSICFGR_HSITRIM_Pos);

    } else {


    	// Check the HSI State
    	if (hsistate != RCC_HSI_OFF) {

    		// Enable the Internal High Speed oscillator (HSI, HSIDIV2,HSIDIV4, or HSIDIV8)
    		MODIFY_REG(RCC->CR, RCC_CR_HSION | RCC_CR_HSIDIV , hsistate);

    		// Wait till HSI is ready
    		tickstart = BSP_GetTick();
    		while ((RCC->CR & RCC_CR_HSIRDY) == 0U) if ((uint32_t)(BSP_GetTick() - tickstart) > 2) return BSP_TIMEOUT;

    		// Adjusts the Internal High Speed oscillator (HSI) calibration value (REV_V only)
    		MODIFY_REG(RCC->HSICFGR, RCC_HSICFGR_HSITRIM, hsicalvalue << RCC_HSICFGR_HSITRIM_Pos);

    	} else {

    		// Disable the Internal High Speed oscillator (HSI)
    		CLEAR_BIT(RCC->CR, RCC_CR_HSION);

    	  	// Wait till HSI is disabled
    	  	tickstart = BSP_GetTick();
    	  	while ((RCC->CR & RCC_CR_HSIRDY) != 0U) if ((uint32_t)(BSP_GetTick() - tickstart) > 2) return BSP_TIMEOUT;
    	}

    }

    return BSP_OK;
}


// Configure LSE Oscillator
uint8_t BSP_STM32_RCC_ConfigLSE(uint32_t lsestate) {
	uint32_t tickstart = 0;

	// Enable write access to Backup domain
    PWR->CR1 |= PWR_CR1_DBP;

    // Wait for Backup domain Write protection disable
    tickstart = BSP_GetTick();

    while ((PWR->CR1 & PWR_CR1_DBP) == 0U) if ((BSP_GetTick() - tickstart) > 100) return BSP_TIMEOUT;

    // Set the new LSE configuration
    switch (lsestate) {
    case RCC_LSE_ON:
    	SET_BIT(RCC->BDCR, RCC_BDCR_LSEON);
    	break;
    case RCC_LSE_OFF:
    	CLEAR_BIT(RCC->BDCR, RCC_BDCR_LSEON);
    	CLEAR_BIT(RCC->BDCR, RCC_BDCR_LSEBYP);
    	break;
    case RCC_LSE_BYPASS:
    	SET_BIT(RCC->BDCR, RCC_BDCR_LSEBYP);
    	SET_BIT(RCC->BDCR, RCC_BDCR_LSEON);
    	break;
	default:
		CLEAR_BIT(RCC->BDCR, RCC_BDCR_LSEON);
		CLEAR_BIT(RCC->BDCR, RCC_BDCR_LSEBYP);
    }


    // Check the LSE State
    tickstart = BSP_GetTick();
    if (lsestate != RCC_LSE_OFF) {

    	// Wait till LSE is ready
    	while ((RCC->BDCR & RCC_BDCR_LSERDY) == 0U) if ((BSP_GetTick() - tickstart) > 5000) return BSP_TIMEOUT;

    } else {

    	// Wait till LSE is disabled
    	while ((RCC->BDCR & RCC_BDCR_LSERDY) != 0U) if ((BSP_GetTick() - tickstart) > 5000) return BSP_TIMEOUT;

    }
    return BSP_OK;
}

// Configure HSI48 Oscillator
uint8_t BSP_STM32_RCC_ConfigHSI48(uint32_t hsi48state) {

	uint32_t tickstart = BSP_GetTick();

    // Check the HSI48 State
    if (hsi48state != RCC_HSI48_OFF) {
    	// Enable the Internal Low Speed oscillator (HSI48)
    	SET_BIT(RCC->CR, RCC_CR_HSI48ON);

    	// Wait till HSI48 is ready
    	while ((RCC->CR & RCC_CR_HSI48RDY) == 0U) if ((BSP_GetTick() - tickstart) > 2) return BSP_TIMEOUT;

    } else {

    	// Disable the Internal Low Speed oscillator (HSI48)
    	CLEAR_BIT(RCC->CR, RCC_CR_HSI48ON);

    	/* Wait till HSI48 is ready */
    	while ((RCC->CR & RCC_CR_HSI48RDY) != 0U) if ((BSP_GetTick() - tickstart) > 2) return BSP_TIMEOUT;

    }

    return BSP_OK;
}


uint8_t BSP_STM32_RCC_ConfigPLL1(uint32_t pllstate, uint32_t pllsource, uint32_t pllrge, uint32_t pllvcosel, uint32_t m, uint32_t n, uint32_t p, uint32_t q, uint32_t r) {

	uint32_t tickstart = 0;

	// Check if the PLL is used as system clock or not
	if ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL1) {

		if (pllstate == RCC_PLL_ON) {

			// Disable the main PLL
			CLEAR_BIT(RCC->CR, RCC_CR_PLL1ON);

			// Wait till PLL is disabled
			tickstart = BSP_GetTick();
			while ((RCC->CR & RCC_CR_PLL1RDY) != 0U) if ((BSP_GetTick() - tickstart) > 2) return BSP_TIMEOUT;

			// Configure the main PLL clock source, multiplication and division factors
			MODIFY_REG(RCC->PLLCKSELR, (RCC_PLLCKSELR_PLLSRC | RCC_PLLCKSELR_DIVM1) , (pllsource | ( m << 4)));
			WRITE_REG (RCC->PLL1DIVR , (((n - 1) & RCC_PLL1DIVR_N1) | (((p - 1) << 9U) & RCC_PLL1DIVR_P1) | (((q - 1) << 16U) & RCC_PLL1DIVR_Q1) | (((r - 1) << 24U) & RCC_PLL1DIVR_R1)));

			// Disable PLLFRACN
			CLEAR_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLL1FRACEN);

			// Configure PLL PLL1FRACN (always 0 for this project)
			MODIFY_REG(RCC->PLL1FRACR, RCC_PLL1FRACR_FRACN1, 0 << RCC_PLL1FRACR_FRACN1_Pos);

			// Select PLL1 input reference frequency range: VCI
			MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLL1RGE, pllrge);

			// Select PLL1 output frequency range : VCO
			MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLL1VCOSEL, pllvcosel);

			// Enable PLL System Clock output
			SET_BIT(RCC->PLLCFGR, RCC_PLL1_DIVP);

			// Enable PLL1Q Clock output
			SET_BIT(RCC->PLLCFGR, RCC_PLL1_DIVQ);

			// Enable PLL1R  Clock output
			SET_BIT(RCC->PLLCFGR, RCC_PLL1_DIVR);

			// Enable PLL1FRACN
			SET_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLL1FRACEN);

			// Enable the main PLL
			SET_BIT(RCC->CR, RCC_CR_PLL1ON);

			// Wait till PLL is ready
			tickstart = BSP_GetTick();
			while ((RCC->CR & RCC_CR_PLL1RDY) == 0U) if ((BSP_GetTick() - tickstart) > 2) return BSP_TIMEOUT;

		} else {

			// Disable the main PLL
			CLEAR_BIT(RCC->CR, RCC_CR_PLL1ON);

			// Wait till PLL is disabled
			tickstart = BSP_GetTick();
			while ((RCC->CR & RCC_CR_PLL1RDY) != 0U) if ((BSP_GetTick() - tickstart) > 2) return BSP_TIMEOUT;

		}

    } else {

    	// PLL1 already used as SYSCLK source, therefore full configuration cannot be completed
    	return BSP_ERROR;

    }

	return BSP_OK;

}


uint8_t BSP_STM32_RCC_ConfigPLL3(uint32_t pllrge, uint32_t pllvcosel, uint32_t m, uint32_t n, uint32_t p, uint32_t q, uint32_t r, uint32_t frac) {

	uint32_t tickstart = 0;

	// Check that PLL3 OSC clock source is already set
	if ((RCC->PLLCKSELR & RCC_PLLCKSELR_PLLSRC) == RCC_PLLSOURCE_NONE) return BSP_ERROR;

	// Disable PLL3
	CLEAR_BIT(RCC->CR, RCC_CR_PLL3ON);

    // Wait till PLL3 is disabled
	tickstart = BSP_GetTick();
	while ((RCC->CR & RCC_CR_PLL3RDY) != 0U) if ((BSP_GetTick() - tickstart) > 2) return BSP_TIMEOUT;

    // Configure the PLL3  multiplication and division factors
	MODIFY_REG(RCC->PLLCKSELR, RCC_PLLCKSELR_DIVM3, (m << 20));
    WRITE_REG (RCC->PLL3DIVR, (((n - 1) & RCC_PLL3DIVR_N3) | (((p - 1) << 9U) & RCC_PLL3DIVR_P3) | (((q -1) << 16U) & RCC_PLL3DIVR_Q3) | (((r - 1) << 24U) & RCC_PLL3DIVR_R3)));

	// Select PLL3 input reference frequency range: VCI
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLL3RGE, pllrge);

	// Select PLL3 output frequency range : VCO
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLL3VCOSEL, pllvcosel);

	// Disable PLL3FRACN
    CLEAR_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLL3FRACEN);

	// Configures PLL3 clock Fractional Part Of The Multiplication Factor
    MODIFY_REG(RCC->PLL3FRACR, RCC_PLL3FRACR_FRACN3, frac << RCC_PLL3FRACR_FRACN3_Pos);

	// Enable PLL3FRACN
    SET_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLL3FRACEN);

	// Enable the PLL3 clock outputs
    SET_BIT(RCC->PLLCFGR, RCC_PLL3_DIVP);
    SET_BIT(RCC->PLLCFGR, RCC_PLL3_DIVQ);
    SET_BIT(RCC->PLLCFGR, RCC_PLL3_DIVR);

	// Enable PLL3
    SET_BIT(RCC->CR, RCC_CR_PLL3ON);

    // Wait till PLL3 is enabled
	tickstart = BSP_GetTick();
	while ((RCC->CR & RCC_CR_PLL3RDY) == 0U) if ((BSP_GetTick() - tickstart) > 2) return BSP_TIMEOUT;

	return BSP_OK;
}

uint8_t BSP_STM32_RCC_ClockConfig(uint32_t clocktype, uint32_t SYSSource, uint32_t SYSDiv, uint32_t AHBDiv, uint32_t APB1Div, uint32_t APB2Div, uint32_t APB3Div, uint32_t APB4Div, uint32_t flatency) {

	uint32_t tickstart = 0;

	// To correctly read data from FLASH memory, the number of wait states (LATENCY) must be correctly programmed
	// according to the frequency of the CPU clock (HCLK) and the supply voltage of the device.

	// Increasing the CPU frequency
	if (flatency > (READ_BIT((FLASH->ACR), FLASH_ACR_LATENCY)))  {

	    // Program the new number of wait states to the LATENCY bits in the FLASH_ACR register
		MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, flatency);

	    // Check that the new number of wait states is taken into account to access the Flash memory by reading the FLASH_ACR register
	    if ((READ_BIT((FLASH->ACR), FLASH_ACR_LATENCY)) != flatency) return BSP_ERROR;

	}

	// Clocks Configuration
	if ((clocktype & RCC_CLOCKTYPE_D1PCLK1) == RCC_CLOCKTYPE_D1PCLK1) {

		if (APB3Div > (RCC->D1CFGR & RCC_D1CFGR_D1PPRE)) MODIFY_REG(RCC->D1CFGR, RCC_D1CFGR_D1PPRE, APB3Div);

	}

	if ((clocktype & RCC_CLOCKTYPE_PCLK1) == RCC_CLOCKTYPE_PCLK1) {

		if (APB1Div > (RCC->D2CFGR & RCC_D2CFGR_D2PPRE1)) MODIFY_REG(RCC->D2CFGR, RCC_D2CFGR_D2PPRE1, APB1Div);

	}

	if ((clocktype & RCC_CLOCKTYPE_PCLK2) == RCC_CLOCKTYPE_PCLK2) {

		if (APB2Div > (RCC->D2CFGR & RCC_D2CFGR_D2PPRE2)) MODIFY_REG(RCC->D2CFGR, RCC_D2CFGR_D2PPRE2, APB2Div);

	}

	if ((clocktype & RCC_CLOCKTYPE_D3PCLK1) == RCC_CLOCKTYPE_D3PCLK1) {

	    if (APB4Div > (RCC->D3CFGR & RCC_D3CFGR_D3PPRE)) MODIFY_REG(RCC->D3CFGR, RCC_D3CFGR_D3PPRE, APB4Div);

	}

	if ((clocktype & RCC_CLOCKTYPE_HCLK) == RCC_CLOCKTYPE_HCLK) {

		if (AHBDiv > (RCC->D1CFGR & RCC_D1CFGR_HPRE)) MODIFY_REG(RCC->D1CFGR, RCC_D1CFGR_HPRE, AHBDiv);

	}

	if ((clocktype & RCC_CLOCKTYPE_SYSCLK) == RCC_CLOCKTYPE_SYSCLK) {

		MODIFY_REG(RCC->D1CFGR, RCC_D1CFGR_D1CPRE, SYSDiv);

		switch (SYSSource) {
		case RCC_SYSCLKSOURCE_HSE:
			if ((RCC->CR & RCC_CR_HSERDY) == 0) return BSP_ERROR;
			break;
		case RCC_SYSCLKSOURCE_PLLCLK:
			if ((RCC->CR & RCC_CR_PLL1RDY) == 0) return BSP_ERROR;
			break;
		case RCC_SYSCLKSOURCE_CSI:
			if ((RCC->CR & RCC_CR_CSIRDY) == 0) return BSP_ERROR;
			break;
		default:
			if ((RCC->CR & RCC_CR_HSIRDY) == 0) return BSP_ERROR;
		}

		MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, SYSSource);

		// Wait until selected Sys Clock is ready
		tickstart = BSP_GetTick();
		while ((RCC->CFGR & RCC_CFGR_SWS) != (SYSSource << RCC_CFGR_SWS_Pos)) if ((BSP_GetTick() - tickstart) > 5000) return BSP_TIMEOUT;

	}

	if ((clocktype & RCC_CLOCKTYPE_HCLK) == RCC_CLOCKTYPE_HCLK) {

		if (AHBDiv < (RCC->D1CFGR & RCC_D1CFGR_HPRE)) MODIFY_REG(RCC->D1CFGR, RCC_D1CFGR_HPRE, AHBDiv);

	}

	// Decreasing the number of wait states because of lower CPU frequency
	if (flatency < (READ_BIT((FLASH->ACR), FLASH_ACR_LATENCY))) {

		// Program the new number of wait states to the LATENCY bits in the FLASH_ACR register
		MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, flatency);

		// Check that the new number of wait states is taken into account to access the Flash memory by reading the FLASH_ACR register
		if ((READ_BIT((FLASH->ACR), FLASH_ACR_LATENCY)) != flatency) return BSP_ERROR;

	}

	if ((clocktype & RCC_CLOCKTYPE_D1PCLK1) == RCC_CLOCKTYPE_D1PCLK1) {

	    if (APB3Div < (RCC->D1CFGR & RCC_D1CFGR_D1PPRE)) MODIFY_REG(RCC->D1CFGR, RCC_D1CFGR_D1PPRE, APB3Div);

	}

	if ((clocktype & RCC_CLOCKTYPE_PCLK1) == RCC_CLOCKTYPE_PCLK1) {

		if (APB1Div < (RCC->D2CFGR & RCC_D2CFGR_D2PPRE1)) MODIFY_REG(RCC->D2CFGR, RCC_D2CFGR_D2PPRE1, APB1Div);

	}

	if ((clocktype & RCC_CLOCKTYPE_PCLK2) == RCC_CLOCKTYPE_PCLK2) {

		if (APB2Div < (RCC->D2CFGR & RCC_D2CFGR_D2PPRE2)) MODIFY_REG(RCC->D2CFGR, RCC_D2CFGR_D2PPRE2, APB2Div);

	}

	if ((clocktype & RCC_CLOCKTYPE_D3PCLK1) == RCC_CLOCKTYPE_D3PCLK1) {

		if (APB4Div < (RCC->D3CFGR & RCC_D3CFGR_D3PPRE)) MODIFY_REG(RCC->D3CFGR, RCC_D3CFGR_D3PPRE, APB4Div);

	}

	return BSP_OK;
}


uint32_t BSP_STM32_RCC_GetSysClockFreq(void) {

	uint32_t pllp = 0;
	uint32_t pllsource = 0;
	uint32_t pllm = 0;
	uint32_t pllfracen = 0;
	uint32_t hsivalue = 0;
    float_t fracn1 = 0;
    float_t pllvco = 0;
    uint32_t sysclockfreq = 0;

    // Get SYSCLK source

    switch (RCC->CFGR & RCC_CFGR_SWS) {

    case RCC_CFGR_SWS_HSI:  // HSI used as system clock source

    	if ((RCC->CR & RCC_CR_HSIDIV_Msk) != 0U) {

    		sysclockfreq = (uint32_t)(HSI_VALUE >> (READ_BIT(RCC->CR, RCC_CR_HSIDIV) >> 3));

    	} else {

    		sysclockfreq = (uint32_t) HSI_VALUE;
    	}

    	break;

    case RCC_CFGR_SWS_CSI:  // CSI used as system clock  source

    	sysclockfreq = CSI_VALUE;
    	break;

    case RCC_CFGR_SWS_HSE:  // HSE used as system clock  source

    	sysclockfreq = HSE_VALUE;
    	break;

    case RCC_CFGR_SWS_PLL1:  // PLL1 used as system clock  source

    	// PLL_VCO = (HSE_VALUE or HSI_VALUE or CSI_VALUE/ PLLM) * PLLN      SYSCLK = PLL_VCO / PLLR

    	pllsource = (RCC->PLLCKSELR & RCC_PLLCKSELR_PLLSRC);
    	pllm = ((RCC->PLLCKSELR & RCC_PLLCKSELR_DIVM1) >> 4)  ;
    	pllfracen = ((RCC-> PLLCFGR & RCC_PLLCFGR_PLL1FRACEN) >> RCC_PLLCFGR_PLL1FRACEN_Pos);
    	fracn1 = (float_t)(uint32_t)(pllfracen * ((RCC->PLL1FRACR & RCC_PLL1FRACR_FRACN1) >> 3));

    	if (pllm != 0U) {
    		switch (pllsource) {

    		case RCC_PLLSOURCE_HSI:  // HSI used as PLL clock source

    			if ((RCC->CR & RCC_CR_HSIDIV_Msk) != 0U) {

    				hsivalue = (HSI_VALUE >> (READ_BIT(RCC->CR, RCC_CR_HSIDIV) >> 3));
    				pllvco = ((float_t)hsivalue / (float_t)pllm) * ((float_t)(uint32_t)(RCC->PLL1DIVR & RCC_PLL1DIVR_N1) + (fracn1 / (float_t)0x2000) + (float_t)1);

    			} else {

    				pllvco = ((float_t)HSI_VALUE / (float_t)pllm) * ((float_t)(uint32_t)(RCC->PLL1DIVR & RCC_PLL1DIVR_N1) + (fracn1 / (float_t)0x2000) + (float_t)1);

    			}
    			break;

    		case RCC_PLLSOURCE_CSI:  // CSI used as PLL clock source

    			pllvco = ((float_t)CSI_VALUE / (float_t)pllm) * ((float_t)(uint32_t)(RCC->PLL1DIVR & RCC_PLL1DIVR_N1) + (fracn1 / (float_t)0x2000) + (float_t)1);
    			break;

    		case RCC_PLLSOURCE_HSE:  // HSE used as PLL clock source

    			pllvco = ((float_t)HSE_VALUE / (float_t)pllm) * ((float_t)(uint32_t)(RCC->PLL1DIVR & RCC_PLL1DIVR_N1) + (fracn1 / (float_t)0x2000) + (float_t)1);
    			break;

    		default:

    			pllvco = ((float_t)CSI_VALUE / (float_t)pllm) * ((float_t)(uint32_t)(RCC->PLL1DIVR & RCC_PLL1DIVR_N1) + (fracn1 / (float_t)0x2000) + (float_t)1);
    			break;

    		}

        pllp = (((RCC->PLL1DIVR & RCC_PLL1DIVR_P1) >> 9) + 1U) ;
        sysclockfreq = (uint32_t)(float_t)(pllvco / (float_t)pllp);

    	} else {

    		sysclockfreq = 0U;
    	}
    	break;

    default:

    	sysclockfreq = CSI_VALUE;
    	break;

    }

    return sysclockfreq;
}


uint8_t BSP_STM32_RCC_WasSystemRestareted(void) {
	uint32_t res = 0;
	res |= (RCC->RSR & RCC_RSR_SFT1RSTF); // System reset from CM7 flag
	res |= (RCC->RSR & RCC_RSR_SFT2RSTF); // System reset from CM4 flag
	return (res)?1:0;
}
