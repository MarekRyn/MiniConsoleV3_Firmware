/*******************************************************************
 * MiniConsole V3 - Board Support Package - Resources
 *
 * Author: Marek Ryn
 * Version: 1.0
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 * - 1.0 	- Initial version
 *******************************************************************/

#ifndef INC_BSP_RESOURCES_H_
#define INC_BSP_RESOURCES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "BSP_STM32.h"
#include "BSP_fatfs.h"
#include <stdlib.h>

#define RES_OBJECTS_COUNT	1024

uint8_t BSP_Res_Init(void * resAddr, uint32_t resSize);
void* BSP_Res_Alloc(uint32_t resSize);
void* BSP_Res_Load(char *filename);
uint32_t BSP_Res_GetSize(void * objAddr);

#ifdef __cplusplus
}
#endif

#endif /* INC_BSP_RESOURCES_H_ */
