################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ThirdParty/FreeRTOS/croutine.c \
../ThirdParty/FreeRTOS/event_groups.c \
../ThirdParty/FreeRTOS/list.c \
../ThirdParty/FreeRTOS/queue.c \
../ThirdParty/FreeRTOS/stream_buffer.c \
../ThirdParty/FreeRTOS/tasks.c \
../ThirdParty/FreeRTOS/timers.c 

OBJS += \
./ThirdParty/FreeRTOS/croutine.o \
./ThirdParty/FreeRTOS/event_groups.o \
./ThirdParty/FreeRTOS/list.o \
./ThirdParty/FreeRTOS/queue.o \
./ThirdParty/FreeRTOS/stream_buffer.o \
./ThirdParty/FreeRTOS/tasks.o \
./ThirdParty/FreeRTOS/timers.o 

C_DEPS += \
./ThirdParty/FreeRTOS/croutine.d \
./ThirdParty/FreeRTOS/event_groups.d \
./ThirdParty/FreeRTOS/list.d \
./ThirdParty/FreeRTOS/queue.d \
./ThirdParty/FreeRTOS/stream_buffer.d \
./ThirdParty/FreeRTOS/tasks.d \
./ThirdParty/FreeRTOS/timers.d 


# Each subdirectory must supply rules for building sources it contributes
ThirdParty/FreeRTOS/%.o ThirdParty/FreeRTOS/%.su: ../ThirdParty/FreeRTOS/%.c ThirdParty/FreeRTOS/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F0 -DSTM32F030F4Px -c -I../Inc -I"C:/Espresso/Firmware/volta/Volta_Firmware/volta_device/drivers/inc" -I"C:/Espresso/Firmware/volta/Volta_Firmware/volta_device/ThirdParty/FreeRTOS/include" -I"C:/Espresso/Firmware/volta/Volta_Firmware/volta_device/ThirdParty/FreeRTOS/portable/GCC/ARM_CM0" -I"C:/Espresso/Firmware/volta/Volta_Firmware/volta_device/ThirdParty/FreeRTOS/portable/MemMang" -I"C:/Espresso/Firmware/volta/Volta_Firmware/volta_device/ThirdParty/FreeRTOS" -I"C:/Espresso/Firmware/volta/Volta_Firmware/volta_device/Inc" -I"C:/Espresso/Firmware/volta/Volta_Firmware/volta_device/middleware/inc" -I"C:/Espresso/Firmware/volta/Volta_Firmware/volta_device/drivers/thirdPartyDrivers/inc" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-ThirdParty-2f-FreeRTOS

clean-ThirdParty-2f-FreeRTOS:
	-$(RM) ./ThirdParty/FreeRTOS/croutine.d ./ThirdParty/FreeRTOS/croutine.o ./ThirdParty/FreeRTOS/croutine.su ./ThirdParty/FreeRTOS/event_groups.d ./ThirdParty/FreeRTOS/event_groups.o ./ThirdParty/FreeRTOS/event_groups.su ./ThirdParty/FreeRTOS/list.d ./ThirdParty/FreeRTOS/list.o ./ThirdParty/FreeRTOS/list.su ./ThirdParty/FreeRTOS/queue.d ./ThirdParty/FreeRTOS/queue.o ./ThirdParty/FreeRTOS/queue.su ./ThirdParty/FreeRTOS/stream_buffer.d ./ThirdParty/FreeRTOS/stream_buffer.o ./ThirdParty/FreeRTOS/stream_buffer.su ./ThirdParty/FreeRTOS/tasks.d ./ThirdParty/FreeRTOS/tasks.o ./ThirdParty/FreeRTOS/tasks.su ./ThirdParty/FreeRTOS/timers.d ./ThirdParty/FreeRTOS/timers.o ./ThirdParty/FreeRTOS/timers.su

.PHONY: clean-ThirdParty-2f-FreeRTOS

