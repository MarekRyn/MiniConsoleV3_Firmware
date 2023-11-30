/*
 * tusb_config.h
 *
 *  Created on: May 17, 2023
 *      Author: marek
 */

#ifndef TUSB_CONFIG_H_
#define TUSB_CONFIG_H_

#define CFG_TUSB_DEBUG					0
#define CFG_TUSB_MCU                 	OPT_MCU_STM32H7
#define CFG_TUSB_OS                  	OPT_OS_NONE
#define BOARD_DEVICE_RHPORT_SPEED    	OPT_MODE_FULL_SPEED  // 12mbps
#define BOARD_DEVICE_RHPORT_NUM     	0
#define CFG_TUSB_RHPORT0_MODE       	(OPT_MODE_DEVICE | OPT_MODE_FULL_SPEED)

//--------------------------------------------------------------------
// DEVICE CONFIGURATION
//--------------------------------------------------------------------

#define CFG_TUD_MAX_SPEED     			OPT_MODE_FULL_SPEED
#define CFG_TUD_ENABLED       			1
#ifndef CFG_TUD_ENDPOINT0_SIZE
#define CFG_TUD_ENDPOINT0_SIZE    		64
#endif

#ifndef CFG_TUSB_MEM_SECTION
#define CFG_TUSB_MEM_SECTION
#endif

#ifndef CFG_TUSB_MEM_ALIGN
#define CFG_TUSB_MEM_ALIGN    __attribute__ ((aligned(4)))
#endif


//------------- CLASS -------------//
#define CFG_TUD_CDC              1
#define CFG_TUD_MSC              1
#define CFG_TUD_HID              0
#define CFG_TUD_MIDI             0
#define CFG_TUD_VENDOR           0

// CDC FIFO size of TX and RX
#define CFG_TUD_CDC_RX_BUFSIZE   64
#define CFG_TUD_CDC_TX_BUFSIZE   64

// CDC Endpoint transfer buffer size, more is faster
#define CFG_TUD_CDC_EP_BUFSIZE   64

// MSC Buffer size of Device Mass storage
#define CFG_TUD_MSC_EP_BUFSIZE   8192


#endif /* TUSB_CONFIG_H_ */
