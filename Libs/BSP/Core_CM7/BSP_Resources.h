/*******************************************************************
 * MiniConsole V3 - Board Support Package - Resources
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 *******************************************************************/

#ifndef INC_BSP_RESOURCES_H_
#define INC_BSP_RESOURCES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "BSP_STM32.h"
#include "BSP_fatfs.h"

#define RES_ITEM_NUM	256

typedef struct _RES_ITEM{
	uint32_t	addr;
	uint32_t	size;
} RES_ITEM;

typedef struct _RES_CTX{
	RES_ITEM		items[RES_ITEM_NUM];
	FATFS			fs;
	uint32_t		resAddr;
	uint32_t		resSize;
	uint32_t		resFree;
	uint32_t		resIndex;
} ResCtx_TypeDef;


uint8_t BSP_Res_Init(ResCtx_TypeDef *resctx, uint32_t resAddr, uint32_t resSize);
uint8_t BSP_Res_Load(ResCtx_TypeDef *resctx, char *filename, uint16_t id);
uint32_t BSP_Res_GetAddr(ResCtx_TypeDef *resctx, uint16_t id);
uint32_t BSP_Res_GetSize(ResCtx_TypeDef *resctx, uint16_t id);

#ifdef __cplusplus
}
#endif

#endif /* INC_BSP_RESOURCES_H_ */
