################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS/BSP_diskio.c \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS/BSP_fatfs.c \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS/BSP_ff_gen_drv.c \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS/BSP_sd_diskio.c \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS/ff.c \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS/ffsystem.c \
C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS/ffunicode.c 

OBJS += \
./Libs/FATFS/BSP_diskio.o \
./Libs/FATFS/BSP_fatfs.o \
./Libs/FATFS/BSP_ff_gen_drv.o \
./Libs/FATFS/BSP_sd_diskio.o \
./Libs/FATFS/ff.o \
./Libs/FATFS/ffsystem.o \
./Libs/FATFS/ffunicode.o 

C_DEPS += \
./Libs/FATFS/BSP_diskio.d \
./Libs/FATFS/BSP_fatfs.d \
./Libs/FATFS/BSP_ff_gen_drv.d \
./Libs/FATFS/BSP_sd_diskio.d \
./Libs/FATFS/ff.d \
./Libs/FATFS/ffsystem.d \
./Libs/FATFS/ffunicode.d 


# Each subdirectory must supply rules for building sources it contributes
Libs/FATFS/BSP_diskio.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS/BSP_diskio.c Libs/FATFS/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DCORE_CM7 -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Libs/FATFS/BSP_fatfs.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS/BSP_fatfs.c Libs/FATFS/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DCORE_CM7 -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Libs/FATFS/BSP_ff_gen_drv.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS/BSP_ff_gen_drv.c Libs/FATFS/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DCORE_CM7 -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Libs/FATFS/BSP_sd_diskio.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS/BSP_sd_diskio.c Libs/FATFS/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DCORE_CM7 -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Libs/FATFS/ff.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS/ff.c Libs/FATFS/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DCORE_CM7 -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Libs/FATFS/ffsystem.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS/ffsystem.c Libs/FATFS/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DCORE_CM7 -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Libs/FATFS/ffunicode.o: C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS/ffunicode.c Libs/FATFS/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DCORE_CM7 -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Libs-2f-FATFS

clean-Libs-2f-FATFS:
	-$(RM) ./Libs/FATFS/BSP_diskio.cyclo ./Libs/FATFS/BSP_diskio.d ./Libs/FATFS/BSP_diskio.o ./Libs/FATFS/BSP_diskio.su ./Libs/FATFS/BSP_fatfs.cyclo ./Libs/FATFS/BSP_fatfs.d ./Libs/FATFS/BSP_fatfs.o ./Libs/FATFS/BSP_fatfs.su ./Libs/FATFS/BSP_ff_gen_drv.cyclo ./Libs/FATFS/BSP_ff_gen_drv.d ./Libs/FATFS/BSP_ff_gen_drv.o ./Libs/FATFS/BSP_ff_gen_drv.su ./Libs/FATFS/BSP_sd_diskio.cyclo ./Libs/FATFS/BSP_sd_diskio.d ./Libs/FATFS/BSP_sd_diskio.o ./Libs/FATFS/BSP_sd_diskio.su ./Libs/FATFS/ff.cyclo ./Libs/FATFS/ff.d ./Libs/FATFS/ff.o ./Libs/FATFS/ff.su ./Libs/FATFS/ffsystem.cyclo ./Libs/FATFS/ffsystem.d ./Libs/FATFS/ffsystem.o ./Libs/FATFS/ffsystem.su ./Libs/FATFS/ffunicode.cyclo ./Libs/FATFS/ffunicode.d ./Libs/FATFS/ffunicode.o ./Libs/FATFS/ffunicode.su

.PHONY: clean-Libs-2f-FATFS

