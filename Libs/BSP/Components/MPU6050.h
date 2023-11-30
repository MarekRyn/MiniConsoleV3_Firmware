/*******************************************************************
 * MiniConsole V3 - Driver - MPU6050 - Gyro & Accelerometer IC
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
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
#include "../Libs/kalman.h"



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

uint8_t	BSP_DRV_IMU_Init(IMU_HandleTypeDef *himu, I2C_TypeDef *hi2c);
uint8_t BSP_DRV_IMU_Calibrate(IMU_HandleTypeDef *himu);
void BSP_DRV_IMU_Parse(IMU_HandleTypeDef *himu);



#ifdef __cplusplus
}
#endif

#endif /* INC_MPU6050_H_ */
