/*******************************************************************
 * MiniConsole V3 - Board Support Package - STM32 GPIO
 *******************************************************************/

#include "BSP_STM32_GPIO.h"

#define GPIO_NUMBER           (16U)
#if defined(DUAL_CORE)
#define EXTI_CPU1             (0x01000000U)
#define EXTI_CPU2             (0x02000000U)
#endif /*DUAL_CORE*/


uint8_t BSP_STM32_GPIO_Init(GPIO_TypeDef  *GPIOx, uint32_t Pins, uint32_t Mode, uint32_t Pull, uint32_t Speed, uint32_t Alternate) {

	uint32_t position = 0x00U;
	uint32_t iocurrent = 0;
	uint32_t temp = 0;
	EXTI_Core_TypeDef *EXTI_CurrentCPU;

	#if defined(DUAL_CORE) && defined(CORE_CM4)
		EXTI_CurrentCPU = EXTI_D2; // EXTI for CM4 CPU
	#else
		EXTI_CurrentCPU = EXTI_D1; // EXTI for CM7 CPU
	#endif

	// Configure the port pins
	while (((Pins) >> position) != 0x00U) {

		// Get current io position
	    iocurrent = (Pins) & (1UL << position);

	    if (iocurrent != 0x00U) {

	      // --------------------- GPIO Mode Configuration ------------------------
	      // In case of Output or Alternate function mode selection

	      if (((Mode & GPIO_MODE) == MODE_OUTPUT) || ((Mode & GPIO_MODE) == MODE_AF)) {

	    	  // Configure the IO Speed
	    	  temp = GPIOx->OSPEEDR;
	    	  temp &= ~(GPIO_OSPEEDR_OSPEED0 << (position * 2U));
	    	  temp |= (Speed << (position * 2U));
	    	  GPIOx->OSPEEDR = temp;

	    	  // Configure the IO Output Type
	    	  temp = GPIOx->OTYPER;
	    	  temp &= ~(GPIO_OTYPER_OT0 << position) ;
	    	  temp |= (((Mode & OUTPUT_TYPE) >> OUTPUT_TYPE_Pos) << position);
	    	  GPIOx->OTYPER = temp;
	      }

	      if ((Mode & GPIO_MODE) != MODE_ANALOG) {

	    	  // Activate the Pull-up or Pull down resistor for the current IO
	    	  temp = GPIOx->PUPDR;
	    	  temp &= ~(GPIO_PUPDR_PUPD0 << (position * 2U));
	    	  temp |= ((Pull) << (position * 2U));
	    	  GPIOx->PUPDR = temp;

	      }

	      // In case of Alternate function mode selection
	      if ((Mode & GPIO_MODE) == MODE_AF) {

	    	  // Configure Alternate function mapped with the current IO
	    	  temp = GPIOx->AFR[position >> 3U];
	    	  temp &= ~(0xFU << ((position & 0x07U) * 4U));
	    	  temp |= ((Alternate) << ((position & 0x07U) * 4U));
	    	  GPIOx->AFR[position >> 3U] = temp;

	      }

	      /* Configure IO Direction mode (Input, Output, Alternate or Analog) */
	      temp = GPIOx->MODER;
	      temp &= ~(GPIO_MODER_MODE0 << (position * 2U));
	      temp |= ((Mode & GPIO_MODE) << (position * 2U));
	      GPIOx->MODER = temp;

	      //--------------------- EXTI Mode Configuration ------------------------
	      // Configure the External Interrupt or event for the current IO

	      if ((Mode & EXTI_MODE) != 0x00U) {

	        temp = SYSCFG->EXTICR[position >> 2U];
	        temp &= ~(0x0FUL << (4U * (position & 0x03U)));
	        temp |= (GPIO_GET_INDEX(GPIOx) << (4U * (position & 0x03U)));
	        SYSCFG->EXTICR[position >> 2U] = temp;

	        // Clear Rising Falling edge configuration
	        temp = EXTI->RTSR1;
	        temp &= ~(iocurrent);
	        if ((Mode & TRIGGER_RISING) != 0x00U) temp |= iocurrent;
	        EXTI->RTSR1 = temp;

	        temp = EXTI->FTSR1;
	        temp &= ~(iocurrent);
	        if ((Mode & TRIGGER_FALLING) != 0x00U) temp |= iocurrent;
	        EXTI->FTSR1 = temp;

	        temp = EXTI_CurrentCPU->EMR1;
	        temp &= ~(iocurrent);
	        if ((Mode & EXTI_EVT) != 0x00U) temp |= iocurrent;
	        EXTI_CurrentCPU->EMR1 = temp;

	        // Clear EXTI line configuration
	        temp = EXTI_CurrentCPU->IMR1;
	        temp &= ~(iocurrent);
	        if ((Mode & EXTI_IT) != 0x00U) temp |= iocurrent;
	        EXTI_CurrentCPU->IMR1 = temp;
	      }
	    }

	    position++;
	  }

	return BSP_OK;
}


uint32_t BSP_STM32_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin) {
	return ((GPIOx->IDR & GPIO_Pin) > 0)?GPIO_PIN_SET:GPIO_PIN_RESET;
}

uint8_t BSP_STM32_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin, uint32_t PinState) {
	GPIOx->BSRR=((PinState==GPIO_PIN_SET)?GPIO_Pin:GPIO_Pin << GPIO_NUMBER);
	return BSP_OK;
}

uint8_t BSP_STM32_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin) {
	uint32_t odr = GPIOx->ODR;
	GPIOx->BSRR = ((odr & GPIO_Pin) << GPIO_NUMBER) | (~odr & GPIO_Pin);
	return BSP_OK;
}
