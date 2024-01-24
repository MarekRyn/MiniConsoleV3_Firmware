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

static uint8_t 	usb_mode = USB_MODE_NONE;

void cdc_task(void);

//--------------------------------------------------------------------+
// BSP USB functions
//--------------------------------------------------------------------+

uint8_t BSP_USB_Init_MSC() {
	usb_mode = USB_MODE_MSC;
	BSP_STM32_PCD_EnableUSBVoltageDetector();
	tud_init(TUD_OPT_RHPORT);
	return BSP_OK;
}

uint8_t BSP_USB_Init_CDC() {
	usb_mode = USB_MODE_CDC;
	BSP_STM32_PCD_EnableUSBVoltageDetector();
	tud_init(TUD_OPT_RHPORT);
	return BSP_OK;
}

uint8_t BSP_USB_Disconnect() {
	usb_mode = USB_MODE_NONE;
	tud_disconnect();
	BSP_STM32_PCD_DisableUSBVoltageDetector();
	return BSP_OK;
}

void BSP_USB_Task() {
	if (usb_mode == USB_MODE_NONE) return;

	tud_task();

	switch (usb_mode) {
	case USB_MODE_MSC:
		break;
	case USB_MODE_CDC:
		cdc_task();
		break;
	case USB_MODE_HID:
		// hid_task();
		break;
	}
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

char const* string_desc_arr [] =
{
  (const char[]) { 0x09, 0x04 }, // 0: is supported language is English (0x0409)
  USB_MANUFACTURER,              // 1: Manufacturer
  USB_PRODUCT,		             // 2: Product
  USB_SERIALNO,                  // 3: Serials, should use chip ID
  "MSC"						     // 4: Device name MSC
  "CDC"							 // 5: Device name CDC
  "HID"							 // 6: Device name HID
};


tusb_desc_device_t const desc_device_msc =
{
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

uint8_t const desc_fs_conf_msc[] =
{
	// Config number, interface count, string index, total length, attribute, power in mA
	TUD_CONFIG_DESCRIPTOR(1, 1, 0, (TUD_CONFIG_DESC_LEN + TUD_MSC_DESC_LEN), 0x00, 2000),
	// Interface number, string index, EP Out & EP In address, EP size
	TUD_MSC_DESCRIPTOR(0, 4, 0x01, 0x81, 64),
};

tusb_desc_device_t const desc_device_cdc =
{
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


uint8_t const desc_fs_conf_cdc[] =
{
  // Config number, interface count, string index, total length, attribute, power in mA
  TUD_CONFIG_DESCRIPTOR(1, 2, 0, (TUD_CONFIG_DESC_LEN + TUD_CDC_DESC_LEN), 0x00, 2000),

  // Interface number, string index, EP notification address and size, EP data address (out, in) and size.
  TUD_CDC_DESCRIPTOR(0, 5, 0x81, 8, 0x02, 0x82, 64),
};
static uint16_t _desc_str[32] = {0};

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void) {}

// Invoked when device is unmounted
void tud_umount_cb(void) {}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en) {
	(void) remote_wakeup_en;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void) {}

// Invoked when received GET DEVICE DESCRIPTOR
// Application return pointer to descriptor
uint8_t const * tud_descriptor_device_cb(void) {
	switch (usb_mode) {
	case USB_MODE_MSC:
		return (uint8_t const *) &desc_device_msc;
	case USB_MODE_CDC:
		return (uint8_t const *) &desc_device_cdc;
	}
	return 0;
}

// Invoked when received GET CONFIGURATION DESCRIPTOR
// Application return pointer to descriptor
// Descriptor contents must exist long enough for transfer to complete
uint8_t const * tud_descriptor_configuration_cb(uint8_t index) {
	(void) index; // for multiple configurations
	switch (usb_mode) {
	case USB_MODE_MSC:
		return desc_fs_conf_msc;
	case USB_MODE_CDC:
		return desc_fs_conf_cdc;
	}
	return 0;
}

// Invoked when received GET STRING DESCRIPTOR request
// Application return pointer to descriptor, whose contents must exist long enough for transfer to complete
uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
	(void) langid;

	uint8_t chr_count = 0;

	if (index == 0) {
		memcpy(&_desc_str[1], string_desc_arr[0], 2);
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
		for(uint8_t i=0; i<chr_count; i++) _desc_str[1+i] = str[i];
	}

	// first byte is length (including header), second byte is string type
	_desc_str[0] = (uint16_t) ((TUSB_DESC_STRING << 8 ) | (2*chr_count + 2));

	return _desc_str;
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



//--------------------------------------------------------------------+
// USB CDC
//--------------------------------------------------------------------+

void cdc_task(void)
{
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



















