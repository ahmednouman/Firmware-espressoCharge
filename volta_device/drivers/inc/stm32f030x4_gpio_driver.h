/*
 * Version: 0.1.1
 * Workflow version: 0.0.1
 *
 * file: stm32f030x4_gpio_driver.h
 *
 * Created on: Jun 30, 2023
 * Author: Ahmed Ali
 * Company: espresso Displays
 * Project: Volta
 */

#ifndef INC_STM32F030X4_GPIO_DRIVER_H_
#define INC_STM32F030X4_GPIO_DRIVER_H_

#include "stm32f030x4.h"

/*
 *Configuration structure for a GPIO pin
 */
typedef struct
{
	uint8_t GPIO_PinNumber;
	uint8_t GPIO_PinMode;			
	uint8_t GPIO_PinSpeed;			
	uint8_t GPIO_PinPuPdControl;
	uint8_t GPIO_PinOPType;
	uint8_t GPIO_PinAltFunMode;
}GPIO_PinConfig_t;

/*
 *Handle structure for a GPIO pin
 */
typedef struct
{
	GPIO_RegDef_t *pGPIOx;       		
	GPIO_PinConfig_t GPIO_PinConfig;   
}GPIO_Handle_t;


/*
 * @GPIO_PIN_NUMBERS
 * GPIO pin numbers
 */

#define GPIO_PIN_NO_1  				1
#define GPIO_PIN_NO_2 				2
#define GPIO_PIN_NO_3				3
#define GPIO_PIN_NO_4				4
#define GPIO_PIN_NO_5  				5
#define GPIO_PIN_NO_6  				6
#define GPIO_PIN_NO_7  				7
#define GPIO_PIN_NO_9  				9
#define GPIO_PIN_NO_10  			10


/*
 * @GPIO_PIN_MODES
 * GPIO pin possible modes
 */
#define GPIO_MODE_IN 		0
#define GPIO_MODE_OUT 		1
#define GPIO_MODE_ALTFN 	2
#define GPIO_MODE_ANALOG 	3
#define GPIO_MODE_IT_FT     4
#define GPIO_MODE_IT_RT     5
#define GPIO_MODE_IT_RFT    6

/*
 * GPIO pin possible output types
 */
#define GPIO_OP_TYPE_PP   0
#define GPIO_OP_TYPE_OD   1

/*
 * @GPIO_PIN_SPEED
 * GPIO pin possible output speeds
 */
#define GPIO_SPEED_LOW			0
#define GPIO_SPEED_MEDIUM		1
#define GPIO_SPEED_HIGH			3

/*
 * GPIO pin pull up AND pull down configuration macros
 */
#define GPIO_NO_PUPD   		0
#define GPIO_PIN_PU			1
#define GPIO_PIN_PD			2


void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi);
void GPIO_enable_IT_trigger();
/*
 * Init and De-init
 */

void GPIO_Init_Base();

/*
 * Data read and write
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);

/*
 * IRQ Configuration and ISR handling
 */
void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void GPIO_IRQHandling(uint8_t PinNumber);

#endif /* INC_STM32F030X4_GPIO_DRIVER_H_ */
