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

#ifndef INC_MAX98357_H_
#define INC_MAX98357_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "BSP_Common.h"
#include "BSP_STM32.h"

#define AUDIO_EN_PORT	GPIOE
#define AUDIO_EN_PIN	GPIO_PIN_2


uint8_t BSP_DRV_Audio_Enable();
uint8_t BSP_DRV_Audio_Disable();


#ifdef __cplusplus
}
#endif

#endif /* INC_MAX98357_H_ */
