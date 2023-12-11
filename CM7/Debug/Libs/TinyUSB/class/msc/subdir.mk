################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB/class/msc/msc_device.c \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB/class/msc/msc_host.c 

OBJS += \
./Libs/TinyUSB/class/msc/msc_device.o \
./Libs/TinyUSB/class/msc/msc_host.o 

C_DEPS += \
./Libs/TinyUSB/class/msc/msc_device.d \
./Libs/TinyUSB/class/msc/msc_host.d 


# Each subdirectory must supply rules for building sources it contributes
Libs/TinyUSB/class/msc/msc_device.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB/class/msc/msc_device.c Libs/TinyUSB/class/msc/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDUAL_CORE -DCORE_CM7 -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Libs/TinyUSB/class/msc/msc_host.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB/class/msc/msc_host.c Libs/TinyUSB/class/msc/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDUAL_CORE -DCORE_CM7 -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Libs-2f-TinyUSB-2f-class-2f-msc

clean-Libs-2f-TinyUSB-2f-class-2f-msc:
	-$(RM) ./Libs/TinyUSB/class/msc/msc_device.cyclo ./Libs/TinyUSB/class/msc/msc_device.d ./Libs/TinyUSB/class/msc/msc_device.o ./Libs/TinyUSB/class/msc/msc_device.su ./Libs/TinyUSB/class/msc/msc_host.cyclo ./Libs/TinyUSB/class/msc/msc_host.d ./Libs/TinyUSB/class/msc/msc_host.o ./Libs/TinyUSB/class/msc/msc_host.su

.PHONY: clean-Libs-2f-TinyUSB-2f-class-2f-msc

