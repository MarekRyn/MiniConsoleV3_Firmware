/*******************************************************************
 * MiniConsole V3 - Driver - MAX98357 - I2S Audio Amplifier
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 *******************************************************************/

#include "MAX98357A.h"

uint8_t BSP_DRV_Audio_Enable() {

	BSP_STM32_GPIO_WritePin(AUDIO_EN_PORT, AUDIO_EN_PIN, SET);

	return BSP_OK;
}

uint8_t BSP_DRV_Audio_Disable() {

	BSP_STM32_GPIO_WritePin(AUDIO_EN_PORT, AUDIO_EN_PIN, RESET);

	return BSP_OK;
}
