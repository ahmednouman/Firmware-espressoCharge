/*
 * Version: 0.1.1
 * Workflow version: 0.0.1
 *
 * file: stm32f030x4_gpio_driver.c
 *
 * Created on: Jun 30, 2023
 * Author: Ahmed Ali
 * Company: espresso Displays
 * Project: Volta
 */

#include "stm32f030x4_gpio_driver.h"
#include "generic_api.h"


void GPIO_Init_Base()
{
	GPIO_Handle_t IO;

	IO.pGPIOx = GPIOB;
	GPIOB_PCLK_EN();
	IO.pGPIOx->MODER = 0x8U;

	IO.pGPIOx->OSPEEDR = 0xCU;

	IO.pGPIOx->OTYPER = 0x0U;

	IO.pGPIOx->AFR[0] = 0X10U;

    memset(&IO, 0, sizeof(IO));

	IO.pGPIOx = GPIOA;
	GPIOA_PCLK_EN();

	IO.pGPIOx->MODER = 0X2828A0A0;

	IO.pGPIOx->OSPEEDR = 0xC3CF0E0U;

	IO.pGPIOx->PUPDR = 0x24000040;

	IO.pGPIOx->OTYPER = 0x600U;

	IO.pGPIOx->AFR[1] = 0x440U;

	IO.pGPIOx->AFR[0] = 0x11001100;
}

void GPIO_enable_IT_trigger()
{
	EXTI->RTSR = 0x20U;
	EXTI->FTSR = 0x10U;
	SYSCFG_PCLK_EN();
	EXTI->IMR = 0xFF40030U;
}

void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(pGPIOx == GPIOA){
			GPIOA_PCLK_EN();
		}else if(pGPIOx == GPIOB){
			GPIOB_PCLK_EN();
        }
	}else
	{
		if(pGPIOx == GPIOA){
			GPIOA_PCLK_DI();
		}else if(pGPIOx == GPIOB){
			GPIOB_PCLK_DI();
		}
}
}


uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	uint8_t value;
	value = (uint8_t)((pGPIOx->IDR >> PinNumber) & 0x00000001);
	return value;
}

void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{
    if(EnorDi == ENABLE)
    {
        if(IRQNumber <= 31)
        {
            *NVIC_ISER0 |= ( 1 << IRQNumber );
        }

    }else
    {
        if(IRQNumber <= 31)
        {
             *NVIC_ICER0 |= ( 1 << IRQNumber );
        }
    }
}

void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
{
    uint8_t iprx = IRQNumber / 4;
    uint8_t iprx_section = IRQNumber % 4;

    uint8_t shift_amount = ( 8 * iprx_section ) + (8 - NO_PR_BITS_IMPLEMENTED);
    *(NVIC_PR_BASE_ADDR + iprx  ) |= ( IRQPriority <<  shift_amount);
}

void GPIO_IRQHandling(uint8_t PinNumber)
{
    if(EXTI->PR & (1 << PinNumber))
    {
        EXTI->PR |= (1 << PinNumber);
    }
}
