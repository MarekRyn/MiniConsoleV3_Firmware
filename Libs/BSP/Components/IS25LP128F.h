/*******************************************************************
 * MiniConsole V3 - Driver - IS25LP128F - QSPI Memory
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 *******************************************************************/


#ifndef __IS25LP128F_H
#define __IS25LP128F_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "BSP_Common.h"
#include "BSP_STM32.h"

#define QSPI_CFG_SECTOR_SIZE	4096	// In bytes
#define QSPI_CFG_BLOCK_SIZE		8		// In sectors
#define QSPI_CFG_BLOCK_COUNT	512		// Number of blocks
#define QSPI_CFG_MEM_SIZE		QSPI_CFG_SECTOR_SIZE * QSPI_CFG_BLOCK_SIZE * QSPI_CFG_BLOCK_COUNT
#define QSPI_CFG_PAGE_SIZE		256		// Number of bytes for single page (for write operation)
#define QSPI_CFG_ADDR_SIZE		3		// Number of address bytes
#define QSPI_CFG_DUMMY_CLKS		8		// Number of dummy cycles


#define QSPI_MODE_SPI			0		// 1-Line SPI Mode
#define QSPI_MODE_QSPI			1		// 4-Line SPI Mode
#define QSPI_MODE_QPI			2		// 4-Line QPI Mode

#define QSPI_CHIP_CID			0x17	// Chip ID
#define QSPI_CHIP_MID			0x9D	// Manufacturer ID


#define QSPI_CMD_NORD			0x03	// Normal Read Mode (addr 3 bytes or 4 bytes)
#define QSPI_CMD_4NORD			0x13	// Normal Read Mode (addr 4 bytes only)
#define QSPI_CMD_FRD			0x0B	// Fast Read Mode (addr 3 bytes or 4 bytes)
#define QSPI_CMD_4FRD			0x0C	// Fast Read Mode (addr 4 bytes only)
#define QSPI_CMD_FRDIO			0xBB	// Fast Read Dual I/O (addr 3 bytes or 4 bytes)
#define QSPI_CMD_4FRDIO			0xBC	// Fast Read Dual I/O (addr 4 bytes only)
#define QSPI_CMD_FRDO			0x3B	// Fast Read Dual Output (addr 3 bytes or 4 bytes)
#define QSPI_CMD_4FRDO			0x0C	// Fast Read Dual Output (addr 4 bytes only)
#define QSPI_CMD_FRQIO			0xEB	// Fast Read Quad I/O (addr 3 bytes or 4 bytes)
#define QSPI_CMD_4FRQIO			0xEC	// Fast Read Quad I/O (addr 4 bytes only)
#define QSPI_CMD_FRQO			0x6B	// Fast Read Quad Output (addr 3 bytes or 4 bytes)
#define QSPI_CMD_4FQO			0x6C	// Fast Read Quad Output (addr 4 bytes only)
#define QSPI_CMD_FRDTR			0x0D	// Fast Read DTR Mode (addr 3 bytes or 4 bytes)
#define QSPI_CMD_4FRDTR			0x0E	// Fast Read DTR Mode (addr 4 bytes only)
#define QSPI_CMD_FRDDTR			0xBD	// Fast Read Dual I/O DTR (addr 3 bytes or 4 bytes)
#define QSPI_CMD_4FRDDTR		0xBE	// Fast Read Dual I/O DTR (addr 4 bytes only)
#define QSPI_CMD_FRQDTR			0xED	// Fast Read Quad I/O DTR (addr 3 bytes or 4 bytes)
#define QSPI_CMD_4FRQDTR		0xEE	// Fast Read Quad I/O DTR (addr 4 bytes only)

#define QSPI_CMD_PP				0x02	// Input Page Program (addr 3 bytes or 4 bytes)
#define QSPI_CMD_4PP			0x12	// Input Page Program (addr 4 bytes only)
#define QSPI_CMD_PPQ			0x32	// Input Page Program Quad (addr 3 bytes or 4 bytes)
#define QSPI_CMD_4PPQ			0x34	// Input Page Program Quad (addr 4 bytes only)

#define QSPI_CMD_SER			0xD7	// Sector Erase (addr 3 bytes or 4 bytes)
#define QSPI_CMD_4SER			0x21	// Sector Erase (addr 4 bytes only)
#define QSPI_CMD_BER32			0x52	// Block Erase 32kB (addr 3 bytes or 4 bytes)
#define QSPI_CMD_4BER32			0x5C	// Block Erase 32kB (addr 4 bytes only)
#define QSPI_CMD_BER64			0xD8	// Block Erase 32kB (addr 3 bytes or 4 bytes)
#define QSPI_CMD_4BER64			0xDC	// Block Erase 32kB (addr 4 bytes only)
#define QSPI_CMD_CER			0xC7	// Chip Erase

#define QSPI_CMD_WREN			0x06	// Write Enable
#define QSPI_CMD_WRDI			0x04	// Write Disable

#define QSPI_CMD_RDSR			0x05	// Read Status Register
#define	QSPI_CMD_WRSR			0x01	// Write Status Register
#define QSPI_CMD_RDFR			0x48	// Read Function Register
#define QSPI_CMD_WRFR			0x42	// Write Function Register

#define QSPI_CMD_QPIEN			0x35	// Enter QPI Mode
#define QSPI_CMD_QPIDI			0xF5	// Exit QPI Mode
#define QSPI_CMD_EN4B			0xB7	// Enter 4 byte address mode
#define QSPI_CMD_EX4B			0x29	// Exit 4 byte address mode

#define QSPI_CMD_PERSUS			0x75	// Suspending During Program/Erase
#define QSPI_CMD_PERRSM			0x7A	// Resume Program/Erase

#define QSPI_CMD_DP				0xB9	// Deep Power Down
#define QSPI_CMD_RDPD			0xAB	// Release Power Down
#define QSPI_CMD_RDID			0xAB	// Read ID

#define QSPI_CMD_SRPNV			0x65	// Set Read Parameters (non-volatile)
#define QSPI_CMD_SRPV			0x63	// Set Read Parameters (volatile)
#define QSPI_CMD_SERPNV			0x85	// Set Extended Read Parameters (non-volatile)
#define QSPI_CMD_SERPV			0x83	// Set Extended Read Parameters (volatile)
#define QSPI_CMD_RDRP			0x61	// Read Read Parameters
#define QSPI_CMD_RDERP			0x81	// Read Extended Read Parameters
#define QSPI_CMD_CLERP			0x82	// Clear Extended Read Parameters

#define QSPI_CMD_RDJDID			0x9F	// Read JEDEC ID
#define QSPI_CMD_RDMDID			0x90	// Read Manufacturer and device ID
#define QSPI_CMD_RDJDIDQ		0xAF	// Read JEDEC ID in QPI mode
#define QSPI_CMD_RDUID			0x4B	// Read Unique ID
#define QSPI_CMD_RDSFDP			0x5A	// Read SFDP

#define QSPI_CMD_NOP			0x00	// No operation

#define QSPI_CMD_RSTEN			0x66	// Software Reset Enable
#define QSPI_CMD_RST			0x99	// Software Reset

#define QSPI_CMD_IRER			0x64	// Erase Information Row
#define QSPI_CMD_IRP			0x62	// Program Information Row
#define QSPI_CMD_IRRD			0x68	// Read Information Row

#define QSPI_CMD_SECUNLOCK		0x26	// Sector Unlock (addr 3 bytes or 4 bytes)
#define QSPI_CMD_4SECUNLOCK		0x25	// Sector Unlock (addr 4 bytes only)
#define QSPI_CMD_SECLOCK		0x24	// Sector Lock

#define QSPI_CMD_RDABR			0x14	// Read AutoBoot Register
#define QSPI_CMD_WRABR			0x15	// Write AutoBoot Register
#define QSPI_CMD_RDBR			0x16	// Read Bank Address Register
#define QSPI_CMD_WRBRV			0x17	// Write Bank Address Register (volatile)
#define QSPI_CMD_WRBRNV			0x18	// Write Bank Address Register (non-volatile)

#define QSPI_CMD_RDDYB			0xFA	// Read DYB (addr 3 bytes or 4 bytes)
#define QSPI_CMD_4RDDYB			0xE0	// Read DYB (addr 4 bytes only)
#define QSPI_CMD_WRDYB			0xFB	// Write DYB (addr 3 bytes or 4 bytes)
#define QSPI_CMD_4WRDYB			0xE1	// Write DYB (addr 4 bytes only)
#define QSPI_CMD_RDPPB			0xFC	// Read PPB	(addr 3 bytes or 4 bytes)
#define QSPI_CMD_4RDPPB			0xE2	// Read PPB (addr 4 bytes only)
#define QSPI_CMD_PGPPB			0xFD	// Program PPB individually (addr 3 bytes or 4 bytes)
#define QSPI_CMD_4PGPPB			0xE3	// Program PPB individually (addr 4 bytes only)
#define QSPI_CMD_ERPPB			0xE4	// Erase PPB (as a group)
#define QSPI_CMD_RDASP			0x2B	// Read ASP
#define QSPI_CMD_PGASP			0x2F	// Program ASP
#define QSPI_CMD_RDPLB			0xA7	// Read PPB Lock Bit
#define QSPI_CMD_WRPLB			0xA6	// Write PPB Lock Bit
#define QSPI_CMD_SFRZ			0x91	// Set Freeze Bit
#define QSPI_CMD_RDPWD			0xE7	// Read Password
#define QSPI_CMD_PGPWD			0xE8	// Program Password
#define QSPI_CMD_UNPWD			0xE9	// Unlock Password
#define QSPI_CMD_GBLK			0x7E	// Set All DYB Bits (Gang sector / Block sector)
#define QSPI_CMD_GBUN			0x98	// Clear All DYB Bits (Gang sector / Block sector)




uint8_t	BSP_DRV_QSPI_Reset(QUADSPI_TypeDef * hqspi);
uint8_t BSP_DRV_QSPI_Init(QUADSPI_TypeDef * hqspi);
uint8_t BSP_DRV_QSPI_MemMappedEnable(QUADSPI_TypeDef * hqspi);
uint8_t BSP_DRV_QSPI_WriteBuf(QUADSPI_TypeDef * hqspi, uint32_t address, uint8_t * buf, uint32_t length);
uint8_t BSP_DRV_QSPI_ReadBuf(QUADSPI_TypeDef * hqspi, uint32_t address, uint8_t * buf, uint32_t length);
uint8_t	BSP_DRV_QSPI_ChipErase(QUADSPI_TypeDef * hqspi); // OK
uint8_t BSP_DRV_QSPI_SectorErase(QUADSPI_TypeDef * hqspi, uint32_t address);
uint8_t BSP_DRV_QSPI_ReadUID(QUADSPI_TypeDef * hqspi, uint8_t * buf);


#ifdef __cplusplus
}
#endif

#endif


