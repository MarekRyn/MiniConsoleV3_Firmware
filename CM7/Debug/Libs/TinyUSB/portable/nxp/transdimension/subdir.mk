################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB/portable/nxp/transdimension/dcd_transdimension.c \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB/portable/nxp/transdimension/hcd_transdimension.c 

OBJS += \
./Libs/TinyUSB/portable/nxp/transdimension/dcd_transdimension.o \
./Libs/TinyUSB/portable/nxp/transdimension/hcd_transdimension.o 

C_DEPS += \
./Libs/TinyUSB/portable/nxp/transdimension/dcd_transdimension.d \
./Libs/TinyUSB/portable/nxp/transdimension/hcd_transdimension.d 


# Each subdirectory must supply rules for building sources it contributes
Libs/TinyUSB/portable/nxp/transdimension/dcd_transdimension.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB/portable/nxp/transdimension/dcd_transdimension.c Libs/TinyUSB/portable/nxp/transdimension/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DCORE_CM7 -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Libs/TinyUSB/portable/nxp/transdimension/hcd_transdimension.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB/portable/nxp/transdimension/hcd_transdimension.c Libs/TinyUSB/portable/nxp/transdimension/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DCORE_CM7 -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Libs-2f-TinyUSB-2f-portable-2f-nxp-2f-transdimension

clean-Libs-2f-TinyUSB-2f-portable-2f-nxp-2f-transdimension:
	-$(RM) ./Libs/TinyUSB/portable/nxp/transdimension/dcd_transdimension.cyclo ./Libs/TinyUSB/portable/nxp/transdimension/dcd_transdimension.d ./Libs/TinyUSB/portable/nxp/transdimension/dcd_transdimension.o ./Libs/TinyUSB/portable/nxp/transdimension/dcd_transdimension.su ./Libs/TinyUSB/portable/nxp/transdimension/hcd_transdimension.cyclo ./Libs/TinyUSB/portable/nxp/transdimension/hcd_transdimension.d ./Libs/TinyUSB/portable/nxp/transdimension/hcd_transdimension.o ./Libs/TinyUSB/portable/nxp/transdimension/hcd_transdimension.su

.PHONY: clean-Libs-2f-TinyUSB-2f-portable-2f-nxp-2f-transdimension

