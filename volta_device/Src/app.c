#include <stdio.h>
#include "initialise.h"
#include <stdbool.h>


void start_setup();
void host_comms(void* parameters);
void device_task(void* parameters);
void process_command(uint8_t* cmd_buf, uint8_t frame_len);
void send_ok_msg();
void process_pd_binary_fw(uint8_t* cmd_buf, uint8_t frame_len);
void process_gauge_binary_fw(uint8_t* cmd_buf, uint8_t frame_len);
static void respond_to_host();

uint32_t SystemCoreClock = 8000000;

TaskHandle_t hostComms_handle;
TaskHandle_t deviceTask_handle;
__vo uint8_t system_mode = APP_MODE;
__vo uint8_t pd_mode = 0xFF;

BatteryReport pbatReport;

__vo uint8_t deadBatFlag = 0;
__vo uint8_t bat_soc = 0;

__vo uint8_t msg[MSG_SIZE];
__vo uint8_t encMsg[20];
__vo uint8_t encDataLen;
uint8_t rx_buf[MSG_SIZE] = {-1};
__vo uint8_t rcv_len = 1;
__vo uint8_t rxCount = 0;

__vo uint16_t patchSize = 0;

__vo uint16_t deviceTaskDelay = 1000;
uint8_t leds[NUM_LEDS] = {LED1, LED2, LED3};
uint8_t leds_to_control[NUM_LEDS];
uint16_t current_brightness[NUM_LEDS] = { min_brightness };

__vo bool deviceUpdateLED = false;
__vo uint8_t currentSOC = 0;
__vo uint8_t sourceLEDCount = 0;
__vo bool sourceCountDone = false;
__vo uint8_t lastPowerDirection = 2;
__vo bool knocked = false;
__vo uint8_t knockActiveCount = 0;
__vo bool charge_ok = true;
__vo uint8_t charge_ok_val = 1;
__vo uint8_t chargeOKCycleCount = 0;
__vo bool lowBatteryUserNotified = false;
__vo uint8_t ledLowCount = 0;

__vo bool enableSleepMode = true;

void pd_restart_setup()
{
	pd_interrupt_clear_all(&I2C1Handle);
	pd_interrupt_mask(&I2C1Handle);

	pd_set_sleep_config(&I2C1Handle, ENABLE);
	chg_set_ic_power_mode(&I2C1Handle, LOW_POWER);

//	pd_set_sleep_config(&I2C1Handle, DISABLE);
//	chg_enable_ddc_all(&I2C1Handle);

	//	gauge_sealed(&I2C1Handle);
	gauge_unseal(&I2C1Handle);
	gauge_reset(&I2C1Handle);
	gauge_full_sleep_enable(&I2C1Handle);

	plugAction = pd_get_plug_detect(&I2C1Handle);
	if(plugAction == PD_PLUG_CONNECTED){
		power_direction = pd_get_power_direction(&I2C1Handle);
		if(power_direction == SOURCING){
			pd_swap_to_UFP(&I2C1Handle);
		}
	}

}

int main(void)
{
	delay(2000000);
	IWDG_Init();
	IO_Init();
	I2c_Init();
	USART1_Init();

	pd_restart_setup();

	Knock_PD_trigger_IT_enable();

//	pd_mode = pd_get_mode(&I2C1Handle);
//	if(pd_mode != PD_MODE_APP){
////		led_blink(LED3, 8, 50000, &current_brightness);
//	}
//	force_charge_off(&I2C1Handle);

//	deadBatFlag = pd_get_dead_flag(&I2C1Handle);
//	if(deadBatFlag){
////		pd_clear_dead_flag(&I2C1Handle);
////		pd_mode = pd_get_mode(&I2C1Handle);
////		while(true){
////			led_blink(LED3, 8, 50000);
////		}
//
//	}

//	if(!sleepIWDG){
//		while(1){
//			IWDG_Refresh(&hiwdg);
//			sleepIWDG = true;
//			delay(19000000);
//			led_blink(LED3, 2, 1000000, &current_brightness);
//			IWDG_Refresh(&hiwdg);
//		}
//	}else{
//		IWDG_Refresh(&hiwdg);
//		led_blink(LED3, 1, 500000, &current_brightness);
//	}

//	log_device_info();

 	BaseType_t status;

 	status = xTaskCreate(host_comms, "Host_Comms", 50, NULL, 2, &hostComms_handle);
    configASSERT(status == pdPASS);

 	status = xTaskCreate(device_task, "Device_Task", 35, NULL, 2, &deviceTask_handle);
    configASSERT(status == pdPASS);

    vTaskStartScheduler();

    return 0;
}

void host_comms(void* parameters)
{
    while (USART_ReceiveDataIT(&usart1_handle, &rx_buf[0], rcv_len) != USART_READY );
	while(1)
	{
		if (rx_buf[0] != 0x00 && rx_buf[rxCount-1] == 0x00)
		{
			uint8_t length = rx_buf[0];
			uint8_t cmd_buf[MSG_SIZE] = {0};
			uint8_t cmd_frame_len = 0;
			bool zeroOk = false;
			bool processMsg = false;
			for (uint8_t i = 0; i < rxCount; i++)
			{
				if (i < length && (zeroOk || rx_buf[i] != 0))
				{
					cmd_buf[i] = rx_buf[i];
					cmd_frame_len++;
					zeroOk = false;
				}
				else if (i == length && rx_buf[i] == 0)
				{
					processMsg = true;
				}
				else if (i == length && rx_buf[i] != 0)
				{
					zeroOk = true;
					length += rx_buf[i];
					cmd_frame_len++;
				}
				else
				{
					processMsg = false;
				}

			}
			if (processMsg)
			{
				__vo uint8_t len_byte = cmd_frame_len - 2;
				__vo uint8_t checksum_byte = cmd_buf[len_byte + 1];
				__vo uint8_t checksum_cal = calculateXorChecksum(&cmd_buf[1], len_byte);

				if(checksum_byte == checksum_cal){
					if(system_mode == APP_MODE){
						process_command(cmd_buf, cmd_frame_len);
					}else if(system_mode == PROGRAM_PD_MODE){
						process_pd_binary_fw(cmd_buf, cmd_frame_len);
					}else if(system_mode == PROGRAM_GAUGE_MODE){
						process_gauge_binary_fw(cmd_buf, cmd_frame_len);
					}
				}

			}
			memset(rx_buf, -1, MSG_SIZE);
			rxCount = 0;

		}
		else if (rxCount != 0)
		{
			memset(rx_buf, -1, MSG_SIZE);
			rxCount = 0;
		}
		IWDG_Refresh(&hiwdg);
		vTaskDelay(pdMS_TO_TICKS(40));
		taskYIELD();
	}
}

void process_command(uint8_t* cmd_buf, uint8_t frame_len)
{
	uint8_t temp, temp2;
	if(cmd_buf[1] == 0xF2 && cmd_buf[2] == 0x02 && cmd_buf[3] == 0x01){
		pd_swap_to_sink(&I2C1Handle);
		power_direction = pd_get_power_direction(&I2C1Handle);
		sourceCountDone = false;
		sourceLEDCount = 0;
	}else if(cmd_buf[1] == 0xF2 && cmd_buf[2] == 0x02 && cmd_buf[3] == 0x02){
		pd_swap_to_source(&I2C1Handle);
		vTaskDelay(pdMS_TO_TICKS(1000));
		pd_swap_to_UFP(&I2C1Handle);
		power_direction = pd_get_power_direction(&I2C1Handle);
		deviceUpdateLED = true;
	}else if(cmd_buf[1] == 0xF2 && cmd_buf[2] == 0x02 && cmd_buf[3] == 0xFF && cmd_buf[4] == 0x00){
		pd_reset(&I2C1Handle);
		delay(4000000);
		pd_restart_setup();
		pd_swap_to_UFP(&I2C1Handle);
		plugAction = pd_get_plug_detect(&I2C1Handle);
		if(plugAction == PD_PLUG_CONNECTED){
		    power_direction = pd_get_power_direction(&I2C1Handle);
		}
	}else if(cmd_buf[1] == 0xF2 && cmd_buf[2] == 0x05 && cmd_buf[3] == 0xFF && cmd_buf[4] == 0x00){
		gauge_unseal(&I2C1Handle);
		gauge_reset(&I2C1Handle);
		gauge_full_sleep_enable(&I2C1Handle);
	}else if(cmd_buf[1] == 0xF3 && cmd_buf[2] == 0x01 && cmd_buf[3] == 0x08){
		msg[0] = FW_VERSION & 0xFF;
		msg[1] = (FW_VERSION >> 8) & 0xFF;
		msg[2] = (FW_VERSION >> 16) & 0xFF;
		msg[3] = (FW_VERSION >> 24) & 0xFF;
		encodeData(&msg[0], 4, &encMsg[0], &encDataLen);
		respond_to_host();

	}else if(cmd_buf[1] == 0xF3 && cmd_buf[2] == 0x05 && cmd_buf[3] == 0x10){
		get_bat_report(&I2C1Handle, &pbatReport);
		if(deadBatFlag){
			pbatReport.SOC = 0;
			pbatReport.remainingCapacity[0] = 0;
			pbatReport.remainingCapacity[1] = 0;
			pbatReport.timeToEmpty[0] = 0;
			pbatReport.timeToEmpty[1] = 0;
		}
		msg[0] = pbatReport.SOC;
		msg[1] = pbatReport.SOH;
		msg[2] = pbatReport.remainingCapacity[0];
		msg[3] = pbatReport.remainingCapacity[1];
		msg[4] = pbatReport.fullCapacity[0];
		msg[5] = pbatReport.fullCapacity[1];
		msg[6] = pbatReport.voltage[0];
		msg[7] = pbatReport.voltage[1];
		msg[8] = pbatReport.current[0];
		msg[9] = pbatReport.current[1];
		msg[10] = pbatReport.temperature[0];
		msg[11] = pbatReport.temperature[1];
		msg[12] = pbatReport.timeToEmpty[0];
		msg[13] = pbatReport.timeToEmpty[1];
		msg[14] = pbatReport.timeToFull[0];
		msg[15] = pbatReport.timeToFull[1];
		msg[16] = power_direction;
		encodeData(&msg[0], 17, &encMsg[0], &encDataLen);
		respond_to_host();

	}
//	else if(cmd_buf[1] == 0xF3 && cmd_buf[2] == 0x02 && cmd_buf[3] == 0xAA){
//		pd_get_active_contract_pdo(&I2C1Handle, &msg[0]);
//		encodeData(&msg[1], 4, &encMsg[0], &encDataLen);
//		respond_to_host();
//	}
	else if(cmd_buf[1] == 0xF3 && cmd_buf[2] == 0x02 && cmd_buf[3] == 0x19){
//		pd_mode = pd_get_mode(&I2C1Handle);
//		deadBatFlag = pd_get_dead_flag(&I2C1Handle);
//		msg[0] = pd_mode;
//		msg[1] = deadBatFlag;
//		encodeData(&msg[0], 2, &encMsg[0], &encDataLen);
//		respond_to_host();
	}else if(cmd_buf[1] == 0xF3 && cmd_buf[2] == 0x40){
		uint8_t temp_device_addr = cmd_buf[3], temp_reg_addr = cmd_buf[4], temp_len = cmd_buf[5];
		if(cmd_buf[3] == PD_SLAVE_ADDR){
			temp_len++;
		}
		read_i2c_generic(&I2C1Handle, cmd_buf[3], cmd_buf[4], &msg[0], temp_len);
		encodeData(&msg[0], temp_len, &encMsg[0], &encDataLen);
		respond_to_host();
	}else if(cmd_buf[1] == 0xF2 && cmd_buf[2] == 0x40){
		write_i2c_generic(&I2C1Handle, cmd_buf[3], cmd_buf[4], &cmd_buf[6], cmd_buf[5]);
		send_ok_msg();
	}else if(cmd_buf[1] == 0xF3 && cmd_buf[2] == 0x03 && cmd_buf[3] == 0x58){
		msg[0] = charge_ok_val;
		encodeData(&msg[0], 1, &encMsg[0], &encDataLen);
		respond_to_host();
	}else if(cmd_buf[1] == 0xF0 && cmd_buf[2] == 0x03){
		patchSize = ( (cmd_buf[3] << 8) | (cmd_buf[4]) );
		system_mode = PROGRAM_PD_MODE;
		vTaskSuspend(deviceTask_handle);
		send_ok_msg();
	}else if(cmd_buf[1] == 0xF0 && cmd_buf[2] == 0x05){
		patchSize = ( (cmd_buf[3] << 8) | (cmd_buf[4]) );
		system_mode = PROGRAM_GAUGE_MODE;
		vTaskSuspend(deviceTask_handle);
		send_ok_msg();
	}
}

void process_pd_binary_fw(uint8_t* cmd_buf, uint8_t frame_len)
{
	pd_patch_eprom(&I2C1Handle, &cmd_buf[1], frame_len - 2);
	patchSize = patchSize - (frame_len - 4);
    if(patchSize == 0){
    	send_ok_msg();
    	uint8_t res = pd_reset(&I2C1Handle);
    	delay(4000000);
    	if(res == 0x00){
    		pd_restart_setup();
    		pd_swap_to_UFP(&I2C1Handle);
    		plugAction = pd_get_plug_detect(&I2C1Handle);
    		if(plugAction == PD_PLUG_CONNECTED){
    			power_direction = pd_get_power_direction(&I2C1Handle);
    		}
    		system_mode = APP_MODE;
    		vTaskResume(deviceTask_handle);
    	}
    }
	send_ok_msg();
}

void process_gauge_binary_fw(uint8_t* cmd_buf, uint8_t frame_len)
{
	if(cmd_buf[1] == 0x57){
		gauge_flash_write_binary(&I2C1Handle, &cmd_buf[1], frame_len - 4 );
	}else if(cmd_buf[1] == 0x43){
		gauge_flash_read_binary(&I2C1Handle, &cmd_buf[1], frame_len - 4 );
	}else if(cmd_buf[1] == 0x58){
		uint32_t wait_val = cmd_buf[2] * 100;
		vTaskDelay(pdMS_TO_TICKS(wait_val));
	}

	patchSize = patchSize - 1;
	if(patchSize == 0){
		send_ok_msg();
		gauge_unseal(&I2C1Handle);
		gauge_reset(&I2C1Handle);
		system_mode = APP_MODE;
		vTaskResume(deviceTask_handle);

	}
	send_ok_msg();
}

void send_ok_msg()
{
	uint8_t temp, temp2;
	msg[0] = 0x4F;
	msg[1] = 0x4B;
	encodeData(&msg[0], 2, &encMsg[0], &encDataLen);
	temp = encDataLen;
	temp2 = 0;
	while(temp > 0){
	    while( USART_SendDataIT(&usart1_handle, &encMsg[temp2], 1) != USART_READY);
		temp--;
		temp2++;
	}
}

void respond_to_host()
{
	uint8_t temp, temp2;
	temp = encDataLen;
	temp2 = 0;
	while(temp > 0){
	    while( USART_SendDataIT(&usart1_handle, &encMsg[temp2], 1) != USART_READY);
		temp--;
		temp2++;
	}
}

void device_task(void* parameters)
{
	__vo uint8_t led_to_glow, pd_int_pin;
	__vo uint16_t brightness_val;

	__vo uint16_t battery_voltage = 0, battery_current = 0xFFFF;

	__vo bool chargeOK_prev;

	while(1)
	{
		pd_int_pin = GPIO_ReadFromInputPin(GPIOA, PD_INTERRUPT);
		if(pd_int_pin == 0){
			pd_restart_setup();
		}

		pd_mode = pd_get_mode(&I2C1Handle);
		enableSleepMode = true;
		if(pd_mode != PD_MODE_APP){
			led_blink(LED3, 8, 50000, &current_brightness);
			enableSleepMode = false;
//			if(battery_voltage >= 16700){
//				pd_reset(&I2C1Handle);
//				delay(4000000);
//				pd_restart_setup();
//			}
//			IWDG_Refresh(&hiwdg);
//			pd_reset(&I2C1Handle);
//			pd_restart_setup();
		}

		if(plugAction == PD_PLUG_CONNECTED){

			get_bat_SOC(&I2C1Handle, &pbatReport);
			get_bat_voltage(&I2C1Handle, &pbatReport);
			get_bat_current(&I2C1Handle, &pbatReport);
			bat_soc = pbatReport.SOC;
			if(deadBatFlag){
				bat_soc = 0;
			}

			if(bat_soc != currentSOC){
				deviceUpdateLED = true;
			}
			battery_voltage = (pbatReport.voltage[1] << 8) | pbatReport.voltage[0];
			battery_current = (pbatReport.current[1] << 8) | pbatReport.current[0];

			if(deviceUpdateLED || (lastPowerDirection != power_direction))
			{
				if(lastPowerDirection != power_direction){
					chargeOKCycleCount = 0;
				}
				if(!sourceCountDone){
					led_to_glow = led_logic_handler(bat_soc, plugAction, power_direction, false, &leds_to_control);
					IWDG_Refresh(&hiwdg);
					brighten_leds(&leds, &leds_to_control, &current_brightness);
					IWDG_Refresh(&hiwdg);
					if(power_direction == SINKING){
						deviceTaskDelay = 700;
					}else{
						deviceTaskDelay = 1000;
					}
				}
				deviceUpdateLED = false;
			}
			if(power_direction == SINKING)
			{
				if(battery_current > 10){
					charge_ok = true;
					charge_ok_val = 1;
					chargeOKCycleCount = 0;
				}else if( battery_current <= 10 && chargeOKCycleCount == 3){
					charge_ok = false;
					charge_ok_val = 0;
				}else{
					charge_ok = false;
					charge_ok_val = 0;
					chargeOKCycleCount++;
				}
				if(led_to_glow != 0xFF && charge_ok){
					if(chargeOK_prev != charge_ok){
						deviceUpdateLED = true;
					}else{
					    IWDG_Refresh(&hiwdg);
						dim_led(led_to_glow, &current_brightness);
						vTaskDelay(pdMS_TO_TICKS(500));
						brighten_led(led_to_glow, &current_brightness);
						if(bat_soc < 10){
							lowBatteryUserNotified = false;
						}
						IWDG_Refresh(&hiwdg);
					}

				}else if(!charge_ok && chargeOKCycleCount == 3){
					if(bat_soc < 99){
						IWDG_Refresh(&hiwdg);
						brighten_leds(&leds, &leds_to_control, &current_brightness);
						dim_leds(&leds, &leds_to_control, &current_brightness);
						led_blink(LED1, 3, 150000, &current_brightness);
						IWDG_Refresh(&hiwdg);
					}
				}
				sourceLEDCount = 0;
				sourceCountDone = false;
			}else if(power_direction == SOURCING)
			{
				if(bat_soc < 10){
					if(!knocked){
						if(!lowBatteryUserNotified){
							IWDG_Refresh(&hiwdg);
							led_blink(LED1, 3, 150000, &current_brightness);
							IWDG_Refresh(&hiwdg);
						}
					}else{
						if(lowBatteryUserNotified){
							if(ledLowCount < 4){
								led_blink(LED1, 3, 150000, &current_brightness);
								ledLowCount++;
							}else{
								knocked = false;
								ledLowCount = 0;
							}
						}else{
							lowBatteryUserNotified = true;
							knocked = false;
						}
					}
				}else{
					if(sourceLEDCount == 6){
						IWDG_Refresh(&hiwdg);
						dim_leds(&leds, &leds_to_control, &current_brightness);
						IWDG_Refresh(&hiwdg);
						sourceLEDCount = 0;
						sourceCountDone = true;
					}else{
						if(!sourceCountDone){
							sourceLEDCount++;
						}
					}
				}
			}
		}else
		{
			deviceTaskDelay = 1000;
			if(!knocked && deviceUpdateLED){
				IWDG_Refresh(&hiwdg);
				dim_leds(&leds, &leds_to_control, &current_brightness);
				deviceUpdateLED = false;
			}else if(knocked){
				if(bat_soc < 10){
					if(ledLowCount < 4){
						led_blink(LED1, 3, 150000, &current_brightness);
						ledLowCount++;
					}else{
						knocked = false;
						ledLowCount = 0;
					}
				}else{
					if(knockActiveCount == 6){
						IWDG_Refresh(&hiwdg);
						dim_leds(&leds, &leds_to_control, &current_brightness);
						knockActiveCount = 0;
						knocked = false;
					}else if(deviceUpdateLED){
						led_to_glow = led_logic_handler(bat_soc, plugAction, power_direction, false, &leds_to_control);
						IWDG_Refresh(&hiwdg);
						brighten_leds(&leds, &leds_to_control, &current_brightness);
						deviceUpdateLED = false;
					}
					knockActiveCount++;
				}

			}


			if(!knocked){
				IWDG_Refresh(&hiwdg);
				dim_leds(&leds, &leds_to_control, &current_brightness);
				IWDG_Refresh(&hiwdg);
				if(enableSleepMode){
					prepare_to_sleep();
					prepare_to_wakeup();
				}

			}

		}

		currentSOC = bat_soc;
		lastPowerDirection = power_direction;
		chargeOK_prev = charge_ok;

		IWDG_Refresh(&hiwdg);
		vTaskDelay(pdMS_TO_TICKS(deviceTaskDelay));
		taskYIELD();
	}
}

void EXTI4_15_IRQHandler(void)
{
	GPIO_IRQHandling(KNOCK_INTERRUPT);
    uint8_t pd_val, knock_val;
    pd_val = GPIO_ReadFromInputPin(GPIOA, PD_INTERRUPT);
    knock_val = GPIO_ReadFromInputPin(GPIOA, KNOCK_INTERRUPT);
    delay(100000);
    if(pd_val == 0)
    {
    	GPIO_IRQHandling(PD_INTERRUPT);
    	if(system_mode == APP_MODE){
    		deviceUpdateLED = true;
    		chargeOKCycleCount = 0;
    		sourceCountDone = false;
    		knocked = false;
    		sourceLEDCount = 0;
    		ledLowCount = 0;
    		lowBatteryUserNotified = false;
    		pd_IRQHandling(&I2C1Handle);
    	}
    }
    else{
    	GPIO_IRQHandling(KNOCK_INTERRUPT);
    	GPIO_IRQHandling(PD_INTERRUPT);
    	knocked = true;
    	knockActiveCount = 0;
    	deviceUpdateLED = true;
		get_bat_SOC(&I2C1Handle, &pbatReport);
		bat_soc = pbatReport.SOC;
    	led_logic_handler(bat_soc, plugAction, power_direction, knocked, &leds_to_control);
		sourceCountDone = false;
		sourceLEDCount = 0;
    }

    if(system_mode == PROGRAM_PD_MODE || system_mode == PROGRAM_GAUGE_MODE)
    {
    	GPIO_IRQHandling(PD_INTERRUPT);
    	GPIO_IRQHandling(KNOCK_INTERRUPT);
    }
}

void I2C1_IRQHandler(void)
{
	I2C_EV_IRQHandling(&I2C1Handle);
}

void USART1_IRQHandler(void)
{
	USART_IRQHandling(&usart1_handle);
}

void USART_ApplicationEventCallback( USART_Handle_t *pUSARTHandle,uint8_t ApEv)
{
   if(ApEv == USART_EVENT_RX_CMPLT)
   {
       rx_buf[rxCount++] = (uint8_t)(pUSARTHandle->pUSARTx->RDR & (uint8_t)0xFF);
       if (rxCount >= MSG_SIZE)
       {
    	   rxCount = 0;
       }
       while (USART_ReceiveDataIT(&usart1_handle, &rx_buf[rxCount], rcv_len) != USART_READY );

   }else if (ApEv == USART_EVENT_TX_CMPLT)
   {

   }
}


