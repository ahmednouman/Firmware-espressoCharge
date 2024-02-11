/*
 * Version: 0.1.1
 * Workflow version: 0.0.1
 *
 * stm32f030x4_rcc_driver.h
 *
 * Created on: Jun 30, 2023
 * Author: Ahmed Ali
 * Company: espresso Displays
 * Project: Volta
 */
#ifndef INC_STM32F030X4_RCC_DRIVER_H_
#define INC_STM32F030X4_RCC_DRIVER_H_

#include "stm32f030x4.h"

uint32_t RCC_GetPCLKValue(void);


uint32_t  RCC_GetPLLOutputClock(void);

#endif /* INC_STM32F030X4_RCC_DRIVER_H_ */
