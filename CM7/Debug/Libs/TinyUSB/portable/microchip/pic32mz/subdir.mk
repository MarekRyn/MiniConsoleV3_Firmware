################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB/portable/microchip/pic32mz/dcd_pic32mz.c 

OBJS += \
./Libs/TinyUSB/portable/microchip/pic32mz/dcd_pic32mz.o 

C_DEPS += \
./Libs/TinyUSB/portable/microchip/pic32mz/dcd_pic32mz.d 


# Each subdirectory must supply rules for building sources it contributes
Libs/TinyUSB/portable/microchip/pic32mz/dcd_pic32mz.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB/portable/microchip/pic32mz/dcd_pic32mz.c Libs/TinyUSB/portable/microchip/pic32mz/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDUAL_CORE -DCORE_CM7 -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Libs-2f-TinyUSB-2f-portable-2f-microchip-2f-pic32mz

clean-Libs-2f-TinyUSB-2f-portable-2f-microchip-2f-pic32mz:
	-$(RM) ./Libs/TinyUSB/portable/microchip/pic32mz/dcd_pic32mz.cyclo ./Libs/TinyUSB/portable/microchip/pic32mz/dcd_pic32mz.d ./Libs/TinyUSB/portable/microchip/pic32mz/dcd_pic32mz.o ./Libs/TinyUSB/portable/microchip/pic32mz/dcd_pic32mz.su

.PHONY: clean-Libs-2f-TinyUSB-2f-portable-2f-microchip-2f-pic32mz

