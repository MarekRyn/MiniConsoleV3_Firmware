################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7/BSP.c \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7/BSP_Audio.c \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7/BSP_Data.c \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7/BSP_Fonts.c \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7/BSP_IMU.c \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7/BSP_Inputs.c \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7/BSP_LCD.c \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7/BSP_LCD_TP.c \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7/BSP_QSPI.c \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7/BSP_Resources.c \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7/BSP_SDCARD.c \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7/BSP_SDRAM.c \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7/BSP_STM32.c \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7/BSP_Serial.c \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7/BSP_USB.c 

OBJS += \
./Libs/BSP/Core_CM7/BSP.o \
./Libs/BSP/Core_CM7/BSP_Audio.o \
./Libs/BSP/Core_CM7/BSP_Data.o \
./Libs/BSP/Core_CM7/BSP_Fonts.o \
./Libs/BSP/Core_CM7/BSP_IMU.o \
./Libs/BSP/Core_CM7/BSP_Inputs.o \
./Libs/BSP/Core_CM7/BSP_LCD.o \
./Libs/BSP/Core_CM7/BSP_LCD_TP.o \
./Libs/BSP/Core_CM7/BSP_QSPI.o \
./Libs/BSP/Core_CM7/BSP_Resources.o \
./Libs/BSP/Core_CM7/BSP_SDCARD.o \
./Libs/BSP/Core_CM7/BSP_SDRAM.o \
./Libs/BSP/Core_CM7/BSP_STM32.o \
./Libs/BSP/Core_CM7/BSP_Serial.o \
./Libs/BSP/Core_CM7/BSP_USB.o 

C_DEPS += \
./Libs/BSP/Core_CM7/BSP.d \
./Libs/BSP/Core_CM7/BSP_Audio.d \
./Libs/BSP/Core_CM7/BSP_Data.d \
./Libs/BSP/Core_CM7/BSP_Fonts.d \
./Libs/BSP/Core_CM7/BSP_IMU.d \
./Libs/BSP/Core_CM7/BSP_Inputs.d \
./Libs/BSP/Core_CM7/BSP_LCD.d \
./Libs/BSP/Core_CM7/BSP_LCD_TP.d \
./Libs/BSP/Core_CM7/BSP_QSPI.d \
./Libs/BSP/Core_CM7/BSP_Resources.d \
./Libs/BSP/Core_CM7/BSP_SDCARD.d \
./Libs/BSP/Core_CM7/BSP_SDRAM.d \
./Libs/BSP/Core_CM7/BSP_STM32.d \
./Libs/BSP/Core_CM7/BSP_Serial.d \
./Libs/BSP/Core_CM7/BSP_USB.d 


# Each subdirectory must supply rules for building sources it contributes
Libs/BSP/Core_CM7/BSP.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7/BSP.c Libs/BSP/Core_CM7/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDUAL_CORE -DCORE_CM7 -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Libs/BSP/Core_CM7/BSP_Audio.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7/BSP_Audio.c Libs/BSP/Core_CM7/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDUAL_CORE -DCORE_CM7 -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Libs/BSP/Core_CM7/BSP_Data.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7/BSP_Data.c Libs/BSP/Core_CM7/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDUAL_CORE -DCORE_CM7 -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Libs/BSP/Core_CM7/BSP_Fonts.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7/BSP_Fonts.c Libs/BSP/Core_CM7/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDUAL_CORE -DCORE_CM7 -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Libs/BSP/Core_CM7/BSP_IMU.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7/BSP_IMU.c Libs/BSP/Core_CM7/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDUAL_CORE -DCORE_CM7 -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Libs/BSP/Core_CM7/BSP_Inputs.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7/BSP_Inputs.c Libs/BSP/Core_CM7/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDUAL_CORE -DCORE_CM7 -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Libs/BSP/Core_CM7/BSP_LCD.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7/BSP_LCD.c Libs/BSP/Core_CM7/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDUAL_CORE -DCORE_CM7 -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Libs/BSP/Core_CM7/BSP_LCD_TP.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7/BSP_LCD_TP.c Libs/BSP/Core_CM7/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDUAL_CORE -DCORE_CM7 -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Libs/BSP/Core_CM7/BSP_QSPI.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7/BSP_QSPI.c Libs/BSP/Core_CM7/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDUAL_CORE -DCORE_CM7 -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Libs/BSP/Core_CM7/BSP_Resources.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7/BSP_Resources.c Libs/BSP/Core_CM7/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDUAL_CORE -DCORE_CM7 -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Libs/BSP/Core_CM7/BSP_SDCARD.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7/BSP_SDCARD.c Libs/BSP/Core_CM7/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDUAL_CORE -DCORE_CM7 -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Libs/BSP/Core_CM7/BSP_SDRAM.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7/BSP_SDRAM.c Libs/BSP/Core_CM7/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDUAL_CORE -DCORE_CM7 -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Libs/BSP/Core_CM7/BSP_STM32.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7/BSP_STM32.c Libs/BSP/Core_CM7/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDUAL_CORE -DCORE_CM7 -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Libs/BSP/Core_CM7/BSP_Serial.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7/BSP_Serial.c Libs/BSP/Core_CM7/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDUAL_CORE -DCORE_CM7 -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Libs/BSP/Core_CM7/BSP_USB.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7/BSP_USB.c Libs/BSP/Core_CM7/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDUAL_CORE -DCORE_CM7 -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Libs-2f-BSP-2f-Core_CM7

clean-Libs-2f-BSP-2f-Core_CM7:
	-$(RM) ./Libs/BSP/Core_CM7/BSP.cyclo ./Libs/BSP/Core_CM7/BSP.d ./Libs/BSP/Core_CM7/BSP.o ./Libs/BSP/Core_CM7/BSP.su ./Libs/BSP/Core_CM7/BSP_Audio.cyclo ./Libs/BSP/Core_CM7/BSP_Audio.d ./Libs/BSP/Core_CM7/BSP_Audio.o ./Libs/BSP/Core_CM7/BSP_Audio.su ./Libs/BSP/Core_CM7/BSP_Data.cyclo ./Libs/BSP/Core_CM7/BSP_Data.d ./Libs/BSP/Core_CM7/BSP_Data.o ./Libs/BSP/Core_CM7/BSP_Data.su ./Libs/BSP/Core_CM7/BSP_Fonts.cyclo ./Libs/BSP/Core_CM7/BSP_Fonts.d ./Libs/BSP/Core_CM7/BSP_Fonts.o ./Libs/BSP/Core_CM7/BSP_Fonts.su ./Libs/BSP/Core_CM7/BSP_IMU.cyclo ./Libs/BSP/Core_CM7/BSP_IMU.d ./Libs/BSP/Core_CM7/BSP_IMU.o ./Libs/BSP/Core_CM7/BSP_IMU.su ./Libs/BSP/Core_CM7/BSP_Inputs.cyclo ./Libs/BSP/Core_CM7/BSP_Inputs.d ./Libs/BSP/Core_CM7/BSP_Inputs.o ./Libs/BSP/Core_CM7/BSP_Inputs.su ./Libs/BSP/Core_CM7/BSP_LCD.cyclo ./Libs/BSP/Core_CM7/BSP_LCD.d ./Libs/BSP/Core_CM7/BSP_LCD.o ./Libs/BSP/Core_CM7/BSP_LCD.su ./Libs/BSP/Core_CM7/BSP_LCD_TP.cyclo ./Libs/BSP/Core_CM7/BSP_LCD_TP.d ./Libs/BSP/Core_CM7/BSP_LCD_TP.o ./Libs/BSP/Core_CM7/BSP_LCD_TP.su ./Libs/BSP/Core_CM7/BSP_QSPI.cyclo ./Libs/BSP/Core_CM7/BSP_QSPI.d ./Libs/BSP/Core_CM7/BSP_QSPI.o ./Libs/BSP/Core_CM7/BSP_QSPI.su ./Libs/BSP/Core_CM7/BSP_Resources.cyclo ./Libs/BSP/Core_CM7/BSP_Resources.d ./Libs/BSP/Core_CM7/BSP_Resources.o ./Libs/BSP/Core_CM7/BSP_Resources.su ./Libs/BSP/Core_CM7/BSP_SDCARD.cyclo ./Libs/BSP/Core_CM7/BSP_SDCARD.d ./Libs/BSP/Core_CM7/BSP_SDCARD.o ./Libs/BSP/Core_CM7/BSP_SDCARD.su ./Libs/BSP/Core_CM7/BSP_SDRAM.cyclo ./Libs/BSP/Core_CM7/BSP_SDRAM.d ./Libs/BSP/Core_CM7/BSP_SDRAM.o ./Libs/BSP/Core_CM7/BSP_SDRAM.su ./Libs/BSP/Core_CM7/BSP_STM32.cyclo ./Libs/BSP/Core_CM7/BSP_STM32.d ./Libs/BSP/Core_CM7/BSP_STM32.o ./Libs/BSP/Core_CM7/BSP_STM32.su ./Libs/BSP/Core_CM7/BSP_Serial.cyclo ./Libs/BSP/Core_CM7/BSP_Serial.d ./Libs/BSP/Core_CM7/BSP_Serial.o ./Libs/BSP/Core_CM7/BSP_Serial.su ./Libs/BSP/Core_CM7/BSP_USB.cyclo ./Libs/BSP/Core_CM7/BSP_USB.d ./Libs/BSP/Core_CM7/BSP_USB.o ./Libs/BSP/Core_CM7/BSP_USB.su

.PHONY: clean-Libs-2f-BSP-2f-Core_CM7

