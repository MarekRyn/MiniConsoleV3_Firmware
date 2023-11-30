################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB/portable/raspberrypi/pio_usb/dcd_pio_usb.c \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB/portable/raspberrypi/pio_usb/hcd_pio_usb.c 

OBJS += \
./Libs/TinyUSB/portable/raspberrypi/pio_usb/dcd_pio_usb.o \
./Libs/TinyUSB/portable/raspberrypi/pio_usb/hcd_pio_usb.o 

C_DEPS += \
./Libs/TinyUSB/portable/raspberrypi/pio_usb/dcd_pio_usb.d \
./Libs/TinyUSB/portable/raspberrypi/pio_usb/hcd_pio_usb.d 


# Each subdirectory must supply rules for building sources it contributes
Libs/TinyUSB/portable/raspberrypi/pio_usb/dcd_pio_usb.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB/portable/raspberrypi/pio_usb/dcd_pio_usb.c Libs/TinyUSB/portable/raspberrypi/pio_usb/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDUAL_CORE -DCORE_CM7 -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Libs/TinyUSB/portable/raspberrypi/pio_usb/hcd_pio_usb.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB/portable/raspberrypi/pio_usb/hcd_pio_usb.c Libs/TinyUSB/portable/raspberrypi/pio_usb/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDUAL_CORE -DCORE_CM7 -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Libs-2f-TinyUSB-2f-portable-2f-raspberrypi-2f-pio_usb

clean-Libs-2f-TinyUSB-2f-portable-2f-raspberrypi-2f-pio_usb:
	-$(RM) ./Libs/TinyUSB/portable/raspberrypi/pio_usb/dcd_pio_usb.cyclo ./Libs/TinyUSB/portable/raspberrypi/pio_usb/dcd_pio_usb.d ./Libs/TinyUSB/portable/raspberrypi/pio_usb/dcd_pio_usb.o ./Libs/TinyUSB/portable/raspberrypi/pio_usb/dcd_pio_usb.su ./Libs/TinyUSB/portable/raspberrypi/pio_usb/hcd_pio_usb.cyclo ./Libs/TinyUSB/portable/raspberrypi/pio_usb/hcd_pio_usb.d ./Libs/TinyUSB/portable/raspberrypi/pio_usb/hcd_pio_usb.o ./Libs/TinyUSB/portable/raspberrypi/pio_usb/hcd_pio_usb.su

.PHONY: clean-Libs-2f-TinyUSB-2f-portable-2f-raspberrypi-2f-pio_usb

