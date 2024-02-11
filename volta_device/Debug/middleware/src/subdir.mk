################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../middleware/src/CHGBQ25731.c \
../middleware/src/PDTPS257550.c \
../middleware/src/fuel_gauge.c \
../middleware/src/initialise.c 

OBJS += \
./middleware/src/CHGBQ25731.o \
./middleware/src/PDTPS257550.o \
./middleware/src/fuel_gauge.o \
./middleware/src/initialise.o 

C_DEPS += \
./middleware/src/CHGBQ25731.d \
./middleware/src/PDTPS257550.d \
./middleware/src/fuel_gauge.d \
./middleware/src/initialise.d 


# Each subdirectory must supply rules for building sources it contributes
middleware/src/%.o middleware/src/%.su: ../middleware/src/%.c middleware/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F0 -DSTM32F030F4Px -c -I../Inc -I"C:/Espresso/Firmware/volta/Volta_Firmware/volta_device/drivers/inc" -I"C:/Espresso/Firmware/volta/Volta_Firmware/volta_device/ThirdParty/FreeRTOS/include" -I"C:/Espresso/Firmware/volta/Volta_Firmware/volta_device/ThirdParty/FreeRTOS/portable/GCC/ARM_CM0" -I"C:/Espresso/Firmware/volta/Volta_Firmware/volta_device/ThirdParty/FreeRTOS/portable/MemMang" -I"C:/Espresso/Firmware/volta/Volta_Firmware/volta_device/ThirdParty/FreeRTOS" -I"C:/Espresso/Firmware/volta/Volta_Firmware/volta_device/Inc" -I"C:/Espresso/Firmware/volta/Volta_Firmware/volta_device/middleware/inc" -I"C:/Espresso/Firmware/volta/Volta_Firmware/volta_device/drivers/thirdPartyDrivers/inc" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-middleware-2f-src

clean-middleware-2f-src:
	-$(RM) ./middleware/src/CHGBQ25731.d ./middleware/src/CHGBQ25731.o ./middleware/src/CHGBQ25731.su ./middleware/src/PDTPS257550.d ./middleware/src/PDTPS257550.o ./middleware/src/PDTPS257550.su ./middleware/src/fuel_gauge.d ./middleware/src/fuel_gauge.o ./middleware/src/fuel_gauge.su ./middleware/src/initialise.d ./middleware/src/initialise.o ./middleware/src/initialise.su

.PHONY: clean-middleware-2f-src

