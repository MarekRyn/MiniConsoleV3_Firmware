/*******************************************************************
 * MiniConsole V3 - Board Support Package - STM32 CORE
 *
 * Author: Marek Ryn
 * Version: 0.1b
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 *******************************************************************/

#ifndef STM32H7_BSP_STM32_CORE_H_
#define STM32H7_BSP_STM32_CORE_H_

#include "BSP_Common.h"

extern uint32_t SystemCoreClock;             /*!< System Domain1 Clock Frequency  */
extern uint32_t SystemD2Clock;               /*!< System Domain2 Clock Frequency  */
extern const  uint8_t D1CorePrescTable[16] ; /*!< D1CorePrescTable prescalers table values */


#define NVIC_PRIORITYGROUP_0         		((uint32_t)0x00000007) 		// 0 bits for pre-emption priority, 4 bits for subpriority
#define NVIC_PRIORITYGROUP_1         		((uint32_t)0x00000006) 		// 1 bits for pre-emption priority, 3 bits for subpriority
#define NVIC_PRIORITYGROUP_2         		((uint32_t)0x00000005) 		// 2 bits for pre-emption priority, 2 bits for subpriority
#define NVIC_PRIORITYGROUP_3         		((uint32_t)0x00000004) 		// 3 bits for pre-emption priority, 1 bits for subpriority
#define NVIC_PRIORITYGROUP_4         		((uint32_t)0x00000003) 		// 4 bits for pre-emption priority, 0 bits for subpriority


// CORTEX_MPU_HFNMI_PRIVDEF_Control MPU HFNMI and PRIVILEGED Access control
#define  MPU_HFNMI_PRIVDEF_NONE      		((uint32_t)0x00000000)
#define  MPU_HARDFAULT_NMI           		((uint32_t)0x00000002)
#define  MPU_PRIVILEGED_DEFAULT      		((uint32_t)0x00000004)
#define  MPU_HFNMI_PRIVDEF           		((uint32_t)0x00000006)

// CORTEX_MPU_Region_Enable CORTEX MPU Region Enable
#define  MPU_REGION_ENABLE     				((uint8_t)0x01)
#define  MPU_REGION_DISABLE   	 			((uint8_t)0x00)

// CORTEX_MPU_Instruction_Access CORTEX MPU Instruction Access
#define  MPU_INSTRUCTION_ACCESS_ENABLE      ((uint8_t)0x00)
#define  MPU_INSTRUCTION_ACCESS_DISABLE     ((uint8_t)0x01)

// CORTEX_MPU_Access_Shareable CORTEX MPU Instruction Access Shareable
#define  MPU_ACCESS_SHAREABLE        		((uint8_t)0x01)
#define  MPU_ACCESS_NOT_SHAREABLE    		((uint8_t)0x00)

// CORTEX_MPU_Access_Cacheable CORTEX MPU Instruction Access Cacheable
#define  MPU_ACCESS_CACHEABLE         		((uint8_t)0x01)
#define  MPU_ACCESS_NOT_CACHEABLE     		((uint8_t)0x00)

// CORTEX_MPU_Access_Bufferable CORTEX MPU Instruction Access Bufferable
#define  MPU_ACCESS_BUFFERABLE         		((uint8_t)0x01)
#define  MPU_ACCESS_NOT_BUFFERABLE     		((uint8_t)0x00)

// CORTEX_MPU_TEX_Levels MPU TEX Levels
#define  MPU_TEX_LEVEL0    					((uint8_t)0x00)
#define  MPU_TEX_LEVEL1    					((uint8_t)0x01)
#define  MPU_TEX_LEVEL2    					((uint8_t)0x02)

// CORTEX_MPU_Region_Size CORTEX MPU Region Size
#define   MPU_REGION_SIZE_32B      			((uint8_t)0x04)
#define   MPU_REGION_SIZE_64B      			((uint8_t)0x05)
#define   MPU_REGION_SIZE_128B     			((uint8_t)0x06)
#define   MPU_REGION_SIZE_256B     			((uint8_t)0x07)
#define   MPU_REGION_SIZE_512B     			((uint8_t)0x08)
#define   MPU_REGION_SIZE_1KB      			((uint8_t)0x09)
#define   MPU_REGION_SIZE_2KB      			((uint8_t)0x0A)
#define   MPU_REGION_SIZE_4KB      			((uint8_t)0x0B)
#define   MPU_REGION_SIZE_8KB      			((uint8_t)0x0C)
#define   MPU_REGION_SIZE_16KB     			((uint8_t)0x0D)
#define   MPU_REGION_SIZE_32KB     			((uint8_t)0x0E)
#define   MPU_REGION_SIZE_64KB     			((uint8_t)0x0F)
#define   MPU_REGION_SIZE_128KB    			((uint8_t)0x10)
#define   MPU_REGION_SIZE_256KB    			((uint8_t)0x11)
#define   MPU_REGION_SIZE_512KB    			((uint8_t)0x12)
#define   MPU_REGION_SIZE_1MB      			((uint8_t)0x13)
#define   MPU_REGION_SIZE_2MB      			((uint8_t)0x14)
#define   MPU_REGION_SIZE_4MB      			((uint8_t)0x15)
#define   MPU_REGION_SIZE_8MB      			((uint8_t)0x16)
#define   MPU_REGION_SIZE_16MB     			((uint8_t)0x17)
#define   MPU_REGION_SIZE_32MB     			((uint8_t)0x18)
#define   MPU_REGION_SIZE_64MB     			((uint8_t)0x19)
#define   MPU_REGION_SIZE_128MB    			((uint8_t)0x1A)
#define   MPU_REGION_SIZE_256MB    			((uint8_t)0x1B)
#define   MPU_REGION_SIZE_512MB    			((uint8_t)0x1C)
#define   MPU_REGION_SIZE_1GB      			((uint8_t)0x1D)
#define   MPU_REGION_SIZE_2GB      			((uint8_t)0x1E)
#define   MPU_REGION_SIZE_4GB      			((uint8_t)0x1F)

// CORTEX_MPU_Region_Permission_Attributes CORTEX MPU Region Permission Attributes
#define  MPU_REGION_NO_ACCESS      			((uint8_t)0x00)
#define  MPU_REGION_PRIV_RW        			((uint8_t)0x01)
#define  MPU_REGION_PRIV_RW_URO    			((uint8_t)0x02)
#define  MPU_REGION_FULL_ACCESS    			((uint8_t)0x03)
#define  MPU_REGION_PRIV_RO        			((uint8_t)0x05)
#define  MPU_REGION_PRIV_RO_URO    			((uint8_t)0x06)

// CORTEX_MPU_Region_Number CORTEX MPU Region Number
#define  MPU_REGION_NUMBER0    				((uint8_t)0x00)
#define  MPU_REGION_NUMBER1    				((uint8_t)0x01)
#define  MPU_REGION_NUMBER2    				((uint8_t)0x02)
#define  MPU_REGION_NUMBER3    				((uint8_t)0x03)
#define  MPU_REGION_NUMBER4    				((uint8_t)0x04)
#define  MPU_REGION_NUMBER5    				((uint8_t)0x05)
#define  MPU_REGION_NUMBER6    				((uint8_t)0x06)
#define  MPU_REGION_NUMBER7    				((uint8_t)0x07)
#define  MPU_REGION_NUMBER8    				((uint8_t)0x08)
#define  MPU_REGION_NUMBER9    				((uint8_t)0x09)
#define  MPU_REGION_NUMBER10   				((uint8_t)0x0A)
#define  MPU_REGION_NUMBER11   				((uint8_t)0x0B)
#define  MPU_REGION_NUMBER12   				((uint8_t)0x0C)
#define  MPU_REGION_NUMBER13   				((uint8_t)0x0D)
#define  MPU_REGION_NUMBER14   				((uint8_t)0x0E)
#define  MPU_REGION_NUMBER15   				((uint8_t)0x0F)

typedef struct {
	uint8_t                Enable;        		// Specifies the status of the region. This parameter can be a value of @ref CORTEX_MPU_Region_Enable
	uint8_t                Number;          	// Specifies the number of the region to protect. This parameter can be a value of @ref CORTEX_MPU_Region_Number
	uint32_t               BaseAddress;			// Specifies the base address of the region to protect.
	uint8_t                Size;                // Specifies the size of the region to protect. This parameter can be a value of @ref CORTEX_MPU_Region_Size
	uint8_t                SubRegionDisable;    // Specifies the number of the subregion protection to disable. This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF
	uint8_t                TypeExtField;        // Specifies the TEX field level. This parameter can be a value of @ref CORTEX_MPU_TEX_Levels
	uint8_t                AccessPermission;    // Specifies the region access permission type. This parameter can be a value of @ref CORTEX_MPU_Region_Permission_Attributes
	uint8_t                DisableExec;         // Specifies the instruction access status. This parameter can be a value of @ref CORTEX_MPU_Instruction_Access
	uint8_t                IsShareable;         // Specifies the shareability status of the protected region. This parameter can be a value of @ref CORTEX_MPU_Access_Shareable
	uint8_t                IsCacheable;         // Specifies the cacheable status of the region protected. This parameter can be a value of @ref CORTEX_MPU_Access_Cacheable
	uint8_t                IsBufferable;        // Specifies the bufferable status of the protected region. This parameter can be a value of @ref CORTEX_MPU_Access_Bufferable
} MPU_Region_TypeDef;



#define __BSP_ART_ENABLE()   SET_BIT(ART->CTR, ART_CTR_EN)
#define __BSP_ART_DISABLE()   CLEAR_BIT(ART->CTR, ART_CTR_EN)
#define __BSP_ART_CONFIG_BASE_ADDRESS(__BASE_ADDRESS__)   MODIFY_REG(ART->CTR, ART_CTR_PCACHEADDR, (((__BASE_ADDRESS__) >> 12U) & 0x000FFF00UL))

uint8_t BSP_STM32_NVIC_EnableIRQ(IRQn_Type IRQn);
uint8_t BSP_STM32_NVIC_SetPriority(IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority);
uint8_t BSP_STM32_NVIC_SetPriorityGrouping(uint32_t PriorityGroup);

uint8_t BSP_STM32_MPU_Enable(uint32_t MPU_Control);
uint8_t BSP_STM32_MPU_Disable();
uint8_t BSP_STM32_MPU_ConfigRegion(MPU_Region_TypeDef *MPU_Init);

#endif /* STM32H7_BSP_STM32_CORE_H_ */
