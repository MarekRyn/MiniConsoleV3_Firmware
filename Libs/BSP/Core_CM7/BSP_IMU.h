/*******************************************************************
 * MiniConsole V3 - Board Support Package - Accelerometer & Gyro IC
 *
 * Author: Marek Ryn
 * Version: 1.0
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 * - 1.0	- First stable release
 *******************************************************************/

#ifndef BSP_IMU_H_
#define BSP_IMU_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "BSP_STM32.h"
#include "MPU6050.h"	// IMU IC driver - model MPU6050

extern BUF_RAM	IMU_HandleTypeDef		BSP_himu;

uint8_t BSP_IMU_LoadCalData(void);
uint8_t BSP_IMU_SaveCalData(void);
uint8_t BSP_IMU_Init(void);
uint8_t	BSP_IMU_Calibrate(void);
uint8_t BSP_IMU_Reset(void);

#ifdef __cplusplus
}
#endif

#endif /* BSP_IMU_H_ */
