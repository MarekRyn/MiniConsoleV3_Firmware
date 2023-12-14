################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/gui.c \
../Src/main.c \
../Src/page_apps.c \
../Src/page_audio.c \
../Src/page_bt.c \
../Src/page_imu.c \
../Src/page_inputs.c \
../Src/page_main.c \
../Src/page_screen.c \
../Src/syscalls.c \
../Src/sysmem.c 

OBJS += \
./Src/gui.o \
./Src/main.o \
./Src/page_apps.o \
./Src/page_audio.o \
./Src/page_bt.o \
./Src/page_imu.o \
./Src/page_inputs.o \
./Src/page_main.o \
./Src/page_screen.o \
./Src/syscalls.o \
./Src/sysmem.o 

C_DEPS += \
./Src/gui.d \
./Src/main.d \
./Src/page_apps.d \
./Src/page_audio.d \
./Src/page_bt.d \
./Src/page_imu.d \
./Src/page_inputs.d \
./Src/page_main.d \
./Src/page_screen.d \
./Src/syscalls.d \
./Src/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DCORE_CM7 -DSTM32H7 -DSTM32H747xx -c -I../Inc -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/CMSIS" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/ST" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Components" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Core_CM7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/Libs" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/BSP/STM32H7" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/TinyUSB" -I"C:/Projects/STM32/Mini-Console-3-Firmware/Libs/FATFS" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/gui.cyclo ./Src/gui.d ./Src/gui.o ./Src/gui.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/page_apps.cyclo ./Src/page_apps.d ./Src/page_apps.o ./Src/page_apps.su ./Src/page_audio.cyclo ./Src/page_audio.d ./Src/page_audio.o ./Src/page_audio.su ./Src/page_bt.cyclo ./Src/page_bt.d ./Src/page_bt.o ./Src/page_bt.su ./Src/page_imu.cyclo ./Src/page_imu.d ./Src/page_imu.o ./Src/page_imu.su ./Src/page_inputs.cyclo ./Src/page_inputs.d ./Src/page_inputs.o ./Src/page_inputs.su ./Src/page_main.cyclo ./Src/page_main.d ./Src/page_main.o ./Src/page_main.su ./Src/page_screen.cyclo ./Src/page_screen.d ./Src/page_screen.o ./Src/page_screen.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su

.PHONY: clean-Src

