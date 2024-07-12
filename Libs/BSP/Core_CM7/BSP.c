/*******************************************************************
 * MiniConsole V3 - Board Support Package - CM7
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 *******************************************************************/

// Includes

#include "BSP.h"

DTC_MRAM void * BSP_Driver[1024] = {
		// Placeholder for app init and main
		[0] = 				NULL,
		[1] =				NULL,
		[2 ... 7] =			NULL,

		// BSP Structures
		[8] = 				&BSP_hlcdtp,
		[9] = 				&BSP_himu,
		[10] = 				&BSP_hinputs,
		[11 ... 15] = 		NULL,

		// System functions
		[16] =				BSP_PWR_Restart,
		[17] = 				BSP_PWR_ShutDown,
		[18] =				BSP_Delay,
		[19] =				BSP_GetTick,
		[20 ... 31] =		NULL,

		// LCD and Touch Panel Library
		[32] = 				BSP_LCD_Init,
		[33] = 				BSP_LCD_FrameReady,
		[34] = 				BSP_LCD_GetEditPermission,
		[35] =				BSP_LCD_SetBackLight,
		[36] =				BSP_LCD_GetBackLight,
		[37] =				BSP_LCD_BacklLightOff,
		[38] =				BSP_LCD_BackLightOn,
		[39] =				BSP_LCD_GetFrameTime,
		[40 ... 64] =		NULL,
		[64] =				BSP_LCD_TP_RegisterArea,
		[65] =				BSP_LCD_TP_RemoveArea,
		[66] =				BSP_LCD_TP_RemoveAreaRange,
		[67] =				BSP_LCD_TP_RemoveAllAreas,
		[68 ... 127] =		NULL,


		// G2D Library
		[128] = 			G2D_ClearFrame,
		[129] =				G2D_FillFrame,
		[130] = 			G2D_CopyPrevFrame,
		[131] =				G2D_CopyScrollPrevFrame,
		[132] =				G2D_DrawPixel,
		[133] = 			G2D_DrawHLine,
		[134] =				G2D_DrawHLineBlend,
		[135] = 			G2D_DrawVLine,
		[136] = 			G2D_DrawVLineBlend,
		[137] = 			G2D_DrawLine,
		[138] = 			G2D_DrawRect,
		[139] = 			G2D_DrawFillRect,
		[140] =				G2D_DrawFillRectBlend,
		[141] =				G2D_DrawCircle,
		[142] =				G2D_DrawFillCircle,
		[143] = 			G2D_DrawFillCircleBlend,
		[144] = 			G2D_DrawRoundRect,
		[145] =				G2D_DrawFillRoundRect,
		[146] = 			G2D_DrawFillRoundRectBlend,
		[147] =				G2D_Text,
		[148] =				G2D_TextBlend,
		[149] =				G2D_GetTextHeight,
		[150] = 			G2D_DrawBitmapBlend,
		[151] = 			G2D_DrawBitmapBlendC,
		[152] =				G2D_DrawBitmap,
		[153] =				G2D_DrawBitmapC,
		[154] = 			G2D_DrawBitmapRotate,
		[155] = 			G2D_DrawBitmapRotateC,
		[156] =				G2D_DrawIcon,
		[157] = 			G2D_DrawIconC,
		[158] =				G2D_DrawIconBlend,
		[159] =				G2D_DrawIconBlendC,
		[160] =				G2D_GetIconHeight,
		[161] =				G2D_GetIconWidth,
		[162] =				G2D_DrawJPEG,
		[163] =				G2D_DrawJPEGC,
		[164] =				G2D_DrawLastJPEG,
		[165] =				G2D_DrawLastJPEGC,
		[166] =				G2D_DecodeJPEG,
		[167] =				G2D_DrawTile,
		[168] = 			G2D_DrawTileC,
		[169] =				G2D_DrawTileBlend,
		[170] =				G2D_DrawTileBlendC,
		[171] =				G2D_Color,
		[172] =				G2D_Alpha,
		[173] =				G2D_CopyBuf,
		[174] =				G2D_CopyBufBlend,
		[175] =				G2D_CacheFrame,
		[176] =				G2D_RestoreFrame,
		[177 ... 255] =		NULL,

		// Audio Library
		[256] =				BSP_Audio_SetMasterVolume,
		[257] =				BSP_Audio_GetMasterVolume,
		[258] =				BSP_Audio_GetMasterVolumeL,
		[259] =				BSP_Audio_GetMasterVolumeR,
		[260] =				BSP_Audio_SetMasterVolumeLR,
		[261] =				BSP_Audio_IncMasterVolume,
		[262] =				BSP_Audio_DecMasterVolume,
		[263] =				BSP_Audio_SetChannelVolume,
		[264] =				BSP_Audio_SetChannelVolumeLR,
		[265] =				BSP_Audio_IncChannelVolume,
		[266] =				BSP_Audio_DecChannelVolume,
		[267] =				NULL,
		[268] =				BSP_Audio_LinkSourceMP3,
		[269] =				BSP_Audio_LinkSourceMOD,
		[270] = 			BSP_Audio_LinkSourceRAW,
		[271] = 			BSP_Audio_ChannelPLay,
		[272] = 			BSP_Audio_ChannelStop,
		[273] =				BSP_Audio_ChannelPause,
		[274] =				BSP_Audio_RegisterStatusCallback,
		[275] =				BSP_Audio_GetStatusParam,
		[276] = 			BSP_Audio_GetFreeChannel,
		[277 ... 383] = 	NULL,

		// Resources
		[384] =				BSP_Res_Init,
		[385] = 			BSP_Res_Alloc,
		[386] =				BSP_Res_Free,
		[387] = 			BSP_Res_Load,
		[388] =				BSP_Res_GetSize,

		// Remaining set as NULL
		[389 ... 1023] = 	NULL
};


uint8_t BSP_BOARD_Init_0(void) {

	// Config MPU
	BSP_STM32_MPU_Init();

	// Enable I-Cache
	SCB_EnableICache();

	// Enable D-Cache
	SCB_EnableDCache();

	// Wait until CPU2 boots and enters in stop mode or timeout
	if (BSP_WaitForCM4StopMode()) return BSP_ERROR;

	// Setting NVIC grouping
	BSP_STM32_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	// STM32 Initialization - System Clocks
	if (BSP_STM32_Init_SysClocks()) return BSP_ERROR;

	// Enable SYSCFG Clock
	__BSP_RCC_SYSCFG_CLK_ENABLE();

	// Initializing GPIOB i GPIOG clocks
	__BSP_RCC_GPIOB_CLK_ENABLE();
	__BSP_RCC_GPIOG_CLK_ENABLE();

	// Initialization PWR pins (for PWR_BUTTON and PWR_HOLD)
    BSP_STM32_GPIO_WritePin(GPIOG, GPIO_PIN_9, GPIO_PIN_SET);
    BSP_STM32_GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_INPUT, GPIO_NOPULL, 0, 0);
    BSP_STM32_GPIO_Init(GPIOG, GPIO_PIN_9, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, 0);

	return BSP_OK;
}


uint8_t BSP_BOARD_Init_1(void) {
	// STM32 Initialization - Peripheral Clocks
	if (BSP_STM32_Init_PeriphClocks()) return BSP_ERROR;

	// STM32 Initialization - GPIO Pins
	if (BSP_STM32_Init_GPIO()) return BSP_ERROR;

	// SDRAM Initialization
	if (BSP_SDRAM_Init(120)) return BSP_ERROR;

	// PWR Initialization
	if (BSP_PWR_Init()) return BSP_ERROR;

	// STM32 Initialization - NVIC - Interrupts
	if (BSP_STM32_Init_NVIC()) return BSP_ERROR;

	// IMU Initialization
	if (BSP_IMU_Init()) return BSP_ERROR;

	// Joystick and Keyboard Initialization
	if (BSP_Inputs_Init()) return BSP_ERROR;

	// Touch Panel Initialization
	if (BSP_LCD_TP_Init()) return BSP_ERROR;

	// SDCARD Initialization
	if (BSP_SDCARD_Init()) return BSP_ERROR;

	// USART2 Initialization - communication through debug connection
	if (BSP_Serial_Init()) return BSP_ERROR;

	// Wake up core CM4 (Audio System)
	if (BSP_WakeUpCM4()) return BSP_ERROR;

	// Audio Initialization
	if (BSP_Audio_Init()) return BSP_ERROR;

	// QSPI Initialization
	if (BSP_QSPI_Init()) return BSP_ERROR;

	return BSP_OK;
}


// Configuring Basic Interrupts
void NMI_Handler(void) {

	while (1) {}

}

void HardFault_Handler(void) {
	volatile uint32_t junk = 1;

	while (junk) {}

}

void MemManage_Handler(void) {
	volatile uint32_t junk = 1;

	while (junk) {}
}

void BusFault_Handler(void) {

	while (1) {}

}

void UsageFault_Handler(void) {

	while (1) {}

}

void SVC_Handler(void) {

}

void DebugMon_Handler(void) {

}

void PendSV_Handler(void) {

}

void SysTick_Handler(void) {

  BSP_IncTick();

}

void BSP_Error_Handler(void) {

  __disable_irq();
  while (1) {}

}

/*******************************************************************
* INT HANDLERS FOR GPIO GROUP EXTIS shared between subsystems
 *******************************************************************/

// INT Handlers for Touch Panel, MPU6050


__weak void EXTI13_IRQHandler(void) {
	return;
}


__weak void EXTI11_IRQHandler(void) {
	return;
}


void EXTI15_10_IRQHandler(void) {
	if (EXTI->PR1 & GPIO_PIN_13) {
		EXTI->PR1 |= ~(GPIO_PIN_13);
		// Call to Touch Panel INT handler
		EXTI13_IRQHandler();
	}

	if (EXTI->PR1 & GPIO_PIN_11) {
		EXTI->PR1 |= ~(GPIO_PIN_11);
		// Call to IMU INT handler
		EXTI11_IRQHandler();
	}
}

// INT Handler for BLE Module

void EXTI9_5_IRQHandler(void)
{
	if (EXTI->PR1 & GPIO_PIN_6) {
		EXTI->PR1 |= ~(GPIO_PIN_6);
		// Call to BLE module INT handler
		BSP_BLE_INTHandler();
	}
}
