################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM4/BSP.c \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM4/BSP_Audio.c \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM4/BSP_Audio_Tables.c \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM4/BSP_STM32.c 

OBJS += \
./Libs/BSP/Core_CM4/BSP.o \
./Libs/BSP/Core_CM4/BSP_Audio.o \
./Libs/BSP/Core_CM4/BSP_Audio_Tables.o \
./Libs/BSP/Core_CM4/BSP_STM32.o 

C_DEPS += \
./Libs/BSP/Core_CM4/BSP.d \
./Libs/BSP/Core_CM4/BSP_Audio.d \
./Libs/BSP/Core_CM4/BSP_Audio_Tables.d \
./Libs/BSP/Core_CM4/BSP_STM32.d 


# Each subdirectory must supply rules for building sources it contributes
Libs/BSP/Core_CM4/BSP.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM4/BSP.c Libs/BSP/Core_CM4/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DCORE_CM4 -DDUAL_CORE -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM4" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Libs/BSP/Core_CM4/BSP_Audio.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM4/BSP_Audio.c Libs/BSP/Core_CM4/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DCORE_CM4 -DDUAL_CORE -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM4" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Libs/BSP/Core_CM4/BSP_Audio_Tables.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM4/BSP_Audio_Tables.c Libs/BSP/Core_CM4/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DCORE_CM4 -DDUAL_CORE -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM4" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Libs/BSP/Core_CM4/BSP_STM32.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM4/BSP_STM32.c Libs/BSP/Core_CM4/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DCORE_CM4 -DDUAL_CORE -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM4" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Libs-2f-BSP-2f-Core_CM4

clean-Libs-2f-BSP-2f-Core_CM4:
	-$(RM) ./Libs/BSP/Core_CM4/BSP.cyclo ./Libs/BSP/Core_CM4/BSP.d ./Libs/BSP/Core_CM4/BSP.o ./Libs/BSP/Core_CM4/BSP.su ./Libs/BSP/Core_CM4/BSP_Audio.cyclo ./Libs/BSP/Core_CM4/BSP_Audio.d ./Libs/BSP/Core_CM4/BSP_Audio.o ./Libs/BSP/Core_CM4/BSP_Audio.su ./Libs/BSP/Core_CM4/BSP_Audio_Tables.cyclo ./Libs/BSP/Core_CM4/BSP_Audio_Tables.d ./Libs/BSP/Core_CM4/BSP_Audio_Tables.o ./Libs/BSP/Core_CM4/BSP_Audio_Tables.su ./Libs/BSP/Core_CM4/BSP_STM32.cyclo ./Libs/BSP/Core_CM4/BSP_STM32.d ./Libs/BSP/Core_CM4/BSP_STM32.o ./Libs/BSP/Core_CM4/BSP_STM32.su

.PHONY: clean-Libs-2f-BSP-2f-Core_CM4

