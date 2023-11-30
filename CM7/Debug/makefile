################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include Libs/TinyUSB/portable/wch/ch32v307/subdir.mk
-include Libs/TinyUSB/portable/valentyusb/eptri/subdir.mk
-include Libs/TinyUSB/portable/ti/msp430x5xx/subdir.mk
-include Libs/TinyUSB/portable/template/subdir.mk
-include Libs/TinyUSB/portable/synopsys/dwc2/subdir.mk
-include Libs/TinyUSB/portable/sunxi/subdir.mk
-include Libs/TinyUSB/portable/st/stm32_fsdev/subdir.mk
-include Libs/TinyUSB/portable/sony/cxd56/subdir.mk
-include Libs/TinyUSB/portable/renesas/rusb2/subdir.mk
-include Libs/TinyUSB/portable/raspberrypi/rp2040/subdir.mk
-include Libs/TinyUSB/portable/raspberrypi/pio_usb/subdir.mk
-include Libs/TinyUSB/portable/ohci/subdir.mk
-include Libs/TinyUSB/portable/nxp/transdimension/subdir.mk
-include Libs/TinyUSB/portable/nxp/lpc_ip3511/subdir.mk
-include Libs/TinyUSB/portable/nxp/lpc17_40/subdir.mk
-include Libs/TinyUSB/portable/nxp/khci/subdir.mk
-include Libs/TinyUSB/portable/nuvoton/nuc505/subdir.mk
-include Libs/TinyUSB/portable/nuvoton/nuc121/subdir.mk
-include Libs/TinyUSB/portable/nuvoton/nuc120/subdir.mk
-include Libs/TinyUSB/portable/nordic/nrf5x/subdir.mk
-include Libs/TinyUSB/portable/mindmotion/mm32/subdir.mk
-include Libs/TinyUSB/portable/microchip/samx7x/subdir.mk
-include Libs/TinyUSB/portable/microchip/samg/subdir.mk
-include Libs/TinyUSB/portable/microchip/samd/subdir.mk
-include Libs/TinyUSB/portable/microchip/pic32mz/subdir.mk
-include Libs/TinyUSB/portable/microchip/pic/subdir.mk
-include Libs/TinyUSB/portable/mentor/musb/subdir.mk
-include Libs/TinyUSB/portable/espressif/esp32sx/subdir.mk
-include Libs/TinyUSB/portable/ehci/subdir.mk
-include Libs/TinyUSB/portable/dialog/da146xx/subdir.mk
-include Libs/TinyUSB/portable/chipidea/ci_hs/subdir.mk
-include Libs/TinyUSB/portable/bridgetek/ft9xx/subdir.mk
-include Libs/TinyUSB/host/subdir.mk
-include Libs/TinyUSB/device/subdir.mk
-include Libs/TinyUSB/common/subdir.mk
-include Libs/TinyUSB/class/video/subdir.mk
-include Libs/TinyUSB/class/vendor/subdir.mk
-include Libs/TinyUSB/class/usbtmc/subdir.mk
-include Libs/TinyUSB/class/net/subdir.mk
-include Libs/TinyUSB/class/msc/subdir.mk
-include Libs/TinyUSB/class/midi/subdir.mk
-include Libs/TinyUSB/class/hid/subdir.mk
-include Libs/TinyUSB/class/dfu/subdir.mk
-include Libs/TinyUSB/class/cdc/subdir.mk
-include Libs/TinyUSB/class/bth/subdir.mk
-include Libs/TinyUSB/class/audio/subdir.mk
-include Libs/TinyUSB/subdir.mk
-include Libs/ST/subdir.mk
-include Libs/FATFS/subdir.mk
-include Libs/BSP/STM32H7/subdir.mk
-include Libs/BSP/Libs/subdir.mk
-include Libs/BSP/Core_CM7/subdir.mk
-include Libs/BSP/Components/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := Mini-Console-3-Firmware_CM7
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Mini-Console-3-Firmware_CM7.elf \

MAP_FILES += \
Mini-Console-3-Firmware_CM7.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Mini-Console-3-Firmware_CM7.list \


# All Target
all: main-build

# Main-build Target
main-build: Mini-Console-3-Firmware_CM7.elf secondary-outputs

# Tool invocations
Mini-Console-3-Firmware_CM7.elf Mini-Console-3-Firmware_CM7.map: $(OBJS) $(USER_OBJS) C:\Projects\STM32\Mini-Console-3-Firmware\CM7\STM32H747BGTX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "Mini-Console-3-Firmware_CM7.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m7 -T"C:\Projects\STM32\Mini-Console-3-Firmware\CM7\STM32H747BGTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Mini-Console-3-Firmware_CM7.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Mini-Console-3-Firmware_CM7.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Mini-Console-3-Firmware_CM7.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) Mini-Console-3-Firmware_CM7.elf Mini-Console-3-Firmware_CM7.list Mini-Console-3-Firmware_CM7.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
