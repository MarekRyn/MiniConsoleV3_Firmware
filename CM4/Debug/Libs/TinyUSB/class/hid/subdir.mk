################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB/class/hid/hid_device.c \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB/class/hid/hid_host.c 

OBJS += \
./Libs/TinyUSB/class/hid/hid_device.o \
./Libs/TinyUSB/class/hid/hid_host.o 

C_DEPS += \
./Libs/TinyUSB/class/hid/hid_device.d \
./Libs/TinyUSB/class/hid/hid_host.d 


# Each subdirectory must supply rules for building sources it contributes
Libs/TinyUSB/class/hid/hid_device.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB/class/hid/hid_device.c Libs/TinyUSB/class/hid/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DCORE_CM4 -DDUAL_CORE -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM4" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Libs/TinyUSB/class/hid/hid_host.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB/class/hid/hid_host.c Libs/TinyUSB/class/hid/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DCORE_CM4 -DDUAL_CORE -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM4" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Libs-2f-TinyUSB-2f-class-2f-hid

clean-Libs-2f-TinyUSB-2f-class-2f-hid:
	-$(RM) ./Libs/TinyUSB/class/hid/hid_device.cyclo ./Libs/TinyUSB/class/hid/hid_device.d ./Libs/TinyUSB/class/hid/hid_device.o ./Libs/TinyUSB/class/hid/hid_device.su ./Libs/TinyUSB/class/hid/hid_host.cyclo ./Libs/TinyUSB/class/hid/hid_host.d ./Libs/TinyUSB/class/hid/hid_host.o ./Libs/TinyUSB/class/hid/hid_host.su

.PHONY: clean-Libs-2f-TinyUSB-2f-class-2f-hid

