#include <string.h>
#include "initialise.h"

I2C_Handle_t I2C1Handle;
USART_Handle_t usart1_handle;
TIM_HandleTypeDef htim3;

IWDG_HandleTypeDef hiwdg;

void IO_Init()
{
	GPIO_Init_Base();

	TIM_OC_InitTypeDef timPWM_Config;

	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 0;
	htim3.Init.Period = 0xFFFF;

    TIM3_PCLK_EN();

	GPIO_IRQPriorityConfig(TIM3_IRQn, 15);
	GPIO_IRQInterruptConfig(TIM3_IRQn, ENABLE);

	TIM_Base_SetConfig(htim3.Instance, &htim3.Init);

	memset(&timPWM_Config, 0, sizeof(timPWM_Config));
	timPWM_Config.OCMode = TIM_OCMODE_PWM1;
    timPWM_Config.OCPolarity = TIM_OCPOLARITY_LOW;
	timPWM_Config.Pulse = (htim3.Init.Period * 0) / 100 ;

    TIM_PWM_ConfigChannel(&htim3, &timPWM_Config, TIM_CHANNEL_1);
	TIM_PWM_Start(&htim3, TIM_CHANNEL_1);

    TIM_PWM_ConfigChannel(&htim3, &timPWM_Config, TIM_CHANNEL_2);
	TIM_PWM_Start(&htim3, TIM_CHANNEL_2);

    TIM_PWM_ConfigChannel(&htim3, &timPWM_Config, TIM_CHANNEL_4);
	TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
}

void USART1_Init(void)
{
	usart1_handle.pUSARTx = USART1;
	usart1_handle.USART_Config.USART_Baud = USART_STD_BAUD_115200;

	USART_Init(&usart1_handle); 

    GPIO_IRQInterruptConfig(IRQ_NO_USART1, ENABLE);

    //USART_PeripheralControl(USART1, ENABLE);
    usart1_handle.pUSARTx->CR1 |= (1 << USART_CR1_UE);

    //disable
    //usart1_handle.pUSARTx->CR1 &= ~(1 << USART_CR1_UE);
} 

I2C_Handle_t* I2c_Init()
{
	I2C1Handle.pI2Cx = I2C1;
	I2C_Init(&I2C1Handle);

	GPIO_IRQInterruptConfig(IRQ_NO_I2C1, ENABLE);

	// I2C_PeripheralControl(I2C1,ENABLE);
	I2C1Handle.pI2Cx->CR1 |= (1 << I2C_CR1_PE);

	//disable
	//I2C1Handle.pI2Cx->CR1 &= ~(1 << 0);

    return &I2C1Handle;
}

void Knock_PD_trigger_IT_enable()
{
	GPIO_enable_IT_trigger();
	GPIO_IRQPriorityConfig(IRQ_NO_EXTI4_15, 14);
	GPIO_IRQInterruptConfig(IRQ_NO_EXTI4_15, ENABLE);
}


void setLED(uint32_t channel, uint16_t brightness)
{
	__TIM_SET_COMPARE(&htim3, channel, brightness);
}

//void knock_IRQHandling(uint8_t battery_soc, uint8_t plug_action, uint8_t pwr_direction)
//{
//	if(plugAction == PD_NO_PLUG)
//	{
//		uint16_t brightness = 0;
//		if (battery_soc < 30){
//			setLED( LED2, 0);
//			setLED( LED3, 0);
//			while(brightness < htim3.Init.Period)
//			{
//				if(brightness > 65535 - 50){
//					brightness = 0xFFFF;
//				}else{
//					brightness += 50;
//				}
//				setLED( LED1, brightness);
//				delay(1000);
//			}
//		}else if(battery_soc >= 30 && battery_soc < 60){
//			setLED( LED3, 0);
//			while(brightness < htim3.Init.Period)
//			{
//				if(brightness > 65535 - 50){
//					brightness = 0xFFFF;
//				}else{
//					brightness += 50;
//				}
//				setLED( LED1, brightness);
//				setLED( LED2, brightness);
//				delay(1000);
//			}
//		}else if(battery_soc >= 60 && battery_soc < 95){
//			while(brightness < htim3.Init.Period)
//			{
//				if(brightness > 65535 - 50){
//					brightness = 0xFFFF;
//				}else{
//					brightness += 50;
//				}
//				setLED( LED1, brightness);
//				setLED( LED2, brightness);
//				setLED( LED3, brightness);
//				delay(1000);
//			}
//		}else{
//			while(brightness < htim3.Init.Period)
//			{
//				if(brightness > 65535 - 50){
//					brightness = 0xFFFF;
//				}else{
//					brightness += 50;
//				}
//				setLED( LED1, brightness);
//				setLED( LED2, brightness);
//				setLED( LED3, brightness);
//				delay(1000);
//
//			}
//		}
//
//	}
//}

uint8_t led_logic_handler(uint8_t battery_soc, uint8_t plug_action, uint8_t pwr_direction, bool knock, uint8_t* active_leds)
{
	uint8_t led_to_glow = 0xFF;
		if(pwr_direction == SOURCING || knock)
		{
			if (battery_soc >= 10 && battery_soc < 33){
				active_leds[0] = 1;
				active_leds[1] = 0;
				active_leds[2] = 0;
			}else if(battery_soc >= 33 && battery_soc < 90){
				active_leds[0] = 1;
				active_leds[1] = 1;
				active_leds[2] = 0;
			}else if(battery_soc >= 90){
				active_leds[0] = 1;
				active_leds[1] = 1;
				active_leds[2] = 1;
			}else{
				active_leds[0] = 0;
				active_leds[1] = 0;
				active_leds[2] = 0;
			}
			return 0xFF;
		}

		else if(pwr_direction == SINKING)
		{
			if (battery_soc < 33){
				active_leds[0] = 1;
				active_leds[1] = 0;
				active_leds[2] = 0;
				led_to_glow = LED1;
			}else if(battery_soc >= 33 && battery_soc < 90){
				active_leds[0] = 1;
				active_leds[1] = 1;
				active_leds[2] = 0;
				led_to_glow = LED2;
			}else if(battery_soc >= 90 && battery_soc < 100){
				active_leds[0] = 1;
				active_leds[1] = 1;
				active_leds[2] = 1;
				led_to_glow = LED3;
			}else{
				active_leds[0] = 1;
				active_leds[1] = 1;
				active_leds[2] = 1;
				led_to_glow = 0xFF;
			}
		}

		return led_to_glow;
	}

void led_glow_cycle(uint8_t battery_soc, uint8_t plug_action, uint8_t pwr_direction, uint8_t led_to_glow)
{
	__vo uint16_t brightness_val = 0;
	if(pwr_direction == SINKING)
	{
		while(power_direction == SINKING)
		{
			if(led_to_glow != 0xFF){
				brightness_val = brighten_led(led_to_glow, brightness_val);
				brightness_val = dim_led(led_to_glow, brightness_val);
				brightness_val = 0;
			}
			return led_to_glow;
		}
	}
}

void led_blink(uint8_t led, uint8_t cycle_num, uint32_t wait_time, uint16_t *brightness)
{
	__vo uint8_t led_index = 0xFF;

	switch (led) {
	    case LED1:
	        led_index = 0;
	        break;
	    case LED2:
	        led_index = 1;
	        break;
	    case LED3:
	        led_index = 2;
	        break;
	    default:
	        led_index = 0xFF;
	        break;
	}

	for(uint8_t i=0; i < cycle_num; i++){
		setLED(led, max_brightness);
		brightness[led_index] = max_brightness;
		delay(wait_time);
		setLED(led, min_brightness);
		brightness[led_index] = min_brightness;
		delay(wait_time);
	}

}

uint8_t get_led_to_glow(uint8_t battery_soc)
{
	__vo uint8_t led_to_glow;
	if (battery_soc < 30){
		led_to_glow = LED1;
	}else if(battery_soc >= 30 && battery_soc < 60){
		led_to_glow = LED2;
	}else if(battery_soc >= 60 && battery_soc < 95){
		led_to_glow = LED3;
	}else{
		led_to_glow = 0xFF;
	}
	return led_to_glow;
}

uint16_t dim_led(uint8_t ledToGlow, uint16_t brightness[])
{
	volatile uint16_t result;
	__vo uint8_t led_index = 0xFF;

	switch (ledToGlow) {
	    case LED1:
	        led_index = 0;
	        break;
	    case LED2:
	        led_index = 1;
	        break;
	    case LED3:
	        led_index = 2;
	        break;
	    default:
	        result = 0;
	        led_index = 0xFF;
	        break;
	}
	result = brightness[led_index];

	while(result > 0)
	{
		result-= 100;
		setLED( ledToGlow, result);
		brightness[led_index] = result;
		vTaskDelay(pdUS_TO_TICKS(1000));
		taskYIELD();
		if(result < 100){
			result = 0;
			setLED( ledToGlow, 0);
			brightness[led_index] = result;
		}
	}

	return result;
}

uint16_t brighten_led(uint8_t ledToGlow, uint16_t brightness[])
{
	volatile uint16_t result;
	__vo uint8_t led_index = 0xFF;

	switch (ledToGlow) {
	    case LED1:
	        led_index = 0;
	        break;
	    case LED2:
	        led_index = 1;
	        break;
	    case LED3:
	        led_index = 2;
	        break;
	    default:
	        result = 0;
	        led_index = 0xFF;
	        break;
	}
	result = brightness[led_index];

	while(result < htim3.Init.Period)
	{
		result += 200;
		setLED( ledToGlow, result);
		brightness[led_index] = result;
		vTaskDelay(pdUS_TO_TICKS(1000));
		taskYIELD();
		if(result > 65535 - 200){
			result = 65535;
			setLED( ledToGlow, 0xFFFF);
			brightness[led_index] = result;
		}
	}
	return result;
}

void dim_leds(uint8_t leds[], uint8_t active_leds[], uint16_t brightness[])
{
    uint16_t result[NUM_LEDS];

    for (uint8_t i = 0; i < NUM_LEDS; i++) {
        result[i] = brightness[i];
    }

    while (1) {
    	uint8_t allDimmed = 1;

        for (uint8_t i = 0; i < NUM_LEDS; i++) {
        	if(active_leds[i] == 0){
        		continue;
        	}
            if (result[i] > min_brightness) {
                allDimmed = 0;
                result[i] -= 50;
                setLED(leds[i], result[i]);
                brightness[i] = result[i];
            } else {
                setLED(leds[i], min_brightness);
                brightness[i] = min_brightness;
            }
            if(result[i] < min_brightness + 50){
            	setLED(leds[i], min_brightness);
            	brightness[i] = min_brightness;
            	allDimmed = 1;
            }
        }

        if (allDimmed) {
            break;
        }

        vTaskDelay(pdUS_TO_TICKS(1000));
        taskYIELD();
    }
}

void brighten_leds(uint8_t *leds, uint8_t *active_leds, uint16_t *brightness)
{
    uint16_t result[NUM_LEDS];

    for (uint8_t i = 0; i < NUM_LEDS; i++) {
        result[i] = brightness[i];
    }

    while (1) {
    	uint8_t allBrightened = 1;

        for (uint8_t i = 0; i < NUM_LEDS; i++) {
        	if(active_leds[i] == 0){
        		continue;
        	}
            if (result[i] < htim3.Init.Period) {
                allBrightened = 0;
                result[i] += 50;
                setLED(leds[i], result[i]);
                brightness[i] = result[i];
            } else {
            	setLED( leds[i], max_brightness);
            	brightness[i] = max_brightness;
            }
            if(result[i] > max_brightness - 50){
            	setLED( leds[i], max_brightness);
            	brightness[i] = max_brightness;
            	allBrightened = 1;
            }

        }

        if (allBrightened) {
            break;
        }

        vTaskDelay(pdUS_TO_TICKS(1000));
        taskYIELD();
    }
}

void IWDG_Init()
{
	hiwdg.Instance = IWDG;
	hiwdg.Init.Prescaler = IWDG_PRESCALER_256;
	hiwdg.Init.Window = 0;
	hiwdg.Init.Reload = 0xFFFF;

	WRITE_REG(hiwdg.Instance->KR, IWDG_KEY_ENABLE);
	WRITE_REG(hiwdg.Instance->KR, IWDG_KEY_WRITE_ACCESS_ENABLE);

	hiwdg.Instance->PR = hiwdg.Init.Prescaler;
    hiwdg.Instance->RLR = hiwdg.Init.Reload;

	while ((hiwdg.Instance->SR & IWDG_KERNEL_UPDATE_FLAGS) != 0x00u)
	{

	}

	if (hiwdg.Instance->WINR != hiwdg.Init.Window)
	{
//		hiwdg.Instance->WINR = hiwdg.Init.Window;
	}else
	{
		WRITE_REG(hiwdg.Instance->KR, IWDG_KEY_RELOAD);
	}
}

void IWDG_DInit()
{
	hiwdg.Instance = IWDG;
	hiwdg.Init.Prescaler = IWDG_PRESCALER_128;
	hiwdg.Init.Window = 0;
	hiwdg.Init.Reload = 0xFFFF;

	WRITE_REG(hiwdg.Instance->KR, IWDG_KEY_ENABLE);
	WRITE_REG(hiwdg.Instance->KR, IWDG_KEY_WRITE_ACCESS_DISABLE);

	hiwdg.Instance->PR = hiwdg.Init.Prescaler;
    hiwdg.Instance->RLR = hiwdg.Init.Reload;

	while ((hiwdg.Instance->SR & IWDG_KERNEL_UPDATE_FLAGS) != 0x00u)
	{

	}

	if (hiwdg.Instance->WINR != hiwdg.Init.Window)
	{
//		hiwdg.Instance->WINR = hiwdg.Init.Window;
	}else
	{
		WRITE_REG(hiwdg.Instance->KR, IWDG_KEY_RELOAD);
	}
}

void IWDG_Refresh(IWDG_HandleTypeDef *hiwdg)
{
	WRITE_REG(hiwdg->Instance->KR, IWDG_KEY_RELOAD);
}

void read_i2c_generic(I2C_Handle_t *pI2CHandle, uint8_t device_addr, uint8_t reg_addr, uint8_t *buf, uint8_t len){
	   while(I2C_MasterSendDataIT(pI2CHandle, &reg_addr, 1, device_addr, I2C_ENABLE_SR) != I2C_READY);

	   pI2CHandle->rxCmplt = RESET;
	   while(I2C_MasterReceiveDataIT(pI2CHandle, buf, len, device_addr, I2C_ENABLE_SR) != I2C_READY);

	   while(pI2CHandle->rxCmplt != SET)
	   {

	   }
	   pI2CHandle->rxCmplt = RESET;
}

void write_i2c_generic(I2C_Handle_t *pI2CHandle, uint8_t device_addr, uint8_t reg_addr, uint8_t *data, uint8_t len){
    while(I2C_MasterSendDataIT(pI2CHandle, data, len , device_addr, I2C_ENABLE_SR) != I2C_READY);
    delay(200000);
}

void prepare_to_sleep()
{
	PWR_PCLK_EN();
	suspend_systick_tim();

	GPIOB_PCLK_DI();
	TIM3_PCLK_DI();
//	I2C1_PCLK_DI();
	USART1_PCLK_DI();

	uint32_t tmpreg = 0;
	tmpreg = PWR->CR;
	tmpreg &= CR_DS_MASK;
	PWR->CR = tmpreg;

	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

	__WFI();

	SCB->SCR &= (uint32_t)~((uint32_t)SCB_SCR_SLEEPDEEP_Msk);
}

void prepare_to_wakeup()
{

	PWR_PCLK_DI();
	GPIOB_PCLK_EN();
	TIM3_PCLK_EN();
//	I2C1_PCLK_EN();
	USART1_PCLK_EN();

	resume_systick_tim();
}

void log_device_info(){
	#ifdef ENABLE_SYSTEM_LOG

	char msg[80];
	__vo uint8_t temp1;

	update_msg_and_log(msg, "espressoCharge Hardware Version : ", HW_BUILD);
	update_msg_and_log(msg, "espressoCharge Firmware Version : ", FW_VERSION);

	uint8_t rcv_buf[5] = { 0 };

	gauge_get_firmware_version(&I2C1Handle, &rcv_buf[0]);
	update_msg_and_log(msg, "Fuel Gauge Firmware Version : ", convertBufferTo32Bit(rcv_buf, 2));
	gauge_get_control_status(&I2C1Handle, &rcv_buf[0]);
	update_msg_and_log(msg, "Fuel Gauge Control Status Register LowByte : ", rcv_buf[0]);
	update_msg_and_log(msg, "Fuel Gauge Control Status Register HighByte : ", rcv_buf[1]);


	pd_read_register(&I2C1Handle, 0x03, &rcv_buf[0], 5);//mode
	update_msg_and_log(msg, "PD Boot Mode in : ", convertBufferTo32Bit(&rcv_buf[1], 4));
	pd_read_register(&I2C1Handle, 0x04, &rcv_buf[0], 5);//type
	update_msg_and_log(msg, "PD Type : ", convertBufferTo32Bit(&rcv_buf[1], 4));
	pd_read_register(&I2C1Handle, 0x0F, &rcv_buf[0], 5);//version
	update_msg_and_log(msg, "PD Chip Version : ", convertBufferTo32Bit(&rcv_buf[1], 4));
	 __vo uint8_t plug_status = pd_get_plug_detect(&I2C1Handle);
	 update_msg_and_log(msg, "PD Plug Action : ", plug_status);
	 if(plug_status == PD_PLUG_CONNECTED){
		 msg_only_and_log(msg, " * USB-C Cable Plugged :  YES");
	 }else{
		 msg_only_and_log(msg, " * USB-C Cable Plugged :  NO");
	 }
	 __vo uint8_t charge_direction = pd_get_power_direction(&I2C1Handle);
	 update_msg_and_log(msg, "PD Power Direction : ", charge_direction);
	 if(charge_direction == SOURCING){
		 msg_only_and_log(msg, " * espressoCharge Power Direction is :  DISCHARGING | SOURCE MODE");
	 }else if(charge_direction == SINKING){
		 msg_only_and_log(msg, " * espressoCharge Power Direction is :  CHARGING | SINK MODE");
	 }else{
		 msg_only_and_log(msg, " * espressoCharge Power Direction is :  UNKOWN");
	 }
//	uint32_t PDOs[7];
//	uint8_t num_of_pdos;
//	pd_get_rx_source_caps(&I2C1Handle, &PDOs, &num_of_pdos);
//	update_msg_and_log(msg, "PD Received Number of SOURCE PDOs : ", num_of_pdos);
//	for(uint8_t i = 0; i < num_of_pdos; i++){
//		update_msg_and_log(msg, " * Received SOURCE PDOs : ", i+1);
//		update_msg_and_log(msg, "     * => : ", PDOs[i]);
//	}
//	pd_get_rx_sink_caps(&I2C1Handle, &PDOs, &num_of_pdos);
//	update_msg_and_log(msg, "PD Received Number of SINK PDOs : ", num_of_pdos);
//	for(uint8_t i = 0; i < num_of_pdos; i++){
//		update_msg_and_log(msg, " * Received SINK PDOs : ", i+1);
//		update_msg_and_log(msg, "     * => : ", PDOs[i]);
//	}
//	 __vo uint32_t activePDO =  pd_get_active_contract_pdo(&I2C1Handle);
//	 update_msg_and_log(msg, " * PD Active PDO : ", num_of_pdos);
//	__vo uint8_t pd_sleep_mode =  pd_get_sleep_config(&I2C1Handle);
//	update_msg_and_log(msg, " PD Sleep Mode Enabled : ", num_of_pdos);

//	memset(rcv_buf, 0, 5);
//	chg_get_ManufacturerID(&I2C1Handle, &rcv_buf[0]);
//	update_msg_and_log_with_byte(msg, "Charger IC Manufacturer ID : ", rcv_buf[0]);
//	memset(rcv_buf, 0, 5);
//	chg_get_DeviceID(&I2C1Handle, &rcv_buf[0]);
//	update_msg_and_log(msg, "Charger IC Device ID : ", rcv_buf[0]);
//	memset(rcv_buf, 0, 5);
//	chg_get_ChargeOption0(&I2C1Handle, &rcv_buf[0]);
//	update_msg_and_log(msg, "Charger IC ChargeOption0 Reg : ", convertBufferTo32Bit(rcv_buf, 2));
//	memset(rcv_buf, 0, 5);
//	chg_get_ChargeCurrent(&I2C1Handle, &rcv_buf[0]);
//	update_msg_and_log(msg, "Charger IC ChargeCurrent Reg : ", convertBufferTo32Bit(rcv_buf, 2));
//	memset(rcv_buf, 0, 5);
//	chg_get_ChargeVoltage(&I2C1Handle, &rcv_buf[0]);
//	update_msg_and_log(msg, "Charger IC ChargeVoltage Reg : ", convertBufferTo32Bit(rcv_buf, 2));
//	memset(rcv_buf, 0, 5);
//	chg_get_OTGVoltage(&I2C1Handle, &rcv_buf[0]);
//	update_msg_and_log(msg, "Charger IC OTGVoltage Reg : ", convertBufferTo32Bit(rcv_buf, 2));
//	memset(rcv_buf, 0, 5);
//	chg_get_OTGCurrent(&I2C1Handle, &rcv_buf[0]);
//	update_msg_and_log(msg, "Charger IC OTGCurrent Reg : ", convertBufferTo32Bit(rcv_buf, 2));
//	memset(rcv_buf, 0, 5);
//	chg_get_ChargerStatus(&I2C1Handle, &rcv_buf[0]);
//	update_msg_and_log(msg, "Charger IC ChargeStatus Reg : ", convertBufferTo32Bit(rcv_buf, 2));
//	memset(rcv_buf, 0, 5);
//	chg_get_ProchotStatus(&I2C1Handle, &rcv_buf[0]);
//	update_msg_and_log(msg, "Charger IC ProchotStatus Reg : ", convertBufferTo32Bit(rcv_buf, 2));
//	memset(rcv_buf, 0, 5);

//	temp1 = chg_get_ic_power_mode(&I2C1Handle);
//	if(temp1 == LOW_POWER){
//		msg_only_and_log(msg, " * Charger IC Power Mode is :  LOW POWER");
//	}else if(temp1 == NORMAL){
//		msg_only_and_log(msg, " * Charger IC Power Mode is :  NORMAL");
//	}
//	msg_only_and_log(msg, " $Enabling Charger IC ADCs [ENABLING NORMAL POWER MODE]");
//	chg_enable_ddc_all(&I2C1Handle);
//	chg_get_ADCVBUS_PSYS(&I2C1Handle, &rcv_buf[0]);
//	update_msg_and_log(msg, "Charger IC ADCVBUS_PSYS Reg : ", convertBufferTo32Bit(rcv_buf, 2));
//	chg_get_ADCVSYS_VBAT(&I2C1Handle, &rcv_buf[0]);
//	update_msg_and_log(msg, "Charger IC ADCVSYS_VBAT Reg : ", convertBufferTo32Bit(rcv_buf, 2));
//	memset(rcv_buf, 0, 5);
//	chg_get_ADCOption(&I2C1Handle, &rcv_buf[0]);
//	update_msg_and_log(msg, "Charger IC ADCOption Reg : ", convertBufferTo32Bit(rcv_buf, 2));
//	msg_only_and_log(msg, " $Disabling Charger IC ADCs [ENABLING LOW POWER MODE]");
//	chg_disable_ddc_all(&I2C1Handle);

	#endif
}


