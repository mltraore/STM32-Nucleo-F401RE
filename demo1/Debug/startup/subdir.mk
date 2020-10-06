################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../startup/startup_stm32f401xx.s 

OBJS += \
./startup/startup_stm32f401xx.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo $(PWD)
	arm-none-eabi-as -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -I"/home/mohamedlassine/Desktop/stm32NucleOF401RE/demo1/StdPeriph_Driver/inc" -I"/home/mohamedlassine/Desktop/stm32NucleOF401RE/demo1/inc" -I"/home/mohamedlassine/Desktop/stm32NucleOF401RE/demo1/CMSIS/device" -I"/home/mohamedlassine/Desktop/stm32NucleOF401RE/demo1/CMSIS/core" -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


