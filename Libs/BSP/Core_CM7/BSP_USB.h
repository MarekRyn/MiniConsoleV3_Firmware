/*******************************************************************
 * MiniConsole V3 - Board Support Package - USB interface
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 *******************************************************************/

#ifndef BSP_USB_H_
#define BSP_USB_H_

#include "BSP_STM32.h"
#include "BSP_SDCARD.h"
#include "tusb.h"

#define USB_MANUFACTURER	"MarekRyn"
#define USB_PRODUCT			"MiniConsoleV3"
#define USB_SERIALNO		"123456789012"
#define USB_REVISION		"V3"

#define USB_VID   			0xCafe  // Vendor ID

#define USB_PID_MSC			0x4000  // Product ID for MSC mode
#define USB_PID_CDC			0x4001  // Product ID for CDC mode
#define USB_PID_HID			0x4002  // Product ID for HID mode

uint8_t BSP_USB_Init_MSC(void);
uint8_t BSP_USB_Init_CDC(void);
uint8_t BSP_USB_Init_HID(void);
uint8_t BSP_USB_Disconnect(void);
void BSP_USB_Task(void);
uint8_t BSP_USB_IsConnected(void);

void BSP_USB_HID_Mouse(uint8_t buttons, int8_t dx, int8_t dy, int8_t scrl_dx, int8_t scrl_dy);
void BSP_USB_HID_Keyboard(uint8_t modifier, uint8_t * pkeycodes, uint8_t keycount);
void BSP_USB_HID_Gamepad(uint32_t buttons, uint8_t hat, int8_t x, int8_t y, int8_t z, int8_t rx, int8_t ry, int8_t rz);
void BSP_USB_HID_Ctrl(uint16_t command);


#endif /* BSP_USB_H_ */
