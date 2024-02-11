################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ThirdParty/FreeRTOS/portable/GCC/ARM_CM0/port.c 

OBJS += \
./ThirdParty/FreeRTOS/portable/GCC/ARM_CM0/port.o 

C_DEPS += \
./ThirdParty/FreeRTOS/portable/GCC/ARM_CM0/port.d 


# Each subdirectory must supply rules for building sources it contributes
ThirdParty/FreeRTOS/portable/GCC/ARM_CM0/%.o ThirdParty/FreeRTOS/portable/GCC/ARM_CM0/%.su: ../ThirdParty/FreeRTOS/portable/GCC/ARM_CM0/%.c ThirdParty/FreeRTOS/portable/GCC/ARM_CM0/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F0 -DSTM32F030F4Px -c -I../Inc -I"C:/Espresso/Firmware/volta/Volta_Firmware/volta_device/drivers/inc" -I"C:/Espresso/Firmware/volta/Volta_Firmware/volta_device/ThirdParty/FreeRTOS/include" -I"C:/Espresso/Firmware/volta/Volta_Firmware/volta_device/ThirdParty/FreeRTOS/portable/GCC/ARM_CM0" -I"C:/Espresso/Firmware/volta/Volta_Firmware/volta_device/ThirdParty/FreeRTOS/portable/MemMang" -I"C:/Espresso/Firmware/volta/Volta_Firmware/volta_device/ThirdParty/FreeRTOS" -I"C:/Espresso/Firmware/volta/Volta_Firmware/volta_device/Inc" -I"C:/Espresso/Firmware/volta/Volta_Firmware/volta_device/middleware/inc" -I"C:/Espresso/Firmware/volta/Volta_Firmware/volta_device/drivers/thirdPartyDrivers/inc" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-ThirdParty-2f-FreeRTOS-2f-portable-2f-GCC-2f-ARM_CM0

clean-ThirdParty-2f-FreeRTOS-2f-portable-2f-GCC-2f-ARM_CM0:
	-$(RM) ./ThirdParty/FreeRTOS/portable/GCC/ARM_CM0/port.d ./ThirdParty/FreeRTOS/portable/GCC/ARM_CM0/port.o ./ThirdParty/FreeRTOS/portable/GCC/ARM_CM0/port.su

.PHONY: clean-ThirdParty-2f-FreeRTOS-2f-portable-2f-GCC-2f-ARM_CM0

