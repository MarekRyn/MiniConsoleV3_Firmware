/*******************************************************************
 * MiniConsole V3 - Board Support Package - STM32 RCC
 *
 * Author: Marek Ryn
 * Version: 1.0
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 * - 1.0	- Initial version
 *******************************************************************/

#ifndef STM32H7_BSP_STM32_RCC_H_
#define STM32H7_BSP_STM32_RCC_H_

#include "BSP_Common.h"
#include "BSP_STM32.h"

#define RCC_HSI_VALUE	25000000;

#define RCC_HSE_OFF                    (0x00000000U)
#define RCC_HSE_ON                     RCC_CR_HSEON
#define RCC_HSE_BYPASS                 ((uint32_t)(RCC_CR_HSEBYP | RCC_CR_HSEON))
#define RCC_HSE_BYPASS_DIGITAL         ((uint32_t)(RCC_CR_HSEEXT | RCC_CR_HSEBYP | RCC_CR_HSEON))

#define RCC_HSI_OFF                      (0x00000000U)
#define RCC_HSI_ON                       RCC_CR_HSION

#define RCC_HSI_DIV1                     (RCC_CR_HSIDIV_1 | RCC_CR_HSION) // HSI_DIV1 clock activation
#define RCC_HSI_DIV2                     (RCC_CR_HSIDIV_2 | RCC_CR_HSION) // HSI_DIV2 clock activation
#define RCC_HSI_DIV4                     (RCC_CR_HSIDIV_4 | RCC_CR_HSION) // HSI_DIV4 clock activation
#define RCC_HSI_DIV8                     (RCC_CR_HSIDIV | RCC_CR_HSION)   // HSI_DIV8 clock activation

#define RCC_HCLK_DIV1                RCC_D1CFGR_HPRE_DIV1
#define RCC_HCLK_DIV2                RCC_D1CFGR_HPRE_DIV2
#define RCC_HCLK_DIV4                RCC_D1CFGR_HPRE_DIV4
#define RCC_HCLK_DIV8                RCC_D1CFGR_HPRE_DIV8
#define RCC_HCLK_DIV16               RCC_D1CFGR_HPRE_DIV16
#define RCC_HCLK_DIV64               RCC_D1CFGR_HPRE_DIV64
#define RCC_HCLK_DIV128              RCC_D1CFGR_HPRE_DIV128
#define RCC_HCLK_DIV256              RCC_D1CFGR_HPRE_DIV256
#define RCC_HCLK_DIV512              RCC_D1CFGR_HPRE_DIV512

#define RCC_SYSCLK_DIV1                RCC_D1CFGR_D1CPRE_DIV1
#define RCC_SYSCLK_DIV2                RCC_D1CFGR_D1CPRE_DIV2
#define RCC_SYSCLK_DIV4                RCC_D1CFGR_D1CPRE_DIV4
#define RCC_SYSCLK_DIV8                RCC_D1CFGR_D1CPRE_DIV8
#define RCC_SYSCLK_DIV16               RCC_D1CFGR_D1CPRE_DIV16
#define RCC_SYSCLK_DIV64               RCC_D1CFGR_D1CPRE_DIV64
#define RCC_SYSCLK_DIV128              RCC_D1CFGR_D1CPRE_DIV128
#define RCC_SYSCLK_DIV256              RCC_D1CFGR_D1CPRE_DIV256
#define RCC_SYSCLK_DIV512              RCC_D1CFGR_D1CPRE_DIV512

#define RCC_APB1_DIV1                  RCC_D2CFGR_D2PPRE1_DIV1
#define RCC_APB1_DIV2                  RCC_D2CFGR_D2PPRE1_DIV2
#define RCC_APB1_DIV4                  RCC_D2CFGR_D2PPRE1_DIV4
#define RCC_APB1_DIV8                  RCC_D2CFGR_D2PPRE1_DIV8
#define RCC_APB1_DIV16                 RCC_D2CFGR_D2PPRE1_DIV16

#define RCC_APB2_DIV1                  RCC_D2CFGR_D2PPRE2_DIV1
#define RCC_APB2_DIV2                  RCC_D2CFGR_D2PPRE2_DIV2
#define RCC_APB2_DIV4                  RCC_D2CFGR_D2PPRE2_DIV4
#define RCC_APB2_DIV8                  RCC_D2CFGR_D2PPRE2_DIV8
#define RCC_APB2_DIV16                 RCC_D2CFGR_D2PPRE2_DIV16

#define RCC_APB3_DIV1                  RCC_D1CFGR_D1PPRE_DIV1
#define RCC_APB3_DIV2                  RCC_D1CFGR_D1PPRE_DIV2
#define RCC_APB3_DIV4                  RCC_D1CFGR_D1PPRE_DIV4
#define RCC_APB3_DIV8                  RCC_D1CFGR_D1PPRE_DIV8
#define RCC_APB3_DIV16                 RCC_D1CFGR_D1PPRE_DIV16

#define RCC_APB4_DIV1                  RCC_D3CFGR_D3PPRE_DIV1
#define RCC_APB4_DIV2                  RCC_D3CFGR_D3PPRE_DIV2
#define RCC_APB4_DIV4                  RCC_D3CFGR_D3PPRE_DIV4
#define RCC_APB4_DIV8                  RCC_D3CFGR_D3PPRE_DIV8
#define RCC_APB4_DIV16                 RCC_D3CFGR_D3PPRE_DIV16



#define RCC_LSE_OFF                    (0x00000000U)
#define RCC_LSE_ON                     RCC_BDCR_LSEON
#define RCC_LSE_BYPASS                 ((uint32_t)(RCC_BDCR_LSEBYP | RCC_BDCR_LSEON))

#define RCC_HSI48_OFF                      ((uint8_t)0x00)
#define RCC_HSI48_ON                       ((uint8_t)0x01)

#define RCC_PLL_NONE                   (0x00000000U)
#define RCC_PLL_OFF                    (0x00000001U)
#define RCC_PLL_ON                     (0x00000002U)

#define RCC_PLL1_DIVP                RCC_PLLCFGR_DIVP1EN
#define RCC_PLL1_DIVQ                RCC_PLLCFGR_DIVQ1EN
#define RCC_PLL1_DIVR                RCC_PLLCFGR_DIVR1EN

#define RCC_PLL3_DIVP                RCC_PLLCFGR_DIVP3EN
#define RCC_PLL3_DIVQ                RCC_PLLCFGR_DIVQ3EN
#define RCC_PLL3_DIVR                RCC_PLLCFGR_DIVR3EN

#define RCC_CLOCKTYPE_SYSCLK           (0x00000001U)
#define RCC_CLOCKTYPE_HCLK             (0x00000002U)
#define RCC_CLOCKTYPE_D1PCLK1          (0x00000004U)
#define RCC_CLOCKTYPE_PCLK1            (0x00000008U)
#define RCC_CLOCKTYPE_PCLK2            (0x00000010U)
#define RCC_CLOCKTYPE_D3PCLK1          (0x00000020U)

#define RCC_SYSCLKSOURCE_CSI           RCC_CFGR_SW_CSI
#define RCC_SYSCLKSOURCE_HSI           RCC_CFGR_SW_HSI
#define RCC_SYSCLKSOURCE_HSE           RCC_CFGR_SW_HSE
#define RCC_SYSCLKSOURCE_PLLCLK        RCC_CFGR_SW_PLL1

#define RCC_PLLSOURCE_HSI              (0x00000000U)
#define RCC_PLLSOURCE_CSI              (0x00000001U)
#define RCC_PLLSOURCE_HSE              (0x00000002U)
#define RCC_PLLSOURCE_NONE             (0x00000003U)

#define RCC_RTCCLKSOURCE_NO_CLK          (0x00000000U)
#define RCC_RTCCLKSOURCE_LSE             (0x00000100U)
#define RCC_RTCCLKSOURCE_LSI             (0x00000200U)
#define RCC_RTCCLKSOURCE_HSE_DIV2        (0x00002300U)
#define RCC_RTCCLKSOURCE_HSE_DIV3        (0x00003300U)
#define RCC_RTCCLKSOURCE_HSE_DIV4        (0x00004300U)
#define RCC_RTCCLKSOURCE_HSE_DIV5        (0x00005300U)
#define RCC_RTCCLKSOURCE_HSE_DIV6        (0x00006300U)
#define RCC_RTCCLKSOURCE_HSE_DIV7        (0x00007300U)
#define RCC_RTCCLKSOURCE_HSE_DIV8        (0x00008300U)
#define RCC_RTCCLKSOURCE_HSE_DIV9        (0x00009300U)
#define RCC_RTCCLKSOURCE_HSE_DIV10       (0x0000A300U)
#define RCC_RTCCLKSOURCE_HSE_DIV11       (0x0000B300U)
#define RCC_RTCCLKSOURCE_HSE_DIV12       (0x0000C300U)
#define RCC_RTCCLKSOURCE_HSE_DIV13       (0x0000D300U)
#define RCC_RTCCLKSOURCE_HSE_DIV14       (0x0000E300U)
#define RCC_RTCCLKSOURCE_HSE_DIV15       (0x0000F300U)
#define RCC_RTCCLKSOURCE_HSE_DIV16       (0x00010300U)
#define RCC_RTCCLKSOURCE_HSE_DIV17       (0x00011300U)
#define RCC_RTCCLKSOURCE_HSE_DIV18       (0x00012300U)
#define RCC_RTCCLKSOURCE_HSE_DIV19       (0x00013300U)
#define RCC_RTCCLKSOURCE_HSE_DIV20       (0x00014300U)
#define RCC_RTCCLKSOURCE_HSE_DIV21       (0x00015300U)
#define RCC_RTCCLKSOURCE_HSE_DIV22       (0x00016300U)
#define RCC_RTCCLKSOURCE_HSE_DIV23       (0x00017300U)
#define RCC_RTCCLKSOURCE_HSE_DIV24       (0x00018300U)
#define RCC_RTCCLKSOURCE_HSE_DIV25       (0x00019300U)
#define RCC_RTCCLKSOURCE_HSE_DIV26       (0x0001A300U)
#define RCC_RTCCLKSOURCE_HSE_DIV27       (0x0001B300U)
#define RCC_RTCCLKSOURCE_HSE_DIV28       (0x0001C300U)
#define RCC_RTCCLKSOURCE_HSE_DIV29       (0x0001D300U)
#define RCC_RTCCLKSOURCE_HSE_DIV30       (0x0001E300U)
#define RCC_RTCCLKSOURCE_HSE_DIV31       (0x0001F300U)
#define RCC_RTCCLKSOURCE_HSE_DIV32       (0x00020300U)
#define RCC_RTCCLKSOURCE_HSE_DIV33       (0x00021300U)
#define RCC_RTCCLKSOURCE_HSE_DIV34       (0x00022300U)
#define RCC_RTCCLKSOURCE_HSE_DIV35       (0x00023300U)
#define RCC_RTCCLKSOURCE_HSE_DIV36       (0x00024300U)
#define RCC_RTCCLKSOURCE_HSE_DIV37       (0x00025300U)
#define RCC_RTCCLKSOURCE_HSE_DIV38       (0x00026300U)
#define RCC_RTCCLKSOURCE_HSE_DIV39       (0x00027300U)
#define RCC_RTCCLKSOURCE_HSE_DIV40       (0x00028300U)
#define RCC_RTCCLKSOURCE_HSE_DIV41       (0x00029300U)
#define RCC_RTCCLKSOURCE_HSE_DIV42       (0x0002A300U)
#define RCC_RTCCLKSOURCE_HSE_DIV43       (0x0002B300U)
#define RCC_RTCCLKSOURCE_HSE_DIV44       (0x0002C300U)
#define RCC_RTCCLKSOURCE_HSE_DIV45       (0x0002D300U)
#define RCC_RTCCLKSOURCE_HSE_DIV46       (0x0002E300U)
#define RCC_RTCCLKSOURCE_HSE_DIV47       (0x0002F300U)
#define RCC_RTCCLKSOURCE_HSE_DIV48       (0x00030300U)
#define RCC_RTCCLKSOURCE_HSE_DIV49       (0x00031300U)
#define RCC_RTCCLKSOURCE_HSE_DIV50       (0x00032300U)
#define RCC_RTCCLKSOURCE_HSE_DIV51       (0x00033300U)
#define RCC_RTCCLKSOURCE_HSE_DIV52       (0x00034300U)
#define RCC_RTCCLKSOURCE_HSE_DIV53       (0x00035300U)
#define RCC_RTCCLKSOURCE_HSE_DIV54       (0x00036300U)
#define RCC_RTCCLKSOURCE_HSE_DIV55       (0x00037300U)
#define RCC_RTCCLKSOURCE_HSE_DIV56       (0x00038300U)
#define RCC_RTCCLKSOURCE_HSE_DIV57       (0x00039300U)
#define RCC_RTCCLKSOURCE_HSE_DIV58       (0x0003A300U)
#define RCC_RTCCLKSOURCE_HSE_DIV59       (0x0003B300U)
#define RCC_RTCCLKSOURCE_HSE_DIV60       (0x0003C300U)
#define RCC_RTCCLKSOURCE_HSE_DIV61       (0x0003D300U)
#define RCC_RTCCLKSOURCE_HSE_DIV62       (0x0003E300U)
#define RCC_RTCCLKSOURCE_HSE_DIV63       (0x0003F300U)

#define RCC_I2C123CLKSOURCE_D2PCLK1      (0x00000000U)
#define RCC_I2C4CLKSOURCE_D3PCLK1      	 (0x00000000U)
#define RCC_SPI123CLKSOURCE_PLL2         RCC_D2CCIP1R_SPI123SEL_0

#define RCC_QSPICLKSOURCE_D1HCLK       (0x00000000U)
#define RCC_QSPICLKSOURCE_PLL          RCC_D1CCIPR_QSPISEL_0
#define RCC_QSPICLKSOURCE_PLL2         RCC_D1CCIPR_QSPISEL_1
#define RCC_QSPICLKSOURCE_CLKP         RCC_D1CCIPR_QSPISEL

#define RCC_SDMMCCLKSOURCE_PLL           (0x00000000U)
#define RCC_SDMMCCLKSOURCE_PLL2           RCC_D1CCIPR_SDMMCSEL

#define RCC_ADCCLKSOURCE_PLL2       (0x00000000U)
#define RCC_ADCCLKSOURCE_PLL3       RCC_D3CCIPR_ADCSEL_0
#define RCC_ADCCLKSOURCE_CLKP       RCC_D3CCIPR_ADCSEL_1

#define RCC_USART234578CLKSOURCE_D2PCLK1    (0x00000000U)

#define RCC_USBCLKSOURCE_PLL                  RCC_D2CCIP2R_USBSEL_0
#define RCC_USBCLKSOURCE_PLL3                 RCC_D2CCIP2R_USBSEL_1
#define RCC_USBCLKSOURCE_HSI48                RCC_D2CCIP2R_USBSEL

#define RCC_LSEDRIVE_LOW                 (0x00000000U) 			// LSE low drive capability
#define RCC_LSEDRIVE_MEDIUMLOW           RCC_BDCR_LSEDRV_0      // LSE medium low drive capability
#define RCC_LSEDRIVE_MEDIUMHIGH          RCC_BDCR_LSEDRV_1      // LSE medium high drive capability
#define RCC_LSEDRIVE_HIGH                RCC_BDCR_LSEDRV        // LSE high drive capability

#define RCC_HSICALIBRATION_DEFAULT       (0x40U)         // Default HSI calibration trimming value for STM32H7 rev.V and above. (0x20 value for rev.Y handled within __HAL_RCC_HSI_CALIBRATIONVALUE_ADJUST macro )


#define RCC_PLL1VCIRANGE_0                RCC_PLLCFGR_PLL1RGE_0        // Clock range frequency between 1 and 2 MHz
#define RCC_PLL1VCIRANGE_1                RCC_PLLCFGR_PLL1RGE_1        // Clock range frequency between 2 and 4 MHz
#define RCC_PLL1VCIRANGE_2                RCC_PLLCFGR_PLL1RGE_2        // Clock range frequency between 4 and 8 MHz
#define RCC_PLL1VCIRANGE_3                RCC_PLLCFGR_PLL1RGE_3        // Clock range frequency between 8 and 16 MHz

#define RCC_PLL2VCIRANGE_0                RCC_PLLCFGR_PLL2RGE_0        // Clock range frequency between 1 and 2 MHz
#define RCC_PLL2VCIRANGE_1                RCC_PLLCFGR_PLL2RGE_1        // Clock range frequency between 2 and 4 MHz
#define RCC_PLL2VCIRANGE_2                RCC_PLLCFGR_PLL2RGE_2        // Clock range frequency between 4 and 8 MHz
#define RCC_PLL2VCIRANGE_3                RCC_PLLCFGR_PLL2RGE_3        // Clock range frequency between 8 and 16 MHz

#define RCC_PLL3VCIRANGE_0                RCC_PLLCFGR_PLL3RGE_0        // Clock range frequency between 1 and 2 MHz
#define RCC_PLL3VCIRANGE_1                RCC_PLLCFGR_PLL3RGE_1        // Clock range frequency between 2 and 4 MHz
#define RCC_PLL3VCIRANGE_2                RCC_PLLCFGR_PLL3RGE_2        // Clock range frequency between 4 and 8 MHz
#define RCC_PLL3VCIRANGE_3                RCC_PLLCFGR_PLL3RGE_3        // Clock range frequency between 8 and 16 MHz

#define RCC_PLL1VCOWIDE                 (0x00000000U)
#define RCC_PLL1VCOMEDIUM               RCC_PLLCFGR_PLL1VCOSEL

#define RCC_PLL2VCOWIDE                 (0x00000000U)
#define RCC_PLL2VCOMEDIUM               RCC_PLLCFGR_PLL2VCOSEL

#define RCC_PLL3VCOWIDE                 (0x00000000U)
#define RCC_PLL3VCOMEDIUM               RCC_PLLCFGR_PLL3VCOSEL

#define RCC_FLAG_MASK  ((uint8_t)0x1F)

#define FLASH_LATENCY_0          FLASH_ACR_LATENCY_0WS   // FLASH Zero Latency cycle
#define FLASH_LATENCY_1          FLASH_ACR_LATENCY_1WS   // FLASH One Latency cycle
#define FLASH_LATENCY_2          FLASH_ACR_LATENCY_2WS   // FLASH Two Latency cycles
#define FLASH_LATENCY_3          FLASH_ACR_LATENCY_3WS   // FLASH Three Latency cycles
#define FLASH_LATENCY_4          FLASH_ACR_LATENCY_4WS   // FLASH Four Latency cycles
#define FLASH_LATENCY_5          FLASH_ACR_LATENCY_5WS   // FLASH Five Latency cycles
#define FLASH_LATENCY_6          FLASH_ACR_LATENCY_6WS   // FLASH Six Latency cycles
#define FLASH_LATENCY_7          FLASH_ACR_LATENCY_7WS   // FLASH Seven Latency cycles
#define FLASH_LATENCY_8          FLASH_ACR_LATENCY_8WS   // FLASH Eight Latency cycle
#define FLASH_LATENCY_9          FLASH_ACR_LATENCY_9WS   // FLASH Nine Latency cycle
#define FLASH_LATENCY_10         FLASH_ACR_LATENCY_10WS  // FLASH Ten Latency cycles
#define FLASH_LATENCY_11         FLASH_ACR_LATENCY_11WS  // FLASH Eleven Latency cycles
#define FLASH_LATENCY_12         FLASH_ACR_LATENCY_12WS  // FLASH Twelve Latency cycles
#define FLASH_LATENCY_13         FLASH_ACR_LATENCY_13WS  // FLASH Thirteen Latency cycles
#define FLASH_LATENCY_14         FLASH_ACR_LATENCY_14WS  // FLASH Fourteen Latency cycles
#define FLASH_LATENCY_15         FLASH_ACR_LATENCY_15WS  // FLASH Fifteen Latency cycles


#define __BSP_RCC_BACKUPRESET_FORCE()   SET_BIT(RCC->BDCR, RCC_BDCR_BDRST)
#define __BSP_RCC_BACKUPRESET_RELEASE() CLEAR_BIT(RCC->BDCR, RCC_BDCR_BDRST)

#define __BSP_RCC_RTC_ENABLE() SET_BIT(RCC->BDCR, RCC_BDCR_RTCEN)
#define __BSP_RCC_RTC_CLKPRESCALER(__RTCCLKSource__) (((__RTCCLKSource__) & RCC_BDCR_RTCSEL) == RCC_BDCR_RTCSEL) ? MODIFY_REG(RCC->CFGR, RCC_CFGR_RTCPRE, (((__RTCCLKSource__) & 0xFFFFCFFU) >> 4)) : CLEAR_BIT(RCC->CFGR, RCC_CFGR_RTCPRE)
#define __BSP_RCC_RTC_CONFIG(__RTCCLKSource__) do { __BSP_RCC_RTC_CLKPRESCALER(__RTCCLKSource__); RCC->BDCR |= ((__RTCCLKSource__) & 0x00000FFFU); } while (0)


#define __BSP_RCC_GPIOA_CLK_ENABLE()	do { __IO uint32_t tmpreg; SET_BIT(RCC->AHB4ENR, RCC_AHB4ENR_GPIOAEN); tmpreg = READ_BIT(RCC->AHB4ENR, RCC_AHB4ENR_GPIOAEN); UNUSED(tmpreg); } while(0)
#define __BSP_RCC_GPIOB_CLK_ENABLE()	do { __IO uint32_t tmpreg; SET_BIT(RCC->AHB4ENR, RCC_AHB4ENR_GPIOBEN); tmpreg = READ_BIT(RCC->AHB4ENR, RCC_AHB4ENR_GPIOBEN); UNUSED(tmpreg); } while(0)
#define __BSP_RCC_GPIOC_CLK_ENABLE()	do { __IO uint32_t tmpreg; SET_BIT(RCC->AHB4ENR, RCC_AHB4ENR_GPIOCEN); tmpreg = READ_BIT(RCC->AHB4ENR, RCC_AHB4ENR_GPIOCEN); UNUSED(tmpreg); } while(0)
#define __BSP_RCC_GPIOD_CLK_ENABLE()	do { __IO uint32_t tmpreg; SET_BIT(RCC->AHB4ENR, RCC_AHB4ENR_GPIODEN); tmpreg = READ_BIT(RCC->AHB4ENR, RCC_AHB4ENR_GPIODEN); UNUSED(tmpreg); } while(0)
#define __BSP_RCC_GPIOE_CLK_ENABLE()	do { __IO uint32_t tmpreg; SET_BIT(RCC->AHB4ENR, RCC_AHB4ENR_GPIOEEN); tmpreg = READ_BIT(RCC->AHB4ENR, RCC_AHB4ENR_GPIOEEN); UNUSED(tmpreg); } while(0)
#define __BSP_RCC_GPIOF_CLK_ENABLE()	do { __IO uint32_t tmpreg; SET_BIT(RCC->AHB4ENR, RCC_AHB4ENR_GPIOFEN); tmpreg = READ_BIT(RCC->AHB4ENR, RCC_AHB4ENR_GPIOFEN); UNUSED(tmpreg); } while(0)
#define __BSP_RCC_GPIOG_CLK_ENABLE()	do { __IO uint32_t tmpreg; SET_BIT(RCC->AHB4ENR, RCC_AHB4ENR_GPIOGEN); tmpreg = READ_BIT(RCC->AHB4ENR, RCC_AHB4ENR_GPIOGEN); UNUSED(tmpreg); } while(0)
#define __BSP_RCC_GPIOH_CLK_ENABLE()	do { __IO uint32_t tmpreg; SET_BIT(RCC->AHB4ENR, RCC_AHB4ENR_GPIOHEN); tmpreg = READ_BIT(RCC->AHB4ENR, RCC_AHB4ENR_GPIOHEN); UNUSED(tmpreg); } while(0)
#define __BSP_RCC_GPIOI_CLK_ENABLE()	do { __IO uint32_t tmpreg; SET_BIT(RCC->AHB4ENR, RCC_AHB4ENR_GPIOIEN); tmpreg = READ_BIT(RCC->AHB4ENR, RCC_AHB4ENR_GPIOIEN); UNUSED(tmpreg); } while(0)

#define __BSP_RCC_SYSCFG_CLK_ENABLE()   do { __IO uint32_t tmpreg; SET_BIT(RCC->APB4ENR, RCC_APB4ENR_SYSCFGEN); tmpreg = READ_BIT(RCC->APB4ENR, RCC_APB4ENR_SYSCFGEN); UNUSED(tmpreg); } while(0)
#define __BSP_RCC_ART_CLK_ENABLE()      do { __IO uint32_t tmpreg; SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_ARTEN); tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_ARTEN); UNUSED(tmpreg); } while(0)
#define __BSP_RCC_HSEM_CLK_ENABLE()		do { __IO uint32_t tmpreg; SET_BIT(RCC->AHB4ENR, RCC_AHB4ENR_HSEMEN); tmpreg = READ_BIT(RCC->AHB4ENR, RCC_AHB4ENR_HSEMEN); UNUSED(tmpreg); } while(0)
#define __BSP_RCC_FMC_CLK_ENABLE()		do { __IO uint32_t tmpreg; SET_BIT(RCC->AHB3ENR, RCC_AHB3ENR_FMCEN); tmpreg = READ_BIT(RCC->AHB3ENR, RCC_AHB3ENR_FMCEN); UNUSED(tmpreg); } while(0)
#define __BSP_RCC_LTDC_CLK_ENABLE()		do { __IO uint32_t tmpreg; SET_BIT(RCC->APB3ENR, RCC_APB3ENR_LTDCEN); tmpreg = READ_BIT(RCC->APB3ENR, RCC_APB3ENR_LTDCEN); UNUSED(tmpreg); } while(0)
#define __BSP_RCC_DMA2D_CLK_ENABLE()	do { __IO uint32_t tmpreg; SET_BIT(RCC->AHB3ENR, RCC_AHB3ENR_DMA2DEN); tmpreg = READ_BIT(RCC->AHB3ENR, RCC_AHB3ENR_DMA2DEN); UNUSED(tmpreg); } while(0)
#define __BSP_RCC_DMA1_CLK_ENABLE()		do { __IO uint32_t tmpreg; SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_DMA1EN); tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_DMA1EN); UNUSED(tmpreg); } while(0)
#define __BSP_RCC_BDMA_CLK_ENABLE()		do { __IO uint32_t tmpreg; SET_BIT(RCC->AHB4ENR, RCC_AHB4ENR_BDMAEN); tmpreg = READ_BIT(RCC->AHB4ENR, RCC_AHB4ENR_BDMAEN); UNUSED(tmpreg); } while(0)
#define __BSP_RCC_ADC12_CLK_ENABLE()	do { __IO uint32_t tmpreg; SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_ADC12EN); tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_ADC12EN); UNUSED(tmpreg); } while(0)
#define __BSP_RCC_I2C1_CLK_ENABLE()		do { __IO uint32_t tmpreg; SET_BIT(RCC->APB1LENR, RCC_APB1LENR_I2C1EN);	tmpreg = READ_BIT(RCC->APB1LENR, RCC_APB1LENR_I2C1EN);	UNUSED(tmpreg); } while(0)
#define __BSP_RCC_I2C4_CLK_ENABLE()		do { __IO uint32_t tmpreg; SET_BIT(RCC->APB4ENR, RCC_APB4ENR_I2C4EN); tmpreg = READ_BIT(RCC->APB4ENR, RCC_APB4ENR_I2C4EN); UNUSED(tmpreg); } while(0)
#define __BSP_RCC_I2S3_CLK_ENABLE()		do { __IO uint32_t tmpreg; SET_BIT(RCC->APB1LENR, RCC_APB1LENR_SPI3EN); tmpreg = READ_BIT(RCC->APB1LENR, RCC_APB1LENR_SPI3EN); UNUSED(tmpreg); } while(0)
#define __BSP_RCC_QSPI_CLK_ENABLE()		do { __IO uint32_t tmpreg; SET_BIT(RCC->AHB3ENR, RCC_AHB3ENR_QSPIEN); tmpreg = READ_BIT(RCC->AHB3ENR, RCC_AHB3ENR_QSPIEN); UNUSED(tmpreg); } while(0)
#define __BSP_RCC_SDMMC2_CLK_ENABLE()	do { __IO uint32_t tmpreg; SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_SDMMC2EN); tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_SDMMC2EN); UNUSED(tmpreg); } while(0)
#define __BSP_RCC_TIM2_CLK_ENABLE()		do { __IO uint32_t tmpreg; SET_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM2EN); tmpreg = READ_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM2EN); UNUSED(tmpreg); } while(0)
#define __BSP_RCC_TIM3_CLK_ENABLE()		do { __IO uint32_t tmpreg; SET_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM3EN); tmpreg = READ_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM3EN); UNUSED(tmpreg); } while(0)
#define __BSP_RCC_TIM4_CLK_ENABLE()		do { __IO uint32_t tmpreg; SET_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM4EN); tmpreg = READ_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM4EN); UNUSED(tmpreg); } while(0)
#define __BSP_RCC_UART5_CLK_ENABLE()	do { __IO uint32_t tmpreg; SET_BIT(RCC->APB1LENR, RCC_APB1LENR_UART5EN); tmpreg = READ_BIT(RCC->APB1LENR, RCC_APB1LENR_UART5EN); UNUSED(tmpreg); } while(0)
#define __BSP_RCC_USART2_CLK_ENABLE()	do { __IO uint32_t tmpreg; SET_BIT(RCC->APB1LENR, RCC_APB1LENR_USART2EN); tmpreg = READ_BIT(RCC->APB1LENR, RCC_APB1LENR_USART2EN); UNUSED(tmpreg); } while(0)
#define __BSP_RCC_JPGDECEN_CLK_ENABLE() do { __IO uint32_t tmpreg; SET_BIT(RCC->AHB3ENR, RCC_AHB3ENR_JPGDECEN); tmpreg = READ_BIT(RCC->AHB3ENR, RCC_AHB3ENR_JPGDECEN); UNUSED(tmpreg); } while(0)
#define __BSP_RCC_USB2_FS_CLK_ENABLE()  do { __IO uint32_t tmpreg; SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_USB2OTGHSEN); tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_USB2OTGHSEN); UNUSED(tmpreg); } while(0)
#define __BSP_RCC_BKPRAM_CLK_ENABLE()   do { __IO uint32_t tmpreg; SET_BIT(RCC->AHB4ENR, RCC_AHB4ENR_BKPRAMEN); tmpreg = READ_BIT(RCC->AHB4ENR, RCC_AHB4ENR_BKPRAMEN); UNUSED(tmpreg); } while(0)
#define __BSP_RCC_CRC_CLK_ENABLE()   	do { __IO uint32_t tmpreg; SET_BIT(RCC->AHB4ENR, RCC_AHB4ENR_CRCEN); tmpreg = READ_BIT(RCC->AHB4ENR, RCC_AHB4ENR_CRCEN); UNUSED(tmpreg); } while(0)


uint8_t BSP_STM32_RCC_CLKConfig_RTC(void);
uint8_t BSP_STM32_RCC_CLKConfig_FMC(void);
uint8_t BSP_STM32_RCC_CLKConfig_I2C1(void);
uint8_t BSP_STM32_RCC_CLKConfig_I2C4(void);
uint8_t BSP_STM32_RCC_CLKConfig_I2S3(void);
uint8_t BSP_STM32_RCC_CLKConfig_QSPI(void);
uint8_t BSP_STM32_RCC_CLKConfig_SDMMC2(void);
uint8_t BSP_STM32_RCC_CLKConfig_UART5(void);
uint8_t BSP_STM32_RCC_CLKConfig_USART2(void);
uint8_t BSP_STM32_RCC_CLKConfig_USBFS(void);
uint8_t BSP_STM32_RCC_CLKConfig_ADC(void);
uint8_t	BSP_STM32_RCC_CLKConfig_LTDC(void);

uint8_t BSP_STM32_RCC_PLLClockSource(uint32_t pllsource);
uint8_t BSP_STM32_RCC_LSEConfigDrive(uint32_t lsedrive);

uint8_t BSP_STM32_RCC_ConfigHSE(uint32_t hsestate);
uint8_t BSP_STM32_RCC_ConfigHSI(uint32_t hsistate, uint32_t hsicalvalue);
uint8_t BSP_STM32_RCC_ConfigLSE(uint32_t lsestate);
uint8_t BSP_STM32_RCC_ConfigHSI48(uint32_t hsi48state);
uint8_t BSP_STM32_RCC_ConfigPLL1(uint32_t pllstate, uint32_t pllsource, uint32_t pllrge, uint32_t pllvcosel, uint32_t m, uint32_t n, uint32_t p, uint32_t q, uint32_t r);
uint8_t BSP_STM32_RCC_ConfigPLL3(uint32_t pllrge, uint32_t pllvcosel, uint32_t m, uint32_t n, uint32_t p, uint32_t q, uint32_t r, uint32_t frac);
uint8_t BSP_STM32_RCC_ClockConfig(uint32_t clocktype, uint32_t SYSSource, uint32_t SYSDiv, uint32_t AHBDiv, uint32_t APB1Div, uint32_t APB2Div, uint32_t APB3Div, uint32_t APB4Div, uint32_t flatency);
uint32_t BSP_STM32_RCC_GetSysClockFreq(void);
uint8_t BSP_STM32_RCC_WasSystemRestareted(void);

#endif /* STM32H7_BSP_STM32_RCC_H_ */
