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
#include "page_inputs.h"
#include "page_imu.h"
#include "page_audio.h"
#include "page_screen.h"
#include "page_bt.h"

__IO uint32_t state0 = STATE0_PWR_UP;
__IO uint32_t state1 = STATE1_APPS;

int main(void)
{

	// Starting up of MiniConsole;
	state0 = STATE0_PWR_UP;

	if (BSP_BOARD_Init_CM7()) BSP_Error_Handler();

	// MiniConsole Initiated;
	state0 = STATE0_INITIATED;


	BSP_LCD_Init(LCD_COLOR_MODE_RGB888, LCD_BUFFER_MODE_DOUBLE, C_BLACK, NULL);

	// If "Menu_Button" pressed during startup than console switches into USB MSC mode (active until RESET)
	if (BSP_hinputs.buttons.btn_MENU > 0) {
		BSP_USB_Init_MSC();
		while (1) BSP_USB_Task();
	}

	// TODO: Splash Screen


	// Initialize Backlight
	BSP_LCD_InitBackLight(10);
	BSP_LCD_SetBackLight(80, 25);

	// Entering Bootloader
	state0 = STATE0_BOOTLOADER;


	// Load resources
	ResCtx_TypeDef resctx = {0};
	BSP_Res_Init(&resctx, 0xC0000000, 48 * 1024 * 1024);
	BSP_Res_Load(&resctx, "/_/wallpaper01.jpg", 0);

	G2D_DecodeJPEG(BSP_Res_GetAddr(&resctx, 0), BSP_Res_GetSize(&resctx, 0));

	page_init_main();
	page_init_apps();
	page_init_inputs();
	page_init_imu();
	page_init_audio();
	page_init_screen();
	page_init_bt();


    /* Loop forever */
	for(;;) {

		if (!BSP_LCD_GetEditPermission()) continue;

		G2D_DrawLastJPEG(0, 0); // Background
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
		}

		BSP_LCD_FrameReady();
	}
}
