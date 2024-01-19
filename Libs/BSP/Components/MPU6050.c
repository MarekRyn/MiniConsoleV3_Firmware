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

#include "MPU6050.h"

KALMAN_HandleTypeDef KALMAN_pitch = {0};
KALMAN_HandleTypeDef KALMAN_roll = {0};
KALMAN_HandleTypeDef KALMAN_yaw = {0};


uint8_t	BSP_DRV_IMU_Init(IMU_HandleTypeDef *himu, I2C_TypeDef *hi2c) {
	// Resetting IMU global variable
	himu->cal.c_ox = 0;
	himu->cal.c_oy = 0;
	himu->cal.c_oz = 0;
	himu->cal.c_x = 0;
	himu->cal.c_y = 0;
	himu->cal.c_z = 0;
	himu->cal.progress = 0;

	// Reseting MPU
	if (BSP_STM32_I2C_RegWrite(hi2c, (IMU_I2C_ADDR << 1), IMU_REG_PWR_MGMT, 0x80, 100)) return BSP_ERROR;
	BSP_Delay(100);

	// Setting MPU Power mode to "Wake Up Device"
	if (BSP_STM32_I2C_RegWrite(hi2c, (IMU_I2C_ADDR << 1), IMU_REG_PWR_MGMT, 0x03, 100)) return BSP_ERROR;

	// Setting MPU Gyro range to +/- 500 deg/sec
	if (BSP_STM32_I2C_RegWrite(hi2c, (IMU_I2C_ADDR << 1), IMU_REG_GYRO_CFG, 0x08, 100)) return BSP_ERROR;

	// Setting MPU Accel range to +/- 2 G
	if (BSP_STM32_I2C_RegWrite(hi2c, (IMU_I2C_ADDR << 1), IMU_REG_ACCEL_CFG, 0x00, 100)) return BSP_ERROR;

	// Setting digital signal filtering (low pass filter = 94Hz / delay 3ms)
	if (BSP_STM32_I2C_RegWrite(hi2c, (IMU_I2C_ADDR << 1), IMU_REG_CONFIG, 0x02, 100)) return BSP_ERROR;

	// Setting update frequency divider to 25Hz (sampling rate = 1kHz / (1 + divider))
	if (BSP_STM32_I2C_RegWrite(hi2c, (IMU_I2C_ADDR << 1), IMU_REG_SMPLRT_DIV, 39, 100)) return BSP_ERROR;

	// Configuring INT pin to ACTIVE HIGH LOGIC, PUSH-PULL, 50us PULSE, CLEAR BY ANY READ OPERATION
	if (BSP_STM32_I2C_RegWrite(hi2c, (IMU_I2C_ADDR << 1), IMU_REG_INT_CFG, 0x10, 100)) return BSP_ERROR;

	// Enabling INT generation on IMU data ready event
	if (BSP_STM32_I2C_RegWrite(hi2c, (IMU_I2C_ADDR << 1), IMU_REG_INT_EN, 0x01, 100)) return BSP_ERROR;

	// Initializing Kalman filters
	KALMAN_Init(&KALMAN_pitch);
	KALMAN_Init(&KALMAN_roll);
	KALMAN_Init(&KALMAN_yaw);

	return BSP_OK;
}


uint8_t BSP_DRV_IMU_Calibrate(IMU_HandleTypeDef *himu) {
	himu->cal.c_ox = 0;
	himu->cal.c_oy = 0;
	himu->cal.c_oz = 0;
	himu->cal.c_x = 0;
	himu->cal.c_y = 0;
	himu->cal.c_z = 0;
	himu->cal.progress = 128;
	return BSP_OK;
}


void BSP_DRV_IMU_Parse(IMU_HandleTypeDef *himu) {

	// Parsing data received from IMU - integer calculations

	int32_t y = (int16_t)((uint16_t)(himu->raw_data[0] << 8) + himu->raw_data[1]);
	int32_t x = (int16_t)((uint16_t)(himu->raw_data[2] << 8) + himu->raw_data[3]);
	int32_t z = (int16_t)((uint16_t)(himu->raw_data[4] << 8) + himu->raw_data[5]);
	z = -z;

	int32_t oy = (int16_t)((uint16_t)(himu->raw_data[8] << 8) + himu->raw_data[9]);
	int32_t ox = (int16_t)((uint16_t)(himu->raw_data[10] << 8) + himu->raw_data[11]);
	int32_t oz = (int16_t)((uint16_t)(himu->raw_data[12] << 8) + himu->raw_data[13]);

	int32_t t = (int16_t)((uint16_t)(himu->raw_data[6] << 8) + himu->raw_data[7]);

	if (himu->cal.progress > 0) {
		// If calibration enabled, collecting data for calibration
		himu->cal.c_y += y;
		himu->cal.c_x += x;
		himu->cal.c_z += (z - 16384);
		himu->cal.c_oy += oy;
		himu->cal.c_ox += ox;
		himu->cal.c_oz += oz;
		himu->cal.progress -= 1;
		if (himu->cal.progress == 0) {
			himu->cal.c_y = himu->cal.c_y / 128;
			himu->cal.c_x = himu->cal.c_x / 128;
			himu->cal.c_z = himu->cal.c_z / 128;
			himu->cal.c_oy = himu->cal.c_oy / 128;
			himu->cal.c_ox = himu->cal.c_ox / 128;
			himu->cal.c_oz = himu->cal.c_oz / 128;
		}
	} else {
		// If no calibration in progress than correcting data by calibration factors
		y -= himu->cal.c_y;
		x -= himu->cal.c_x;
		z -= himu->cal.c_z;
		oy -= himu->cal.c_oy;
		ox -= himu->cal.c_ox;
		oz -= himu->cal.c_oz;
	}


	// Calculating floats
	himu->data.y = (float)y / 16384.0;
	himu->data.x = (float)x / 16384.0;
	himu->data.z = (float)z / 16384.0;

	himu->data.oy = (float)oy / 65.5;
	himu->data.ox = (float)ox / 65.5;
	himu->data.oz = (float)oz / 65.5;

	himu->data.t = (float)t / 340.0 + 36.53;

	// Calculating pitch, roll, yaw
	float acc_pitch = 57.29577951 * atan2f(himu->data.y, himu->data.z);
	himu->pos.pitch = KALMAN_Update(&KALMAN_pitch, acc_pitch, himu->data.ox);

	float acc_roll = 57.29577951 * atan2f(-himu->data.x, himu->data.z);
	himu->pos.roll = KALMAN_Update(&KALMAN_roll, acc_roll, himu->data.oy);

	float acc_yaw = 57.29577951 * atan2f(-himu->data.x, himu->data.y);
	himu->pos.yaw = KALMAN_Update(&KALMAN_yaw, acc_yaw, himu->data.oz);

	// Activity timestamp
	if ((himu->data.ox > 2) || (himu->data.oy > 2) || (himu->data.oz > 2)) himu->timestamp = BSP_GetTick();

}
