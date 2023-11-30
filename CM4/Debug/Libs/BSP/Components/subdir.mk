################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components/IS25LP128F.c \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components/IS42S32160F.c \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components/MAX98357A.c \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components/MDBT42Q512KV2.c \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components/MPU6050.c \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components/RVT50AQTNWC00.c 

OBJS += \
./Libs/BSP/Components/IS25LP128F.o \
./Libs/BSP/Components/IS42S32160F.o \
./Libs/BSP/Components/MAX98357A.o \
./Libs/BSP/Components/MDBT42Q512KV2.o \
./Libs/BSP/Components/MPU6050.o \
./Libs/BSP/Components/RVT50AQTNWC00.o 

C_DEPS += \
./Libs/BSP/Components/IS25LP128F.d \
./Libs/BSP/Components/IS42S32160F.d \
./Libs/BSP/Components/MAX98357A.d \
./Libs/BSP/Components/MDBT42Q512KV2.d \
./Libs/BSP/Components/MPU6050.d \
./Libs/BSP/Components/RVT50AQTNWC00.d 


# Each subdirectory must supply rules for building sources it contributes
Libs/BSP/Components/IS25LP128F.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components/IS25LP128F.c Libs/BSP/Components/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DCORE_CM4 -DDUAL_CORE -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM4" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Libs/BSP/Components/IS42S32160F.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components/IS42S32160F.c Libs/BSP/Components/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DCORE_CM4 -DDUAL_CORE -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM4" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Libs/BSP/Components/MAX98357A.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components/MAX98357A.c Libs/BSP/Components/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DCORE_CM4 -DDUAL_CORE -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM4" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Libs/BSP/Components/MDBT42Q512KV2.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components/MDBT42Q512KV2.c Libs/BSP/Components/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DCORE_CM4 -DDUAL_CORE -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM4" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Libs/BSP/Components/MPU6050.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components/MPU6050.c Libs/BSP/Components/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DCORE_CM4 -DDUAL_CORE -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM4" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Libs/BSP/Components/RVT50AQTNWC00.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components/RVT50AQTNWC00.c Libs/BSP/Components/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DCORE_CM4 -DDUAL_CORE -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM4" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Libs-2f-BSP-2f-Components

clean-Libs-2f-BSP-2f-Components:
	-$(RM) ./Libs/BSP/Components/IS25LP128F.cyclo ./Libs/BSP/Components/IS25LP128F.d ./Libs/BSP/Components/IS25LP128F.o ./Libs/BSP/Components/IS25LP128F.su ./Libs/BSP/Components/IS42S32160F.cyclo ./Libs/BSP/Components/IS42S32160F.d ./Libs/BSP/Components/IS42S32160F.o ./Libs/BSP/Components/IS42S32160F.su ./Libs/BSP/Components/MAX98357A.cyclo ./Libs/BSP/Components/MAX98357A.d ./Libs/BSP/Components/MAX98357A.o ./Libs/BSP/Components/MAX98357A.su ./Libs/BSP/Components/MDBT42Q512KV2.cyclo ./Libs/BSP/Components/MDBT42Q512KV2.d ./Libs/BSP/Components/MDBT42Q512KV2.o ./Libs/BSP/Components/MDBT42Q512KV2.su ./Libs/BSP/Components/MPU6050.cyclo ./Libs/BSP/Components/MPU6050.d ./Libs/BSP/Components/MPU6050.o ./Libs/BSP/Components/MPU6050.su ./Libs/BSP/Components/RVT50AQTNWC00.cyclo ./Libs/BSP/Components/RVT50AQTNWC00.d ./Libs/BSP/Components/RVT50AQTNWC00.o ./Libs/BSP/Components/RVT50AQTNWC00.su

.PHONY: clean-Libs-2f-BSP-2f-Components

