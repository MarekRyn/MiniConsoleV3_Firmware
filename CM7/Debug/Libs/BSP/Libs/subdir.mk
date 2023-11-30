################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs/graph2d.c \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs/hxcmod.c \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs/kalman.c 

OBJS += \
./Libs/BSP/Libs/graph2d.o \
./Libs/BSP/Libs/hxcmod.o \
./Libs/BSP/Libs/kalman.o 

C_DEPS += \
./Libs/BSP/Libs/graph2d.d \
./Libs/BSP/Libs/hxcmod.d \
./Libs/BSP/Libs/kalman.d 


# Each subdirectory must supply rules for building sources it contributes
Libs/BSP/Libs/graph2d.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs/graph2d.c Libs/BSP/Libs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDUAL_CORE -DCORE_CM7 -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Libs/BSP/Libs/hxcmod.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs/hxcmod.c Libs/BSP/Libs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDUAL_CORE -DCORE_CM7 -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Libs/BSP/Libs/kalman.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs/kalman.c Libs/BSP/Libs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDUAL_CORE -DCORE_CM7 -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Libs-2f-BSP-2f-Libs

clean-Libs-2f-BSP-2f-Libs:
	-$(RM) ./Libs/BSP/Libs/graph2d.cyclo ./Libs/BSP/Libs/graph2d.d ./Libs/BSP/Libs/graph2d.o ./Libs/BSP/Libs/graph2d.su ./Libs/BSP/Libs/hxcmod.cyclo ./Libs/BSP/Libs/hxcmod.d ./Libs/BSP/Libs/hxcmod.o ./Libs/BSP/Libs/hxcmod.su ./Libs/BSP/Libs/kalman.cyclo ./Libs/BSP/Libs/kalman.d ./Libs/BSP/Libs/kalman.o ./Libs/BSP/Libs/kalman.su

.PHONY: clean-Libs-2f-BSP-2f-Libs

