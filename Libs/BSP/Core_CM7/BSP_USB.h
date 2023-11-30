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

#define USB_MODE_NONE		0
#define USB_MODE_MSC		1
#define USB_MODE_CDC		2
#define USB_MODE_HID		3


#define USB_MANUFACTURER	"MarekRyn"
#define USB_PRODUCT			"MiniConsoleV3"
#define USB_SERIALNO		"123456789012"
#define USB_REVISION		"V3"

#define USB_VID   			0xCafe  // Vendor ID

#define USB_PID_MSC			0x4000  // Product ID for MSC mode
#define USB_PID_CDC			0x4001  // Product ID for CDC mode
#define USB_PID_HID			0x4002  // Product ID for HID mode


uint8_t BSP_USB_Init_MSC();
uint8_t BSP_USB_Init_CDC();
uint8_t BSP_USB_Disconnect();
void BSP_USB_Task();




#endif /* BSP_USB_H_ */
