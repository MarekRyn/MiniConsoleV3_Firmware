/*******************************************************************
 * MiniConsole V3 - Board Support Package - STM32
********************************************************************/

#include "BSP_STM32.h"

#define HSEM_ID_0 (0U) // HW semaphore 0

__IO static uint32_t tickvalue = 0;
__IO static uint32_t tickfrequency = 1;


uint8_t BSP_STM32_MPU_Init(void) {
	MPU_Region_TypeDef MPU_InitStruct = {0};

	// Disables the MPU
	BSP_STM32_MPU_Disable();

	// 0x00: CONFIGURING MPU FOR ALL ADDRESS SPACE (4GB) TO AVOID INCORRECT SPECULATIVE READ
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER0;
	MPU_InitStruct.BaseAddress = 0x00000000;
	MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
	MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
	MPU_InitStruct.SubRegionDisable = 0x87; // <- Excluded areas of memory
	MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
	BSP_STM32_MPU_ConfigRegion(&MPU_InitStruct);

	// 0x01: CONFIGURING MPU FOR ITCMRAM (0x00000000, 64kB)
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER1;
	MPU_InitStruct.BaseAddress = 0x00000000;
	MPU_InitStruct.Size = MPU_REGION_SIZE_64KB;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
	MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
	MPU_InitStruct.SubRegionDisable = 0x00;
	MPU_InitStruct.AccessPermission = MPU_REGION_PRIV_RW;
	BSP_STM32_MPU_ConfigRegion(&MPU_InitStruct);

	// 0x02: CONFIGURING MPU FOR FLASH (0x08000000, 512kB)
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER2;
	MPU_InitStruct.BaseAddress = 0x08000000;
	MPU_InitStruct.Size = MPU_REGION_SIZE_512KB;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
	MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
	MPU_InitStruct.SubRegionDisable = 0x00;
	MPU_InitStruct.AccessPermission = MPU_REGION_PRIV_RW;
	BSP_STM32_MPU_ConfigRegion(&MPU_InitStruct);

	// 0x03: CONFIGURING MPU FOR DTCMRAM (0x20000000, 128kB)
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER3;
	MPU_InitStruct.BaseAddress = 0x20000000;
	MPU_InitStruct.Size = MPU_REGION_SIZE_128KB;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
	MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
	MPU_InitStruct.SubRegionDisable = 0x00;
	MPU_InitStruct.AccessPermission = MPU_REGION_PRIV_RW;
	BSP_STM32_MPU_ConfigRegion(&MPU_InitStruct);

	// 0x04: CONFIGURING MPU FOR OS_RAM + APP_RAM (0x24000000, 512kB)
	// 0x05: CONFIGURING MPU FOR OS_RAM (0x24000000, 128kB) - Restricting access to first 128kB
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER4;
	MPU_InitStruct.BaseAddress = 0x24000000;
	MPU_InitStruct.Size = MPU_REGION_SIZE_512KB;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
	MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
	MPU_InitStruct.SubRegionDisable = 0x00;
	MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
	BSP_STM32_MPU_ConfigRegion(&MPU_InitStruct);

	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER5;
	MPU_InitStruct.BaseAddress = 0x24000000;
	MPU_InitStruct.Size = MPU_REGION_SIZE_128KB;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
	MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
	MPU_InitStruct.SubRegionDisable = 0x00;
	MPU_InitStruct.AccessPermission = MPU_REGION_PRIV_RW;
	BSP_STM32_MPU_ConfigRegion(&MPU_InitStruct);

	// 0x06: CONFIGURING MPU FOR SND_RAM (0x30040000, 32kB)
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER6;
	MPU_InitStruct.BaseAddress = 0x30040000;
	MPU_InitStruct.Size = MPU_REGION_SIZE_32KB;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
	MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
	MPU_InitStruct.SubRegionDisable = 0x00;
	MPU_InitStruct.AccessPermission = MPU_REGION_PRIV_RW;
	BSP_STM32_MPU_ConfigRegion(&MPU_InitStruct);

	// 0x07: CONFIGURING MPU FOR BUF_RAM (0x38000000, 32kB)
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER7;
	MPU_InitStruct.BaseAddress = 0x38000000;
	MPU_InitStruct.Size = MPU_REGION_SIZE_32KB;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
	MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
	MPU_InitStruct.SubRegionDisable = 0x00;
	MPU_InitStruct.AccessPermission = MPU_REGION_PRIV_RW;
	BSP_STM32_MPU_ConfigRegion(&MPU_InitStruct);

	// 0x08: CONFIGURING MPU FOR SH_RAM (0x38008000, 32kB)
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER8;
	MPU_InitStruct.BaseAddress = 0x38008000;
	MPU_InitStruct.Size = MPU_REGION_SIZE_32KB;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
	MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
	MPU_InitStruct.SubRegionDisable = 0x00;
	MPU_InitStruct.AccessPermission = MPU_REGION_PRIV_RW_URO;
	BSP_STM32_MPU_ConfigRegion(&MPU_InitStruct);

	// 0x09: CONFIGURING MPU FOR BK_RAM (0x38800000, 4kB)
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER9;
	MPU_InitStruct.BaseAddress = 0x38800000;
	MPU_InitStruct.Size = MPU_REGION_SIZE_4KB;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
	MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
	MPU_InitStruct.SubRegionDisable = 0x00;
	MPU_InitStruct.AccessPermission = MPU_REGION_PRIV_RW;
	BSP_STM32_MPU_ConfigRegion(&MPU_InitStruct);

	// 0x0A: CONFIGURING MPU FOR PERIPHERAL REGISTERS (0x40000000, 512kB)
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER10;
	MPU_InitStruct.BaseAddress = 0x40000000;
	MPU_InitStruct.Size = MPU_REGION_SIZE_512KB;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
	MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
	MPU_InitStruct.SubRegionDisable = 0x00;
	MPU_InitStruct.AccessPermission = MPU_REGION_PRIV_RW;
	BSP_STM32_MPU_ConfigRegion(&MPU_InitStruct);

	// 0x0B: CONFIGURING MPU FOR APP_FLASH (0x90000000, 16MB)
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER11;
	MPU_InitStruct.BaseAddress = 0x90000000;
	MPU_InitStruct.Size = MPU_REGION_SIZE_16MB;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
	MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
	MPU_InitStruct.SubRegionDisable = 0x00;
	MPU_InitStruct.AccessPermission = MPU_REGION_PRIV_RW_URO;
	BSP_STM32_MPU_ConfigRegion(&MPU_InitStruct);

	// 0x0C: CONFIGURING MPU FOR APP_SDRAM + VIDEO_SDRAM (0xC0000000, 64MB) - General use memory
	// 0x0D: CONFIGURING MPU FOR VIDEO_SDRAM (0xC3000000, 16MB) - restricting access to last 16MB for Video Memory
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER12;
	MPU_InitStruct.BaseAddress = 0xC0000000;
	MPU_InitStruct.Size = MPU_REGION_SIZE_64MB;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
	MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
	MPU_InitStruct.SubRegionDisable = 0x00;
	MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
	BSP_STM32_MPU_ConfigRegion(&MPU_InitStruct);

	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER13;
	MPU_InitStruct.BaseAddress = 0xC3000000;
	MPU_InitStruct.Size = MPU_REGION_SIZE_16MB;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
	MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
	MPU_InitStruct.SubRegionDisable = 0x00;
	MPU_InitStruct.AccessPermission = MPU_REGION_PRIV_RW;
	BSP_STM32_MPU_ConfigRegion(&MPU_InitStruct);

	// Enables the MPU
	BSP_STM32_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

	return BSP_OK;
}


uint8_t BSP_STM32_Init_SysClocks(void) {

	uint32_t common_system_clock = BSP_STM32_RCC_GetSysClockFreq() >> ((D1CorePrescTable[(RCC->D1CFGR & RCC_D1CFGR_D1CPRE)>> RCC_D1CFGR_D1CPRE_Pos]) & 0x1FU);
	SystemD2Clock = (common_system_clock >> ((D1CorePrescTable[(RCC->D1CFGR & RCC_D1CFGR_HPRE)>> RCC_D1CFGR_HPRE_Pos]) & 0x1FU));
	SystemCoreClock = common_system_clock;
	BSP_TickInit(SystemCoreClock, 1, (1UL << __NVIC_PRIO_BITS));

	// Starting SYSCFG clock
	__BSP_RCC_SYSCFG_CLK_ENABLE();

	//Supply configuration update enable
	BSP_STM32_PWR_ConfigSupply(PWR_SMPS_1V8_SUPPLIES_LDO);

	// Configure the main internal regulator output voltage
	BSP_STM32_PWR_VoltageScalingConfig(PWR_REGULATOR_VOLTAGE_SCALE0);

	// Waiting until required voltage is set
	BSP_STM32_PWR_WaitForVOSRDY();

	// Configure LSE Drive Capability
	BSP_STM32_PWR_EnableBkUpAccess();
	BSP_STM32_RCC_LSEConfigDrive(RCC_LSEDRIVE_MEDIUMHIGH);

	// Setting the PLL clock source
	BSP_STM32_RCC_PLLClockSource(RCC_PLLSOURCE_HSE);

	// Initializes the RCC Oscillators according to the specified parameters in the RCC_OscInitTypeDef structure.
	BSP_STM32_RCC_ConfigHSE(RCC_HSE_BYPASS);
	BSP_STM32_RCC_ConfigHSI(RCC_HSI_DIV1, RCC_HSICALIBRATION_DEFAULT);
	BSP_STM32_RCC_ConfigLSE(RCC_LSE_ON);
	BSP_STM32_RCC_ConfigHSI48(RCC_HSI48_ON);

	// For 480/240MHz M: 5 / N: 192 / P: 2 / Q: 16 / R: 2
	// For 400/200MHz M: 2 / N: 160 / P: 2 / Q: 16 / R: 2
	BSP_STM32_RCC_ConfigPLL1(RCC_PLL_ON, RCC_PLLSOURCE_HSE, RCC_PLL1VCIRANGE_2, RCC_PLL1VCOWIDE, 5, 192, 2, 16, 2);

	// Initializes the CPU, AHB and APB buses clocks
	uint32_t clocktype = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_D3PCLK1 | RCC_CLOCKTYPE_D1PCLK1;

	BSP_STM32_RCC_ClockConfig(clocktype, RCC_SYSCLKSOURCE_PLLCLK, RCC_SYSCLK_DIV1, RCC_HCLK_DIV2, RCC_APB1_DIV2, RCC_APB2_DIV2, RCC_APB3_DIV2, RCC_APB4_DIV2, FLASH_LATENCY_2);

	common_system_clock = BSP_STM32_RCC_GetSysClockFreq() >> ((D1CorePrescTable[(RCC->D1CFGR & RCC_D1CFGR_D1CPRE) >> RCC_D1CFGR_D1CPRE_Pos]) & 0x1FU);
	SystemD2Clock = (common_system_clock >> ((D1CorePrescTable[(RCC->D1CFGR & RCC_D1CFGR_HPRE) >> RCC_D1CFGR_HPRE_Pos]) & 0x1FU));
	SystemCoreClock = common_system_clock;

	BSP_TickInit(common_system_clock, 1, (1UL << __NVIC_PRIO_BITS));
	return BSP_OK;
}

uint8_t BSP_STM32_Init_PeriphClocks(void) {

	// Starting peripherals clock - SYSCFG
	__BSP_RCC_SYSCFG_CLK_ENABLE();

	// Starting peripherals clock - HSEM
	__BSP_RCC_HSEM_CLK_ENABLE();

	// Configuring PLL3
	BSP_STM32_RCC_ConfigPLL3(RCC_PLL3VCIRANGE_2, RCC_PLL3VCOMEDIUM, 5, 30, 2, 2, 10, 0);

	// Starting peripherals clock - ADC
	BSP_STM32_RCC_CLKConfig_ADC();

	// Starting peripherals clock - LTDC
	BSP_STM32_RCC_CLKConfig_LTDC();

	// Starting peripherals clocks - RTC
    if (BSP_STM32_RCC_CLKConfig_RTC()) return BSP_ERROR;
    __BSP_RCC_RTC_ENABLE();

    if (BSP_STM32_RTC_Init(RTC, 127, 255)) return BSP_ERROR;

    // Starting Backup RAM
    // __BSP_RCC_BKPRAM_CLK_ENABLE();

	// Enable SYSCFG Clock
	__BSP_RCC_SYSCFG_CLK_ENABLE();

	// Starting GPIOs clocks
	__BSP_RCC_GPIOA_CLK_ENABLE();
	__BSP_RCC_GPIOB_CLK_ENABLE();
	__BSP_RCC_GPIOC_CLK_ENABLE();
	__BSP_RCC_GPIOD_CLK_ENABLE();
	__BSP_RCC_GPIOE_CLK_ENABLE();
	__BSP_RCC_GPIOF_CLK_ENABLE();
	__BSP_RCC_GPIOG_CLK_ENABLE();
	__BSP_RCC_GPIOH_CLK_ENABLE();
	__BSP_RCC_GPIOI_CLK_ENABLE();

	// Starting peripherals clocks - FMC (SDRAM Controller)
	if (BSP_STM32_RCC_CLKConfig_FMC()) return BSP_ERROR;
	__BSP_RCC_FMC_CLK_ENABLE();

	// Starting peripherals clocks - LTDC (LCD Controller)
	__BSP_RCC_LTDC_CLK_ENABLE();

	// Starting peripherals clocks - DMA2D
    __BSP_RCC_DMA2D_CLK_ENABLE();

    // Starting peripherals clocks - JPEG
    __BSP_RCC_JPGDECEN_CLK_ENABLE();

    // Starting peripherals clocks - DMA1
    __BSP_RCC_DMA1_CLK_ENABLE();

    // Starting peripherals clocks - BDMA
    __BSP_RCC_BDMA_CLK_ENABLE();

    // Starting peripherals clocks - ADC1 - Joystick readout
    __BSP_RCC_ADC12_CLK_ENABLE();

    // Starting peripherals clocks - I2C1 - Touch panel
    if (BSP_STM32_RCC_CLKConfig_I2C1()) return BSP_ERROR;
    __BSP_RCC_I2C1_CLK_ENABLE();

    // Starting peripherals clocks - I2C4 - IMU
    if (BSP_STM32_RCC_CLKConfig_I2C4()) return BSP_ERROR;
    __BSP_RCC_I2C4_CLK_ENABLE();

    // Starting peripherials clocks - I2S3 <<<<<<<<<< Started in CM4 Project for audio
    //    if (BSP_STM32_RCC_CLKConfig_I2S3()) return BSP_ERROR;
    //    __BSP_RCC_I2S3_CLK_ENABLE();

    // Starting peripherals clocks - QSPI
    if (BSP_STM32_RCC_CLKConfig_QSPI()) return BSP_ERROR;
    __BSP_RCC_QSPI_CLK_ENABLE();

    // Starting peripherals clocks - SDMMC2
    if (BSP_STM32_RCC_CLKConfig_SDMMC2()) return BSP_ERROR;
    __BSP_RCC_SDMMC2_CLK_ENABLE();

    // Starting peripherals clocks - TIM2 - Keyboard and Joystick readout
    __BSP_RCC_TIM2_CLK_ENABLE();

    // Starting peripherals clocks - TIM3 - PWM for LCD backlight
    __BSP_RCC_TIM3_CLK_ENABLE();

    // Starting peripherals clocks - TIM4 - Power saving timeout
    __BSP_RCC_TIM4_CLK_ENABLE();

    // Starting peripherals clocks - UART5 - Communication with bluetooth
    if (BSP_STM32_RCC_CLKConfig_UART5()) return BSP_ERROR;
    __BSP_RCC_UART5_CLK_ENABLE();

    // Starting peripherals clocks - USART2 - Communication with debug connection
    if (BSP_STM32_RCC_CLKConfig_USART2()) return BSP_ERROR;
    __BSP_RCC_USART2_CLK_ENABLE();

    // Starting peripherals clock - USB_OTG_FS
    if (BSP_STM32_RCC_CLKConfig_USBFS()) return BSP_ERROR;
    __BSP_RCC_USB2_FS_CLK_ENABLE();

    // Starting peripherals clock - CRC - not required
    // __BSP_RCC_CRC_CLK_ENABLE();

	return BSP_OK;
}


uint8_t BSP_STM32_Init_NVIC(void) {

	uint32_t pp = 0;
	uint32_t sp = 0;

	/************** PRIORITY 0 *************/
	pp = 0;		sp = 0;

	// Configuring NVIC for LTDC
	BSP_STM32_NVIC_SetPriority(LTDC_IRQn, pp, sp);
    BSP_STM32_NVIC_EnableIRQ(LTDC_IRQn);

    // Configuring NVIC for DMA2D
    BSP_STM32_NVIC_SetPriority(DMA2D_IRQn, pp, sp);
    BSP_STM32_NVIC_EnableIRQ(DMA2D_IRQn);

    // Configure NVIC for USBFS
    BSP_STM32_NVIC_SetPriority(OTG_FS_IRQn, pp, sp);
    BSP_STM32_NVIC_EnableIRQ(OTG_FS_IRQn);

    // Configuring NVIC for DMA1 - Stream 0 - for I2S3 TX
//    BSP_STM32_NVIC_SetPriority(DMA1_Stream0_IRQn, pp, sp);
//    BSP_STM32_NVIC_EnableIRQ(DMA1_Stream0_IRQn);

    // Configuring NVIC for DMA1 - Stream 1 - for ADC1
    BSP_STM32_NVIC_SetPriority(DMA1_Stream1_IRQn, pp, sp);
    BSP_STM32_NVIC_EnableIRQ(DMA1_Stream1_IRQn);

    // Configuring NVIC for DMA1 - Stream 2 - for I2C1 RX
    BSP_STM32_NVIC_SetPriority(DMA1_Stream2_IRQn, pp, sp);
    BSP_STM32_NVIC_EnableIRQ(DMA1_Stream2_IRQn);

    // Configuring NVIC for BDMA - Channel 0 - for I2C4 RX
    BSP_STM32_NVIC_SetPriority(BDMA_Channel0_IRQn, pp, sp);
    BSP_STM32_NVIC_EnableIRQ(BDMA_Channel0_IRQn);


    /************** PRIORITY 1 *************/
	pp = 1;		sp = 0;

	// Configuring NVIC from Core CM4
	BSP_STM32_NVIC_SetPriority(CM4_SEV_IRQn, pp, sp);
	BSP_STM32_NVIC_EnableIRQ(CM4_SEV_IRQn);

    // Configuring NVIC for SDMMC2
    // BSP_STM32_NVIC_SetPriority(SDMMC2_IRQn, pp, sp);
    // BSP_STM32_NVIC_EnableIRQ(SDMMC2_IRQn);


    /************** PRIORITY 2 *************/
    pp = 2;		sp = 0;


    /************** PRIORITY 3 *************/
    pp = 3;		sp = 0;

    // Configure NVIC for I2C1
    BSP_STM32_NVIC_SetPriority(I2C1_EV_IRQn, pp, sp);
    BSP_STM32_NVIC_EnableIRQ(I2C1_EV_IRQn);

    // Configure NVIC for I2C4
    BSP_STM32_NVIC_SetPriority(I2C4_EV_IRQn, pp, sp);
    BSP_STM32_NVIC_EnableIRQ(I2C4_EV_IRQn);

    // Configure NVIC for USART2
    BSP_STM32_NVIC_SetPriority(USART2_IRQn, pp, sp);
    BSP_STM32_NVIC_EnableIRQ(USART2_IRQn);

    // Configuring NVIC for I2S3
//    BSP_NVIC_SetPriority(SPI3_IRQn, pp, sp);
//    BSP_NVIC_EnableIRQ(SPI3_IRQn);


    /************** PRIORITY 4 *************/
    pp = 4;		sp = 0;

    // Configure NVIC for GPIOs
    BSP_STM32_NVIC_SetPriority(EXTI15_10_IRQn, pp, sp); // Touch panel and MPU6050
    BSP_STM32_NVIC_EnableIRQ(EXTI15_10_IRQn);
    BSP_STM32_NVIC_SetPriority(EXTI9_5_IRQn, pp, sp); // BT Module
    BSP_STM32_NVIC_EnableIRQ(EXTI9_5_IRQn);

    //Configure NVIC for TIM2
    BSP_STM32_NVIC_SetPriority(TIM2_IRQn, pp, sp); // Timer for sampling joystick and keyboard
    BSP_STM32_NVIC_EnableIRQ(TIM2_IRQn);

    //Configure NVIC for TIM4
    BSP_STM32_NVIC_SetPriority(TIM4_IRQn, pp, sp); // Timer for PWR saving mode
    BSP_STM32_NVIC_EnableIRQ(TIM4_IRQn);

    return BSP_OK;
}


uint8_t BSP_STM32_Init_GPIO(void) {
	uint32_t Pin = 0;

	// Configuring GPIO pins - FMC
	// PI9   ------> FMC_D30		PI10   ------> FMC_D31		PF0   ------> FMC_A0		PF1   ------> FMC_A1		PF2   ------> FMC_A2
	// PF3   ------> FMC_A3			PF4   ------> FMC_A4		PF5   ------> FMC_A5		PC2_C   ------> FMC_SDNE0	PC3_C   ------> FMC_SDCKE0
	// PA7   ------> FMC_SDNWE		PF11   ------> FMC_SDNRAS	PF12   ------> FMC_A6		PF13   ------> FMC_A7		PF14   ------> FMC_A8
	// PF15   ------> FMC_A9		PG0   ------> FMC_A10		PG1   ------> FMC_A11		PE7   ------> FMC_D4		PE8   ------> FMC_D5
	// PE9   ------> FMC_D6			PE10   ------> FMC_D7		PE11   ------> FMC_D8		PE12   ------> FMC_D9		PE13   ------> FMC_D10
	// PE14   ------> FMC_D11		PE15   ------> FMC_D12		PH8   ------> FMC_D16		PH9   ------> FMC_D17		PH10   ------> FMC_D18
	// PH11   ------> FMC_D19		PH12   ------> FMC_D20		PD8   ------> FMC_D13		PD9   ------> FMC_D14		PD10   ------> FMC_D15
	// PD14   ------> FMC_D0		PD15   ------> FMC_D1		PG2   ------> FMC_A12		PG4   ------> FMC_BA0		PG5   ------> FMC_BA1
	// PG8   ------> FMC_SDCLK		PH13   ------> FMC_D21		PH14   ------> FMC_D22		PH15   ------> FMC_D23		PI0   ------> FMC_D24
	// PI1   ------> FMC_D25		PI2   ------> FMC_D26		PI3   ------> FMC_D27		PD0   ------> FMC_D2		PD1   ------> FMC_D3
	// PG15   ------> FMC_SDNCAS	PE0   ------> FMC_NBL0		PE1   ------> FMC_NBL1		PI4   ------> FMC_NBL2		PI5   ------> FMC_NBL3
	// PI6   ------> FMC_D28		PI7   ------> FMC_D29

	Pin = GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	BSP_STM32_GPIO_Init(GPIOI, Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC);

	Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	BSP_STM32_GPIO_Init(GPIOF, Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC);

	Pin = GPIO_PIN_2 | GPIO_PIN_3;
	BSP_STM32_GPIO_Init(GPIOC, Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC);

	Pin = GPIO_PIN_7;
	BSP_STM32_GPIO_Init(GPIOA, Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC);

	Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_15;
	BSP_STM32_GPIO_Init(GPIOG, Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC);

	Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_0 | GPIO_PIN_1;
	BSP_STM32_GPIO_Init(GPIOE, Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC);

	Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	BSP_STM32_GPIO_Init(GPIOH, Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC);

	Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_0 | GPIO_PIN_1;
	BSP_STM32_GPIO_Init(GPIOD, Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC);

	// Configuring GPIO pins - LTDC
	// PE4     ------> LTDC_B0   	PE5     ------> LTDC_G0    PE6     ------> LTDC_G1		PI11     ------> LTDC_G6   PF10     ------> LTDC_DE
    // PC0     ------> LTDC_R5   	PA1     ------> LTDC_R2    PA2     ------> LTDC_R1		PH4     ------> LTDC_G4    PA3     ------> LTDC_B5
	// PA4     ------> LTDC_VSYNC   PA5     ------> LTDC_R4    PA6     ------> LTDC_G2		PB0     ------> LTDC_R3    PB1     ------> LTDC_R6
    // PI15     ------> LTDC_R0		PB11     ------> LTDC_G5   PG6     ------> LTDC_R7		PG7     ------> LTDC_CLK   PC6     ------> LTDC_HSYNC
    // PC9     ------> LTDC_G3		PA8     ------> LTDC_B3    PA10     ------> LTDC_B4		PD3     ------> LTDC_G7    PG10     ------> LTDC_B2
    // PG12     ------> LTDC_B1     PB8     ------> LTDC_B6    PB9     ------> LTDC_B7

    Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
    BSP_STM32_GPIO_Init(GPIOE, Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, GPIO_AF14_LTDC);

    Pin = GPIO_PIN_11;
    BSP_STM32_GPIO_Init(GPIOI, Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, GPIO_AF9_LTDC);

    Pin = GPIO_PIN_10;
    BSP_STM32_GPIO_Init(GPIOF, Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, GPIO_AF14_LTDC);

    Pin = GPIO_PIN_0|GPIO_PIN_6;
    BSP_STM32_GPIO_Init(GPIOC, Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, GPIO_AF14_LTDC);

    Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
    BSP_STM32_GPIO_Init(GPIOA, Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, GPIO_AF14_LTDC);

    Pin = GPIO_PIN_4;
    BSP_STM32_GPIO_Init(GPIOC, Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, GPIO_AF14_LTDC);

    Pin = GPIO_PIN_0|GPIO_PIN_1;
    BSP_STM32_GPIO_Init(GPIOB, Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, GPIO_AF9_LTDC);

    Pin = GPIO_PIN_15;
    BSP_STM32_GPIO_Init(GPIOI, Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, GPIO_AF14_LTDC);

    Pin = GPIO_PIN_11|GPIO_PIN_8|GPIO_PIN_9;
    BSP_STM32_GPIO_Init(GPIOB, Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, GPIO_AF14_LTDC);

    Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_10|GPIO_PIN_12;
    BSP_STM32_GPIO_Init(GPIOG, Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, GPIO_AF14_LTDC);

    Pin = GPIO_PIN_9;
    BSP_STM32_GPIO_Init(GPIOC, Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, GPIO_AF10_LTDC);

    Pin = GPIO_PIN_8;
    BSP_STM32_GPIO_Init(GPIOA, Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, GPIO_AF13_LTDC);

    Pin = GPIO_PIN_10;
    BSP_STM32_GPIO_Init(GPIOA, Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, GPIO_AF12_LTDC);

    Pin = GPIO_PIN_3;
    BSP_STM32_GPIO_Init(GPIOD, Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, GPIO_AF14_LTDC);

    // Configuring GPIO pins - ADC1
    // PC4     ------> ADC1_INP4	PC5     ------> ADC1_INP8

    Pin = GPIO_PIN_4|GPIO_PIN_5;
    BSP_STM32_GPIO_Init(GPIOC, Pin, GPIO_MODE_ANALOG, GPIO_NOPULL, 0, 0);

    // Configuring GPIO pins - I2C1
    // PB6     ------> I2C1_SCL    	PB7     ------> I2C1_SDA

    Pin = GPIO_PIN_6|GPIO_PIN_7;
    BSP_STM32_GPIO_Init(GPIOB, Pin, GPIO_MODE_AF_OD, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, GPIO_AF4_I2C1);

    // Configuring GPIO pins - I2C4
    // PD12     ------> I2C4_SCL    PD13     ------> I2C4_SDA

    Pin = GPIO_PIN_12|GPIO_PIN_13;
    BSP_STM32_GPIO_Init(GPIOD, Pin, GPIO_MODE_AF_OD, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, GPIO_AF4_I2C4);

    // Configuring GPIO pins - I2S3
    // PA15 (JTDI)     ------> I2S3_WS		PC10     ------> I2S3_CK		PC12     ------> I2S3_SDO

//    Pin = GPIO_PIN_15;
//    BSP_STM32_GPIO_Init(GPIOA, Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, GPIO_AF6_SPI3);
//
//    Pin = GPIO_PIN_10|GPIO_PIN_12;
//    BSP_STM32_GPIO_Init(GPIOC, Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, GPIO_AF6_SPI3);

    // Configuring GPIO pins - QSPI
    // PF6     ------> QUADSPI_BK1_IO3		PF7     ------> QUADSPI_BK1_IO2		PF8     ------> QUADSPI_BK1_IO0
    // PF9     ------> QUADSPI_BK1_IO1		PB2     ------> QUADSPI_CLK			PB10     ------> QUADSPI_BK1_NCS

    Pin = GPIO_PIN_6|GPIO_PIN_7;
    BSP_STM32_GPIO_Init(GPIOF, Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH, GPIO_AF9_QUADSPI);

    Pin = GPIO_PIN_8|GPIO_PIN_9;
    BSP_STM32_GPIO_Init(GPIOF, Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH, GPIO_AF10_QUADSPI);

    Pin = GPIO_PIN_2|GPIO_PIN_10;
    BSP_STM32_GPIO_Init(GPIOB, Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH, GPIO_AF9_QUADSPI);

    // Configuring GPIO pins - SDMMC2
    // PC1     ------> SDMMC2_CK		PA0     ------> SDMMC2_CMD		PB14     ------> SDMMC2_D0
    // PB15     ------> SDMMC2_D1		PG11     ------> SDMMC2_D2		PB4 (NJTRST)     ------> SDMMC2_D3

    Pin = GPIO_PIN_1;
    BSP_STM32_GPIO_Init(GPIOC, Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF9_SDIO2);

    Pin = GPIO_PIN_0;
    BSP_STM32_GPIO_Init(GPIOA, Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF9_SDIO2);

    Pin = GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_4;
    BSP_STM32_GPIO_Init(GPIOB, Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF9_SDIO2);

    Pin = GPIO_PIN_11;
    BSP_STM32_GPIO_Init(GPIOG, Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF10_SDIO2);

    // Configuring GPIO pins - TIM3
    // PC8 ------> TIM3 PWM Output

    Pin = GPIO_PIN_8;
    BSP_STM32_GPIO_Init(GPIOC, Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, GPIO_AF2_TIM3);

    // Configuring GPIO pins - UART5
    // PB12     ------> UART5_RX		PB13     ------> UART5_TX

    Pin = GPIO_PIN_12|GPIO_PIN_13;
    BSP_STM32_GPIO_Init(GPIOB, Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, GPIO_AF14_UART5);

    // Configuring GPIO pins - USART2
    // PD5     ------> USART2_TX		PD6     ------> USART2_RX

    Pin = GPIO_PIN_5|GPIO_PIN_6;
    BSP_STM32_GPIO_Init(GPIOD, Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, GPIO_AF7_USART2);

    // Configuring GPIO pins - USB_OTG_FS
    // PA11     ------> USB_OTG_FS_DM	PA12     ------> USB_OTG_FS_DP

    Pin = GPIO_PIN_11|GPIO_PIN_12;
    BSP_STM32_GPIO_Init(GPIOA, Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH, GPIO_AF10_OTG1_FS);

    // Configuring GPIO pins - inputs and outputs
    // PA9	------>	INPUT  - BUTTON 6
    // PB5	------> INPUT  - BUTTON PWR
    // PC7	------> INPUT  - BUTTON 7
    // PC11	------> INPUT  - BUTTON 4
    // PC13	------> INPUT  - BUTTON 3
    // PD2	------> INPUT  - BUTTON 1
    // PD4	------> INPUT  - BUTTON 0
    // PD7	------> INPUT  - PWR LO SIGNAL
    // PD11	------> EXTI   - MPU6050 INT
    // PE2	------> OUTPUT - AUDIO ENABLE		- DEFAULT: LOW
    // PE3	------> INPUT  - BUTTON Menu
    // PG3	------> INPUT  - BUTTON 5
    // PG9	------> OUTPUT - PWR HOLD			- DEFAULT: HIGH
    // PG13	------> EXTI   - TOUCH PANEL INT
    // PG14	------> OUTPUT - TOUCH PANEL RST	- DEFAULT: LOW
    // PH2	------> INPUT  - SD CARD DETECT
    // PH3	------> OUTPUT - BLE RST			- DEFAULT: LOW
    // PH5	------> INPUT  - BUTTON JOYCON
    // PH6	------> EXTI   - BLE INT
    // PH7	------> OUTPUT - BLE GPIO			- DEFAULT: LOW
    // PI8	------> INPUT  - BUTTON 2

//    BSP_STM32_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_RESET);
    BSP_STM32_GPIO_WritePin(GPIOH, GPIO_PIN_3|GPIO_PIN_7, GPIO_PIN_RESET);
    BSP_STM32_GPIO_WritePin(GPIOG, GPIO_PIN_9, GPIO_PIN_SET);
    BSP_STM32_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_RESET);

//    Pin = GPIO_PIN_2;
//    BSP_STM32_GPIO_Init(GPIOE, Pin, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, 0);

    Pin = GPIO_PIN_3;
    BSP_STM32_GPIO_Init(GPIOE, Pin, GPIO_MODE_INPUT, GPIO_NOPULL, 0, 0);

    Pin = GPIO_PIN_8;
    BSP_STM32_GPIO_Init(GPIOI, Pin, GPIO_MODE_INPUT, GPIO_NOPULL, 0, 0);

    Pin = GPIO_PIN_13|GPIO_PIN_7|GPIO_PIN_11;
    BSP_STM32_GPIO_Init(GPIOC, Pin, GPIO_MODE_INPUT, GPIO_NOPULL, 0, 0);

    Pin = GPIO_PIN_2;
    BSP_STM32_GPIO_Init(GPIOH, Pin, GPIO_MODE_INPUT, GPIO_NOPULL, 0, 0);

    Pin = GPIO_PIN_3|GPIO_PIN_7;
    BSP_STM32_GPIO_Init(GPIOH, Pin, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, 0);

    Pin = GPIO_PIN_5;
    BSP_STM32_GPIO_Init(GPIOH, Pin, GPIO_MODE_INPUT, GPIO_NOPULL, 0, 0);

    Pin = GPIO_PIN_6;
    BSP_STM32_GPIO_Init(GPIOH, Pin, GPIO_MODE_IT_RISING, GPIO_NOPULL, 0, 0);

    Pin = GPIO_PIN_11;
    BSP_STM32_GPIO_Init(GPIOD, Pin, GPIO_MODE_IT_RISING, GPIO_NOPULL, 0, 0);

    Pin = GPIO_PIN_3;
    BSP_STM32_GPIO_Init(GPIOG, Pin, GPIO_MODE_INPUT, GPIO_NOPULL, 0, 0);

    Pin = GPIO_PIN_9;
    BSP_STM32_GPIO_Init(GPIOA, Pin, GPIO_MODE_INPUT, GPIO_NOPULL, 0, 0);

    Pin = GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_7;
    BSP_STM32_GPIO_Init(GPIOD, Pin, GPIO_MODE_INPUT, GPIO_NOPULL, 0, 0);

    Pin = GPIO_PIN_9|GPIO_PIN_14;
    BSP_STM32_GPIO_Init(GPIOG, Pin, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, 0);

    Pin = GPIO_PIN_13;
    BSP_STM32_GPIO_Init(GPIOG, Pin, GPIO_MODE_IT_RISING, GPIO_NOPULL, 0, 0);

    Pin = GPIO_PIN_5;
    BSP_STM32_GPIO_Init(GPIOB, Pin, GPIO_MODE_INPUT, GPIO_NOPULL, 0, 0);

	return BSP_OK;
}

uint8_t BSP_WaitForCM4StopMode(void) {
	uint32_t timeout = 0xFFFF;

	// Wait until CM4 enters STOP mode
	while (((RCC->CR & RCC_CR_D2CKRDY) != RESET) && (timeout-- > 0));
	if ( timeout < 0 ) return BSP_ERROR;

	return BSP_OK;
}

uint8_t BSP_WakeUpCM4(void) {
	uint32_t timeout = 0xFFFF;

	// Toggle HSEM semaphore
	BSP_STM32_HSEM_FastTake(HSEM_ID_0);
	BSP_STM32_HSEM_FastRelease(HSEM_ID_0);

	// Wait until CM4 wakes up from stop mode
	while(((RCC->CR & RCC_CR_D2CKRDY) == RESET) && (timeout-- > 0));
	if ( timeout == 0 ) return BSP_ERROR;
	return BSP_OK;
}


uint8_t BSP_TickInit(uint32_t CoreClock, uint32_t TickFreq, uint32_t TickPriority) {

	if (TickFreq == 0) return BSP_ERROR;

	// Set SysTick frequency
	if (SysTick_Config(CoreClock / (1000UL / TickFreq))) return BSP_ERROR;
	tickfrequency = TickFreq;

	// Set SysTick priority
	if (TickPriority > (1UL << __NVIC_PRIO_BITS)) return BSP_ERROR;
	BSP_STM32_NVIC_SetPriority(SysTick_IRQn, TickPriority, 0U);

	return BSP_OK;
}


void BSP_IncTick(void) {

	tickvalue += tickfrequency;

}

uint32_t BSP_GetTick(void) {

	return tickvalue;
}

void BSP_Delay(uint32_t delay) {

	uint32_t wait = delay + tickfrequency;
	uint32_t tickstart = BSP_GetTick();

	while ((BSP_GetTick() - tickstart) < wait) {}

}



__weak void BSP_LCD_TP_INTHandler() {
	return;
}
__weak void BSP_IMU_INTHandler() {
	return;
}

__weak void BSP_BLE_INTHandler() {
	return;
}
