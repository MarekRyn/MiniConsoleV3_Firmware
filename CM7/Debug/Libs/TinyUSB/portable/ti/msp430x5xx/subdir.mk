################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB/portable/ti/msp430x5xx/dcd_msp430x5xx.c 

OBJS += \
./Libs/TinyUSB/portable/ti/msp430x5xx/dcd_msp430x5xx.o 

C_DEPS += \
./Libs/TinyUSB/portable/ti/msp430x5xx/dcd_msp430x5xx.d 


# Each subdirectory must supply rules for building sources it contributes
Libs/TinyUSB/portable/ti/msp430x5xx/dcd_msp430x5xx.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB/portable/ti/msp430x5xx/dcd_msp430x5xx.c Libs/TinyUSB/portable/ti/msp430x5xx/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDUAL_CORE -DCORE_CM7 -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Libs-2f-TinyUSB-2f-portable-2f-ti-2f-msp430x5xx

clean-Libs-2f-TinyUSB-2f-portable-2f-ti-2f-msp430x5xx:
	-$(RM) ./Libs/TinyUSB/portable/ti/msp430x5xx/dcd_msp430x5xx.cyclo ./Libs/TinyUSB/portable/ti/msp430x5xx/dcd_msp430x5xx.d ./Libs/TinyUSB/portable/ti/msp430x5xx/dcd_msp430x5xx.o ./Libs/TinyUSB/portable/ti/msp430x5xx/dcd_msp430x5xx.su

.PHONY: clean-Libs-2f-TinyUSB-2f-portable-2f-ti-2f-msp430x5xx

