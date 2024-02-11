################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ThirdParty/FreeRTOS/portable/MemMang/heap_4.c 

OBJS += \
./ThirdParty/FreeRTOS/portable/MemMang/heap_4.o 

C_DEPS += \
./ThirdParty/FreeRTOS/portable/MemMang/heap_4.d 


# Each subdirectory must supply rules for building sources it contributes
ThirdParty/FreeRTOS/portable/MemMang/%.o ThirdParty/FreeRTOS/portable/MemMang/%.su: ../ThirdParty/FreeRTOS/portable/MemMang/%.c ThirdParty/FreeRTOS/portable/MemMang/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F0 -DSTM32F030F4Px -c -I../Inc -I"C:/Espresso/Firmware/volta/Volta_Firmware/volta_device/drivers/inc" -I"C:/Espresso/Firmware/volta/Volta_Firmware/volta_device/ThirdParty/FreeRTOS/include" -I"C:/Espresso/Firmware/volta/Volta_Firmware/volta_device/ThirdParty/FreeRTOS/portable/GCC/ARM_CM0" -I"C:/Espresso/Firmware/volta/Volta_Firmware/volta_device/ThirdParty/FreeRTOS/portable/MemMang" -I"C:/Espresso/Firmware/volta/Volta_Firmware/volta_device/ThirdParty/FreeRTOS" -I"C:/Espresso/Firmware/volta/Volta_Firmware/volta_device/Inc" -I"C:/Espresso/Firmware/volta/Volta_Firmware/volta_device/middleware/inc" -I"C:/Espresso/Firmware/volta/Volta_Firmware/volta_device/drivers/thirdPartyDrivers/inc" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-ThirdParty-2f-FreeRTOS-2f-portable-2f-MemMang

clean-ThirdParty-2f-FreeRTOS-2f-portable-2f-MemMang:
	-$(RM) ./ThirdParty/FreeRTOS/portable/MemMang/heap_4.d ./ThirdParty/FreeRTOS/portable/MemMang/heap_4.o ./ThirdParty/FreeRTOS/portable/MemMang/heap_4.su

.PHONY: clean-ThirdParty-2f-FreeRTOS-2f-portable-2f-MemMang

