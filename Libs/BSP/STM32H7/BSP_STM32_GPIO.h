/*******************************************************************
 * MiniConsole V3 - Board Support Package - STM32 GPIO
 *
 * Author: Marek Ryn
 * Version: 1.0
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 * - 1.0	- Initial version
 *******************************************************************/

#ifndef STM32H7_BSP_STM32_GPIO_H_
#define STM32H7_BSP_STM32_GPIO_H_

#include "BSP_Common.h"

#define	SET							1
#define RESET						0

#define GPIO_PIN_0                 ((uint16_t)0x0001)  // Pin 0 selected
#define GPIO_PIN_1                 ((uint16_t)0x0002)  // Pin 1 selected
#define GPIO_PIN_2                 ((uint16_t)0x0004)  // Pin 2 selected
#define GPIO_PIN_3                 ((uint16_t)0x0008)  // Pin 3 selected
#define GPIO_PIN_4                 ((uint16_t)0x0010)  // Pin 4 selected
#define GPIO_PIN_5                 ((uint16_t)0x0020)  // Pin 5 selected
#define GPIO_PIN_6                 ((uint16_t)0x0040)  // Pin 6 selected
#define GPIO_PIN_7                 ((uint16_t)0x0080)  // Pin 7 selected
#define GPIO_PIN_8                 ((uint16_t)0x0100)  // Pin 8 selected
#define GPIO_PIN_9                 ((uint16_t)0x0200)  // Pin 9 selected
#define GPIO_PIN_10                ((uint16_t)0x0400)  // Pin 10 selected
#define GPIO_PIN_11                ((uint16_t)0x0800)  // Pin 11 selected
#define GPIO_PIN_12                ((uint16_t)0x1000)  // Pin 12 selected
#define GPIO_PIN_13                ((uint16_t)0x2000)  // Pin 13 selected
#define GPIO_PIN_14                ((uint16_t)0x4000)  // Pin 14 selected
#define GPIO_PIN_15                ((uint16_t)0x8000)  // Pin 15 selected
#define GPIO_PIN_All               ((uint16_t)0xFFFF)  // All pins selected

#define GPIO_PIN_MASK              (0x0000FFFFU) // PIN mask for assert test

#define GPIO_MODE_Pos              0u
#define GPIO_MODE                  (0x3uL << GPIO_MODE_Pos)
#define MODE_INPUT                 (0x0uL << GPIO_MODE_Pos)
#define MODE_OUTPUT                (0x1uL << GPIO_MODE_Pos)
#define MODE_AF                    (0x2uL << GPIO_MODE_Pos)
#define MODE_ANALOG                (0x3uL << GPIO_MODE_Pos)
#define OUTPUT_TYPE_Pos            4u
#define OUTPUT_TYPE                (0x1uL << OUTPUT_TYPE_Pos)
#define OUTPUT_PP                  (0x0uL << OUTPUT_TYPE_Pos)
#define OUTPUT_OD                  (0x1uL << OUTPUT_TYPE_Pos)
#define EXTI_MODE_Pos              16u
#define EXTI_MODE                  (0x3uL << EXTI_MODE_Pos)
#define EXTI_IT                    (0x1uL << EXTI_MODE_Pos)
#define EXTI_EVT                   (0x2uL << EXTI_MODE_Pos)
#define TRIGGER_MODE_Pos           20u
#define TRIGGER_MODE               (0x7uL << TRIGGER_MODE_Pos)
#define TRIGGER_RISING             (0x1uL << TRIGGER_MODE_Pos)
#define TRIGGER_FALLING            (0x2uL << TRIGGER_MODE_Pos)
#define TRIGGER_LEVEL              (0x4uL << TRIGGER_MODE_Pos)

#define GPIO_PIN_RESET				0U
#define GPIO_PIN_SET				1U

#define GPIO_GET_INDEX(__GPIOx__)  (((__GPIOx__) == (GPIOA))? 0UL :\
                                    ((__GPIOx__) == (GPIOB))? 1UL :\
                                    ((__GPIOx__) == (GPIOC))? 2UL :\
                                    ((__GPIOx__) == (GPIOD))? 3UL :\
                                    ((__GPIOx__) == (GPIOE))? 4UL :\
                                    ((__GPIOx__) == (GPIOF))? 5UL :\
                                    ((__GPIOx__) == (GPIOG))? 6UL :\
                                    ((__GPIOx__) == (GPIOH))? 7UL :\
                                    ((__GPIOx__) == (GPIOI))? 8UL :\
                                    ((__GPIOx__) == (GPIOJ))? 9UL : 10UL)

#define GPIO_MODE_INPUT                 MODE_INPUT                                                  /*!< Input Floating Mode                                                */
#define GPIO_MODE_OUTPUT_PP             (MODE_OUTPUT | OUTPUT_PP)                                   /*!< Output Push Pull Mode                                              */
#define GPIO_MODE_OUTPUT_OD             (MODE_OUTPUT | OUTPUT_OD)                                   /*!< Output Open Drain Mode                                             */
#define GPIO_MODE_AF_PP                 (MODE_AF | OUTPUT_PP)                                       /*!< Alternate Function Push Pull Mode                                  */
#define GPIO_MODE_AF_OD                 (MODE_AF | OUTPUT_OD)                                       /*!< Alternate Function Open Drain Mode                                 */
#define GPIO_MODE_ANALOG                MODE_ANALOG                                                 /*!< Analog Mode                                                        */
#define GPIO_MODE_IT_RISING             (MODE_INPUT | EXTI_IT | TRIGGER_RISING)                     /*!< External Interrupt Mode with Rising edge trigger detection         */
#define GPIO_MODE_IT_FALLING            (MODE_INPUT | EXTI_IT | TRIGGER_FALLING)                    /*!< External Interrupt Mode with Falling edge trigger detection        */
#define GPIO_MODE_IT_RISING_FALLING     (MODE_INPUT | EXTI_IT | TRIGGER_RISING | TRIGGER_FALLING)   /*!< External Interrupt Mode with Rising/Falling edge trigger detection */

#define GPIO_MODE_EVT_RISING            (MODE_INPUT | EXTI_EVT | TRIGGER_RISING)                    /*!< External Event Mode with Rising edge trigger detection             */
#define GPIO_MODE_EVT_FALLING           (MODE_INPUT | EXTI_EVT | TRIGGER_FALLING)                   /*!< External Event Mode with Falling edge trigger detection            */
#define GPIO_MODE_EVT_RISING_FALLING    (MODE_INPUT | EXTI_EVT | TRIGGER_RISING | TRIGGER_FALLING)  /*!< External Event Mode with Rising/Falling edge trigger detection     */

#define  GPIO_SPEED_FREQ_LOW         (0x00000000U)  /*!< Low speed     */
#define  GPIO_SPEED_FREQ_MEDIUM      (0x00000001U)  /*!< Medium speed  */
#define  GPIO_SPEED_FREQ_HIGH        (0x00000002U)  /*!< Fast speed    */
#define  GPIO_SPEED_FREQ_VERY_HIGH   (0x00000003U)  /*!< High speed    */

#define  GPIO_NOPULL        (0x00000000U)   /*!< No Pull-up or Pull-down activation  */
#define  GPIO_PULLUP        (0x00000001U)   /*!< Pull-up activation                  */
#define  GPIO_PULLDOWN      (0x00000002U)   /*!< Pull-down activation                */

#define GPIO_AF0_RTC_50Hz      ((uint8_t)0x00)  /* RTC_50Hz Alternate Function mapping                                                     */
#define GPIO_AF0_MCO           ((uint8_t)0x00)  /* MCO (MCO1 and MCO2) Alternate Function mapping                                          */
#define GPIO_AF0_SWJ           ((uint8_t)0x00)  /* SWJ (SWD and JTAG) Alternate Function mapping                                           */
#define GPIO_AF0_LCDBIAS       ((uint8_t)0x00)  /* LCDBIAS Alternate Function mapping                                                      */
#define GPIO_AF0_TRACE         ((uint8_t)0x00)  /* TRACE Alternate Function mapping                                                        */
#define GPIO_AF1_TIM1          ((uint8_t)0x01)  /* TIM1 Alternate Function mapping   */
#define GPIO_AF1_TIM2          ((uint8_t)0x01)  /* TIM2 Alternate Function mapping   */
#define GPIO_AF1_TIM16         ((uint8_t)0x01)  /* TIM16 Alternate Function mapping  */
#define GPIO_AF1_TIM17         ((uint8_t)0x01)  /* TIM17 Alternate Function mapping  */
#define GPIO_AF1_LPTIM1        ((uint8_t)0x01)  /* LPTIM1 Alternate Function mapping */
#define GPIO_AF2_TIM3          ((uint8_t)0x02)  /* TIM3 Alternate Function mapping   */
#define GPIO_AF2_TIM4          ((uint8_t)0x02)  /* TIM4 Alternate Function mapping   */
#define GPIO_AF2_TIM5          ((uint8_t)0x02)  /* TIM5 Alternate Function mapping   */
#define GPIO_AF2_TIM12         ((uint8_t)0x02)  /* TIM12 Alternate Function mapping  */
#define GPIO_AF2_SAI1          ((uint8_t)0x02)  /* SAI1 Alternate Function mapping   */
#define GPIO_AF2_TIM15         ((uint8_t)0x02)  /* TIM15 Alternate Function mapping : available on STM32H7A3xxx/STM32H7B3xxx/STM32H7B0xxx and STM32H72xxx/STM32H73xxx */
#define GPIO_AF3_TIM8          ((uint8_t)0x03)  /* TIM8 Alternate Function mapping   */
#define GPIO_AF3_LPTIM2        ((uint8_t)0x03)  /* LPTIM2 Alternate Function mapping */
#define GPIO_AF3_DFSDM1        ((uint8_t)0x03)  /* DFSDM Alternate Function mapping  */
#define GPIO_AF3_LPTIM3        ((uint8_t)0x03)  /* LPTIM3 Alternate Function mapping */
#define GPIO_AF3_LPTIM4        ((uint8_t)0x03)  /* LPTIM4 Alternate Function mapping */
#define GPIO_AF3_LPTIM5        ((uint8_t)0x03)  /* LPTIM5 Alternate Function mapping */
#define GPIO_AF3_LPUART        ((uint8_t)0x03)  /* LPUART Alternate Function mapping */
#define GPIO_AF4_I2C1          ((uint8_t)0x04)  /* I2C1 Alternate Function mapping   */
#define GPIO_AF4_I2C2          ((uint8_t)0x04)  /* I2C2 Alternate Function mapping   */
#define GPIO_AF4_I2C3          ((uint8_t)0x04)  /* I2C3 Alternate Function mapping   */
#define GPIO_AF4_I2C4          ((uint8_t)0x04)  /* I2C4 Alternate Function mapping   */
#define GPIO_AF4_TIM15         ((uint8_t)0x04)  /* TIM15 Alternate Function mapping  */
#define GPIO_AF4_CEC           ((uint8_t)0x04)  /* CEC Alternate Function mapping    */
#define GPIO_AF4_LPTIM2        ((uint8_t)0x04)  /* LPTIM2 Alternate Function mapping */
#define GPIO_AF4_USART1        ((uint8_t)0x04)  /* USART1 Alternate Function mapping */
#define GPIO_AF4_DFSDM1        ((uint8_t)0x04)  /* DFSDM  Alternate Function mapping */
#define GPIO_AF4_DCMI          ((uint8_t)0x04)   /* DCMI Alternate Function mapping : available on STM32H7A3xxx/STM32H7B3xxx/STM32H7B0xxx and STM32H72xxx/STM32H73xxx */
#define GPIO_AF5_SPI1          ((uint8_t)0x05)  /* SPI1 Alternate Function mapping   */
#define GPIO_AF5_SPI2          ((uint8_t)0x05)  /* SPI2 Alternate Function mapping   */
#define GPIO_AF5_SPI3          ((uint8_t)0x05)  /* SPI3 Alternate Function mapping   */
#define GPIO_AF5_SPI4          ((uint8_t)0x05)  /* SPI4 Alternate Function mapping   */
#define GPIO_AF5_SPI5          ((uint8_t)0x05)  /* SPI5 Alternate Function mapping   */
#define GPIO_AF5_SPI6          ((uint8_t)0x05)  /* SPI6 Alternate Function mapping   */
#define GPIO_AF5_CEC           ((uint8_t)0x05)  /* CEC  Alternate Function mapping   */
#define GPIO_AF6_SPI2          ((uint8_t)0x06)  /* SPI2 Alternate Function mapping   */
#define GPIO_AF6_SPI3          ((uint8_t)0x06)  /* SPI3 Alternate Function mapping   */
#define GPIO_AF6_SAI1          ((uint8_t)0x06)  /* SAI1 Alternate Function mapping   */
#define GPIO_AF6_I2C4          ((uint8_t)0x06)  /* I2C4 Alternate Function mapping   */
#define GPIO_AF6_DFSDM1        ((uint8_t)0x06)  /* DFSDM Alternate Function mapping  */
#define GPIO_AF6_UART4         ((uint8_t)0x06)  /* UART4 Alternate Function mapping  */
#define GPIO_AF7_SPI2          ((uint8_t)0x07)  /* SPI2 Alternate Function mapping   */
#define GPIO_AF7_SPI3          ((uint8_t)0x07)  /* SPI3 Alternate Function mapping   */
#define GPIO_AF7_SPI6          ((uint8_t)0x07)  /* SPI6 Alternate Function mapping   */
#define GPIO_AF7_USART1        ((uint8_t)0x07)  /* USART1 Alternate Function mapping */
#define GPIO_AF7_USART2        ((uint8_t)0x07)  /* USART2 Alternate Function mapping */
#define GPIO_AF7_USART3        ((uint8_t)0x07)  /* USART3 Alternate Function mapping */
#define GPIO_AF7_USART6        ((uint8_t)0x07)  /* USART6 Alternate Function mapping */
#define GPIO_AF7_UART7         ((uint8_t)0x07)  /* UART7 Alternate Function mapping  */
#define GPIO_AF7_SDMMC1        ((uint8_t)0x07)  /* SDMMC1 Alternate Function mapping */
#define GPIO_AF8_SPI6          ((uint8_t)0x08)  /* SPI6 Alternate Function mapping   */
#define GPIO_AF8_UART4         ((uint8_t)0x08)  /* UART4 Alternate Function mapping  */
#define GPIO_AF8_UART5         ((uint8_t)0x08)  /* UART5 Alternate Function mapping  */
#define GPIO_AF8_UART8         ((uint8_t)0x08)  /* UART8 Alternate Function mapping  */
#define GPIO_AF8_SPDIF         ((uint8_t)0x08)  /* SPDIF Alternate Function mapping  */
#define GPIO_AF8_LPUART        ((uint8_t)0x08)  /* LPUART Alternate Function mapping */
#define GPIO_AF8_SDMMC1        ((uint8_t)0x08)  /* SDMMC1 Alternate Function mapping */
#define GPIO_AF9_FDCAN1        ((uint8_t)0x09)  /* FDCAN1 Alternate Function mapping   */
#define GPIO_AF9_FDCAN2        ((uint8_t)0x09)  /* FDCAN2 Alternate Function mapping   */
#define GPIO_AF9_TIM13         ((uint8_t)0x09)  /* TIM13 Alternate Function mapping    */
#define GPIO_AF9_TIM14         ((uint8_t)0x09)  /* TIM14 Alternate Function mapping    */
#define GPIO_AF9_SDMMC2        ((uint8_t)0x09)  /* SDMMC2 Alternate Function mapping   */
#define GPIO_AF9_LTDC          ((uint8_t)0x09)  /* LTDC Alternate Function mapping     */
#define GPIO_AF9_SPDIF         ((uint8_t)0x09)  /* SPDIF Alternate Function mapping    */
#define GPIO_AF9_FMC           ((uint8_t)0x09)  /* FMC Alternate Function mapping      */
#define GPIO_AF9_QUADSPI       ((uint8_t)0x09)  /* QUADSPI Alternate Function mapping  */
#define GPIO_AF10_SDMMC2        ((uint8_t)0x0A)  /* SDMMC2 Alternate Function mapping                                           */
#define GPIO_AF10_COMP1         ((uint8_t)0x0A)  /* COMP1 Alternate Function mapping                                            */
#define GPIO_AF10_COMP2         ((uint8_t)0x0A)  /* COMP2 Alternate Function mapping                                            */
#define GPIO_AF10_CRS_SYNC      ((uint8_t)0x0A)  /* CRS Sync Alternate Function mapping : available on STM32H7 Rev.B and above  */
#define GPIO_AF10_OTG1_FS       ((uint8_t)0x0A)  /* OTG1_FS Alternate Function mapping : available on STM32H7A3xxx/STM32H7B3xxx/STM32H7B0xxx and STM32H72xxx/STM32H73xxx */
#define GPIO_AF10_OTG1_HS       ((uint8_t)0x0A)  /* OTG1_HS Alternate Function mapping                                          */
#define GPIO_AF10_TIM8          ((uint8_t)0x0A)  /* TIM8 Alternate Function mapping                                             */
#define GPIO_AF10_FMC           ((uint8_t)0x0A)  /* FMC Alternate Function mapping : available on STM32H7A3xxx/STM32H7B3xxx/STM32H7B0xxx and STM32H72xxx/STM32H73xxx */
#define GPIO_AF10_LTDC          ((uint8_t)0x0A)  /* LTDC Alternate Function mapping                                             */
#define GPIO_AF10_QUADSPI       ((uint8_t)0x0A)  /* QUADSPI Alternate Function mapping                                          */
#define GPIO_AF11_SWP           ((uint8_t)0x0B)  /* SWP Alternate Function mapping     */
#define GPIO_AF11_MDIOS         ((uint8_t)0x0B)  /* MDIOS Alternate Function mapping   */
#define GPIO_AF11_UART7         ((uint8_t)0x0B)  /* UART7 Alternate Function mapping   */
#define GPIO_AF11_SDMMC2        ((uint8_t)0x0B)  /* SDMMC2 Alternate Function mapping  */
#define GPIO_AF11_DFSDM1        ((uint8_t)0x0B)  /* DFSDM1 Alternate Function mapping  */
#define GPIO_AF11_COMP1         ((uint8_t)0x0B)  /* COMP1 Alternate Function mapping   */
#define GPIO_AF11_COMP2         ((uint8_t)0x0B)  /* COMP2 Alternate Function mapping   */
#define GPIO_AF11_TIM1          ((uint8_t)0x0B)  /* TIM1 Alternate Function mapping    */
#define GPIO_AF11_TIM8          ((uint8_t)0x0B)  /* TIM8 Alternate Function mapping    */
#define GPIO_AF11_I2C4          ((uint8_t)0x0B)  /* I2C4 Alternate Function mapping    */
#define GPIO_AF12_FMC           ((uint8_t)0x0C)  /* FMC Alternate Function mapping     */
#define GPIO_AF12_SDMMC1        ((uint8_t)0x0C)  /* SDMMC1 Alternate Function mapping  */
#define GPIO_AF12_MDIOS         ((uint8_t)0x0C)  /* MDIOS Alternate Function mapping   */
#define GPIO_AF12_COMP1         ((uint8_t)0x0C)  /* COMP1 Alternate Function mapping   */
#define GPIO_AF12_COMP2         ((uint8_t)0x0C)  /* COMP2 Alternate Function mapping   */
#define GPIO_AF12_TIM1          ((uint8_t)0x0C)  /* TIM1 Alternate Function mapping    */
#define GPIO_AF12_TIM8          ((uint8_t)0x0C)  /* TIM8 Alternate Function mapping    */
#define GPIO_AF12_LTDC          ((uint8_t)0x0C)  /* LTDC Alternate Function mapping    */
#define GPIO_AF13_DCMI          ((uint8_t)0x0D)   /* DCMI Alternate Function mapping  */
#define GPIO_AF13_COMP1         ((uint8_t)0x0D)   /* COMP1 Alternate Function mapping */
#define GPIO_AF13_COMP2         ((uint8_t)0x0D)   /* COMP2 Alternate Function mapping */
#define GPIO_AF13_TIM1          ((uint8_t)0x0D)    /* TIM1 Alternate Function mapping */
#define GPIO_AF13_LTDC          ((uint8_t)0x0D)   /* LTDC Alternate Function mapping  */
#define GPIO_AF14_LTDC         ((uint8_t)0x0E)   /* LTDC Alternate Function mapping  */
#define GPIO_AF14_UART5        ((uint8_t)0x0E)   /* UART5 Alternate Function mapping */
#define GPIO_AF15_EVENTOUT      ((uint8_t)0x0F)  /* EVENTOUT Alternate Function mapping */

#define GPIO_AF7_SDIO1                            GPIO_AF7_SDMMC1
#define GPIO_AF8_SDIO1                            GPIO_AF8_SDMMC1
#define GPIO_AF12_SDIO1                           GPIO_AF12_SDMMC1
#define GPIO_AF9_SDIO2                            GPIO_AF9_SDMMC2
#define GPIO_AF10_SDIO2                           GPIO_AF10_SDMMC2
#define GPIO_AF11_SDIO2                           GPIO_AF11_SDMMC2


uint8_t BSP_STM32_GPIO_Init(GPIO_TypeDef  *GPIOx, uint32_t Pins, uint32_t Mode, uint32_t Pull, uint32_t Speed, uint32_t Alternate);
uint32_t BSP_STM32_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin);
uint8_t BSP_STM32_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin, uint32_t PinState);
uint8_t BSP_STM32_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin);

#endif /* STM32H7_BSP_STM32_GPIO_H_ */
