################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/src/generic_api.c \
../drivers/src/stm32F030x4_timer_driver.c \
../drivers/src/stm32f030x4_gpio_driver.c \
../drivers/src/stm32f030x4_i2c_driver.c \
../drivers/src/stm32f030x4_rcc_driver.c \
../drivers/src/stm32f030x4_usart_driver.c 

OBJS += \
./drivers/src/generic_api.o \
./drivers/src/stm32F030x4_timer_driver.o \
./drivers/src/stm32f030x4_gpio_driver.o \
./drivers/src/stm32f030x4_i2c_driver.o \
./drivers/src/stm32f030x4_rcc_driver.o \
./drivers/src/stm32f030x4_usart_driver.o 

C_DEPS += \
./drivers/src/generic_api.d \
./drivers/src/stm32F030x4_timer_driver.d \
./drivers/src/stm32f030x4_gpio_driver.d \
./drivers/src/stm32f030x4_i2c_driver.d \
./drivers/src/stm32f030x4_rcc_driver.d \
./drivers/src/stm32f030x4_usart_driver.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/src/%.o drivers/src/%.su: ../drivers/src/%.c drivers/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F0 -DSTM32F030F4Px -c -I../Inc -I"C:/Espresso/Firmware/volta/Volta_Firmware/volta_device/drivers/inc" -I"C:/Espresso/Firmware/volta/Volta_Firmware/volta_device/ThirdParty/FreeRTOS/include" -I"C:/Espresso/Firmware/volta/Volta_Firmware/volta_device/ThirdParty/FreeRTOS/portable/GCC/ARM_CM0" -I"C:/Espresso/Firmware/volta/Volta_Firmware/volta_device/ThirdParty/FreeRTOS/portable/MemMang" -I"C:/Espresso/Firmware/volta/Volta_Firmware/volta_device/ThirdParty/FreeRTOS" -I"C:/Espresso/Firmware/volta/Volta_Firmware/volta_device/Inc" -I"C:/Espresso/Firmware/volta/Volta_Firmware/volta_device/middleware/inc" -I"C:/Espresso/Firmware/volta/Volta_Firmware/volta_device/drivers/thirdPartyDrivers/inc" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-drivers-2f-src

clean-drivers-2f-src:
	-$(RM) ./drivers/src/generic_api.d ./drivers/src/generic_api.o ./drivers/src/generic_api.su ./drivers/src/stm32F030x4_timer_driver.d ./drivers/src/stm32F030x4_timer_driver.o ./drivers/src/stm32F030x4_timer_driver.su ./drivers/src/stm32f030x4_gpio_driver.d ./drivers/src/stm32f030x4_gpio_driver.o ./drivers/src/stm32f030x4_gpio_driver.su ./drivers/src/stm32f030x4_i2c_driver.d ./drivers/src/stm32f030x4_i2c_driver.o ./drivers/src/stm32f030x4_i2c_driver.su ./drivers/src/stm32f030x4_rcc_driver.d ./drivers/src/stm32f030x4_rcc_driver.o ./drivers/src/stm32f030x4_rcc_driver.su ./drivers/src/stm32f030x4_usart_driver.d ./drivers/src/stm32f030x4_usart_driver.o ./drivers/src/stm32f030x4_usart_driver.su

.PHONY: clean-drivers-2f-src

