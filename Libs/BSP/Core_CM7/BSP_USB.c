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

#include "BSP_USB.h"

enum enum_usbmode {
	USB_MODE_NONE,		// USB communication not active
	USB_MODE_MSC,		// Device mode - Mass Storage
	USB_MODE_CDC,		// Device mode - Serial port
	USB_MODE_HID		// Device mode - Keyboard, Mouse, Game Controller
};

enum {
  REPORT_ID_KEYBOARD = 1,
  REPORT_ID_MOUSE,
  REPORT_ID_CONSUMER_CONTROL,
  REPORT_ID_GAMEPAD,
  REPORT_ID_COUNT
};

typedef struct {
	enum enum_usbmode		mode;
	uint8_t					connected;
	uint16_t				desc_str[32];
	hid_keyboard_report_t	keyboard;		// Keyboard report
	hid_keyboard_led_bm_t	k_leds;
	uint8_t					k_flag;
	hid_mouse_report_t		mouse;			// Mouse report
	uint8_t					m_flag;
	hid_gamepad_report_t	gamepad;		// Gamepad report
	uint8_t					g_flag;
	uint16_t				ctrl;			// Consumer Ctrl report
	uint8_t					c_flag;
} USB_ctx_TypeDef;


static USB_ctx_TypeDef 	usb_ctx = {0};

static void msc_task(void);
static void cdc_task(void);
static void hid_task(void);

//--------------------------------------------------------------------+
// BSP USB functions
//--------------------------------------------------------------------+

uint8_t BSP_USB_Init_MSC(void) {
	usb_ctx.mode = USB_MODE_MSC;
	BSP_STM32_PCD_EnableUSBVoltageDetector();
	tud_init(TUD_OPT_RHPORT);
	return BSP_OK;
}

uint8_t BSP_USB_Init_CDC(void) {
	usb_ctx.mode = USB_MODE_CDC;
	BSP_STM32_PCD_EnableUSBVoltageDetector();
	tud_init(TUD_OPT_RHPORT);
	return BSP_OK;
}

uint8_t BSP_USB_Init_HID(void) {
	usb_ctx.mode = USB_MODE_HID;
	BSP_STM32_PCD_EnableUSBVoltageDetector();
	tud_init(TUD_OPT_RHPORT);
	return BSP_OK;
}


uint8_t BSP_USB_Disconnect(void) {
	usb_ctx.mode = USB_MODE_NONE;
	tud_disconnect();
	BSP_STM32_PCD_DisableUSBVoltageDetector();
	return BSP_OK;
}

void BSP_USB_Task(void) {
	if (usb_ctx.mode == USB_MODE_NONE) return;

	tud_task();

	if (!usb_ctx.connected) return;

	switch (usb_ctx.mode) {
	case USB_MODE_MSC:
		msc_task();
		break;
	case USB_MODE_CDC:
		cdc_task();
		break;
	case USB_MODE_HID:
		hid_task();
		break;
	default:
		return;
	}
}


uint8_t BSP_USB_IsConnected(void) {
	return usb_ctx.connected;
}


void BSP_USB_HID_Mouse(uint8_t buttons, int8_t dx, int8_t dy, int8_t scrl_dx, int8_t scrl_dy) {
	usb_ctx.mouse.buttons = buttons;
	usb_ctx.mouse.x = dx;
	usb_ctx.mouse.y = dy;
	usb_ctx.mouse.pan = scrl_dx;
	usb_ctx.mouse.wheel = scrl_dy;
	usb_ctx.m_flag = 1;
}

void BSP_USB_HID_Keyboard(uint8_t modifier, uint8_t * pkeycodes, uint8_t keycount) {
	if (keycount > 6) keycount = 6;
	memset(&usb_ctx.keyboard.keycode, 0, 6);
	if ((keycount > 0) && (pkeycodes)) memcpy(&usb_ctx.keyboard.keycode, pkeycodes, keycount);
	usb_ctx.keyboard.modifier = modifier;
	usb_ctx.keyboard.reserved = 0;
	usb_ctx.k_flag = 1;
}

void BSP_USB_HID_Gamepad(uint32_t buttons, uint8_t hat, int8_t x, int8_t y, int8_t z, int8_t rx, int8_t ry, int8_t rz) {
	usb_ctx.gamepad.buttons = buttons;
	usb_ctx.gamepad.hat = hat;
	usb_ctx.gamepad.x = x;
	usb_ctx.gamepad.y = y;
	usb_ctx.gamepad.z = z;
	usb_ctx.gamepad.rx = rx;
	usb_ctx.gamepad.ry = ry;
	usb_ctx.gamepad.rz = rz;
	usb_ctx.g_flag = 1;
}

void BSP_USB_HID_Ctrl(uint16_t command) {
	usb_ctx.ctrl = command;
	usb_ctx.c_flag = 1;
}


//--------------------------------------------------------------------+
// USB global IRQ handler
//--------------------------------------------------------------------+

void OTG_FS_IRQHandler(void) {
	tud_int_handler(BOARD_DEVICE_RHPORT_NUM);
}

//--------------------------------------------------------------------+
// Device Descriptors
//--------------------------------------------------------------------+

char const* string_desc_arr [] = {
  (const char[]) { 0x09, 0x04 }, // 0: is supported language is English (0x0409)
  USB_MANUFACTURER,              // 1: Manufacturer
  USB_PRODUCT,		             // 2: Product
  USB_SERIALNO,                  // 3: Serials, should use chip ID
  "MSC"						     // 4: Device name MSC
  "CDC"							 // 5: Device name CDC
  "HID"							 // 6: Device name HID
};


tusb_desc_device_t const desc_device_msc = {
	.bLength            = sizeof(tusb_desc_device_t),
	.bDescriptorType    = TUSB_DESC_DEVICE,
	.bcdUSB             = 0x0200,
	.bDeviceClass       = 0x00,
  	.bDeviceSubClass    = 0x00,
	.bDeviceProtocol    = 0x00,
	.bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,
	.idVendor           = USB_VID,
	.idProduct          = USB_PID_MSC,
	.bcdDevice          = 0x0100,
	.iManufacturer      = 0x01,
	.iProduct           = 0x02,
	.iSerialNumber      = 0x03,
	.bNumConfigurations = 0x01
};

uint8_t const desc_fs_conf_msc[] = {
	// Config number, interface count, string index, total length, attribute, power in mA
	TUD_CONFIG_DESCRIPTOR(1, 1, 0, (TUD_CONFIG_DESC_LEN + TUD_MSC_DESC_LEN), 0x00, 2000),
	// Interface number, string index, EP Out & EP In address, EP size
	TUD_MSC_DESCRIPTOR(0, 4, 0x01, 0x81, 64),
};

tusb_desc_device_t const desc_device_cdc = {
    .bLength            = sizeof(tusb_desc_device_t),
    .bDescriptorType    = TUSB_DESC_DEVICE,
    .bcdUSB             = 0x0200,
    .bDeviceClass       = TUSB_CLASS_MISC,
    .bDeviceSubClass    = MISC_SUBCLASS_COMMON,
    .bDeviceProtocol    = MISC_PROTOCOL_IAD,
    .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,
    .idVendor           = USB_VID,
    .idProduct          = USB_PID_CDC,
    .bcdDevice          = 0x0100,
    .iManufacturer      = 0x01,
    .iProduct           = 0x02,
    .iSerialNumber      = 0x03,
    .bNumConfigurations = 0x01
};


uint8_t const desc_fs_conf_cdc[] = {
  // Config number, interface count, string index, total length, attribute, power in mA
  TUD_CONFIG_DESCRIPTOR(1, 2, 0, (TUD_CONFIG_DESC_LEN + TUD_CDC_DESC_LEN), 0x00, 2000),

  // Interface number, string index, EP notification address and size, EP data address (out, in) and size.
  TUD_CDC_DESCRIPTOR(0, 5, 0x81, 8, 0x02, 0x82, 64),
};

tusb_desc_device_t const desc_device_hid = {
    .bLength            = sizeof(tusb_desc_device_t),
    .bDescriptorType    = TUSB_DESC_DEVICE,
    .bcdUSB             = 0x0200,
    .bDeviceClass       = 0x00,
    .bDeviceSubClass    = 0x00,
    .bDeviceProtocol    = MISC_PROTOCOL_IAD,
    .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,
    .idVendor           = USB_VID,
    .idProduct          = USB_PID_HID,
    .bcdDevice          = 0x0100,
    .iManufacturer      = 0x01,
    .iProduct           = 0x02,
    .iSerialNumber      = 0x03,
    .bNumConfigurations = 0x01
};

const uint8_t desc_hid_report[] = {
  TUD_HID_REPORT_DESC_KEYBOARD( HID_REPORT_ID(REPORT_ID_KEYBOARD         )),
  TUD_HID_REPORT_DESC_MOUSE   ( HID_REPORT_ID(REPORT_ID_MOUSE            )),
  TUD_HID_REPORT_DESC_CONSUMER( HID_REPORT_ID(REPORT_ID_CONSUMER_CONTROL )),
  TUD_HID_REPORT_DESC_GAMEPAD ( HID_REPORT_ID(REPORT_ID_GAMEPAD          ))
};

// Invoked when received GET HID REPORT DESCRIPTOR
// Application return pointer to descriptor
// Descriptor contents must exist long enough for transfer to complete
uint8_t const * tud_hid_descriptor_report_cb(uint8_t instance) {
  (void) instance;
  return desc_hid_report;
}

uint8_t const desc_fs_conf_hid[] = {
  // Config number, interface count, string index, total length, attribute, power in mA
  TUD_CONFIG_DESCRIPTOR(1, 1, 0, (TUD_CONFIG_DESC_LEN + TUD_HID_DESC_LEN), 0x00, 2000),

  // Interface number, string index, protocol, report descriptor len, EP In address, size & polling interval
  TUD_HID_DESCRIPTOR(0, 0, HID_ITF_PROTOCOL_NONE, sizeof(desc_hid_report), 0x81, CFG_TUD_HID_EP_BUFSIZE, 5)
};


//--------------------------------------------------------------------+
// Generic USB Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void) {
	usb_ctx.connected = 1;
}

// Invoked when device is unmounted
void tud_umount_cb(void) {
	usb_ctx.connected = 0;
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en) {
	usb_ctx.connected = 0;
	(void) remote_wakeup_en;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void) {
	usb_ctx.connected = 1;
}

// Invoked when received GET DEVICE DESCRIPTOR
// Application return pointer to descriptor
uint8_t const * tud_descriptor_device_cb(void) {
	switch (usb_ctx.mode) {
	case USB_MODE_MSC:
		return (uint8_t const *) &desc_device_msc;
	case USB_MODE_CDC:
		return (uint8_t const *) &desc_device_cdc;
	case USB_MODE_HID:
		return (uint8_t const *) &desc_device_hid;
	default:
		return 0;
	}
	return 0;
}

// Invoked when received GET CONFIGURATION DESCRIPTOR
// Application return pointer to descriptor
// Descriptor contents must exist long enough for transfer to complete
uint8_t const * tud_descriptor_configuration_cb(uint8_t index) {
	(void) index; // for multiple configurations
	switch (usb_ctx.mode) {
	case USB_MODE_MSC:
		return desc_fs_conf_msc;
	case USB_MODE_CDC:
		return desc_fs_conf_cdc;
	case USB_MODE_HID:
		return desc_fs_conf_hid;
	default:
		return 0;
	}
	return 0;
}

// Invoked when received GET STRING DESCRIPTOR request
// Application return pointer to descriptor, whose contents must exist long enough for transfer to complete
uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
	(void) langid;

	uint8_t chr_count = 0;

	if (index == 0) {
		memcpy(&usb_ctx.desc_str[1], string_desc_arr[0], 2);
		chr_count = 1;
	} else {
		// Note: the 0xEE index string is a Microsoft OS 1.0 Descriptors.
		// https://docs.microsoft.com/en-us/windows-hardware/drivers/usbcon/microsoft-defined-usb-descriptors

		if ( !(index < sizeof(string_desc_arr)/sizeof(string_desc_arr[0])) ) return NULL;

		const char* str = string_desc_arr[index];

		// Cap at max char
		chr_count = (uint8_t) strlen(str);
		if ( chr_count > 31 ) chr_count = 31;

		// Convert ASCII string into UTF-16
		for(uint8_t i=0; i<chr_count; i++) usb_ctx.desc_str[1+i] = str[i];
	}

	// first byte is length (including header), second byte is string type
	usb_ctx.desc_str[0] = (uint16_t) ((TUSB_DESC_STRING << 8 ) | (2*chr_count + 2));

	return usb_ctx.desc_str;
}

//--------------------------------------------------------------------+
// USB MSC
//--------------------------------------------------------------------+


// Invoked when received SCSI READ10 command
int32_t tud_msc_read10_cb(uint8_t lun, uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize) {

	(void) lun;
	(void) offset; // Offset always 0 when MSC buff is larger than block size

	uint32_t bn = bufsize >> 9; // Divide by 512

	//if (HAL_SD_ReadBlocks(&hsd2, buffer, lba, bn, HAL_MAX_DELAY)) return -1;
	if (BSP_SDCARD_ReadBlocks(buffer, lba, bn)) return -1;

	return (int32_t) bufsize;
}

// Invoked when received SCSI WRITE10 command
int32_t tud_msc_write10_cb(uint8_t lun, uint32_t lba, uint32_t offset, uint8_t* buffer, uint32_t bufsize) {

	(void) lun;
	(void) offset; // Offset always 0 when MSC buff is larger than block size

	uint32_t bn = bufsize >> 9; // Divide by 512

	//if (HAL_SD_WriteBlocks(&hsd2, buffer, lba, bn, HAL_MAX_DELAY)) return -1;
	if (BSP_SDCARD_WriteBlocks(buffer, lba, bn)) return -1;

	return (int32_t) bufsize;
}

// Invoked when received SCSI_CMD_INQUIRY
void tud_msc_inquiry_cb(uint8_t lun, uint8_t vendor_id[8], uint8_t product_id[16], uint8_t product_rev[4]) {

	(void) lun;

	const char vid[] = USB_MANUFACTURER;
	const char pid[] = USB_PRODUCT;
	const char rev[] = USB_REVISION;

	memcpy(vendor_id  , vid, strlen(vid));
	memcpy(product_id , pid, strlen(pid));
	memcpy(product_rev, rev, strlen(rev));
}

// Invoked when received Test Unit Ready command.
// return true allowing host to read/write this LUN e.g SD card inserted
bool tud_msc_test_unit_ready_cb(uint8_t lun) {
	(void) lun;
	return true;
}

// Invoked when received SCSI_CMD_READ_CAPACITY_10 and SCSI_CMD_READ_FORMAT_CAPACITY to determine the disk size
// Application update block count and block size
void tud_msc_capacity_cb(uint8_t lun, uint32_t* block_count, uint16_t* block_size) {

	(void) lun;

	*block_count  = BSP_SDCARD_GetBlockNo();
	*block_size = BSP_SDCARD_GetBlockSize();
}

// Invoked when received an SCSI command not in built-in list below.
// - READ_CAPACITY10, READ_FORMAT_CAPACITY, INQUIRY, TEST_UNIT_READY, START_STOP_UNIT, MODE_SENSE6, REQUEST_SENSE
// - READ10 and WRITE10 has their own callbacks
int32_t tud_msc_scsi_cb(uint8_t lun, uint8_t const scsi_cmd[16], void* buffer, uint16_t bufsize) {

	void const* response = NULL;
	int32_t resplen = 0;

	// most scsi handled is input
	bool in_xfer = true;

	switch (scsi_cmd[0]) {

    default:
      // Set Sense = Invalid Command Operation
      tud_msc_set_sense(lun, SCSI_SENSE_ILLEGAL_REQUEST, 0x20, 0x00);

      // negative means error -> tinyusb could stall and/or response with failed status
      resplen = -1;
      break;

	}

	// return resplen must not larger than bufsize
	if (resplen > bufsize) resplen = bufsize;

	if (response && (resplen > 0)) {
		if(in_xfer) {
			memcpy(buffer, response, (size_t) resplen);
		} else {
			// SCSI output
		}
	}

	return (int32_t) resplen;
}

// Invoked when received Start Stop Unit command
bool tud_msc_start_stop_cb(uint8_t lun, uint8_t power_condition, bool start, bool load_eject) {

	(void) lun;
	(void) power_condition;

	if ( load_eject ) {
		if (start) {
			// load disk storage
		} else {
			// unload disk storage
		}
	}

	return true;
}

// Invoked when received REQUEST_SENSE
int32_t tud_msc_request_sense_cb(uint8_t lun, void* buffer, uint16_t bufsize) {
	(void) lun;
	(void) buffer;
	(void) bufsize;
	return 0;
}

// Invoked to check if device is writable as part of SCSI WRITE10
bool tud_msc_is_writable_cb(uint8_t lun) {
	(void) lun;
	return true;
}

static void msc_task(void) {

}

//--------------------------------------------------------------------+
// USB CDC
//--------------------------------------------------------------------+

void cdc_task(void) {
  // connected() check for DTR bit
  // Most but not all terminal client set this when making connection
  // if ( tud_cdc_connected() )
  {
    // connected and there are data available
    if ( tud_cdc_available() )
    {
      // read data
      char buf[64];
      uint32_t count = tud_cdc_read(buf, sizeof(buf));
      (void) count;

      // Echo back
      // Note: Skip echo by commenting out write() and write_flush()
      // for throughput test e.g
      //    $ dd if=/dev/zero of=/dev/ttyACM0 count=10000
      tud_cdc_write(buf, count);
      tud_cdc_write_flush();
    }
  }
}


//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+


// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen) {
  // TODO not Implemented
  (void) instance;
  (void) report_id;
  (void) report_type;
  (void) buffer;
  (void) reqlen;

  return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize) {
	(void) instance;
	if (report_type != HID_REPORT_TYPE_OUTPUT) return;
	if (report_id != REPORT_ID_KEYBOARD) return;
	if (bufsize == 0) return;
	usb_ctx.k_leds = buffer[0];
}

static void hid_task(void) {

	if (usb_ctx.k_flag) {
		while (!tud_hid_ready()) tud_task();
		tud_hid_n_report(0, REPORT_ID_KEYBOARD, &usb_ctx.keyboard, sizeof(hid_keyboard_report_t));
		usb_ctx.k_flag = 0;
	}

	if (usb_ctx.m_flag) {
		while (!tud_hid_ready()) tud_task();
		tud_hid_n_report(0, REPORT_ID_MOUSE, &usb_ctx.mouse, sizeof(hid_mouse_report_t));
		usb_ctx.m_flag = 0;
	}

	if (usb_ctx.g_flag) {
		while (!tud_hid_ready()) tud_task();
		tud_hid_n_report(0, REPORT_ID_GAMEPAD, &usb_ctx.gamepad, sizeof(hid_gamepad_report_t));
		usb_ctx.g_flag = 0;
	}

	if (usb_ctx.c_flag) {
		while (!tud_hid_ready()) tud_task();
		tud_hid_n_report(0, REPORT_ID_CONSUMER_CONTROL, &usb_ctx.ctrl, 2);
		usb_ctx.c_flag = 0;
	}

}

















