################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/flash.c \
../source/gpio.c \
../source/main.c \
../source/mtb.c \
../source/port.c \
../source/queue.c \
../source/semihost_hardfault.c \
../source/uart.c 

C_DEPS += \
./source/flash.d \
./source/gpio.d \
./source/main.d \
./source/mtb.d \
./source/port.d \
./source/queue.d \
./source/semihost_hardfault.d \
./source/uart.d 

OBJS += \
./source/flash.o \
./source/gpio.o \
./source/main.o \
./source/mtb.o \
./source/port.o \
./source/queue.o \
./source/semihost_hardfault.o \
./source/uart.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MKL46Z256VLL4 -DCPU_MKL46Z256VLL4_cm0plus -DSDK_OS_BAREMETAL -DFSL_RTOS_BM -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"D:\MCU\MCU_Moc\MOC\Bootloader-Mooc\board" -I"D:\MCU\MCU_Moc\MOC\Bootloader-Mooc\source" -I"D:\MCU\MCU_Moc\MOC\Bootloader-Mooc" -I"D:\MCU\MCU_Moc\MOC\Bootloader-Mooc\CMSIS" -I"D:\MCU\MCU_Moc\MOC\Bootloader-Mooc\startup" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/flash.d ./source/flash.o ./source/gpio.d ./source/gpio.o ./source/main.d ./source/main.o ./source/mtb.d ./source/mtb.o ./source/port.d ./source/port.o ./source/queue.d ./source/queue.o ./source/semihost_hardfault.d ./source/semihost_hardfault.o ./source/uart.d ./source/uart.o

.PHONY: clean-source

