/*******************************************************************
 * MiniConsole V3 - Board Support Package - STM32 RTC
 *******************************************************************/

#include "BSP_STM32_RTC.h"

static uint8_t _RTC_ByteToBcd(uint8_t Value)
{
  uint32_t bcdhigh = 0U;
  uint8_t  bcdlow  = Value;

  while (bcdlow >= 10U)
  {
    bcdhigh++;
    bcdlow -= 10U;
  }

  return ((uint8_t)(bcdhigh << 4U) | bcdlow);
}


static uint8_t _RTC_BcdToByte(uint8_t Value)
{
  uint8_t tmp = 0;
  tmp = ((Value & 0xF0U) >> 4U) * 10U;
  return (tmp + (Value & 0x0FU));
}


static uint8_t _RTC_WaitForSynchro(RTC_TypeDef *hrtc) {

	uint32_t tickstart = BSP_GetTick();

	// Clear RSF flag, keep reserved bits at reset values (setting other flags has no effect)
	hrtc->ISR = ((uint32_t)(RTC_RSF_MASK & RTC_ISR_RESERVED_MASK));

	// Wait the registers to be synchronized
    while ((hrtc->ISR & RTC_ISR_RSF) == 0U)
    	if ((BSP_GetTick() - tickstart) > RTC_TIMEOUT_VALUE) return BSP_TIMEOUT;

    return BSP_OK;
}


static void _RTC_WriteEnable(RTC_TypeDef * hrtc) {
	// Enables write to registers
	hrtc->WPR = 0xCA;
	hrtc->WPR = 0x53;
}


static void _RTC_WriteDisable(RTC_TypeDef * hrtc) {
	// Disable write to registers
	hrtc->WPR = 0xFF;
}


static uint8_t _RTC_EnterInitMode(RTC_TypeDef * hrtc) {

	uint32_t tickstart = 0;

	// Check if the Initialization mode is set
	if ((hrtc->ISR & RTC_ISR_INITF) == 0U) {

		// Set the Initialization mode
		hrtc->ISR = (uint32_t)RTC_INIT_MASK;

		tickstart = BSP_GetTick();

		// Wait till RTC is in INIT state and if timeout is reached exit
		while ((hrtc->ISR & RTC_ISR_INITF) == 0U)
			if((BSP_GetTick()  - tickstart) > RTC_TIMEOUT_VALUE) return BSP_TIMEOUT;
	}
	return BSP_OK;
}


static uint8_t _RTC_ExitInitMode(RTC_TypeDef *hrtc)
{
	// Exit Initialization mode
	CLEAR_BIT(RTC->ISR, RTC_ISR_INITF);

	// If CR_BYPSHAD bit = 0, wait for synchro
	if (READ_BIT(RTC->CR, RTC_CR_BYPSHAD)) {
		// Clear BYPSHAD bit
		CLEAR_BIT(RTC->CR, RTC_CR_BYPSHAD);
    }

	if (_RTC_WaitForSynchro(hrtc)) return BSP_TIMEOUT;

	// Restore BYPSHAD bit
    SET_BIT(RTC->CR, RTC_CR_BYPSHAD);

  return BSP_OK;
}


uint8_t BSP_STM32_RTC_Init(RTC_TypeDef * hrtc, uint32_t AsynchPrediv, uint32_t SynchPrediv) {
	// Preventing stopping of RTC during debug session
	// TODO: This code will be moved to separate part responsible for configuration of debugger interface
	// DBGMCU->APB4FZ1 |= DBGMCU_APB4FZ1_DBG_RTC;
	// DBGMCU->APB4FZ2 |= DBGMCU_APB4FZ2_DBG_RTC;

	// Check whether the calendar needs to be initialized
	if (((hrtc->ISR)  & (RTC_ISR_INITS)) == RTC_ISR_INITS) return BSP_OK;

	// Disable the write protection for RTC registers
	_RTC_WriteEnable(hrtc);

	// Enter Initialization mode
	if (_RTC_EnterInitMode(hrtc)) {
		_RTC_WriteDisable(hrtc);	return BSP_ERROR;
	}

	// Clear RTC_CR FMT, OSEL and POL Bits
	hrtc->CR &= ~(RTC_CR_FMT | RTC_CR_OSEL | RTC_CR_POL);

	// Set RTC_CR register
	hrtc->CR |= (RTC_HOURFORMAT_24 | RTC_OUTPUT_DISABLE | RTC_OUTPUT_POLARITY_HIGH);

	// Configure the RTC PRER
	hrtc->PRER = ((AsynchPrediv & 0x007F) << RTC_PRER_PREDIV_A_Pos) | ((SynchPrediv & 0x7FFF) << RTC_PRER_PREDIV_S_Pos);

	// Exit Initialization mode
	if (_RTC_ExitInitMode(hrtc)) {
		_RTC_WriteDisable(hrtc);	return BSP_ERROR;
	}

	hrtc->OR &= ~(RTC_OR_ALARMOUTTYPE | RTC_OR_OUT_RMP);
	hrtc->OR |= (RTC_OUTPUT_TYPE_OPENDRAIN | RTC_OUTPUT_REMAP_NONE);

	// Enable the write protection for RTC registers
	_RTC_WriteDisable(hrtc);
	return BSP_OK;
}


uint8_t BSP_STM32_RTC_SetTime(RTC_TypeDef * hrtc, uint8_t hour, uint8_t minute, uint8_t second) {
	if (hour >= 24) return BSP_ERROR;
	if (minute >= 59) return BSP_ERROR;
	if (second >= 59) return BSP_ERROR;

	// Disable the write protection for RTC registers
	_RTC_WriteEnable(hrtc);

	// Enter Initialization mode
	if (_RTC_EnterInitMode(hrtc)) {
		_RTC_WriteDisable(hrtc);	return BSP_ERROR;
	}

	// Set the RTC_TR register
	uint32_t tmpreg = (((uint32_t)_RTC_ByteToBcd(hour) << RTC_TR_HU_Pos)  | ((uint32_t)_RTC_ByteToBcd(minute) << RTC_TR_MNU_Pos) | ((uint32_t)_RTC_ByteToBcd(second) << RTC_TR_SU_Pos));
    hrtc->TR = (uint32_t)(tmpreg & RTC_TR_RESERVED_MASK);

    // Configure the RTC_CR register
    hrtc->CR &= ((uint32_t)~RTC_CR_BKP);
    hrtc->CR |= (uint32_t)(RTC_DAYLIGHTSAVING_NONE | RTC_STOREOPERATION_RESET);

    // Exit Initialization mode
    if (_RTC_ExitInitMode(hrtc)) {
    	_RTC_WriteDisable(hrtc);	return BSP_ERROR;
    }

	// Enable the write protection for RTC registers
	_RTC_WriteDisable(hrtc);

	return BSP_OK;
}


uint8_t BSP_STM32_RTC_SetDate(RTC_TypeDef * hrtc, uint16_t year, uint8_t month, uint8_t day) {

	uint8_t mdays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	uint8_t	weekday = 0;
	uint32_t tmpreg = 0;

	// Validating year and month
	if ((year < 2000) || (year > 2099)) return BSP_ERROR;	// STM32 calendar is designed to work properly between 2000 - 2099
	if ((month == 0) || (month > 12)) return BSP_ERROR;

	// Leap Year detection
	if ((year % 4) == 0) 	mdays[1] = 29;
	if ((year % 100) == 0) 	mdays[1] = 28;
	if ((year % 400) == 0) 	mdays[1] = 29;

	// Validating day
	if (day == 0) return BSP_ERROR;
	if (day > mdays[month - 1]) return BSP_ERROR;

	// Calculating day of week using Keith Algorithm
	// https://en.wikipedia.org/wiki/Determination_of_the_day_of_the_week
	// Algorithm modified to meet STM32 uC requirements to give 1 - Monday ... 7 - Sunday
	// Method do not preserve input variables therefore we need to work on copies
	uint16_t y = year;
	uint8_t m = month;
	uint8_t d = day;
	weekday = ((d += m < 3 ? y-- : y - 2, 23 * m / 9 + d + 3 + y / 4 - y / 100 + y / 400) % 7) + 1;

	// Disable the write protection for RTC registers
	_RTC_WriteEnable(hrtc);

	// Enter Initialization mode
	if (_RTC_EnterInitMode(hrtc)) {
		_RTC_WriteDisable(hrtc);	return BSP_ERROR;
	}

	// Set the RTC_DR register
	tmpreg  = (((uint32_t)_RTC_ByteToBcd(year % 100)) << RTC_DR_YU_Pos);
	tmpreg |= (((uint32_t)_RTC_ByteToBcd(month)) << RTC_DR_MU_Pos);
	tmpreg |= (((uint32_t)_RTC_ByteToBcd(day)) << RTC_DR_DU_Pos);
	tmpreg |= (((uint32_t)weekday) << RTC_DR_WDU_Pos);

	hrtc->DR = (uint32_t)(tmpreg & RTC_DR_RESERVED_MASK);

	// Exit Initialization mode
	if (_RTC_ExitInitMode(hrtc)) {
		_RTC_WriteDisable(hrtc);	return BSP_ERROR;
	}

	// Enable the write protection for RTC registers
	_RTC_WriteDisable(hrtc);

	return BSP_OK;
}


uint8_t BSP_STM32_RTC_GetTime(RTC_TypeDef * hrtc, uint8_t * phour, uint8_t * pminute, uint8_t * psecond) {

	uint32_t tmpreg = 0;

	// Get the TR register
	tmpreg = (uint32_t)(hrtc->TR & RTC_TR_RESERVED_MASK);

	// Updating variables
	*phour = _RTC_BcdToByte((uint8_t)((tmpreg & (RTC_TR_HT  | RTC_TR_HU)) >> RTC_TR_HU_Pos));
	*pminute = _RTC_BcdToByte((uint8_t)((tmpreg & (RTC_TR_MNT | RTC_TR_MNU)) >> RTC_TR_MNU_Pos));
	*psecond = _RTC_BcdToByte((uint8_t)((tmpreg & (RTC_TR_ST  | RTC_TR_SU)) >> RTC_TR_SU_Pos));

	return BSP_OK;
}


uint8_t BSP_STM32_RTC_GetDate(RTC_TypeDef * hrtc, uint16_t * pyear, uint8_t * pmonth, uint8_t * pday, uint8_t * pweekday) {

	uint32_t tmpreg = 0;

	// Get the TD register
	tmpreg = (uint32_t)(hrtc->DR & RTC_DR_RESERVED_MASK);

	// Updating variables
	*pyear = 2000 + _RTC_BcdToByte((uint8_t)((tmpreg & (RTC_DR_YT | RTC_DR_YU)) >> RTC_DR_YU_Pos));
	*pmonth = _RTC_BcdToByte((uint8_t)((tmpreg & (RTC_DR_MT | RTC_DR_MU)) >> RTC_DR_MU_Pos));
	*pday = _RTC_BcdToByte((uint8_t)((tmpreg & (RTC_DR_DT | RTC_DR_DU)) >> RTC_DR_DU_Pos));
	*pweekday = (uint8_t)((tmpreg & (RTC_DR_WDU)) >> RTC_DR_WDU_Pos);

	return BSP_OK;
}


uint8_t BSP_STM32_RTC_SetBackupReg(RTC_TypeDef * hrtc, uint8_t reg, uint32_t value) {

	if (reg > 31) return BSP_ERROR;

	uint32_t * bkreg = (uint32_t *)(&hrtc->BKP0R) + reg;

	// Disable the write protection for RTC registers
	_RTC_WriteEnable(hrtc);

	*bkreg = value;

	// Enable the write protection for RTC registers
	_RTC_WriteDisable(hrtc);

	return BSP_OK;
}


uint32_t BSP_STM32_RTC_GetBackupReg(RTC_TypeDef * hrtc, uint8_t reg) {

	if (reg > 31) return 0;
	uint32_t * bkreg = (uint32_t *)(&hrtc->BKP0R) + reg;
	return *bkreg;

}
