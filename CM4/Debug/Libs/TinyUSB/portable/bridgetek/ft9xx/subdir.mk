################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB/portable/bridgetek/ft9xx/dcd_ft9xx.c 

OBJS += \
./Libs/TinyUSB/portable/bridgetek/ft9xx/dcd_ft9xx.o 

C_DEPS += \
./Libs/TinyUSB/portable/bridgetek/ft9xx/dcd_ft9xx.d 


# Each subdirectory must supply rules for building sources it contributes
Libs/TinyUSB/portable/bridgetek/ft9xx/dcd_ft9xx.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB/portable/bridgetek/ft9xx/dcd_ft9xx.c Libs/TinyUSB/portable/bridgetek/ft9xx/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DCORE_CM4 -DDUAL_CORE -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM4" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Libs-2f-TinyUSB-2f-portable-2f-bridgetek-2f-ft9xx

clean-Libs-2f-TinyUSB-2f-portable-2f-bridgetek-2f-ft9xx:
	-$(RM) ./Libs/TinyUSB/portable/bridgetek/ft9xx/dcd_ft9xx.cyclo ./Libs/TinyUSB/portable/bridgetek/ft9xx/dcd_ft9xx.d ./Libs/TinyUSB/portable/bridgetek/ft9xx/dcd_ft9xx.o ./Libs/TinyUSB/portable/bridgetek/ft9xx/dcd_ft9xx.su

.PHONY: clean-Libs-2f-TinyUSB-2f-portable-2f-bridgetek-2f-ft9xx

