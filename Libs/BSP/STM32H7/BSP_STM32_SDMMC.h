/*******************************************************************
 * MiniConsole V3 - Board Support Package - STM32 SDMMC
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 *******************************************************************/

#ifndef STM32H7_BSP_STM32_SDMMC_H_
#define STM32H7_BSP_STM32_SDMMC_H_

#include "BSP_Common.h"
#include "BSP_STM32.h"
#include "BSP_STM32_SDMMC_ll.h"

#define SDMMC_BLOCKSIZE				512

#define SDMMC_CARD_READY          	0x00000001U  // Card state is ready
#define SDMMC_CARD_IDENTIFICATION 	0x00000002U  // Card is in identification state
#define SDMMC_CARD_STANDBY        	0x00000003U  // Card is in standby state
#define SDMMC_CARD_TRANSFER       	0x00000004U  // Card is in transfer state
#define SDMMC_CARD_SENDING        	0x00000005U  // Card is sending an operation
#define SDMMC_CARD_RECEIVING      	0x00000006U  // Card is receiving operation information
#define SDMMC_CARD_PROGRAMMING    	0x00000007U  // Card is in programming state
#define SDMMC_CARD_DISCONNECTED   	0x00000008U  // Card is disconnected
#define SDMMC_CARD_ERROR          	0x000000FFU  // Card response Error

#define CARD_NORMAL_SPEED        	((uint32_t)0x00000000U)    	// Normal Speed Card <12.5Mo/s , Spec Version 1.01
#define CARD_HIGH_SPEED          	((uint32_t)0x00000100U)    	// High Speed Card <25Mo/s , Spec version 2.00
#define CARD_ULTRA_HIGH_SPEED    	((uint32_t)0x00000200U)    	// UHS-I SD Card <50Mo/s for SDR50, DDR5 Cards and <104Mo/s for SDR104, Spec version 3.01

#define CARD_SDSC                  	((uint32_t)0x00000000U)  	// SD Standard Capacity <2Go
#define CARD_SDHC_SDXC             	((uint32_t)0x00000001U)  	// SD High Capacity <32Go, SD Extended Capacity <2To
#define CARD_SECURED               	((uint32_t)0x00000003U)

#define CARD_V1_X                  	((uint32_t)0x00000000U)
#define CARD_V2_X                  	((uint32_t)0x00000001U)


uint32_t BSP_STM32_SDMMC_GetCardState(SDMMC_TypeDef * hsdmmc, TxRxContext_TypeDef * ctx);
uint8_t BSP_STM32_SDMMC_Init(SDMMC_TypeDef * hsdmmc, TxRxContext_TypeDef * ctx);
uint8_t BSP_STM32_SDMMC_ReadBlocks(SDMMC_TypeDef * hsdmmc, TxRxContext_TypeDef * ctx, uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks);
uint8_t BSP_STM32_SDMMC_WriteBlocks(SDMMC_TypeDef * hsdmmc, TxRxContext_TypeDef * ctx, const uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks);


#endif /* STM32H7_BSP_STM32_SDMMC_H_ */
