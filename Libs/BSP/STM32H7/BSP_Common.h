/*******************************************************************
 * MiniConsole V3 - Board Support Package - Common elements
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 *******************************************************************/

#ifndef INC_BSP_COMMON_H_
#define INC_BSP_COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

// Includes
#include <stddef.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
#include <stm32h747xx.h>
#include <stm32h7xx.h>


// Compatibility with some ST Libs
#define SMT32H7
#define HSE_VALUE	25000000
#define HSI_VALUE	64000000
#define LSE_VALUE	32768
#define LSI_VALUE  	32000
#define CSI_VALUE	4000000
#define  TICK_INT_PRIORITY            (0x0FUL) /*!< tick interrupt priority */

#define __weak __attribute__((weak))
#define UNUSED(X) (void)X

#ifdef  USE_FULL_ASSERT
/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr If expr is false, it calls assert_failed function
  *         which reports the name of the source file and the source
  *         line number of the call that failed.
  *         If expr is true, it returns no value.
  * @retval None
  */
  #define assert_param(expr) ((expr) ? (void)0U : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0U)
#endif /* USE_FULL_ASSERT */

typedef enum
{
  HAL_OK       = 0x00,
  HAL_ERROR    = 0x01,
  HAL_BUSY     = 0x02,
  HAL_TIMEOUT  = 0x03
} HAL_StatusTypeDef;


// Memory regions macros
#define ITC_MRAM	__attribute__((section(".itc_mram")))
#define DTC_MRAM	__attribute__((section(".dtc_mram")))

#define OS_FLASH	__attribute__((section(".os_flash")))
#define OS_RAM		__attribute__((section(".os_ram")))

#define APP_FLASH 	__attribute__((section(".app_flash")))
#define APP_RAM		__attribute__((section(".app_ram")))

#define	SND_FLASH	__attribute__((section(".snd_flash")))
#define SND_RAM		__attribute__((section(".snd_ram")))

#define SH0_RAM		__attribute__((section(".sh0_ram")))
#define SH1_RAM		__attribute__((section(".sh1_ram")))
#define BUF_RAM		__attribute__((section(".buf_ram")))
#define BK_RAM		__attribute__((section(".bk_ram")))

// Define
#define TXRX_CTX_MODE_IT	0
#define TXRX_CTX_MODE_DMA	1
#define TXRX_CTX_MODE_BDMA	2

// BSP global definitions
#define BSP_OK		0
#define BSP_ERROR	1
#define BSP_BUSY	2
#define BSP_TIMEOUT	3
#define BSP_SUSPEND	4


// BSP structures - Data Transfer Context

typedef struct _I2CContext {
	uint8_t		*pData; // 4 bytes pointer to 8 bit data buffer
	uint32_t	size;
	uint32_t	index;
	uint32_t	option;
	uint32_t	devaddr;
} I2CContext_TypeDef;


typedef struct _ADCContext {
	uint8_t		reserved;
} ADCContext_TypeDef;


typedef struct _SDMMCContext {
	uint8_t		SDCardVersion;
	uint8_t		SDCardType;
	uint32_t	SDCardClass;
	uint32_t	SDCardSpeed;
	uint32_t	SDCardID[4];
	uint32_t	SDCardSD[4];
	uint32_t	SDCardRelAdd;
	uint32_t 	SDCardBlockNbr;
	uint32_t	SDCardBlockSize;
	uint32_t	SDCardLogBlockNbr;
	uint32_t	SDCardLogBlockSize;
} SDMMCContext_TypeDef;


typedef struct _I2SContext {
	int16_t		*pData; 		// 4 bytes pointer to 16 bit data buffer
	uint32_t	size;
	uint32_t	index;
	uint8_t		status;
} I2SContext_TypeDef;


typedef struct _UARTContext {
	uint8_t		*pDataRX;		// 4 bytes pointer to data RX buffer
	uint8_t		*pDataTX;
	uint32_t	sizeTX;
	uint32_t	sizeRX;
	uint32_t	indexTX;
	uint32_t	indexRX0;
	uint32_t	indexRX1;
} UARTContext_TypeDef;


typedef struct _TxRxContext {
	uint32_t	dma_mode;
	uint32_t	dma_stream;
	uint32_t	dma_index;
	uint32_t	dma_baseregs;
	uint32_t	dma_muxchannel;
	uint32_t	dma_muxchannelstatus;
	uint32_t	dma_muxchannelstatusmask;
	uint32_t	callback_HT;
	uint32_t	callback_TC;
	uint32_t	callback_TE;
	uint8_t		ctxmem[64];
} TxRxContext_TypeDef;


// BSP structures - LCD

typedef struct _LCD_PRIV {
	__IO uint32_t		l_timestamp[2];
	__IO uint32_t		dma2d_state;
	__IO int32_t		bklt_value;
	__IO int32_t		bklt_setting;
	__IO int32_t		bklt_dimspeed;
} LCD_PRIV;

typedef struct _LCD_CONFIG {
	uint32_t	framesize;
	uint32_t	framebuffersize;
	uint8_t		colormode;
	uint8_t		buffermode;
	uint32_t	bgcolor;
	uint8_t		bytesperpixel;
} LCD_CONFIG;

typedef struct _LCD_LAYER {
	__IO uint32_t		Frames[3];
	__IO uint8_t		Frame_IDLE;		// Idling frame
	__IO uint8_t		Frame_EDIT; 	// Frame currently under edit
	__IO uint8_t		Frame_READY;	// Frame rendered and ready to display
	__IO uint8_t		Frame_ACTIVE;	// Frame currently displayed on screen
	__IO uint8_t		Frame_NOUSE;	// Not in use frame
	__IO uint8_t		Frame_PREV;		// Previously rendered frame
} LCD_LAYER;


typedef struct _LCD {
	LCD_PRIV		priv_;
	LCD_CONFIG		config_;
	LCD_LAYER		Layers[2];
	uint32_t		frametime[2];
	uint32_t		JPEGBuf;
} LCD_HandleTypeDef;

// BSP structures - LCD Touch Panel

typedef struct _LCD_TP_DATA {
	uint16_t	x;
	uint16_t	y;
	uint8_t		status;
	uint8_t		pstatus;
	uint8_t		id;
} TP_DATA;

typedef struct _LCD_TP_AREA {
	int16_t		x;
	int16_t		y;
	uint16_t	w;
	uint16_t	h;
	uint8_t		active;
} TP_AREA;

typedef struct _LCD_TP_GEST {
	uint8_t		area;
	uint32_t	gest;
	uint16_t	start_x;
	uint16_t	start_y;
	uint32_t	start_t;
	uint16_t	stop_x;
	uint16_t	stop_y;
	uint32_t	stop_t;
	int16_t		delta_x;
	int16_t		delta_y;
	uint32_t	delta_t;
	float		speed_x;
	float		speed_y;
} TP_GEST;

typedef struct _LCD_TP {
	uint8_t		raw_data[31];
	uint8_t		touch_count;
	TP_DATA		touch_data[5];
	TP_AREA		touch_areas[16];
	TP_GEST		gest_data;
} LCD_TP_HandleTypeDef;


// BSP structures - IMU IC

typedef struct _IMU_DATA {
	float		ox;		// deg/s around X axis
	float		oy;		// deg/s around Y axis
	float		oz;		// deg/s around Z axis
	float		x;		// g acceleration along X axis
	float		y;		// g acceleration along Y axis
	float		z;		// g acceleration along Z axis
	float		t;		// temperature in C deg
} IMU_DATA;

typedef struct _IMU_POS {
	float		pitch;		// device angle around X axis
	float		roll;		// device angle around Y axis
	float		yaw;		// device angle around Z axis
} IMU_POS;

typedef struct _IMU_CAL {
	int32_t		c_ox;
	int32_t		c_oy;
	int32_t		c_oz;
	int32_t		c_x;
	int32_t		c_y;
	int32_t		c_z;
	uint32_t	progress;
	uint32_t	sampleno;
} IMU_CAL;

typedef struct _IMU {
	uint8_t			raw_data[14];
	IMU_CAL			cal;
	IMU_DATA		data;
	IMU_POS			pos;
} IMU_HandleTypeDef;


// BSP structures - Inputs (Buttons and Joystick)

typedef struct _INPUTS_JOY_CAL {
	uint16_t		x_centre_min;
	uint16_t		x_centre_max;
	uint16_t		y_centre_min;
	uint16_t		y_centre_max;
	uint16_t		x_min;
	uint16_t		x_max;
	uint16_t		y_min;
	uint16_t		y_max;
} INPUTS_JOY_CAL;

typedef struct _INPUTS_JOY {
	int16_t			joy_X;		// range from -512 to +512 (0 means center)
	int16_t			joy_Y;		// range from -512 to +512 (0 means center)
} INPUTS_JOY;

typedef struct _INPUTS_BTNS {
	uint8_t			btn_A;
	uint8_t			btn_B;
	uint8_t			btn_C;
	uint8_t			btn_D;
	uint8_t			btn_X_U;
	uint8_t			btn_X_D;
	uint8_t			btn_X_L;
	uint8_t			btn_X_R;
	uint8_t			btn_JOY;
	uint8_t			btn_MENU;
	uint8_t			btn_PWR;
} INPUTS_BTNS;

typedef struct _INPUTS {
	uint16_t		raw_data_joy[2];
	INPUTS_JOY_CAL	joy_cal;
	INPUTS_JOY		joy;
	INPUTS_BTNS		buttons;
} INPUTS_HandleTypeDef;


// BSP structures - Serial connection (USART2)

#define SERIAL_BUF_SIZE		256

typedef struct _SERIAL {
	uint8_t			data_rx[SERIAL_BUF_SIZE];
} SERIAL_HandleTypeDef;


#endif /* INC_BSP_COMMON_H_ */
