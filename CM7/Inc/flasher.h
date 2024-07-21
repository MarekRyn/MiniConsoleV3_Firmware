/*
 * flasher.h
 *
 *  Created on: Jul 19, 2024
 *      Author: marek
 */

#ifndef FLASHER_H_
#define FLASHER_H_

#include "BSP.h"

uint8_t	FLASHER_Init(char * filename);
uint8_t FLASHER_Task(void);
uint8_t	FLASHER_GetProgress(void);
uint8_t FLASHER_Reset(void);
uint8_t FLASHER_IsError(void);

#endif /* FLASHER_H_ */
