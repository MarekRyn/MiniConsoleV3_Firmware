/*
 * kalman.h
 *
 *  Created on: 7 lut 2023
 *      Author: marek
 */

#ifndef LIBS_KALMAN_H_
#define LIBS_KALMAN_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "BSP_Common.h"
#include "BSP_STM32.h"

typedef struct _KALMAN {

	float 		Q_angle; 	// Process noise variance for the accelerometer
	float 		Q_bias;  	// Process noise variance for the gyro bias
	float 		R_measure; 	// Measurement noise variance - this is actually the variance of the measurement noise

	float 		angle; 		// The angle calculated by the Kalman filter - part of the 2x1 state vector
	float 		bias; 		// The gyro bias calculated by the Kalman filter - part of the 2x1 state vector
	float 		rate; 		// Unbiased rate calculated from the rate and the calculated bias - you have to call getAngle to update the rate

	float		P[2][2]; 	// Error covariance matrix - This is a 2x2 matrix
	float 		K[2]; 		// Kalman gain - This is a 2x1 vector
	float 		y; 			// Angle difference
	float 		S; 			// Estimate error

	uint32_t	tick; 		// Last update time stamp

} KALMAN_HandleTypeDef;


void KALMAN_Init(KALMAN_HandleTypeDef *kalman);
float KALMAN_Update(KALMAN_HandleTypeDef *kalman, float newAngle, float newRate);


#ifdef __cplusplus
}
#endif

#endif /* LIBS_KALMAN_H_ */
