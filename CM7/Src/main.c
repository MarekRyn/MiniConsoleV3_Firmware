/*******************************************************************
 * MiniConsole V3 - Firmware
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 *******************************************************************/

#include "main.h"
#include "common.h"
#include "page_main.h"


__IO uint32_t state0 = 0;
__IO uint32_t state1 = 0;

void Bootloader_Task(void) {
	if (!BSP_LCD_GetEditPermission()) return;

	page_render_main();

	switch (state1) {
	case STATE1_APPS:
		page_render_apps();
		break;
	case STATE1_INPUTS:
		page_render_inputs();
		break;
	case STATE1_IMU:
		page_render_imu();
		break;
	case STATE1_AUDIO:
		page_render_audio();
		break;
	case STATE1_SCREEN:
		page_render_screen();
		break;
	case STATE1_BT:
		page_render_bt();
		break;
	case STATE1_DATETIME:
		page_render_clock();
		break;
	case STATE1_POWER:
		uint8_t ret = page_render_pwr();
		if (ret == 100) state0 = STATE0_PWR_DOWN;
		if (ret == 200) state0 = STATE0_RESTARTING;
		break;
	case STATE1_INFO:
		page_render_info();
		break;

	default:
		state0 = STATE0_FAULT;
	}

	BSP_LCD_FrameReady();
}



int main(void)
{

	// Starting up of MiniConsole;
	state0 = (BSP_STM32_RCC_WasSystemRestareted()) ? STATE0_RESTARTED : STATE0_PWR_UP;

	// State0 loop
	while (1) {
		switch (state0) {
		case STATE0_PWR_UP:
			// Initialization of board after pwr up - Stage 0
			state0 = STATE0_PWR_CONFIRMED;
			if (BSP_BOARD_Init_0()) { state0 = STATE0_FAULT; break; };
			if (BSP_STM32_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == GPIO_PIN_SET) state0 = STATE0_PWR_DOWN;
			break;

		case STATE0_PWR_CONFIRMED:
			// Initialization of board after pwr up - Stage 1
			if (BSP_BOARD_Init_1()) {state0 = STATE0_FAULT; break; }
			state0 = STATE0_INITIATED;
			break;

		case STATE0_RESTARTED:
			// Initialization of board after restart
			if (BSP_BOARD_Init_0()) { state0 = STATE0_FAULT; break; };
			if (BSP_BOARD_Init_1()) { state0 = STATE0_FAULT; break; };
			// TODO: Detecting if console restarted due to fault, and entering fault recovery mode
			state0 = STATE0_INITIATED;
			break;

		case STATE0_FAULT_RECOVERY:
			// TODO: Recovery after fault
			state0 = STATE0_INITIATED;
			break;

		case STATE0_INITIATED:
			// Splash screen

			printf("MiniConsole Started\n");

			BSP_LCD_Init(LCD_COLOR_MODE_RGB888, LCD_BUFFER_MODE_DOUBLE, C_BLACK, NULL);

			while (!BSP_LCD_GetEditPermission()) {};
			G2D_ClearFrame();
			G2D_DrawIconC((uint32_t)&ICON_192_Logo, 400, 200, BSP_LCD_Color(C_WHITE,  0xFF), BSP_LCD_Color(C_BLACK, 0xFF));
			G2D_DrawIconC((uint32_t)&ICON_256_MiniConsole, 400, 350, BSP_LCD_Color(C_WHITE,  0xFF), BSP_LCD_Color(C_BLACK, 0xFF));

			BSP_LCD_FrameReady();

			BSP_Delay(500);

			// Play startup sound
			BSP_Audio_SetMasterVolume(128);
			BSP_Audio_SetChannelVolume(0, 150);
			BSP_Audio_LinkStartupSound(0);
			BSP_Audio_ChannelPLay(0, 1);

			// Initialize Backlight
			BSP_LCD_InitBackLight(10);
			BSP_LCD_SetBackLight(80, 25);

			//BSP_Delay(4000);

			// Load default values
			BSP_PWR_LoadConfig();
			BSP_Audio_LoadMasterVolume();
			BSP_LCD_LoadBackLight();

			// TODO: Checking if menu button is pressed during start-up and selecting: bootloader or application
			// Start Temporary section
			// If "Menu_Button" pressed during startup than console switches into USB MSC mode (active until RESET)
			if (BSP_hinputs.buttons.btn_MENU > 0) {
				BSP_USB_Init_MSC();
				while (1) BSP_USB_Task();
			}
			// End Temporary section

			// state0 = STATE0_APPLICATION;
			state0 = STATE0_BOOTLOADER_INIT;
			break;

		case STATE0_BOOTLOADER_INIT:

			BSP_FatFS_Init("0:/");
			BSP_Res_Init((void *)0xC0000000, 48*1024*1024);

			page_loaddata_info();

			state0 = STATE0_BOOTLOADER_MAIN;
			state1 = STATE1_APPS;
			G2D_DecodeJPEG((uint32_t)WP_00, sizeof(WP_00));
			page_init_main();
			page_init_apps();

			break;

		case STATE0_BOOTLOADER_MAIN:
			Bootloader_Task();
			break;

		case STATE0_APPLICATION_INIT:
			state0 = STATE0_APPLICATION_MAIN;
			break;

		case STATE0_APPLICATION_MAIN:
			break;

		case STATE0_RESTARTING:
			// Restart system
			BSP_PWR_Restart();
			break;

		case STATE0_PWR_DOWN:
			// Shut down system
			BSP_PWR_ShutDown();
			break;

		case STATE0_FAULT:
			// Handle error
			BSP_Error_Handler();
			break;

		default:
			BSP_Error_Handler();
			break;
		}
	}

	while(1) {}
}
