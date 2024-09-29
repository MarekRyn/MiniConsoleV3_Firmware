/*******************************************************************
 * MiniConsole V3 - Board Support Package - STM32
 *******************************************************************/

#include "BSP_STM32.h"

__IO static uint32_t tickvalue = 0;
__IO static uint32_t tickfrequency = 1;


uint8_t BSP_STM32_Init_PeriphClocks() {

	// Starting SYSCFG block
	__BSP_RCC_SYSCFG_CLK_ENABLE();

	// Starting GPIOs clocks
	__BSP_RCC_GPIOE_CLK_ENABLE();
	__BSP_RCC_GPIOA_CLK_ENABLE();
	__BSP_RCC_GPIOC_CLK_ENABLE();
	__BSP_RCC_I2S3_CLK_ENABLE();

    // Starting peripherals clocks - DMA1
    __BSP_RCC_DMA1_CLK_ENABLE();

    // Starting peripherials clocks - I2S3
    if (BSP_STM32_RCC_CLKConfig_I2S3()) return BSP_ERROR;
    __BSP_RCC_I2S3_CLK_ENABLE();

	return BSP_OK;
}


uint8_t BSP_STM32_Init_NVIC() {

	uint32_t pp = 0;
	uint32_t sp = 0;

	/************** PRIORITY 0 *************/
	pp = 0;		sp = 0;

    // Configuring NVIC for DMA1 - Stream 0 - for I2S3 TX
    BSP_STM32_NVIC_SetPriority(DMA1_Stream0_IRQn, pp, sp);
    BSP_STM32_NVIC_EnableIRQ(DMA1_Stream0_IRQn);

    // Configuring NVIC for I2S3
    BSP_STM32_NVIC_SetPriority(SPI3_IRQn, pp, sp);
    BSP_STM32_NVIC_EnableIRQ(SPI3_IRQn);

    /************** PRIORITY 1 *************/
	pp = 1;		sp = 0;

	// Configuring NVIC from Core CM7
	BSP_STM32_NVIC_SetPriority(CM7_SEV_IRQn, pp, sp);
	BSP_STM32_NVIC_EnableIRQ(CM7_SEV_IRQn);

    /************** PRIORITY 2 *************/
    pp = 2;		sp = 0;


    /************** PRIORITY 3 *************/
    pp = 3;		sp = 0;


    /************** PRIORITY 4 *************/
    pp = 4;		sp = 0;


    return BSP_OK;
}

uint8_t BSP_STM32_Init_GPIO() {
	uint32_t Pin = 0;

    // Configuring GPIO pins - I2S3
    // PA15 (JTDI)     ------> I2S3_WS		PC10     ------> I2S3_CK		PC12     ------> I2S3_SDO

    Pin = GPIO_PIN_15;
    BSP_STM32_GPIO_Init(GPIOA, Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, GPIO_AF6_SPI3);

    Pin = GPIO_PIN_10|GPIO_PIN_12;
    BSP_STM32_GPIO_Init(GPIOC, Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, GPIO_AF6_SPI3);

    // Configuring GPIO pins - inputs and outputs
    // PE2	------> OUTPUT - AUDIO ENABLE		- DEFAULT: LOW

    BSP_STM32_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_RESET);
    Pin = GPIO_PIN_2;
    BSP_STM32_GPIO_Init(GPIOE, Pin, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, 0);

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
