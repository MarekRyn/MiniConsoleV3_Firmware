/*******************************************************************
 * MiniConsole V3 - Driver - IS42S32160 - SDRAM Memory
 *
 * Author: Marek Ryn
 * Version: 1.0
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 * - 1.0	- Initial version
 *******************************************************************/

#ifndef INC_IS42S32160_H_
#define INC_IS42S32160_H_

#ifdef __cplusplus
extern "C" {
#endif

// Includes

#include "BSP_Common.h"
#include "BSP_STM32.h"

// SDRAM Parameter definitions (CAS = 3)

//#define BSP_SDRAM_SIZE					67108864			// SDRAM Size in bytes
//#define BSP_SDRAM_DATA_WIDTH			32					// SDRAM Data Bus Width in bits
//#define BSP_SDRAM_BANK_NO				4					// Number of internal memory banks
//#define BSP_SDRAM_COL_ADDR_BITS			9					// Number of bits in column address
//#define BSP_SDRAM_ROW_ADDR_BITS			13					// Number of bits in row address
//#define BSP_SDRAM_CAS_LATENCY			3					// CAS Latency
//#define BSP_SDRAM_WRITE_PROTECTION		0					// Write protection (0 - disable / 1 -enable)
//#define BSP_SDRAM_BURST_READ			1					// Burst read (0 - disable / 1 -enable)
//#define BSP_SDRAM_READ_PIPE_DELAY		0					// Read pipe delay (0,1,2 clock cycles)
//#define BSP_SDRAM_TMRD					12					// Load mode register command to active or refresh command in ns
//#define BSP_SDRAM_TXSR					70					// Exit Self-Refresh to Active Time in ns
//#define BSP_SDRAM_TRAS					42					// ACT to PRE command period in ns
//#define	BSP_SDRAM_TRC					60					// REF to REF/ACT to ACT command period in ns
//#define BSP_SDRAM_TWR					2					// Input data to precharge command delay time in clock cycles
//#define BSP_SDRAM_TRP					3					// PRE to ACT command period in clock cycles
//#define BSP_SDRAM_TRCD					3					// Active command to read/write command delay time in clock cycles
//#define BSP_SDRAM_REFRESH_COUNT			8192				// Refresh Count (Rows number)
//#define BSP_SDRAM_REFRESH_PERIOD		64					// Refresh Period in ms

// SDRAM Parameter definitions (CAS = 2)

#define BSP_SDRAM_SIZE					67108864			// SDRAM Size in bytes
#define BSP_SDRAM_DATA_WIDTH			32					// SDRAM Data Bus Width in bits
#define BSP_SDRAM_BANK_NO				4					// Number of internal memory banks
#define BSP_SDRAM_COL_ADDR_BITS			9					// Number of bits in column address
#define BSP_SDRAM_ROW_ADDR_BITS			13					// Number of bits in row address
#define BSP_SDRAM_CAS_LATENCY			2					// CAS Latency
#define BSP_SDRAM_WRITE_PROTECTION		0					// Write protection (0 - disable / 1 -enable)
#define BSP_SDRAM_BURST_READ			1					// Burst read (0 - disable / 1 -enable)
#define BSP_SDRAM_READ_PIPE_DELAY		0					// Read pipe delay (0,1,2 clock cycles)
#define BSP_SDRAM_TMRD					12					// Load mode register command to active or refresh command in ns
#define BSP_SDRAM_TXSR					70					// Exit Self-Refresh to Active Time in ns
#define BSP_SDRAM_TRAS					42					// ACT to PRE command period in ns
#define	BSP_SDRAM_TRC					60					// REF to REF/ACT to ACT command period in ns
#define BSP_SDRAM_TWR					2					// Input data to precharge command delay time in clock cycles
#define BSP_SDRAM_TRP					3					// PRE to ACT command period in clock cycles
#define BSP_SDRAM_TRCD					3					// Active command to read/write command delay time in clock cycles
#define BSP_SDRAM_REFRESH_COUNT			8192				// Refresh Count (Rows number)
#define BSP_SDRAM_REFRESH_PERIOD		64					// Refresh Period in ms



// Driver function definitions

uint8_t	BSP_DRV_SDRAM_Init(FMC_SDRAM_TypeDef *sdram, uint32_t SDBank, uint32_t SDClockPeriod, uint32_t Ck_Freq);


#ifdef __cplusplus
}
#endif

#endif /* INC_IS42S32160_H_ */
