/*******************************************************************
 * MiniConsole V3 - Board Support Package - STM32 CORE
 *******************************************************************/

#include "BSP_STM32_CORE.h"


uint8_t BSP_STM32_NVIC_EnableIRQ(IRQn_Type IRQn) {
	NVIC_EnableIRQ(IRQn);
	return BSP_OK;
}


uint8_t BSP_STM32_NVIC_SetPriority(IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority) {
	uint32_t prioritygroup = NVIC_GetPriorityGrouping();
	NVIC_SetPriority(IRQn, NVIC_EncodePriority(prioritygroup, PreemptPriority, SubPriority));
	return BSP_OK;
}

uint8_t BSP_STM32_NVIC_SetPriorityGrouping(uint32_t PriorityGroup) {
	// Set the PRIGROUP[10:8] bits according to the PriorityGroup parameter value
	NVIC_SetPriorityGrouping(PriorityGroup);
	return BSP_OK;
}

uint8_t BSP_STM32_MPU_Enable(uint32_t MPU_Control) {
	// Enable the MPU
	MPU->CTRL = MPU_Control | MPU_CTRL_ENABLE_Msk;
	// Enable fault exceptions
	SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk;
	// Ensure MPU setting take effects
	  __DSB();
	  __ISB();
	return BSP_OK;
}

uint8_t BSP_STM32_MPU_Disable(void) {
	// Make sure outstanding transfers are done
	__DMB();
	// Disable fault exceptions
	SCB->SHCSR &= ~SCB_SHCSR_MEMFAULTENA_Msk;
	// Disable the MPU and clear the control register
	MPU->CTRL = 0;
	return BSP_OK;
}

uint8_t BSP_STM32_MPU_ConfigRegion(MPU_Region_TypeDef *MPU_Init) {
	// Set the Region number
	MPU->RNR = MPU_Init->Number;

	if (MPU_Init->Enable == MPU_REGION_DISABLE) { MPU->RBAR = 0; MPU->RASR = 0; return BSP_OK; }

	MPU->RBAR = MPU_Init->BaseAddress;
	MPU->RASR = ((uint32_t)MPU_Init->DisableExec             << MPU_RASR_XN_Pos)   |
	            ((uint32_t)MPU_Init->AccessPermission        << MPU_RASR_AP_Pos)   |
	            ((uint32_t)MPU_Init->TypeExtField            << MPU_RASR_TEX_Pos)  |
	            ((uint32_t)MPU_Init->IsShareable             << MPU_RASR_S_Pos)    |
	            ((uint32_t)MPU_Init->IsCacheable             << MPU_RASR_C_Pos)    |
	            ((uint32_t)MPU_Init->IsBufferable            << MPU_RASR_B_Pos)    |
	            ((uint32_t)MPU_Init->SubRegionDisable        << MPU_RASR_SRD_Pos)  |
	            ((uint32_t)MPU_Init->Size                    << MPU_RASR_SIZE_Pos) |
	            ((uint32_t)MPU_Init->Enable                  << MPU_RASR_ENABLE_Pos);

	return BSP_OK;
}
