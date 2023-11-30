/*
 * kalman.c
 *
 *  Created on: 7 lut 2023
 *      Author: marek
 */
#include "kalman.h"

void KALMAN_Init(KALMAN_HandleTypeDef *kalman) {
    // We will set the variables like so, these can also be tuned by the user
    kalman->Q_angle   = 0.001;
    kalman->Q_bias    = 0.003;
    kalman->R_measure = 0.03;

    kalman->angle = 0.0; // Reset the angle
    kalman->bias  = 0.0; // Reset bias

    // Since we assume that the bias is 0 and we know the starting angle (use setAngle), the error covariance matrix is set like so
    // see: http://en.wikipedia.org/wiki/Kalman_filter#Example_application.2C_technical
    kalman->P[0][0] = 0.0;
    kalman->P[0][1] = 0.0;
    kalman->P[1][0] = 0.0;
    kalman->P[1][1] = 0.0;

    kalman->tick = BSP_GetTick();
}

float KALMAN_Update(KALMAN_HandleTypeDef *kalman, float newAngle, float newRate) {
	// newAngle - last calculated angle from accelerometer [deg]
	// newRate - last rate of turn from gyro [deg/sec]


    // Step 1
	uint32_t tick = BSP_GetTick();
	float dt = (float)(tick - kalman->tick) / 1000; // time between updates in seconds
	kalman->tick = tick;

    kalman->rate = newRate - kalman->bias;
    kalman->angle += dt * kalman->rate;

    // Step 2
    // Update estimation error covariance - Project the error covariance ahead
    kalman->P[0][0] += dt * (dt*kalman->P[1][1] - kalman->P[0][1] - kalman->P[1][0] + kalman->Q_angle);
    kalman->P[0][1] -= dt * kalman->P[1][1];
    kalman->P[1][0] -= dt * kalman->P[1][1];
    kalman->P[1][1] += kalman->Q_bias * dt;

    // Step 4
    // Discrete Kalman filter measurement update equations - Measurement Update ("Correct")
    // Calculate Kalman gain - Compute the Kalman gain
    kalman->S = kalman->P[0][0] + kalman->R_measure;

    // Step 5
    kalman->K[0] = kalman->P[0][0] / kalman->S;
    kalman->K[1] = kalman->P[1][0] / kalman->S;

    // Step 3
    // Calculate angle and bias - Update estimate with measurement zk (newAngle)
    kalman->y = newAngle - kalman->angle;

    // Step 6
    kalman->angle += kalman->K[0] * kalman->y;
    kalman->bias += kalman->K[1] * kalman->y;

    // Step 7
    // Calculate estimation error covariance - Update the error covariance
    kalman->P[0][0] -= kalman->K[0] * kalman->P[0][0];
    kalman->P[0][1] -= kalman->K[0] * kalman->P[0][1];
    kalman->P[1][0] -= kalman->K[1] * kalman->P[0][0];
    kalman->P[1][1] -= kalman->K[1] * kalman->P[0][1];

    return kalman->angle;
}
