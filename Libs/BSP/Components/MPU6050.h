/*******************************************************************
 * MiniConsole V3 - Driver - MPU6050 - Gyro & Accelerometer IC
 *
 * Author: Marek Ryn
 * Version: 1.0
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 * - 1.0 	- Initial version
 *******************************************************************/

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

#ifdef __cplusplus
extern "C" {
#endif

// Includes
#include <math.h>
#include "BSP_Common.h"
#include "BSP_STM32.h"
#include "kalman.h"



#define IMU_I2C_SPEED			400						// Interface Speed in kHz
#define IMU_I2C_ADDR			0x68					// Slave I2C Address (7-bit)

#define IMU_REG_SMPLRT_DIV		0x19					// Register for setting up data update frequency
#define	IMU_REG_CONFIG			0x1A					// IMU Configuration
#define	IMU_REG_GYRO_CFG		0x1B					// IMU Gyro configuration
#define	IMU_REG_ACCEL_CFG		0x1C					// IMU Accelerometer configuration
#define	IMU_REG_INT_CFG			0x37					// INT PIN configuration
#define	IMU_REG_INT_EN			0x38					// INT occurrence events configuration
#define IMU_REG_INT_STATUS		0x3A					// INT status register
#define IMU_REG_SIG_PATH_RST	0x68					// Signal path reset
#define IMU_REG_PWR_MGMT		0x6B					// Power Management
#define	IMU_REG_WHO_AM_I		0x75					// Who Am I Register

#define IMU_DATA_ADDR			0x3B					// Address of first data register
#define IMU_DATA_LEN			0x0E					// Total data length to read


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
	uint32_t		timestamp;
} IMU_HandleTypeDef;



uint8_t	BSP_DRV_IMU_Init(IMU_HandleTypeDef *himu, I2C_TypeDef *hi2c);
uint8_t BSP_DRV_IMU_Calibrate(IMU_HandleTypeDef *himu);
void BSP_DRV_IMU_Parse(IMU_HandleTypeDef *himu);



#ifdef __cplusplus
}
#endif

#endif /* INC_MPU6050_H_ */
