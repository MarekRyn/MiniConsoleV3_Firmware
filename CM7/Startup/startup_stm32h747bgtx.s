/**
  ******************************************************************************
  * @file      startup_stm32h747xx.s
  * @author    MCD Application Team
  * @brief     STM32H747xx Devices vector table for GCC based toolchain.
  *            This module performs:
  *                - Set the initial SP
  *                - Set the initial PC == Reset_Handler,
  *                - Set the vector table entries with the exceptions ISR address
  *                - Branches to main in the C library (which eventually
  *                  calls main()).
  *            After Reset the Cortex-M processor is in Thread mode,
  *            priority is Privileged, and the Stack is set to Main.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

  .syntax unified
  .cpu cortex-m7
  .fpu softvfp
  .thumb

.global  g_pfnVectors
.global  Default_Handler

/* start address for the initialization values of the .data section.
defined in linker script */
.word  _sidata
/* start address for the .data section. defined in linker script */
.word  _sdata
/* end address for the .data section. defined in linker script */
.word  _edata
/* start address for the .bss section. defined in linker script */
.word  _sbss
/* end address for the .bss section. defined in linker script */
.word  _ebss
/* stack used for SystemInit_ExtMemCtl; always internal RAM used */

/**
 * @brief  This is the code that gets called when the processor first
 *          starts execution following a reset event. Only the absolutely
 *          necessary set is performed, after which the application
 *          supplied main() routine is called.
 * @param  None
 * @retval : None
*/

    .section  .text.Reset_Handler
  .weak  Reset_Handler
  .type  Reset_Handler, %function
Reset_Handler:
  ldr   sp, =_estack      /* set stack pointer */

/* Call the clock system initialization function.*/
  bl  SystemInit


/* Copy selected program routines from flash to ITCMRAM */
  ldr r0, =_sitcmram
  ldr r1, =_eitcmram
  ldr r2, =_siitcmram
  movs r3, #0
  b LoopCopyITCInit

CopyITCInit:
  ldr r4, [r2, r3]
  str r4, [r0, r3]
  adds r3, r3, #4

LoopCopyITCInit:
  adds r4, r0, r3
  cmp r4, r1
  bcc CopyITCInit


/* Copy the data segment initializers from flash to SRAM */
  ldr r0, =_sdata
  ldr r1, =_edata
  ldr r2, =_sidata
  movs r3, #0
  b LoopCopyDataInit

CopyDataInit:
  ldr r4, [r2, r3]
  str r4, [r0, r3]
  adds r3, r3, #4

LoopCopyDataInit:
  adds r4, r0, r3
  cmp r4, r1
  bcc CopyDataInit


/* Copy selected data from flash to DTCMRAM */
  ldr r0, =_sdtcmram
  ldr r1, =_edtcmram
  ldr r2, =_sidtcmram
  movs r3, #0
  b LoopCopyDTCInit

CopyDTCInit:
  ldr r4, [r2, r3]
  str r4, [r0, r3]
  adds r3, r3, #4

LoopCopyDTCInit:
  adds r4, r0, r3
  cmp r4, r1
  bcc CopyDTCInit


/* Zero fill the bss segment. */
  ldr r2, =_sbss
  ldr r4, =_ebss
  movs r3, #0
  b LoopFillZerobss

FillZerobss:
  str  r3, [r2]
  adds r2, r2, #4

LoopFillZerobss:
  cmp r2, r4
  bcc FillZerobss


/* Call static constructors */
    bl __libc_init_array
/* Call the application's entry point.*/
  bl  main
  bx  lr
.size  Reset_Handler, .-Reset_Handler

/**
 * @brief  This is the code that gets called when the processor receives an
 *         unexpected interrupt.  This simply enters an infinite loop, preserving
 *         the system state for examination by a debugger.
 * @param  None
 * @retval None
*/
    .section  .text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
  b  Infinite_Loop
  .size  Default_Handler, .-Default_Handler
/******************************************************************************
*
* The minimal vector table for a Cortex M. Note that the proper constructs
* must be placed on this to ensure that it ends up at physical address
* 0x0000.0000.
*
*******************************************************************************/
   .section  .isr_vector,"a",%progbits
  .type  g_pfnVectors, %object
  .size  g_pfnVectors, .-g_pfnVectors


g_pfnVectors:
  .word  _estack
  .word  Reset_Handler

  .word  NMI_Handler
  .word  HardFault_Handler
  .word  MemManage_Handler
  .word  BusFault_Handler
  .word  UsageFault_Handler
  .word  0
  .word  0
  .word  0
  .word  0
  .word  SVC_Handler
  .word  DebugMon_Handler
  .word  0
  .word  PendSV_Handler
  .word  SysTick_Handler

  /* External Interrupts */
  .word     WWDG_IRQHandler                   /* Window WatchDog Interrupt ( wwdg1_it, wwdg2_it) */
  .word     PVD_AVD_IRQHandler                /* PVD/AVD through EXTI Line detection */
  .word     TAMP_STAMP_IRQHandler             /* Tamper and TimeStamps through the EXTI line */
  .word     RTC_WKUP_IRQHandler               /* RTC Wakeup through the EXTI line */
  .word     FLASH_IRQHandler                  /* FLASH                        */
  .word     RCC_IRQHandler                    /* RCC                          */
  .word     EXTI0_IRQHandler                  /* EXTI Line0                   */
  .word     EXTI1_IRQHandler                  /* EXTI Line1                   */
  .word     EXTI2_IRQHandler                  /* EXTI Line2                   */
  .word     EXTI3_IRQHandler                  /* EXTI Line3                   */
  .word     EXTI4_IRQHandler                  /* EXTI Line4                   */
  .word     DMA1_Stream0_IRQHandler           /* DMA1 Stream 0                */
  .word     DMA1_Stream1_IRQHandler           /* DMA1 Stream 1                */
  .word     DMA1_Stream2_IRQHandler           /* DMA1 Stream 2                */
  .word     DMA1_Stream3_IRQHandler           /* DMA1 Stream 3                */
  .word     DMA1_Stream4_IRQHandler           /* DMA1 Stream 4                */
  .word     DMA1_Stream5_IRQHandler           /* DMA1 Stream 5                */
  .word     DMA1_Stream6_IRQHandler           /* DMA1 Stream 6                */
  .word     ADC_IRQHandler                    /* ADC1, ADC2 and ADC3s         */
  .word     FDCAN1_IT0_IRQHandler             /* FDCAN1 interrupt line 0      */
  .word     FDCAN2_IT0_IRQHandler             /* FDCAN2 interrupt line 0      */
  .word     FDCAN1_IT1_IRQHandler             /* FDCAN1 interrupt line 1      */
  .word     FDCAN2_IT1_IRQHandler             /* FDCAN2 interrupt line 1      */
  .word     EXTI9_5_IRQHandler                /* External Line[9:5]s          */
  .word     TIM1_BRK_IRQHandler               /* TIM1 Break interrupt         */
  .word     TIM1_UP_IRQHandler                /* TIM1 Update interrupt        */
  .word     TIM1_TRG_COM_IRQHandler           /* TIM1 Trigger and Commutation interrupt */
  .word     TIM1_CC_IRQHandler                /* TIM1 Capture Compare         */
  .word     TIM2_IRQHandler                   /* TIM2                         */
  .word     TIM3_IRQHandler                   /* TIM3                         */
  .word     TIM4_IRQHandler                   /* TIM4                         */
  .word     I2C1_EV_IRQHandler                /* I2C1 Event                   */
  .word     I2C1_ER_IRQHandler                /* I2C1 Error                   */
  .word     I2C2_EV_IRQHandler                /* I2C2 Event                   */
  .word     I2C2_ER_IRQHandler                /* I2C2 Error                   */
  .word     SPI1_IRQHandler                   /* SPI1                         */
  .word     SPI2_IRQHandler                   /* SPI2                         */
  .word     USART1_IRQHandler                 /* USART1                       */
  .word     USART2_IRQHandler                 /* USART2                       */
  .word     USART3_IRQHandler                 /* USART3                       */
  .word     EXTI15_10_IRQHandler              /* External Line[15:10]s        */
  .word     RTC_Alarm_IRQHandler              /* RTC Alarm (A and B) through EXTI Line */
  .word     0                                 /* Reserved                     */
  .word     TIM8_BRK_TIM12_IRQHandler         /* TIM8 Break and TIM12         */
  .word     TIM8_UP_TIM13_IRQHandler          /* TIM8 Update and TIM13        */
  .word     TIM8_TRG_COM_TIM14_IRQHandler     /* TIM8 Trigger and Commutation and TIM14 */
  .word     TIM8_CC_IRQHandler                /* TIM8 Capture Compare         */
  .word     DMA1_Stream7_IRQHandler           /* DMA1 Stream7                 */
  .word     FMC_IRQHandler                    /* FMC                          */
  .word     SDMMC1_IRQHandler                 /* SDMMC1                       */
  .word     TIM5_IRQHandler                   /* TIM5                         */
  .word     SPI3_IRQHandler                   /* SPI3                         */
  .word     UART4_IRQHandler                  /* UART4                        */
  .word     UART5_IRQHandler                  /* UART5                        */
  .word     TIM6_DAC_IRQHandler               /* TIM6 and DAC1&2 underrun errors */
  .word     TIM7_IRQHandler                   /* TIM7                         */
  .word     DMA2_Stream0_IRQHandler           /* DMA2 Stream 0                */
  .word     DMA2_Stream1_IRQHandler           /* DMA2 Stream 1                */
  .word     DMA2_Stream2_IRQHandler           /* DMA2 Stream 2                */
  .word     DMA2_Stream3_IRQHandler           /* DMA2 Stream 3                */
  .word     DMA2_Stream4_IRQHandler           /* DMA2 Stream 4                */
  .word     ETH_IRQHandler                    /* Ethernet                     */
  .word     ETH_WKUP_IRQHandler               /* Ethernet Wakeup through EXTI line */
  .word     FDCAN_CAL_IRQHandler              /* FDCAN calibration unit interrupt  */
  .word     CM7_SEV_IRQHandler                /* CM7 Send event interrupt for CM4  */
  .word     CM4_SEV_IRQHandler                /* CM4 Send event interrupt for CM7  */
  .word     0                                 /* Reserved                     */
  .word     0                                 /* Reserved                     */
  .word     DMA2_Stream5_IRQHandler           /* DMA2 Stream 5                */
  .word     DMA2_Stream6_IRQHandler           /* DMA2 Stream 6                */
  .word     DMA2_Stream7_IRQHandler           /* DMA2 Stream 7                */
  .word     USART6_IRQHandler                 /* USART6                       */
  .word     I2C3_EV_IRQHandler                /* I2C3 event                   */
  .word     I2C3_ER_IRQHandler                /* I2C3 error                   */
  .word     OTG_HS_EP1_OUT_IRQHandler         /* USB OTG HS End Point 1 Out   */
  .word     OTG_HS_EP1_IN_IRQHandler          /* USB OTG HS End Point 1 In    */
  .word     OTG_HS_WKUP_IRQHandler            /* USB OTG HS Wakeup through EXTI */
  .word     OTG_HS_IRQHandler                 /* USB OTG HS                   */
  .word     DCMI_IRQHandler                   /* DCMI                         */
  .word     0                                 /* Reserved                     */
  .word     RNG_IRQHandler                    /* Rng                          */
  .word     FPU_IRQHandler                    /* FPU                          */
  .word     UART7_IRQHandler                  /* UART7                        */
  .word     UART8_IRQHandler                  /* UART8                        */
  .word     SPI4_IRQHandler                   /* SPI4                         */
  .word     SPI5_IRQHandler                   /* SPI5                         */
  .word     SPI6_IRQHandler                   /* SPI6                         */
  .word     SAI1_IRQHandler                   /* SAI1                         */
  .word     LTDC_IRQHandler                   /* LTDC                         */
  .word     LTDC_ER_IRQHandler                /* LTDC error                   */
  .word     DMA2D_IRQHandler                  /* DMA2D                        */
  .word     SAI2_IRQHandler                   /* SAI2                         */
  .word     QUADSPI_IRQHandler                /* QUADSPI                      */
  .word     LPTIM1_IRQHandler                 /* LPTIM1                       */
  .word     CEC_IRQHandler                    /* HDMI_CEC                     */
  .word     I2C4_EV_IRQHandler                /* I2C4 Event                   */
  .word     I2C4_ER_IRQHandler                /* I2C4 Error                   */
  .word     SPDIF_RX_IRQHandler               /* SPDIF_RX                     */
  .word     OTG_FS_EP1_OUT_IRQHandler         /* USB OTG FS End Point 1 Out   */
  .word     OTG_FS_EP1_IN_IRQHandler          /* USB OTG FS End Point 1 In    */
  .word     OTG_FS_WKUP_IRQHandler            /* USB OTG FS Wakeup through EXTI */
  .word     OTG_FS_IRQHandler                 /* USB OTG FS                   */
  .word     DMAMUX1_OVR_IRQHandler            /* DMAMUX1 Overrun interrupt    */
  .word     HRTIM1_Master_IRQHandler          /* HRTIM Master Timer global Interrupt */
  .word     HRTIM1_TIMA_IRQHandler            /* HRTIM Timer A global Interrupt */
  .word     HRTIM1_TIMB_IRQHandler            /* HRTIM Timer B global Interrupt */
  .word     HRTIM1_TIMC_IRQHandler            /* HRTIM Timer C global Interrupt */
  .word     HRTIM1_TIMD_IRQHandler            /* HRTIM Timer D global Interrupt */
  .word     HRTIM1_TIME_IRQHandler            /* HRTIM Timer E global Interrupt */
  .word     HRTIM1_FLT_IRQHandler             /* HRTIM Fault global Interrupt   */
  .word     DFSDM1_FLT0_IRQHandler            /* DFSDM Filter0 Interrupt        */
  .word     DFSDM1_FLT1_IRQHandler            /* DFSDM Filter1 Interrupt        */
  .word     DFSDM1_FLT2_IRQHandler            /* DFSDM Filter2 Interrupt        */
  .word     DFSDM1_FLT3_IRQHandler            /* DFSDM Filter3 Interrupt        */
  .word     SAI3_IRQHandler                   /* SAI3 global Interrupt          */
  .word     SWPMI1_IRQHandler                 /* Serial Wire Interface 1 global interrupt */
  .word     TIM15_IRQHandler                  /* TIM15 global Interrupt      */
  .word     TIM16_IRQHandler                  /* TIM16 global Interrupt      */
  .word     TIM17_IRQHandler                  /* TIM17 global Interrupt      */
  .word     MDIOS_WKUP_IRQHandler             /* MDIOS Wakeup  Interrupt     */
  .word     MDIOS_IRQHandler                  /* MDIOS global Interrupt      */
  .word     JPEG_IRQHandler                   /* JPEG global Interrupt       */
  .word     MDMA_IRQHandler                   /* MDMA global Interrupt       */
  .word     DSI_IRQHandler                    /* DSI global Interrupt        */
  .word     SDMMC2_IRQHandler                 /* SDMMC2 global Interrupt     */
  .word     HSEM1_IRQHandler                  /* HSEM1 global Interrupt      */
  .word     HSEM2_IRQHandler                  /* HSEM1 global Interrupt      */
  .word     ADC3_IRQHandler                   /* ADC3 global Interrupt       */
  .word     DMAMUX2_OVR_IRQHandler            /* DMAMUX Overrun interrupt    */
  .word     BDMA_Channel0_IRQHandler          /* BDMA Channel 0 global Interrupt */
  .word     BDMA_Channel1_IRQHandler          /* BDMA Channel 1 global Interrupt */
  .word     BDMA_Channel2_IRQHandler          /* BDMA Channel 2 global Interrupt */
  .word     BDMA_Channel3_IRQHandler          /* BDMA Channel 3 global Interrupt */
  .word     BDMA_Channel4_IRQHandler          /* BDMA Channel 4 global Interrupt */
  .word     BDMA_Channel5_IRQHandler          /* BDMA Channel 5 global Interrupt */
  .word     BDMA_Channel6_IRQHandler          /* BDMA Channel 6 global Interrupt */
  .word     BDMA_Channel7_IRQHandler          /* BDMA Channel 7 global Interrupt */
  .word     COMP1_IRQHandler                  /* COMP1 global Interrupt     */
  .word     LPTIM2_IRQHandler                 /* LP TIM2 global interrupt   */
  .word     LPTIM3_IRQHandler                 /* LP TIM3 global interrupt   */
  .word     LPTIM4_IRQHandler                 /* LP TIM4 global interrupt   */
  .word     LPTIM5_IRQHandler                 /* LP TIM5 global interrupt   */
  .word     LPUART1_IRQHandler                /* LP UART1 interrupt         */
  .word     WWDG_RST_IRQHandler               /* Window Watchdog reset interrupt (exti_d2_wwdg_it, exti_d1_wwdg_it) */
  .word     CRS_IRQHandler                    /* Clock Recovery Global Interrupt */
  .word     ECC_IRQHandler                    /* ECC diagnostic Global Interrupt */
  .word     SAI4_IRQHandler                   /* SAI4 global interrupt      */
  .word     0                                 /* Reserved                   */
  .word     HOLD_CORE_IRQHandler              /* Hold core interrupt        */
  .word     WAKEUP_PIN_IRQHandler             /* Interrupt for all 6 wake-up pins */

/* Fill until 256 words (for future use) */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */
  .word     0                                 /* Reserved                   */


/*******************************************************************************
* MiniConsole V3 - Vector table for applications (starts at 0x08000400)
*******************************************************************************/

// Placeholder for future use
 .word		0								/* 0000 */
 .word		0								/* 0001 */
 .word		0								/* 0002 */
 .word		0								/* 0003 */
 .word		0								/* 0004 */
 .word		0								/* 0005 */
 .word		0								/* 0006 */
 .word		0								/* 0007 */

// BSP Structures
 .word		BSP_hlcdtp						/* 0008 */
 .word		BSP_himu						/* 0009 */
 .word		BSP_hinputs						/* 0010 */
 .word		0								/* 0011 */
 .word		0								/* 0012 */
 .word		0								/* 0013 */
 .word		0								/* 0014 */
 .word		0								/* 0015 */

// System functions
 .word		BSP_PWR_Restart					/* 0016 */
 .word		BSP_PWR_ShutDown				/* 0017 */
 .word		BSP_Delay						/* 0018 */
 .word		BSP_GetTick						/* 0019 */
 .word		BSP_SetHomeDir					/* 0020 */
 .word		BSP_RTC_GetDate					/* 0021 */
 .word		BSP_RTC_GetTime					/* 0022 */
 .word		BSP_RTC_GetUnixTimestamp		/* 0023 */
 .word		0								/* 0024 */
 .word		0								/* 0025 */
 .word		0								/* 0026 */
 .word		0								/* 0027 */
 .word		0								/* 0028 */
 .word		0								/* 0029 */
 .word		0								/* 0030 */
 .word		BSP_Serial_Transmit				/* 0031 */

// LCD and Touch Panel Library
 .word		BSP_LCD_Init					/* 0032 */
 .word		BSP_LCD_FrameReady				/* 0033 */
 .word		BSP_LCD_GetEditPermission		/* 0034 */
 .word		BSP_LCD_SetBackLight			/* 0035 */
 .word		BSP_LCD_GetBackLight			/* 0036 */
 .word		BSP_LCD_BacklLightOff			/* 0037 */
 .word		BSP_LCD_BackLightOn				/* 0038 */
 .word		BSP_LCD_GetFrameTime			/* 0039 */
 .word		BSP_LCD_UpdateCLUT				/* 0040 */
 .word		BSP_LCD_GetEditFrameAddr		/* 0041 */
 .word		BSP_LCD_SetDisplayWindow		/* 0042 */
 .word		0								/* 0043 */
 .word		0								/* 0044 */
 .word		0								/* 0045 */
 .word		0								/* 0046 */
 .word		0								/* 0047 */
 .word		0								/* 0048 */
 .word		0								/* 0049 */
 .word		0								/* 0050 */
 .word		0								/* 0051 */
 .word		0								/* 0052 */
 .word		0								/* 0053 */
 .word		0								/* 0054 */
 .word		0								/* 0055 */
 .word		0								/* 0056 */
 .word		0								/* 0057 */
 .word		0								/* 0058 */
 .word		0								/* 0059 */
 .word		0								/* 0060 */
 .word		0								/* 0061 */
 .word		0								/* 0062 */
 .word		0								/* 0063 */
 .word		BSP_LCD_TP_RegisterArea			/* 0064 */
 .word		BSP_LCD_TP_RemoveArea			/* 0065 */
 .word		BSP_LCD_TP_RemoveAreaRange		/* 0066 */
 .word		BSP_LCD_TP_RemoveAllAreas		/* 0067 */
 .word		BSP_LCD_TP_Enable				/* 0068 */
 .word		BSP_LCD_TP_Disable				/* 0069 */
 .word		0								/* 0070 */
 .word		0								/* 0071 */
 .word		0								/* 0072 */
 .word		0								/* 0073 */
 .word		0								/* 0074 */
 .word		0								/* 0075 */
 .word		0								/* 0076 */
 .word		0								/* 0077 */
 .word		0								/* 0078 */
 .word		0								/* 0079 */
 .word		0								/* 0080 */
 .word		0								/* 0081 */
 .word		0								/* 0082 */
 .word		0								/* 0083 */
 .word		0								/* 0084 */
 .word		0								/* 0085 */
 .word		0								/* 0086 */
 .word		0								/* 0087 */
 .word		0								/* 0088 */
 .word		0								/* 0089 */
 .word		0								/* 0090 */
 .word		0								/* 0091 */
 .word		0								/* 0092 */
 .word		0								/* 0093 */
 .word		0								/* 0094 */
 .word		0								/* 0095 */

 // Video Library
 .word		BSP_Video_Init					/* 0096 */
 .word		BSP_Video_GetFrame				/* 0097 */
 .word		BSP_Video_DeInit				/* 0098 */
 .word		BSP_Video_Seek					/* 0099 */
 .word		BSP_Video_Rev					/* 0100 */
 .word		BSP_Video_Fwd					/* 0101 */
 .word		BSP_Video_Play					/* 0102 */
 .word		BSP_Video_Stop					/* 0103 */
 .word		BSP_Video_Pause					/* 0104 */
 .word		BSP_Video_SetVolume				/* 0105 */
 .word		BSP_Video_DrawFrame				/* 0106 */
 .word		BSP_Video_DrawFrameC			/* 0107 */
 .word		BSP_Video_GetTotalFrames		/* 0108 */
 .word		BSP_Video_GetCurrentFrame		/* 0109 */
 .word		BSP_Video_GetWidth				/* 0110 */
 .word		BSP_Video_GetHeight				/* 0111 */
 .word		BSP_Video_GetFrameRate			/* 0112 */
 .word		0								/* 0113 */
 .word		0								/* 0114 */
 .word		0								/* 0115 */
 .word		0								/* 0116 */
 .word		0								/* 0117 */
 .word		0								/* 0118 */
 .word		0								/* 0119 */
 .word		0								/* 0120 */
 .word		0								/* 0121 */
 .word		0								/* 0122 */
 .word		0								/* 0123 */
 .word		0								/* 0124 */
 .word		0								/* 0125 */
 .word		0								/* 0126 */
 .word		0								/* 0127 */

// G2D Library
 .word		G2D_ClearFrame					/* 0128 */
 .word		G2D_FillFrame					/* 0129 */
 .word		G2D_CopyPrevFrame				/* 0130 */
 .word		G2D_CopyScrollPrevFrame			/* 0131 */
 .word		G2D_DrawPixel					/* 0132 */
 .word		G2D_DrawHLine					/* 0133 */
 .word		G2D_DrawHLineBlend				/* 0134 */
 .word		G2D_DrawVLine					/* 0135 */
 .word		G2D_DrawVLineBlend				/* 0136 */
 .word		G2D_DrawLine					/* 0137 */
 .word		G2D_DrawRect					/* 0138 */
 .word		G2D_DrawFillRect				/* 0139 */
 .word		G2D_DrawFillRectBlend			/* 0140 */
 .word		G2D_DrawCircle					/* 0141 */
 .word		G2D_DrawFillCircle				/* 0142 */
 .word		G2D_DrawFillCircleBlend			/* 0143 */
 .word		G2D_DrawRoundRect				/* 0144 */
 .word		G2D_DrawFillRoundRect			/* 0145 */
 .word		G2D_DrawFillRoundRectBlend		/* 0146 */
 .word		G2D_Text						/* 0147 */
 .word		G2D_TextBlend					/* 0148 */
 .word		G2D_GetTextHeight				/* 0149 */
 .word		G2D_DrawBitmapBlend				/* 0150 */
 .word		G2D_DrawBitmapBlendC			/* 0151 */
 .word		G2D_DrawBitmap					/* 0152 */
 .word		G2D_DrawBitmapC					/* 0153 */
 .word		G2D_DrawBitmapRotate			/* 0154 */
 .word		G2D_DrawBitmapRotateC			/* 0155 */
 .word		G2D_DrawIcon					/* 0156 */
 .word		G2D_DrawIconC					/* 0157 */
 .word		G2D_DrawIconBlend				/* 0158 */
 .word		G2D_DrawIconBlendC				/* 0159 */
 .word		G2D_GetIconHeight				/* 0160 */
 .word		G2D_GetIconWidth				/* 0161 */
 .word		G2D_DrawJPEG					/* 0162 */
 .word		G2D_DrawJPEGC					/* 0163 */
 .word		G2D_DrawLastJPEG				/* 0164 */
 .word		G2D_DrawLastJPEGC				/* 0165 */
 .word		G2D_DecodeJPEG					/* 0166 */
 .word		G2D_DrawTile					/* 0167 */
 .word		G2D_DrawTileC					/* 0168 */
 .word		G2D_DrawTileBlend				/* 0169 */
 .word		G2D_DrawTileBlendC				/* 0170 */
 .word		G2D_Color						/* 0171 */
 .word		G2D_Alpha						/* 0172 */
 .word		G2D_CopyBuf						/* 0173 */
 .word		G2D_CopyBufBlend				/* 0174 */
 .word		G2D_CacheFrame					/* 0175 */
 .word		G2D_RestoreFrame				/* 0176 */
 .word		G2D_DrawTileBlend				/* 0177 */
 .word		0								/* 0178 */
 .word		0								/* 0179 */
 .word		0								/* 0180 */
 .word		0								/* 0181 */
 .word		0								/* 0182 */
 .word		0								/* 0183 */
 .word		0								/* 0184 */
 .word		0								/* 0185 */
 .word		0								/* 0186 */
 .word		0								/* 0187 */
 .word		0								/* 0188 */
 .word		0								/* 0189 */
 .word		0								/* 0190 */
 .word		0								/* 0191 */
 .word		0								/* 0192 */
 .word		0								/* 0193 */
 .word		0								/* 0194 */
 .word		0								/* 0195 */
 .word		0								/* 0196 */
 .word		0								/* 0197 */
 .word		0								/* 0198 */
 .word		0								/* 0199 */
 .word		0								/* 0200 */
 .word		0								/* 0201 */
 .word		0								/* 0202 */
 .word		0								/* 0203 */
 .word		0								/* 0204 */
 .word		0								/* 0205 */
 .word		0								/* 0206 */
 .word		0								/* 0207 */
 .word		0								/* 0208 */
 .word		0								/* 0209 */
 .word		0								/* 0210 */
 .word		0								/* 0211 */
 .word		0								/* 0212 */
 .word		0								/* 0213 */
 .word		0								/* 0214 */
 .word		0								/* 0215 */
 .word		0								/* 0216 */
 .word		0								/* 0217 */
 .word		0								/* 0218 */
 .word		0								/* 0219 */
 .word		0								/* 0220 */
 .word		0								/* 0221 */
 .word		0								/* 0222 */
 .word		0								/* 0223 */
 .word		0								/* 0224 */
 .word		0								/* 0225 */
 .word		0								/* 0226 */
 .word		0								/* 0227 */
 .word		0								/* 0228 */
 .word		0								/* 0229 */
 .word		0								/* 0230 */
 .word		0								/* 0231 */
 .word		0								/* 0232 */
 .word		0								/* 0233 */
 .word		0								/* 0234 */
 .word		0								/* 0235 */
 .word		0								/* 0236 */
 .word		0								/* 0237 */
 .word		0								/* 0238 */
 .word		0								/* 0239 */
 .word		0								/* 0240 */
 .word		0								/* 0241 */
 .word		0								/* 0242 */
 .word		0								/* 0243 */
 .word		0								/* 0244 */
 .word		0								/* 0245 */
 .word		0								/* 0246 */
 .word		0								/* 0247 */
 .word		0								/* 0248 */
 .word		0								/* 0249 */
 .word		0								/* 0250 */
 .word		0								/* 0251 */
 .word		0								/* 0252 */
 .word		0								/* 0253 */
 .word		0								/* 0254 */
 .word		0								/* 0255 */

// Audio Library
 .word		BSP_Audio_SetMasterVolume		/* 0256 */
 .word		BSP_Audio_GetMasterVolume		/* 0257 */
 .word		BSP_Audio_GetMasterVolumeL		/* 0258 */
 .word		BSP_Audio_GetMasterVolumeR		/* 0259 */
 .word		BSP_Audio_SetMasterVolumeLR		/* 0260 */
 .word		BSP_Audio_IncMasterVolume		/* 0261 */
 .word		BSP_Audio_DecMasterVolume		/* 0262 */
 .word		BSP_Audio_SetChannelVolume		/* 0263 */
 .word		BSP_Audio_SetChannelVolumeLR	/* 0264 */
 .word		BSP_Audio_IncChannelVolume		/* 0265 */
 .word		BSP_Audio_DecChannelVolume		/* 0266 */
 .word		BSP_Audio_LinkSourceMID			/* 0267 */
 .word		BSP_Audio_LinkSourceMP3			/* 0268 */
 .word		BSP_Audio_LinkSourceMOD			/* 0269 */
 .word		BSP_Audio_LinkSourceRAW			/* 0270 */
 .word		BSP_Audio_ChannelPLay			/* 0271 */
 .word		BSP_Audio_ChannelStop			/* 0272 */
 .word		BSP_Audio_ChannelPause			/* 0273 */
 .word		BSP_Audio_RegisterStatusCallback/* 0274 */
 .word		BSP_Audio_GetStatusParam		/* 0275 */
 .word		BSP_Audio_GetFreeChannel		/* 0276 */
 .word		0								/* 0277 */
 .word		0								/* 0278 */
 .word		0								/* 0279 */
 .word		0								/* 0280 */
 .word		0								/* 0281 */
 .word		0								/* 0282 */
 .word		0								/* 0283 */
 .word		0								/* 0284 */
 .word		0								/* 0285 */
 .word		0								/* 0286 */
 .word		0								/* 0287 */
 .word		0								/* 0288 */
 .word		0								/* 0289 */
 .word		0								/* 0290 */
 .word		0								/* 0291 */
 .word		0								/* 0292 */
 .word		0								/* 0293 */
 .word		0								/* 0294 */
 .word		0								/* 0295 */
 .word		0								/* 0296 */
 .word		0								/* 0297 */
 .word		0								/* 0298 */
 .word		0								/* 0299 */
 .word		0								/* 0300 */
 .word		0								/* 0301 */
 .word		0								/* 0302 */
 .word		0								/* 0303 */
 .word		0								/* 0304 */
 .word		0								/* 0305 */
 .word		0								/* 0306 */
 .word		0								/* 0307 */
 .word		0								/* 0308 */
 .word		0								/* 0309 */
 .word		0								/* 0310 */
 .word		0								/* 0311 */
 .word		0								/* 0312 */
 .word		0								/* 0313 */
 .word		0								/* 0314 */
 .word		0								/* 0315 */
 .word		0								/* 0316 */
 .word		0								/* 0317 */
 .word		0								/* 0318 */
 .word		0								/* 0319 */
 .word		0								/* 0320 */
 .word		0								/* 0321 */
 .word		0								/* 0322 */
 .word		0								/* 0323 */
 .word		0								/* 0324 */
 .word		0								/* 0325 */
 .word		0								/* 0326 */
 .word		0								/* 0327 */
 .word		0								/* 0328 */
 .word		0								/* 0329 */
 .word		0								/* 0330 */
 .word		0								/* 0331 */
 .word		0								/* 0332 */
 .word		0								/* 0333 */
 .word		0								/* 0334 */
 .word		0								/* 0335 */
 .word		0								/* 0336 */
 .word		0								/* 0337 */
 .word		0								/* 0338 */
 .word		0								/* 0339 */
 .word		0								/* 0340 */
 .word		0								/* 0341 */
 .word		0								/* 0342 */
 .word		0								/* 0343 */
 .word		0								/* 0344 */
 .word		0								/* 0345 */
 .word		0								/* 0346 */
 .word		0								/* 0347 */
 .word		0								/* 0348 */
 .word		0								/* 0349 */
 .word		0								/* 0350 */
 .word		0								/* 0351 */
 .word		0								/* 0352 */
 .word		0								/* 0353 */
 .word		0								/* 0354 */
 .word		0								/* 0355 */
 .word		0								/* 0356 */
 .word		0								/* 0357 */
 .word		0								/* 0358 */
 .word		0								/* 0359 */
 .word		0								/* 0360 */
 .word		0								/* 0361 */
 .word		0								/* 0362 */
 .word		0								/* 0363 */
 .word		0								/* 0364 */
 .word		0								/* 0365 */
 .word		0								/* 0366 */
 .word		0								/* 0367 */
 .word		0								/* 0368 */
 .word		0								/* 0369 */
 .word		0								/* 0370 */
 .word		0								/* 0371 */
 .word		0								/* 0372 */
 .word		0								/* 0373 */
 .word		0								/* 0374 */
 .word		0								/* 0375 */
 .word		0								/* 0376 */
 .word		0								/* 0377 */
 .word		0								/* 0378 */
 .word		0								/* 0379 */
 .word		0								/* 0380 */
 .word		0								/* 0381 */
 .word		0								/* 0382 */
 .word		0								/* 0383 */

// Resources
 .word		BSP_Res_Init					/* 0384 */
 .word		BSP_Res_Alloc					/* 0385 */
 .word		BSP_Res_Free					/* 0386 */
 .word		BSP_Res_Load					/* 0387 */
 .word		BSP_Res_GetSize					/* 0388 */
 .word		0								/* 0389 */
 .word		0								/* 0390 */
 .word		0								/* 0391 */
 .word		0								/* 0392 */
 .word		0								/* 0393 */
 .word		0								/* 0394 */
 .word		0								/* 0395 */
 .word		0								/* 0396 */
 .word		0								/* 0397 */
 .word		0								/* 0398 */
 .word		0								/* 0399 */
 .word		0								/* 0400 */
 .word		0								/* 0401 */
 .word		0								/* 0402 */
 .word		0								/* 0403 */
 .word		0								/* 0404 */
 .word		0								/* 0405 */
 .word		0								/* 0406 */
 .word		0								/* 0407 */
 .word		0								/* 0408 */
 .word		0								/* 0409 */
 .word		0								/* 0410 */
 .word		0								/* 0411 */
 .word		0								/* 0412 */
 .word		0								/* 0413 */
 .word		0								/* 0414 */
 .word		0								/* 0415 */
 .word		0								/* 0416 */
 .word		0								/* 0417 */
 .word		0								/* 0418 */
 .word		0								/* 0419 */
 .word		0								/* 0420 */
 .word		0								/* 0421 */
 .word		0								/* 0422 */
 .word		0								/* 0423 */
 .word		0								/* 0424 */
 .word		0								/* 0425 */
 .word		0								/* 0426 */
 .word		0								/* 0427 */
 .word		0								/* 0428 */
 .word		0								/* 0429 */
 .word		0								/* 0430 */
 .word		0								/* 0431 */
 .word		0								/* 0432 */
 .word		0								/* 0433 */
 .word		0								/* 0434 */
 .word		0								/* 0435 */
 .word		0								/* 0436 */
 .word		0								/* 0437 */
 .word		0								/* 0438 */
 .word		0								/* 0439 */
 .word		0								/* 0440 */
 .word		0								/* 0441 */
 .word		0								/* 0442 */
 .word		0								/* 0443 */
 .word		0								/* 0444 */
 .word		0								/* 0445 */
 .word		0								/* 0446 */
 .word		0								/* 0447 */

 // FATFS
 .word		f_open							/* 0448 */
 .word		f_close							/* 0449 */
 .word		f_read							/* 0450 */
 .word		f_write							/* 0451 */
 .word		f_lseek							/* 0452 */
 .word		f_truncate						/* 0453 */
 .word		f_sync							/* 0454 */
 .word		f_opendir						/* 0455 */
 .word		f_closedir						/* 0456 */
 .word		f_readdir						/* 0457 */
 .word		0								/* 0458 */
 .word		0								/* 0459 */
 .word		f_mkdir							/* 0460 */
 .word		f_unlink						/* 0461 */
 .word		f_rename						/* 0462 */
 .word		f_stat							/* 0463 */
 .word		0								/* 0464 */
 .word		0								/* 0465 */
 .word		0		 						/* 0466 */
 .word		0								/* 0467 */
 .word		0								/* 0468 */
 .word		0								/* 0469 */
 .word		f_getfree						/* 0470 */
 .word		f_getlabel						/* 0471 */
 .word		0								/* 0472 */
 .word		0								/* 0473 */
 .word		0								/* 0474 */
 .word		0								/* 0475 */
 .word		0								/* 0476 */
 .word		0								/* 0477 */
 .word		0								/* 0478 */
 .word		f_putc							/* 0479 */
 .word		f_puts							/* 0480 */
 .word		f_printf						/* 0481 */
 .word		f_gets							/* 0482 */
 .word		0								/* 0483 */
 .word		0								/* 0484 */
 .word		0								/* 0485 */
 .word		0								/* 0486 */
 .word		0								/* 0487 */
 .word		0								/* 0488 */
 .word		0								/* 0489 */
 .word		0								/* 0490 */
 .word		0								/* 0491 */
 .word		0								/* 0492 */
 .word		0								/* 0493 */
 .word		0								/* 0494 */
 .word		0								/* 0495 */
 .word		0								/* 0496 */
 .word		0								/* 0497 */
 .word		0								/* 0498 */
 .word		0								/* 0499 */
 .word		0								/* 0500 */
 .word		0								/* 0501 */
 .word		0								/* 0502 */
 .word		0								/* 0503 */
 .word		0								/* 0504 */
 .word		0								/* 0505 */
 .word		0								/* 0506 */
 .word		0								/* 0507 */
 .word		0								/* 0508 */
 .word		0								/* 0509 */
 .word		0								/* 0510 */
 .word		0								/* 0511 */

// USB Stack
 .word		BSP_USB_MSC_Init				/* 0512 */
 .word		BSP_USB_CDC_Init				/* 0513 */
 .word		BSP_USB_HID_Init				/* 0514 */
 .word		0								/* 0515 */
 .word		0								/* 0516 */
 .word		0								/* 0517 */
 .word		0								/* 0518 */
 .word		0								/* 0519 */
 .word		BSP_USB_Disconnect				/* 0520 */
 .word		BSP_USB_Task					/* 0521 */
 .word		BSP_USB_IsConnected				/* 0522 */
 .word		0								/* 0523 */
 .word		0								/* 0524 */
 .word		0								/* 0525 */
 .word		0								/* 0526 */
 .word		0								/* 0527 */
 .word		BSP_USB_CDC_RegCbRx				/* 0528 */
 .word		BSP_USB_CDC_RegCbRxChar			/* 0529 */
 .word		BSP_USB_CDC_RegCbTx				/* 0530 */
 .word		BSP_USB_CDC_DataAvailable		/* 0531 */
 .word		BSP_USB_CDC_Read				/* 0532 */
 .word		BSP_USB_CDC_Write				/* 0533 */
 .word		BSP_USB_CDC_WriteFlush			/* 0534 */
 .word		BSP_USB_CDC_ReadFlush			/* 0535 */
 .word		0								/* 0536 */
 .word		0								/* 0537 */
 .word		0								/* 0538 */
 .word		0								/* 0539 */
 .word		0								/* 0540 */
 .word		0								/* 0541 */
 .word		0								/* 0542 */
 .word		0								/* 0543 */
 .word		BSP_USB_HID_RegCbLeds			/* 0544 */
 .word		BSP_USB_HID_Mouse				/* 0545 */
 .word		BSP_USB_HID_Keyboard			/* 0546 */
 .word		BSP_USB_HID_Gamepad				/* 0547 */
 .word		BSP_USB_HID_Ctrl				/* 0548 */
 .word		0								/* 0549 */
 .word		0								/* 0550 */
 .word		0								/* 0551 */
 .word		0								/* 0552 */
 .word		0								/* 0553 */
 .word		0								/* 0554 */
 .word		0								/* 0555 */
 .word		0								/* 0556 */
 .word		0								/* 0557 */
 .word		0								/* 0558 */
 .word		0								/* 0559 */
 .word		0								/* 0560 */
 .word		0								/* 0561 */
 .word		0								/* 0562 */
 .word		0								/* 0563 */
 .word		0								/* 0564 */
 .word		0								/* 0565 */
 .word		0								/* 0566 */
 .word		0								/* 0567 */
 .word		0								/* 0568 */
 .word		0								/* 0569 */
 .word		0								/* 0570 */
 .word		0								/* 0571 */
 .word		0								/* 0572 */
 .word		0								/* 0573 */
 .word		0								/* 0574 */
 .word		0								/* 0575 */
 .word		0								/* 0576 */
 .word		0								/* 0577 */
 .word		0								/* 0578 */
 .word		0								/* 0579 */
 .word		0								/* 0580 */
 .word		0								/* 0581 */
 .word		0								/* 0582 */
 .word		0								/* 0583 */
 .word		0								/* 0584 */
 .word		0								/* 0585 */
 .word		0								/* 0586 */
 .word		0								/* 0587 */
 .word		0								/* 0588 */
 .word		0								/* 0589 */
 .word		0								/* 0590 */
 .word		0								/* 0591 */
 .word		0								/* 0592 */
 .word		0								/* 0593 */
 .word		0								/* 0594 */
 .word		0								/* 0595 */
 .word		0								/* 0596 */
 .word		0								/* 0597 */
 .word		0								/* 0598 */
 .word		0								/* 0599 */
 .word		0								/* 0600 */
 .word		0								/* 0601 */
 .word		0								/* 0602 */
 .word		0								/* 0603 */
 .word		0								/* 0604 */
 .word		0								/* 0605 */
 .word		0								/* 0606 */
 .word		0								/* 0607 */
 .word		0								/* 0608 */
 .word		0								/* 0609 */
 .word		0								/* 0610 */
 .word		0								/* 0611 */
 .word		0								/* 0612 */
 .word		0								/* 0613 */
 .word		0								/* 0614 */
 .word		0								/* 0615 */
 .word		0								/* 0616 */
 .word		0								/* 0617 */
 .word		0								/* 0618 */
 .word		0								/* 0619 */
 .word		0								/* 0620 */
 .word		0								/* 0621 */
 .word		0								/* 0622 */
 .word		0								/* 0623 */
 .word		0								/* 0624 */
 .word		0								/* 0625 */
 .word		0								/* 0626 */
 .word		0								/* 0627 */
 .word		0								/* 0628 */
 .word		0								/* 0629 */
 .word		0								/* 0630 */
 .word		0								/* 0631 */
 .word		0								/* 0632 */
 .word		0								/* 0633 */
 .word		0								/* 0634 */
 .word		0								/* 0635 */
 .word		0								/* 0636 */
 .word		0								/* 0637 */
 .word		0								/* 0638 */
 .word		0								/* 0639 */

// Remaining set as NULL
 .word		0								/* 0640 */
 .word		0								/* 0641 */
 .word		0								/* 0642 */
 .word		0								/* 0643 */
 .word		0								/* 0644 */
 .word		0								/* 0645 */
 .word		0								/* 0646 */
 .word		0								/* 0647 */
 .word		0								/* 0648 */
 .word		0								/* 0649 */
 .word		0								/* 0650 */
 .word		0								/* 0651 */
 .word		0								/* 0652 */
 .word		0								/* 0653 */
 .word		0								/* 0654 */
 .word		0								/* 0655 */
 .word		0								/* 0656 */
 .word		0								/* 0657 */
 .word		0								/* 0658 */
 .word		0								/* 0659 */
 .word		0								/* 0660 */
 .word		0								/* 0661 */
 .word		0								/* 0662 */
 .word		0								/* 0663 */
 .word		0								/* 0664 */
 .word		0								/* 0665 */
 .word		0								/* 0666 */
 .word		0								/* 0667 */
 .word		0								/* 0668 */
 .word		0								/* 0669 */
 .word		0								/* 0670 */
 .word		0								/* 0671 */
 .word		0								/* 0672 */
 .word		0								/* 0673 */
 .word		0								/* 0674 */
 .word		0								/* 0675 */
 .word		0								/* 0676 */
 .word		0								/* 0677 */
 .word		0								/* 0678 */
 .word		0								/* 0679 */
 .word		0								/* 0680 */
 .word		0								/* 0681 */
 .word		0								/* 0682 */
 .word		0								/* 0683 */
 .word		0								/* 0684 */
 .word		0								/* 0685 */
 .word		0								/* 0686 */
 .word		0								/* 0687 */
 .word		0								/* 0688 */
 .word		0								/* 0689 */
 .word		0								/* 0690 */
 .word		0								/* 0691 */
 .word		0								/* 0692 */
 .word		0								/* 0693 */
 .word		0								/* 0694 */
 .word		0								/* 0695 */
 .word		0								/* 0696 */
 .word		0								/* 0697 */
 .word		0								/* 0698 */
 .word		0								/* 0699 */
 .word		0								/* 0700 */
 .word		0								/* 0701 */
 .word		0								/* 0702 */
 .word		0								/* 0703 */
 .word		0								/* 0704 */
 .word		0								/* 0705 */
 .word		0								/* 0706 */
 .word		0								/* 0707 */
 .word		0								/* 0708 */
 .word		0								/* 0709 */
 .word		0								/* 0710 */
 .word		0								/* 0711 */
 .word		0								/* 0712 */
 .word		0								/* 0713 */
 .word		0								/* 0714 */
 .word		0								/* 0715 */
 .word		0								/* 0716 */
 .word		0								/* 0717 */
 .word		0								/* 0718 */
 .word		0								/* 0719 */
 .word		0								/* 0720 */
 .word		0								/* 0721 */
 .word		0								/* 0722 */
 .word		0								/* 0723 */
 .word		0								/* 0724 */
 .word		0								/* 0725 */
 .word		0								/* 0726 */
 .word		0								/* 0727 */
 .word		0								/* 0728 */
 .word		0								/* 0729 */
 .word		0								/* 0730 */
 .word		0								/* 0731 */
 .word		0								/* 0732 */
 .word		0								/* 0733 */
 .word		0								/* 0734 */
 .word		0								/* 0735 */
 .word		0								/* 0736 */
 .word		0								/* 0737 */
 .word		0								/* 0738 */
 .word		0								/* 0739 */
 .word		0								/* 0740 */
 .word		0								/* 0741 */
 .word		0								/* 0742 */
 .word		0								/* 0743 */
 .word		0								/* 0744 */
 .word		0								/* 0745 */
 .word		0								/* 0746 */
 .word		0								/* 0747 */
 .word		0								/* 0748 */
 .word		0								/* 0749 */
 .word		0								/* 0750 */
 .word		0								/* 0751 */
 .word		0								/* 0752 */
 .word		0								/* 0753 */
 .word		0								/* 0754 */
 .word		0								/* 0755 */
 .word		0								/* 0756 */
 .word		0								/* 0757 */
 .word		0								/* 0758 */
 .word		0								/* 0759 */
 .word		0								/* 0760 */
 .word		0								/* 0761 */
 .word		0								/* 0762 */
 .word		0								/* 0763 */
 .word		0								/* 0764 */
 .word		0								/* 0765 */
 .word		0								/* 0766 */
 .word		0								/* 0767 */
 .word		0								/* 0768 */
 .word		0								/* 0769 */
 .word		0								/* 0770 */
 .word		0								/* 0771 */
 .word		0								/* 0772 */
 .word		0								/* 0773 */
 .word		0								/* 0774 */
 .word		0								/* 0775 */
 .word		0								/* 0776 */
 .word		0								/* 0777 */
 .word		0								/* 0778 */
 .word		0								/* 0779 */
 .word		0								/* 0780 */
 .word		0								/* 0781 */
 .word		0								/* 0782 */
 .word		0								/* 0783 */
 .word		0								/* 0784 */
 .word		0								/* 0785 */
 .word		0								/* 0786 */
 .word		0								/* 0787 */
 .word		0								/* 0788 */
 .word		0								/* 0789 */
 .word		0								/* 0790 */
 .word		0								/* 0791 */
 .word		0								/* 0792 */
 .word		0								/* 0793 */
 .word		0								/* 0794 */
 .word		0								/* 0795 */
 .word		0								/* 0796 */
 .word		0								/* 0797 */
 .word		0								/* 0798 */
 .word		0								/* 0799 */
 .word		0								/* 0800 */
 .word		0								/* 0801 */
 .word		0								/* 0802 */
 .word		0								/* 0803 */
 .word		0								/* 0804 */
 .word		0								/* 0805 */
 .word		0								/* 0806 */
 .word		0								/* 0807 */
 .word		0								/* 0808 */
 .word		0								/* 0809 */
 .word		0								/* 0810 */
 .word		0								/* 0811 */
 .word		0								/* 0812 */
 .word		0								/* 0813 */
 .word		0								/* 0814 */
 .word		0								/* 0815 */
 .word		0								/* 0816 */
 .word		0								/* 0817 */
 .word		0								/* 0818 */
 .word		0								/* 0819 */
 .word		0								/* 0820 */
 .word		0								/* 0821 */
 .word		0								/* 0822 */
 .word		0								/* 0823 */
 .word		0								/* 0824 */
 .word		0								/* 0825 */
 .word		0								/* 0826 */
 .word		0								/* 0827 */
 .word		0								/* 0828 */
 .word		0								/* 0829 */
 .word		0								/* 0830 */
 .word		0								/* 0831 */
 .word		0								/* 0832 */
 .word		0								/* 0833 */
 .word		0								/* 0834 */
 .word		0								/* 0835 */
 .word		0								/* 0836 */
 .word		0								/* 0837 */
 .word		0								/* 0838 */
 .word		0								/* 0839 */
 .word		0								/* 0840 */
 .word		0								/* 0841 */
 .word		0								/* 0842 */
 .word		0								/* 0843 */
 .word		0								/* 0844 */
 .word		0								/* 0845 */
 .word		0								/* 0846 */
 .word		0								/* 0847 */
 .word		0								/* 0848 */
 .word		0								/* 0849 */
 .word		0								/* 0850 */
 .word		0								/* 0851 */
 .word		0								/* 0852 */
 .word		0								/* 0853 */
 .word		0								/* 0854 */
 .word		0								/* 0855 */
 .word		0								/* 0856 */
 .word		0								/* 0857 */
 .word		0								/* 0858 */
 .word		0								/* 0859 */
 .word		0								/* 0860 */
 .word		0								/* 0861 */
 .word		0								/* 0862 */
 .word		0								/* 0863 */
 .word		0								/* 0864 */
 .word		0								/* 0865 */
 .word		0								/* 0866 */
 .word		0								/* 0867 */
 .word		0								/* 0868 */
 .word		0								/* 0869 */
 .word		0								/* 0870 */
 .word		0								/* 0871 */
 .word		0								/* 0872 */
 .word		0								/* 0873 */
 .word		0								/* 0874 */
 .word		0								/* 0875 */
 .word		0								/* 0876 */
 .word		0								/* 0877 */
 .word		0								/* 0878 */
 .word		0								/* 0879 */
 .word		0								/* 0880 */
 .word		0								/* 0881 */
 .word		0								/* 0882 */
 .word		0								/* 0883 */
 .word		0								/* 0884 */
 .word		0								/* 0885 */
 .word		0								/* 0886 */
 .word		0								/* 0887 */
 .word		0								/* 0888 */
 .word		0								/* 0889 */
 .word		0								/* 0890 */
 .word		0								/* 0891 */
 .word		0								/* 0892 */
 .word		0								/* 0893 */
 .word		0								/* 0894 */
 .word		0								/* 0895 */
 .word		0								/* 0896 */
 .word		0								/* 0897 */
 .word		0								/* 0898 */
 .word		0								/* 0899 */
 .word		0								/* 0900 */
 .word		0								/* 0901 */
 .word		0								/* 0902 */
 .word		0								/* 0903 */
 .word		0								/* 0904 */
 .word		0								/* 0905 */
 .word		0								/* 0906 */
 .word		0								/* 0907 */
 .word		0								/* 0908 */
 .word		0								/* 0909 */
 .word		0								/* 0910 */
 .word		0								/* 0911 */
 .word		0								/* 0912 */
 .word		0								/* 0913 */
 .word		0								/* 0914 */
 .word		0								/* 0915 */
 .word		0								/* 0916 */
 .word		0								/* 0917 */
 .word		0								/* 0918 */
 .word		0								/* 0919 */
 .word		0								/* 0920 */
 .word		0								/* 0921 */
 .word		0								/* 0922 */
 .word		0								/* 0923 */
 .word		0								/* 0924 */
 .word		0								/* 0925 */
 .word		0								/* 0926 */
 .word		0								/* 0927 */
 .word		0								/* 0928 */
 .word		0								/* 0929 */
 .word		0								/* 0930 */
 .word		0								/* 0931 */
 .word		0								/* 0932 */
 .word		0								/* 0933 */
 .word		0								/* 0934 */
 .word		0								/* 0935 */
 .word		0								/* 0936 */
 .word		0								/* 0937 */
 .word		0								/* 0938 */
 .word		0								/* 0939 */
 .word		0								/* 0940 */
 .word		0								/* 0941 */
 .word		0								/* 0942 */
 .word		0								/* 0943 */
 .word		0								/* 0944 */
 .word		0								/* 0945 */
 .word		0								/* 0946 */
 .word		0								/* 0947 */
 .word		0								/* 0948 */
 .word		0								/* 0949 */
 .word		0								/* 0950 */
 .word		0								/* 0951 */
 .word		0								/* 0952 */
 .word		0								/* 0953 */
 .word		0								/* 0954 */
 .word		0								/* 0955 */
 .word		0								/* 0956 */
 .word		0								/* 0957 */
 .word		0								/* 0958 */
 .word		0								/* 0959 */
 .word		0								/* 0960 */
 .word		0								/* 0961 */
 .word		0								/* 0962 */
 .word		0								/* 0963 */
 .word		0								/* 0964 */
 .word		0								/* 0965 */
 .word		0								/* 0966 */
 .word		0								/* 0967 */
 .word		0								/* 0968 */
 .word		0								/* 0969 */
 .word		0								/* 0970 */
 .word		0								/* 0971 */
 .word		0								/* 0972 */
 .word		0								/* 0973 */
 .word		0								/* 0974 */
 .word		0								/* 0975 */
 .word		0								/* 0976 */
 .word		0								/* 0977 */
 .word		0								/* 0978 */
 .word		0								/* 0979 */
 .word		0								/* 0980 */
 .word		0								/* 0981 */
 .word		0								/* 0982 */
 .word		0								/* 0983 */
 .word		0								/* 0984 */
 .word		0								/* 0985 */
 .word		0								/* 0986 */
 .word		0								/* 0987 */
 .word		0								/* 0988 */
 .word		0								/* 0989 */
 .word		0								/* 0990 */
 .word		0								/* 0991 */
 .word		0								/* 0992 */
 .word		0								/* 0993 */
 .word		0								/* 0994 */
 .word		0								/* 0995 */
 .word		0								/* 0996 */
 .word		0								/* 0997 */
 .word		0								/* 0998 */
 .word		0								/* 0999 */
 .word		0								/* 1000 */
 .word		0								/* 1001 */
 .word		0								/* 1002 */
 .word		0								/* 1003 */
 .word		0								/* 1004 */
 .word		0								/* 1005 */
 .word		0								/* 1006 */
 .word		0								/* 1007 */
 .word		0								/* 1008 */
 .word		0								/* 1009 */
 .word		0								/* 1010 */
 .word		0								/* 1011 */
 .word		0								/* 1012 */
 .word		0								/* 1013 */
 .word		0								/* 1014 */
 .word		0								/* 1015 */
 .word		0								/* 1016 */
 .word		0								/* 1017 */
 .word		0								/* 1018 */
 .word		0								/* 1019 */
 .word		0								/* 1020 */
 .word		0								/* 1021 */
 .word		0								/* 1022 */
 .word		0								/* 1023 */


/*******************************************************************************
*
* Provide weak aliases for each Exception handler to the Default_Handler.
* As they are weak aliases, any function with the same name will override
* this definition.
*
*******************************************************************************/
   .weak      NMI_Handler
   .thumb_set NMI_Handler,Default_Handler

   .weak      HardFault_Handler
   .thumb_set HardFault_Handler,Default_Handler

   .weak      MemManage_Handler
   .thumb_set MemManage_Handler,Default_Handler

   .weak      BusFault_Handler
   .thumb_set BusFault_Handler,Default_Handler

   .weak      UsageFault_Handler
   .thumb_set UsageFault_Handler,Default_Handler

   .weak      SVC_Handler
   .thumb_set SVC_Handler,Default_Handler

   .weak      DebugMon_Handler
   .thumb_set DebugMon_Handler,Default_Handler

   .weak      PendSV_Handler
   .thumb_set PendSV_Handler,Default_Handler

   .weak      SysTick_Handler
   .thumb_set SysTick_Handler,Default_Handler

   .weak      WWDG_IRQHandler
   .thumb_set WWDG_IRQHandler,Default_Handler

   .weak      PVD_AVD_IRQHandler
   .thumb_set PVD_AVD_IRQHandler,Default_Handler

   .weak      TAMP_STAMP_IRQHandler
   .thumb_set TAMP_STAMP_IRQHandler,Default_Handler

   .weak      RTC_WKUP_IRQHandler
   .thumb_set RTC_WKUP_IRQHandler,Default_Handler

   .weak      FLASH_IRQHandler
   .thumb_set FLASH_IRQHandler,Default_Handler

   .weak      RCC_IRQHandler
   .thumb_set RCC_IRQHandler,Default_Handler

   .weak      EXTI0_IRQHandler
   .thumb_set EXTI0_IRQHandler,Default_Handler

   .weak      EXTI1_IRQHandler
   .thumb_set EXTI1_IRQHandler,Default_Handler

   .weak      EXTI2_IRQHandler
   .thumb_set EXTI2_IRQHandler,Default_Handler

   .weak      EXTI3_IRQHandler
   .thumb_set EXTI3_IRQHandler,Default_Handler

   .weak      EXTI4_IRQHandler
   .thumb_set EXTI4_IRQHandler,Default_Handler

   .weak      DMA1_Stream0_IRQHandler
   .thumb_set DMA1_Stream0_IRQHandler,Default_Handler

   .weak      DMA1_Stream1_IRQHandler
   .thumb_set DMA1_Stream1_IRQHandler,Default_Handler

   .weak      DMA1_Stream2_IRQHandler
   .thumb_set DMA1_Stream2_IRQHandler,Default_Handler

   .weak      DMA1_Stream3_IRQHandler
   .thumb_set DMA1_Stream3_IRQHandler,Default_Handler

   .weak      DMA1_Stream4_IRQHandler
   .thumb_set DMA1_Stream4_IRQHandler,Default_Handler

   .weak      DMA1_Stream5_IRQHandler
   .thumb_set DMA1_Stream5_IRQHandler,Default_Handler

   .weak      DMA1_Stream6_IRQHandler
   .thumb_set DMA1_Stream6_IRQHandler,Default_Handler

   .weak      ADC_IRQHandler
   .thumb_set ADC_IRQHandler,Default_Handler

   .weak      FDCAN1_IT0_IRQHandler
   .thumb_set FDCAN1_IT0_IRQHandler,Default_Handler

   .weak      FDCAN2_IT0_IRQHandler
   .thumb_set FDCAN2_IT0_IRQHandler,Default_Handler

   .weak      FDCAN1_IT1_IRQHandler
   .thumb_set FDCAN1_IT1_IRQHandler,Default_Handler

   .weak      FDCAN2_IT1_IRQHandler
   .thumb_set FDCAN2_IT1_IRQHandler,Default_Handler

   .weak      EXTI9_5_IRQHandler
   .thumb_set EXTI9_5_IRQHandler,Default_Handler

   .weak      TIM1_BRK_IRQHandler
   .thumb_set TIM1_BRK_IRQHandler,Default_Handler

   .weak      TIM1_UP_IRQHandler
   .thumb_set TIM1_UP_IRQHandler,Default_Handler

   .weak      TIM1_TRG_COM_IRQHandler
   .thumb_set TIM1_TRG_COM_IRQHandler,Default_Handler

   .weak      TIM1_CC_IRQHandler
   .thumb_set TIM1_CC_IRQHandler,Default_Handler

   .weak      TIM2_IRQHandler
   .thumb_set TIM2_IRQHandler,Default_Handler

   .weak      TIM3_IRQHandler
   .thumb_set TIM3_IRQHandler,Default_Handler

   .weak      TIM4_IRQHandler
   .thumb_set TIM4_IRQHandler,Default_Handler

   .weak      I2C1_EV_IRQHandler
   .thumb_set I2C1_EV_IRQHandler,Default_Handler

   .weak      I2C1_ER_IRQHandler
   .thumb_set I2C1_ER_IRQHandler,Default_Handler

   .weak      I2C2_EV_IRQHandler
   .thumb_set I2C2_EV_IRQHandler,Default_Handler

   .weak      I2C2_ER_IRQHandler
   .thumb_set I2C2_ER_IRQHandler,Default_Handler

   .weak      SPI1_IRQHandler
   .thumb_set SPI1_IRQHandler,Default_Handler

   .weak      SPI2_IRQHandler
   .thumb_set SPI2_IRQHandler,Default_Handler

   .weak      USART1_IRQHandler
   .thumb_set USART1_IRQHandler,Default_Handler

   .weak      USART2_IRQHandler
   .thumb_set USART2_IRQHandler,Default_Handler

   .weak      USART3_IRQHandler
   .thumb_set USART3_IRQHandler,Default_Handler

   .weak      EXTI15_10_IRQHandler
   .thumb_set EXTI15_10_IRQHandler,Default_Handler

   .weak      RTC_Alarm_IRQHandler
   .thumb_set RTC_Alarm_IRQHandler,Default_Handler

   .weak      TIM8_BRK_TIM12_IRQHandler
   .thumb_set TIM8_BRK_TIM12_IRQHandler,Default_Handler

   .weak      TIM8_UP_TIM13_IRQHandler
   .thumb_set TIM8_UP_TIM13_IRQHandler,Default_Handler

   .weak      TIM8_TRG_COM_TIM14_IRQHandler
   .thumb_set TIM8_TRG_COM_TIM14_IRQHandler,Default_Handler

   .weak      TIM8_CC_IRQHandler
   .thumb_set TIM8_CC_IRQHandler,Default_Handler

   .weak      DMA1_Stream7_IRQHandler
   .thumb_set DMA1_Stream7_IRQHandler,Default_Handler

   .weak      FMC_IRQHandler
   .thumb_set FMC_IRQHandler,Default_Handler

   .weak      SDMMC1_IRQHandler
   .thumb_set SDMMC1_IRQHandler,Default_Handler

   .weak      TIM5_IRQHandler
   .thumb_set TIM5_IRQHandler,Default_Handler

   .weak      SPI3_IRQHandler
   .thumb_set SPI3_IRQHandler,Default_Handler

   .weak      UART4_IRQHandler
   .thumb_set UART4_IRQHandler,Default_Handler

   .weak      UART5_IRQHandler
   .thumb_set UART5_IRQHandler,Default_Handler

   .weak      TIM6_DAC_IRQHandler
   .thumb_set TIM6_DAC_IRQHandler,Default_Handler

   .weak      TIM7_IRQHandler
   .thumb_set TIM7_IRQHandler,Default_Handler

   .weak      DMA2_Stream0_IRQHandler
   .thumb_set DMA2_Stream0_IRQHandler,Default_Handler

   .weak      DMA2_Stream1_IRQHandler
   .thumb_set DMA2_Stream1_IRQHandler,Default_Handler

   .weak      DMA2_Stream2_IRQHandler
   .thumb_set DMA2_Stream2_IRQHandler,Default_Handler

   .weak      DMA2_Stream3_IRQHandler
   .thumb_set DMA2_Stream3_IRQHandler,Default_Handler

   .weak      DMA2_Stream4_IRQHandler
   .thumb_set DMA2_Stream4_IRQHandler,Default_Handler

   .weak      ETH_IRQHandler
   .thumb_set ETH_IRQHandler,Default_Handler

   .weak      ETH_WKUP_IRQHandler
   .thumb_set ETH_WKUP_IRQHandler,Default_Handler

   .weak      FDCAN_CAL_IRQHandler
   .thumb_set FDCAN_CAL_IRQHandler,Default_Handler

   .weak      CM7_SEV_IRQHandler
   .thumb_set CM7_SEV_IRQHandler,Default_Handler

   .weak      CM4_SEV_IRQHandler
   .thumb_set CM4_SEV_IRQHandler,Default_Handler

   .weak      DMA2_Stream5_IRQHandler
   .thumb_set DMA2_Stream5_IRQHandler,Default_Handler

   .weak      DMA2_Stream6_IRQHandler
   .thumb_set DMA2_Stream6_IRQHandler,Default_Handler

   .weak      DMA2_Stream7_IRQHandler
   .thumb_set DMA2_Stream7_IRQHandler,Default_Handler

   .weak      USART6_IRQHandler
   .thumb_set USART6_IRQHandler,Default_Handler

   .weak      I2C3_EV_IRQHandler
   .thumb_set I2C3_EV_IRQHandler,Default_Handler

   .weak      I2C3_ER_IRQHandler
   .thumb_set I2C3_ER_IRQHandler,Default_Handler

   .weak      OTG_HS_EP1_OUT_IRQHandler
   .thumb_set OTG_HS_EP1_OUT_IRQHandler,Default_Handler

   .weak      OTG_HS_EP1_IN_IRQHandler
   .thumb_set OTG_HS_EP1_IN_IRQHandler,Default_Handler

   .weak      OTG_HS_WKUP_IRQHandler
   .thumb_set OTG_HS_WKUP_IRQHandler,Default_Handler

   .weak      OTG_HS_IRQHandler
   .thumb_set OTG_HS_IRQHandler,Default_Handler

   .weak      DCMI_IRQHandler
   .thumb_set DCMI_IRQHandler,Default_Handler

   .weak      RNG_IRQHandler
   .thumb_set RNG_IRQHandler,Default_Handler

   .weak      FPU_IRQHandler
   .thumb_set FPU_IRQHandler,Default_Handler

   .weak      UART7_IRQHandler
   .thumb_set UART7_IRQHandler,Default_Handler

   .weak      UART8_IRQHandler
   .thumb_set UART8_IRQHandler,Default_Handler

   .weak      SPI4_IRQHandler
   .thumb_set SPI4_IRQHandler,Default_Handler

   .weak      SPI5_IRQHandler
   .thumb_set SPI5_IRQHandler,Default_Handler

   .weak      SPI6_IRQHandler
   .thumb_set SPI6_IRQHandler,Default_Handler

   .weak      SAI1_IRQHandler
   .thumb_set SAI1_IRQHandler,Default_Handler

   .weak      LTDC_IRQHandler
   .thumb_set LTDC_IRQHandler,Default_Handler

   .weak      LTDC_ER_IRQHandler
   .thumb_set LTDC_ER_IRQHandler,Default_Handler

   .weak      DMA2D_IRQHandler
   .thumb_set DMA2D_IRQHandler,Default_Handler

   .weak      SAI2_IRQHandler
   .thumb_set SAI2_IRQHandler,Default_Handler

   .weak      QUADSPI_IRQHandler
   .thumb_set QUADSPI_IRQHandler,Default_Handler

   .weak      LPTIM1_IRQHandler
   .thumb_set LPTIM1_IRQHandler,Default_Handler

   .weak      CEC_IRQHandler
   .thumb_set CEC_IRQHandler,Default_Handler

   .weak      I2C4_EV_IRQHandler
   .thumb_set I2C4_EV_IRQHandler,Default_Handler

   .weak      I2C4_ER_IRQHandler
   .thumb_set I2C4_ER_IRQHandler,Default_Handler

   .weak      SPDIF_RX_IRQHandler
   .thumb_set SPDIF_RX_IRQHandler,Default_Handler

   .weak      OTG_FS_EP1_OUT_IRQHandler
   .thumb_set OTG_FS_EP1_OUT_IRQHandler,Default_Handler

   .weak      OTG_FS_EP1_IN_IRQHandler
   .thumb_set OTG_FS_EP1_IN_IRQHandler,Default_Handler

   .weak      OTG_FS_WKUP_IRQHandler
   .thumb_set OTG_FS_WKUP_IRQHandler,Default_Handler

   .weak      OTG_FS_IRQHandler
   .thumb_set OTG_FS_IRQHandler,Default_Handler

   .weak      DMAMUX1_OVR_IRQHandler
   .thumb_set DMAMUX1_OVR_IRQHandler,Default_Handler

   .weak      HRTIM1_Master_IRQHandler
   .thumb_set HRTIM1_Master_IRQHandler,Default_Handler

   .weak      HRTIM1_TIMA_IRQHandler
   .thumb_set HRTIM1_TIMA_IRQHandler,Default_Handler

   .weak      HRTIM1_TIMB_IRQHandler
   .thumb_set HRTIM1_TIMB_IRQHandler,Default_Handler

   .weak      HRTIM1_TIMC_IRQHandler
   .thumb_set HRTIM1_TIMC_IRQHandler,Default_Handler

   .weak      HRTIM1_TIMD_IRQHandler
   .thumb_set HRTIM1_TIMD_IRQHandler,Default_Handler

   .weak      HRTIM1_TIME_IRQHandler
   .thumb_set HRTIM1_TIME_IRQHandler,Default_Handler

   .weak      HRTIM1_FLT_IRQHandler
   .thumb_set HRTIM1_FLT_IRQHandler,Default_Handler

   .weak      DFSDM1_FLT0_IRQHandler
   .thumb_set DFSDM1_FLT0_IRQHandler,Default_Handler

   .weak      DFSDM1_FLT1_IRQHandler
   .thumb_set DFSDM1_FLT1_IRQHandler,Default_Handler

   .weak      DFSDM1_FLT2_IRQHandler
   .thumb_set DFSDM1_FLT2_IRQHandler,Default_Handler

   .weak      DFSDM1_FLT3_IRQHandler
   .thumb_set DFSDM1_FLT3_IRQHandler,Default_Handler

   .weak      SAI3_IRQHandler
   .thumb_set SAI3_IRQHandler,Default_Handler

   .weak      SWPMI1_IRQHandler
   .thumb_set SWPMI1_IRQHandler,Default_Handler

   .weak      TIM15_IRQHandler
   .thumb_set TIM15_IRQHandler,Default_Handler

   .weak      TIM16_IRQHandler
   .thumb_set TIM16_IRQHandler,Default_Handler

   .weak      TIM17_IRQHandler
   .thumb_set TIM17_IRQHandler,Default_Handler

   .weak      MDIOS_WKUP_IRQHandler
   .thumb_set MDIOS_WKUP_IRQHandler,Default_Handler

   .weak      MDIOS_IRQHandler
   .thumb_set MDIOS_IRQHandler,Default_Handler

   .weak      JPEG_IRQHandler
   .thumb_set JPEG_IRQHandler,Default_Handler

   .weak      MDMA_IRQHandler
   .thumb_set MDMA_IRQHandler,Default_Handler

   .weak      DSI_IRQHandler
   .thumb_set DSI_IRQHandler,Default_Handler

   .weak      SDMMC2_IRQHandler
   .thumb_set SDMMC2_IRQHandler,Default_Handler

   .weak      HSEM1_IRQHandler
   .thumb_set HSEM1_IRQHandler,Default_Handler

   .weak      HSEM2_IRQHandler
   .thumb_set HSEM2_IRQHandler,Default_Handler

   .weak      ADC3_IRQHandler
   .thumb_set ADC3_IRQHandler,Default_Handler

   .weak      DMAMUX2_OVR_IRQHandler
   .thumb_set DMAMUX2_OVR_IRQHandler,Default_Handler

   .weak      BDMA_Channel0_IRQHandler
   .thumb_set BDMA_Channel0_IRQHandler,Default_Handler

   .weak      BDMA_Channel1_IRQHandler
   .thumb_set BDMA_Channel1_IRQHandler,Default_Handler

   .weak      BDMA_Channel2_IRQHandler
   .thumb_set BDMA_Channel2_IRQHandler,Default_Handler

   .weak      BDMA_Channel3_IRQHandler
   .thumb_set BDMA_Channel3_IRQHandler,Default_Handler

   .weak      BDMA_Channel4_IRQHandler
   .thumb_set BDMA_Channel4_IRQHandler,Default_Handler

   .weak      BDMA_Channel5_IRQHandler
   .thumb_set BDMA_Channel5_IRQHandler,Default_Handler

   .weak      BDMA_Channel6_IRQHandler
   .thumb_set BDMA_Channel6_IRQHandler,Default_Handler

   .weak      BDMA_Channel7_IRQHandler
   .thumb_set BDMA_Channel7_IRQHandler,Default_Handler

   .weak      COMP1_IRQHandler
   .thumb_set COMP1_IRQHandler,Default_Handler

   .weak      LPTIM2_IRQHandler
   .thumb_set LPTIM2_IRQHandler,Default_Handler

   .weak      LPTIM3_IRQHandler
   .thumb_set LPTIM3_IRQHandler,Default_Handler

   .weak      LPTIM4_IRQHandler
   .thumb_set LPTIM4_IRQHandler,Default_Handler

   .weak      LPTIM5_IRQHandler
   .thumb_set LPTIM5_IRQHandler,Default_Handler

   .weak      LPUART1_IRQHandler
   .thumb_set LPUART1_IRQHandler,Default_Handler

   .weak      WWDG_RST_IRQHandler
   .thumb_set WWDG_RST_IRQHandler,Default_Handler

   .weak      CRS_IRQHandler
   .thumb_set CRS_IRQHandler,Default_Handler

   .weak      ECC_IRQHandler
   .thumb_set ECC_IRQHandler,Default_Handler

   .weak      SAI4_IRQHandler
   .thumb_set SAI4_IRQHandler,Default_Handler

   .weak      HOLD_CORE_IRQHandler
   .thumb_set HOLD_CORE_IRQHandler,Default_Handler

   .weak      WAKEUP_PIN_IRQHandler
   .thumb_set WAKEUP_PIN_IRQHandler,Default_Handler


