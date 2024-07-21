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

__IO void (*App_Init)(void) = NULL;
__IO void (*App_Main)(void) = NULL;


void USB_MSC_Task(void) {
	G2D_ClearFrame();
	G2D_DrawIconC((uint32_t)&ICON_192_USB, 400, 240, BSP_LCD_Color(C_WHITE,  0xFF), BSP_LCD_Color(C_BLACK, 0xFF));
	BSP_LCD_FrameReady();

	BSP_USB_Init_MSC();
	while (1) BSP_USB_Task();
}


void Bootloader_Task(void) {
	if (!BSP_LCD_GetEditPermission()) return;

	page_render_main();

	switch (state1) {
	case STATE1_APPS:
		page_render_apps();
		FLASHER_Task();
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
			if (BSP_STM32_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == GPIO_PIN_SET) state0 = STATE0_PWR_DOWN_NOANIM;
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
			state0 = STATE0_APPLICATION_INIT;

			// Checking if menu button is pressed during start-up and selecting: application, bootloader or USB mass storage
			if (BSP_hinputs.buttons.btn_MENU > 0) {
				if (BSP_hinputs.buttons.btn_C == 0) state0 = STATE0_BOOTLOADER_INIT;
				if (BSP_hinputs.buttons.btn_C > 0) state0 = STATE0_USB_MSC;
			}

			// For developing configuration menu
			//state0 = STATE0_BOOTLOADER_INIT;

			// Splash screen
			printf("MiniConsole Started\n");

			BSP_LCD_Init(LCD_COLOR_MODE_RGB888, LCD_BUFFER_MODE_DOUBLE, C_BLACK, NULL);

			while (!BSP_LCD_GetEditPermission()) {};
			G2D_ClearFrame();
			G2D_DrawIconC((uint32_t)&ICON_192_Logo, 400, 200, BSP_LCD_Color(C_WHITE,  0xFF), BSP_LCD_Color(C_BLACK, 0xFF));
			G2D_DrawIconC((uint32_t)&ICON_256_MiniConsole, 400, 350, BSP_LCD_Color(C_WHITE,  0xFF), BSP_LCD_Color(C_BLACK, 0xFF));

			BSP_LCD_FrameReady();

			// Play startup sound
			BSP_Audio_SetMasterVolume(128);
			BSP_Audio_SetChannelVolume(0, 150);
			BSP_Audio_LinkStartupSound(0);
			BSP_Audio_ChannelPLay(0, 1);

			// Initialize Backlight
			BSP_LCD_InitBackLight(10);
			BSP_LCD_SetBackLight(80, 25);

			// Load default values
			BSP_PWR_LoadConfig();
			BSP_Audio_LoadMasterVolume();
			BSP_LCD_LoadBackLight();


			// Display version and mode info

			BSP_Delay(1000);

			G2D_CopyPrevFrame();

			char bspversion[20] = "Version: ";
			strcat(bspversion,BSP_VERSION);

			G2D_Text(660,440, FONT_20_verdana, bspversion, BSP_LCD_Color(C_WHITE, 255), BSP_LCD_Color(C_BLACK, 255));

			if (state0 == STATE0_APPLICATION_INIT) G2D_Text(10, 440, FONT_20_verdana, "Loading application...", BSP_LCD_Color(C_WHITE, 255), BSP_LCD_Color(C_BLACK, 255));
			if (state0 == STATE0_BOOTLOADER_INIT) G2D_Text(10, 440, FONT_20_verdana, "Entering configuration mode...", BSP_LCD_Color(C_WHITE, 255), BSP_LCD_Color(C_BLACK, 255));
			if (state0 == STATE0_USB_MSC) G2D_Text(10, 440, FONT_20_verdana, "Entering USB Drive mode...", BSP_LCD_Color(C_WHITE, 255), BSP_LCD_Color(C_BLACK, 255));

			BSP_LCD_FrameReady();

			// Delay
			BSP_Delay(3000);

			break;

		case STATE0_BOOTLOADER_INIT:

			// Configuring resource manager
			BSP_FatFS_Init();
			BSP_SetHomeDir("0:/");
			BSP_Res_Init((void *)0xC0000000, 48*1024*1024);

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

			// QSPI peripheral entering memory mapped mode
			BSP_QSPI_MemMappedEnable();

			// Parsing application configuration section
			uint32_t * App_Init_Addr = (uint32_t *)0x90000000;
			uint32_t * App_Main_Addr = (uint32_t *)0x90000004;
			char * App_Home_Dir = (char *)0x90000008;

			// Configuring resource manager
			BSP_FatFS_Init();
			BSP_SetHomeDir(App_Home_Dir);

			BSP_Driver[0] = (void *)*App_Init_Addr;
			App_Init = BSP_Driver[0];
			BSP_Driver[1] = (void *)*App_Main_Addr;
			App_Main = BSP_Driver[1];

			// Initiate application
			App_Init();

			state0 = STATE0_APPLICATION_MAIN;
			break;

		case STATE0_APPLICATION_MAIN:

			// Enter Application Main
			App_Main();


			// Infinite loop;
			while(1) {};

			break;


		case STATE0_USB_MSC:
			// Entering USB Drive menu
			USB_MSC_Task();
			break;

		case STATE0_RESTARTING:
			// Restart system
			BSP_PWR_Restart();
			break;

		case STATE0_PWR_DOWN:
			// Shut down system
			BSP_PWR_ShutDown();
			break;

		case STATE0_PWR_DOWN_NOANIM:
			// Shut down system when power button was hold to short time
			BSP_PWR_ShutDownNoAnim();
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
