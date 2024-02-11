#ifndef INITIALISE_H_
#define INITIALISE_H_

#include<stdio.h>
#include<string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "stm32f030x4.h"
#include "stm32f030x4_gpio_driver.h"
#include "stm32f030x4_i2c_driver.h"
#include "stm32f030x4_usart_driver.h"
#include "stm32F030x4_timer_driver.h"
#include "generic_api.h"
#include "system_config.h"
#include "PDTPS257550.h"
#include "CHGBQ25731.h"
#include "fuel_gauge.h"


extern I2C_Handle_t I2C1Handle;
extern USART_Handle_t usart1_handle;
extern TIM_HandleTypeDef htim3;
extern IWDG_HandleTypeDef hiwdg;

void IO_Init();
I2C_Handle_t* I2c_Init();
void USART1_Init(void);
void Knock_PD_trigger_IT_enable();

void setLED(uint32_t channel, uint16_t brightness);
void knock_IRQHandling(uint8_t battery_soc, uint8_t plug_action, uint8_t pwr_direction);

uint8_t led_logic_handler(uint8_t battery_soc, uint8_t plug_action, uint8_t pwr_direction, bool knock, uint8_t* active_leds);
uint16_t dim_led(uint8_t ledToGlow, uint16_t brightness[]);
uint16_t brighten_led(uint8_t ledToGlow, uint16_t brightness[]);
uint8_t get_led_to_glow(uint8_t battery_soc);
void led_glow_cycle(uint8_t battery_soc, uint8_t plug_action, uint8_t pwr_direction, uint8_t led_to_glow);
void led_blink(uint8_t led, uint8_t cycle_num, uint32_t wait_time, uint16_t *brightness);

void dim_leds(uint8_t leds[], uint8_t active_leds[], uint16_t brightness[]);
void brighten_leds(uint8_t *leds, uint8_t *active_leds, uint16_t *brightness);

void IWDG_Init();
void IWDG_DInit();
void IWDG_Refresh();

void read_i2c_generic(I2C_Handle_t *pI2CHandle, uint8_t device_addr, uint8_t reg_addr, uint8_t *buf, uint8_t len);
void write_i2c_generic(I2C_Handle_t *pI2CHandle, uint8_t device_addr, uint8_t reg_addr, uint8_t *data, uint8_t len);

void prepare_to_sleep();
void prepare_to_wakeup();

void log_device_info();



#endif /* INITIALISE_H_ */
