################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/Bootloader_V2.c \
../source/flash.c \
../source/gpio.c \
../source/mtb.c \
../source/port.c \
../source/semihost_hardfault.c \
../source/srec.c \
../source/uart.c 

C_DEPS += \
./source/Bootloader_V2.d \
./source/flash.d \
./source/gpio.d \
./source/mtb.d \
./source/port.d \
./source/semihost_hardfault.d \
./source/srec.d \
./source/uart.d 

OBJS += \
./source/Bootloader_V2.o \
./source/flash.o \
./source/gpio.o \
./source/mtb.o \
./source/port.o \
./source/semihost_hardfault.o \
./source/srec.o \
./source/uart.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MKL46Z256VLL4 -DCPU_MKL46Z256VLL4_cm0plus -DSDK_OS_BAREMETAL -DFSL_RTOS_BM -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"H:\Projects\Projects MCUXPresso IDE\Bootloader\board" -I"H:\Projects\Projects MCUXPresso IDE\Bootloader\source" -I"H:\Projects\Projects MCUXPresso IDE\Bootloader" -I"H:\Projects\Projects MCUXPresso IDE\Bootloader\drivers" -I"H:\Projects\Projects MCUXPresso IDE\Bootloader\CMSIS" -I"H:\Projects\Projects MCUXPresso IDE\Bootloader\utilities" -I"H:\Projects\Projects MCUXPresso IDE\Bootloader\startup" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/Bootloader_V2.d ./source/Bootloader_V2.o ./source/flash.d ./source/flash.o ./source/gpio.d ./source/gpio.o ./source/mtb.d ./source/mtb.o ./source/port.d ./source/port.o ./source/semihost_hardfault.d ./source/semihost_hardfault.o ./source/srec.d ./source/srec.o ./source/uart.d ./source/uart.o

.PHONY: clean-source

