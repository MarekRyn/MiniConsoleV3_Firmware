/*******************************************************************
 * MiniConsole V3 - Board Support Package - STM32 PWR
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 * - 1.0	- Initial version
 *******************************************************************/

#include "BSP_STM32_PWR.h"


uint8_t BSP_STM32_PWR_ConfigSupply(uint32_t SupplySource) {
	uint32_t tick;

	// Validate input parameters
	switch (SupplySource) {
	case PWR_LDO_SUPPLY:
	case PWR_EXTERNAL_SOURCE_SUPPLY:
	case PWR_DIRECT_SMPS_SUPPLY:
	case PWR_SMPS_1V8_SUPPLIES_LDO:
	case PWR_SMPS_2V5_SUPPLIES_LDO:
	case PWR_SMPS_1V8_SUPPLIES_EXT_AND_LDO:
	case PWR_SMPS_2V5_SUPPLIES_EXT_AND_LDO:
	case PWR_SMPS_1V8_SUPPLIES_EXT:
	case PWR_SMPS_2V5_SUPPLIES_EXT:
		break;
	default:
		return BSP_ERROR;
	}

	// PWR Config Mask
	uint32_t pwr_config_mask = (PWR_CR3_SMPSLEVEL_Msk | PWR_CR3_SMPSEXTHP_Msk | PWR_CR3_SMPSEN_Msk | PWR_CR3_LDOEN_Msk | PWR_CR3_BYPASS_Msk);

	// SupplyConfig can be configured only once after Power-On Reset
	if ((PWR->CR3 & (PWR_CR3_SMPSEN | PWR_CR3_LDOEN | PWR_CR3_BYPASS)) != (PWR_CR3_SMPSEN | PWR_CR3_LDOEN)) {
		if ((PWR->CR3 & pwr_config_mask) == SupplySource) return BSP_OK;
		return BSP_ERROR;
	}

	// Setting PWR Supply Source
	MODIFY_REG(PWR->CR3, pwr_config_mask, SupplySource);

	// Wait for voltage to stabilize
	tick = BSP_GetTick();
	while ((PWR->CSR1 & PWR_CSR1_ACTVOSRDY) != PWR_CSR1_ACTVOSRDY) if ((BSP_GetTick() - tick) > 1000) return BSP_TIMEOUT;

	// If no SMPS in use, than return
	if ((SupplySource & PWR_CR3_SMPSEN) == 0) return BSP_OK;

	// Wait for SMPS readiness
	tick = BSP_GetTick();
	while ((PWR->CR3 & PWR_CR3_SMPSEXTRDY) != PWR_CR3_SMPSEXTRDY) if ((BSP_GetTick() - tick) > 1000) return BSP_TIMEOUT;

	return BSP_OK;
}


uint8_t BSP_STM32_PWR_VoltageScalingConfig(uint32_t Regulator) {
	// For STM32H74xxx and STM32H75xxx lines
	__IO uint32_t tmpreg = 0x00;

	// Validating input parameters
	switch (Regulator) {
	case PWR_REGULATOR_VOLTAGE_SCALE0:
	case PWR_REGULATOR_VOLTAGE_SCALE1:
	case PWR_REGULATOR_VOLTAGE_SCALE2:
	case PWR_REGULATOR_VOLTAGE_SCALE3:
		break;
	default:
		return BSP_ERROR;
	}

	// Check voltage scaling to be configured
	if (Regulator == PWR_REGULATOR_VOLTAGE_SCALE0) {
		// Configure the Voltage Scaling 1
		MODIFY_REG(PWR->D3CR, PWR_D3CR_VOS, PWR_REGULATOR_VOLTAGE_SCALE1);
		// Delay after setting the voltage scaling
		tmpreg = READ_BIT(PWR->D3CR, PWR_D3CR_VOS);
		// Enable the PWR overdrive
		SET_BIT(SYSCFG->PWRCR, SYSCFG_PWRCR_ODEN);
		// Delay after setting the syscfg boost setting
		tmpreg = READ_BIT(SYSCFG->PWRCR, SYSCFG_PWRCR_ODEN);
	} else {
		// Disable the PWR overdrive
		CLEAR_BIT(SYSCFG->PWRCR, SYSCFG_PWRCR_ODEN);
		// Delay after setting the syscfg boost setting
		tmpreg = READ_BIT(SYSCFG->PWRCR, SYSCFG_PWRCR_ODEN);
		// Configure the Voltage Scaling x
		MODIFY_REG(PWR->D3CR, PWR_D3CR_VOS, Regulator);
		// Delay after setting the voltage scaling
		tmpreg = READ_BIT(PWR->D3CR, PWR_D3CR_VOS);
	}
	UNUSED(tmpreg);
	return BSP_OK;
}

uint8_t BSP_STM32_PWR_WaitForVOSRDY(void) {
	uint32_t tick = BSP_GetTick();
	while ((PWR->D3CR & PWR_D3CR_VOSRDY) != PWR_D3CR_VOSRDY) if ((BSP_GetTick() - tick) > 1000) return BSP_ERROR;
	return BSP_OK;
}

uint8_t BSP_STM32_PWR_EnableBkUpAccess(void) {
  // Enable access to RTC and backup registers
  SET_BIT (PWR->CR1, PWR_CR1_DBP);
  return BSP_OK;
}

uint8_t BSP_STM32_PWR_ClearPendingEvent(void) {
#if defined (CORE_CM7)
    __WFE ();
#else
    __SEV ();
    __WFE ();
#endif
    return BSP_OK;
}

uint8_t BSP_STM32_PWR_EnterSTOPMode(uint32_t Regulator, uint8_t STOPEntry, uint32_t Domain) {
	// Check the parameters
	switch (Regulator) {
	case PWR_MAINREGULATOR_ON:
	case PWR_LOWPOWERREGULATOR_ON:
		break;
	default:
		return BSP_ERROR;
	}

	switch (STOPEntry) {
	case PWR_STOPENTRY_WFI:
	case PWR_STOPENTRY_WFE:
		break;
	default:
		return BSP_ERROR;
	}

	switch (Domain) {
	case PWR_D1_DOMAIN:
	case PWR_D2_DOMAIN:
	case PWR_D3_DOMAIN:
		break;
	default:
		return BSP_ERROR;
	}

	// Select the regulator state in Stop mode
	MODIFY_REG (PWR->CR1, PWR_CR1_LPDS, Regulator);

	// Select the domain Power Down DeepSleep
	if (Domain == PWR_D1_DOMAIN) {
#if defined (CORE_CM4)
		return BSP_ERROR;
#endif
		// Keep DSTOP mode when D1/CD domain enters Deepsleep
		CLEAR_BIT (PWR->CPUCR, PWR_CPUCR_PDDS_D1);

		// Set SLEEPDEEP bit of Cortex System Control Register
		SET_BIT (SCB->SCR, SCB_SCR_SLEEPDEEP_Msk);

		// Ensure that all instructions are done before entering STOP mode
		__DSB ();
		__ISB ();

		// Select Stop mode entry
		if (STOPEntry == PWR_STOPENTRY_WFI) __WFI ();
			else __WFE ();

		// Clear SLEEPDEEP bit of Cortex-Mx in the System Control Register
		CLEAR_BIT (SCB->SCR, SCB_SCR_SLEEPDEEP_Msk);
		return BSP_OK;
	}

	if (Domain == PWR_D2_DOMAIN) {
#if defined (CORE_CM7)
	  return BSP_ERROR;
#endif

	  // Keep DSTOP mode when D2 domain enters Deepsleep
	  CLEAR_BIT (PWR->CPU2CR, PWR_CPU2CR_PDDS_D2);

	  // Set SLEEPDEEP bit of Cortex System Control Register
	  SET_BIT (SCB->SCR, SCB_SCR_SLEEPDEEP_Msk);

	  // Ensure that all instructions are done before entering STOP mode
	  __DSB ();
	  __ISB ();

	  // Select Stop mode entry
	  if (STOPEntry == PWR_STOPENTRY_WFI) __WFI ();
	  	  else __WFE ();

	  // Clear SLEEPDEEP bit of Cortex-Mx in the System Control Register
	  CLEAR_BIT (SCB->SCR, SCB_SCR_SLEEPDEEP_Msk);

	  return BSP_OK;
	}

#if defined (CORE_CM7)
	// Keep DSTOP mode when D3 domain enters Deepsleep
	CLEAR_BIT (PWR->CPUCR, PWR_CPUCR_PDDS_D3);
#else
	// Keep DSTOP mode when D3 domain enters Deepsleep
	CLEAR_BIT (PWR->CPU2CR, PWR_CPU2CR_PDDS_D3);
#endif

	return BSP_OK;
}


uint8_t BSP_STM32_PWR_Restart(void) {
	NVIC_SystemReset();
	return BSP_OK;
}
