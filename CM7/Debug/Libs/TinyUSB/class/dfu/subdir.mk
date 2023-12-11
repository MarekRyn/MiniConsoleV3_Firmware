################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB/class/dfu/dfu_device.c \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB/class/dfu/dfu_rt_device.c 

OBJS += \
./Libs/TinyUSB/class/dfu/dfu_device.o \
./Libs/TinyUSB/class/dfu/dfu_rt_device.o 

C_DEPS += \
./Libs/TinyUSB/class/dfu/dfu_device.d \
./Libs/TinyUSB/class/dfu/dfu_rt_device.d 


# Each subdirectory must supply rules for building sources it contributes
Libs/TinyUSB/class/dfu/dfu_device.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB/class/dfu/dfu_device.c Libs/TinyUSB/class/dfu/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDUAL_CORE -DCORE_CM7 -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Libs/TinyUSB/class/dfu/dfu_rt_device.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB/class/dfu/dfu_rt_device.c Libs/TinyUSB/class/dfu/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDUAL_CORE -DCORE_CM7 -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Libs-2f-TinyUSB-2f-class-2f-dfu

clean-Libs-2f-TinyUSB-2f-class-2f-dfu:
	-$(RM) ./Libs/TinyUSB/class/dfu/dfu_device.cyclo ./Libs/TinyUSB/class/dfu/dfu_device.d ./Libs/TinyUSB/class/dfu/dfu_device.o ./Libs/TinyUSB/class/dfu/dfu_device.su ./Libs/TinyUSB/class/dfu/dfu_rt_device.cyclo ./Libs/TinyUSB/class/dfu/dfu_rt_device.d ./Libs/TinyUSB/class/dfu/dfu_rt_device.o ./Libs/TinyUSB/class/dfu/dfu_rt_device.su

.PHONY: clean-Libs-2f-TinyUSB-2f-class-2f-dfu

