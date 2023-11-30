/*******************************************************************
 * MiniConsole V3 - Board Support Package - SDRAM
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 *******************************************************************/

#ifndef INC_BSP_SDRAM_H_
#define INC_BSP_SDRAM_H_

#ifdef __cplusplus
extern "C" {
#endif

// Includes

#include "BSP_STM32.h"
#include "../Components/IS42S32160F.h"

#define BSP_SDRAM_ADDR = 0xC0000000;  // Default addr for SDRAM - bank 1

uint8_t	BSP_SDRAM_Init(uint32_t Freq);

#ifdef __cplusplus
}
#endif

#endif /* INC_BSP_SDRAM_H_ */
