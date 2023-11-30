/*******************************************************************
 * MiniConsole V3 - Board Support Package - STM32 QUADSPI
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 *******************************************************************/

#ifndef STM32H7_BSP_STM32_QUADSPI_H_
#define STM32H7_BSP_STM32_QUADSPI_H_

#include "BSP_Common.h"
#include "BSP_STM32.h"

#define QSPI_SAMPLE_SHIFTING_NONE      0x00000000U                   						// No clock cycle shift to sample data
#define QSPI_SAMPLE_SHIFTING_HALFCYCLE ((uint32_t)QUADSPI_CR_SSHIFT)						// 1/2 clock cycle shift to sample data

#define QSPI_CS_HIGH_TIME_1_CYCLE      0x00000000U                                         	// nCS stay high for at least 1 clock cycle between commands
#define QSPI_CS_HIGH_TIME_2_CYCLE      ((uint32_t)QUADSPI_DCR_CSHT_0)                      	// nCS stay high for at least 2 clock cycles between commands
#define QSPI_CS_HIGH_TIME_3_CYCLE      ((uint32_t)QUADSPI_DCR_CSHT_1)                      	// nCS stay high for at least 3 clock cycles between commands
#define QSPI_CS_HIGH_TIME_4_CYCLE      ((uint32_t)QUADSPI_DCR_CSHT_0 | QUADSPI_DCR_CSHT_1) 	// nCS stay high for at least 4 clock cycles between commands
#define QSPI_CS_HIGH_TIME_5_CYCLE      ((uint32_t)QUADSPI_DCR_CSHT_2)                      	// nCS stay high for at least 5 clock cycles between commands
#define QSPI_CS_HIGH_TIME_6_CYCLE      ((uint32_t)QUADSPI_DCR_CSHT_2 | QUADSPI_DCR_CSHT_0) 	// nCS stay high for at least 6 clock cycles between commands
#define QSPI_CS_HIGH_TIME_7_CYCLE      ((uint32_t)QUADSPI_DCR_CSHT_2 | QUADSPI_DCR_CSHT_1) 	// nCS stay high for at least 7 clock cycles between commands
#define QSPI_CS_HIGH_TIME_8_CYCLE      ((uint32_t)QUADSPI_DCR_CSHT)                        	// nCS stay high for at least 8 clock cycles between commands

#define QSPI_CLOCK_MODE_0              0x00000000U                    						// Clk stays low while nCS is released
#define QSPI_CLOCK_MODE_3              ((uint32_t)QUADSPI_DCR_CKMODE) 						// Clk goes high while nCS is released

#define QSPI_FLASH_ID_1                0x00000000U                 							// FLASH 1 selected
#define QSPI_FLASH_ID_2                ((uint32_t)QUADSPI_CR_FSEL) 							// FLASH 2 selected

#define QSPI_DUALFLASH_ENABLE          ((uint32_t)QUADSPI_CR_DFM) 							// Dual-flash mode enabled
#define QSPI_DUALFLASH_DISABLE         0x00000000U                							// Dual-flash mode disabled

#define QSPI_ADDRESS_8_BITS            0x00000000U                      					// 8-bit address
#define QSPI_ADDRESS_16_BITS           ((uint32_t)QUADSPI_CCR_ADSIZE_0) 					// 16-bit address
#define QSPI_ADDRESS_24_BITS           ((uint32_t)QUADSPI_CCR_ADSIZE_1) 					// 24-bit address
#define QSPI_ADDRESS_32_BITS           ((uint32_t)QUADSPI_CCR_ADSIZE)   					// 32-bit address

#define QSPI_ALTERNATE_BYTES_8_BITS    0x00000000U                      					// 8-bit alternate bytes
#define QSPI_ALTERNATE_BYTES_16_BITS   ((uint32_t)QUADSPI_CCR_ABSIZE_0) 					// 16-bit alternate bytes
#define QSPI_ALTERNATE_BYTES_24_BITS   ((uint32_t)QUADSPI_CCR_ABSIZE_1) 					// 24-bit alternate bytes
#define QSPI_ALTERNATE_BYTES_32_BITS   ((uint32_t)QUADSPI_CCR_ABSIZE)   					// 32-bit alternate bytes

#define QSPI_INSTRUCTION_NONE          0x00000000U                     						// No instruction
#define QSPI_INSTRUCTION_1_LINE        ((uint32_t)QUADSPI_CCR_IMODE_0) 						// Instruction on a single line
#define QSPI_INSTRUCTION_2_LINES       ((uint32_t)QUADSPI_CCR_IMODE_1) 						// Instruction on two lines
#define QSPI_INSTRUCTION_4_LINES       ((uint32_t)QUADSPI_CCR_IMODE)   						// Instruction on four lines

#define QSPI_ADDRESS_NONE              0x00000000U                     						// No address
#define QSPI_ADDRESS_1_LINE            ((uint32_t)QUADSPI_CCR_ADMODE_0) 					// Address on a single line
#define QSPI_ADDRESS_2_LINES           ((uint32_t)QUADSPI_CCR_ADMODE_1) 					// Address on two lines
#define QSPI_ADDRESS_4_LINES           ((uint32_t)QUADSPI_CCR_ADMODE)   					// Address on four lines

#define QSPI_ALTERNATE_BYTES_NONE      0x00000000U                      					// No alternate bytes
#define QSPI_ALTERNATE_BYTES_1_LINE    ((uint32_t)QUADSPI_CCR_ABMODE_0) 					// Alternate bytes on a single line
#define QSPI_ALTERNATE_BYTES_2_LINES   ((uint32_t)QUADSPI_CCR_ABMODE_1) 					// Alternate bytes on two lines
#define QSPI_ALTERNATE_BYTES_4_LINES   ((uint32_t)QUADSPI_CCR_ABMODE)   					// Alternate bytes on four lines

#define QSPI_DATA_NONE                 0x00000000U                     						// No data
#define QSPI_DATA_1_LINE               ((uint32_t)QUADSPI_CCR_DMODE_0) 						// Data on a single line
#define QSPI_DATA_2_LINES              ((uint32_t)QUADSPI_CCR_DMODE_1) 						// Data on two lines
#define QSPI_DATA_4_LINES              ((uint32_t)QUADSPI_CCR_DMODE)   						// Data on four lines

#define QSPI_DDR_MODE_DISABLE          0x00000000U                  						// Double data rate mode disabled
#define QSPI_DDR_MODE_ENABLE           ((uint32_t)QUADSPI_CCR_DDRM) 						// Double data rate mode enabled

#define QSPI_DDR_HHC_ANALOG_DELAY      0x00000000U                  						// Delay the data output using analog delay in DDR mode
#define QSPI_DDR_HHC_HALF_CLK_DELAY    ((uint32_t)QUADSPI_CCR_DHHC) 						// Delay the data output by one half of system clock in DDR mode

#define QSPI_SIOO_INST_EVERY_CMD       0x00000000U                  						// Send instruction on every transaction
#define QSPI_SIOO_INST_ONLY_FIRST_CMD  ((uint32_t)QUADSPI_CCR_SIOO) 						// Send instruction only for the first command

#define QSPI_MATCH_MODE_AND            0x00000000U                							// AND match mode between unmasked bits
#define QSPI_MATCH_MODE_OR             ((uint32_t)QUADSPI_CR_PMM) 							// OR match mode between unmasked bits

#define QSPI_AUTOMATIC_STOP_DISABLE    0x00000000U                 							// AutoPolling stops only with abort or QSPI disabling
#define QSPI_AUTOMATIC_STOP_ENABLE     ((uint32_t)QUADSPI_CR_APMS) 							// AutoPolling stops as soon as there is a match

#define QSPI_TIMEOUT_COUNTER_DISABLE   0x00000000U                 							// Timeout counter disabled, nCS remains active
#define QSPI_TIMEOUT_COUNTER_ENABLE    ((uint32_t)QUADSPI_CR_TCEN) 							// Timeout counter enabled, nCS released when timeout expires

#define QSPI_FUNCTIONAL_MODE_INDIRECT_WRITE 0x00000000U                     				// Indirect write mode
#define QSPI_FUNCTIONAL_MODE_INDIRECT_READ  ((uint32_t)QUADSPI_CCR_FMODE_0) 				// Indirect read mode
#define QSPI_FUNCTIONAL_MODE_AUTO_POLLING   ((uint32_t)QUADSPI_CCR_FMODE_1) 				// Automatic polling mode
#define QSPI_FUNCTIONAL_MODE_MEMORY_MAPPED  ((uint32_t)QUADSPI_CCR_FMODE)   				// Memory-mapped mode

#define QSPI_FLAG_BUSY                 QUADSPI_SR_BUSY 										// Busy flag: operation is ongoing
#define QSPI_FLAG_TO                   QUADSPI_SR_TOF  										// Timeout flag: timeout occurs in memory-mapped mode
#define QSPI_FLAG_SM                   QUADSPI_SR_SMF  										// Status match flag: received data matches in autopolling mode
#define QSPI_FLAG_FT                   QUADSPI_SR_FTF 										// Fifo threshold flag: Fifo threshold reached or data left after read from memory is complete
#define QSPI_FLAG_TC                   QUADSPI_SR_TCF  										// Transfer complete flag: programmed number of data have been transferred or the transfer has been aborted
#define QSPI_FLAG_TE                   QUADSPI_SR_TEF  										// Transfer error flag: invalid address is being accessed


uint8_t BSP_STM32_QSPI_Init(QUADSPI_TypeDef * hqspi, uint32_t ClockPrescaler, uint32_t ClockMode, uint32_t FifoThreshold, uint32_t FlashSize);

uint8_t BSP_STM32_QSPI_CfgNewCommand();
uint8_t BSP_STM32_QSPI_CfgInstruction(uint32_t Mode, uint8_t Instruction);
uint8_t BSP_STM32_QSPI_CfgAddress(uint32_t Mode, uint32_t Size, uint32_t Address);
uint8_t BSP_STM32_QSPI_CfgAltBytes(uint32_t Mode, uint32_t Size, uint32_t AltBytes);
uint8_t BSP_STM32_QSPI_CfgDummyCycles(uint32_t DummyCycles);
uint8_t BSP_STM32_QSPI_CfgData(uint32_t Mode, uint8_t * pData, uint32_t Length);
uint8_t	BSP_STM32_QSPI_CfgOptions(uint32_t DDRMode, uint32_t SignalDelay, uint32_t SIOOMode);
uint8_t BSP_STM32_QSPI_CfgAutoPooling(uint32_t mask, uint32_t match, uint32_t interval, uint32_t matchmode);

uint8_t BSP_STM32_QSPI_Abort(QUADSPI_TypeDef * hqspi);
uint8_t BSP_STM32_QSPI_StartReceive(QUADSPI_TypeDef * hqspi);
uint8_t BSP_STM32_QSPI_StartTransmit(QUADSPI_TypeDef * hqspi);
uint8_t	BSP_STM32_QSPI_StartMemMapped(QUADSPI_TypeDef * hqspi);
uint8_t BSP_STM32_QSPI_StartAutoPooling(QUADSPI_TypeDef * hqspi);

#endif /* STM32H7_BSP_STM32_QUADSPI_H_ */
