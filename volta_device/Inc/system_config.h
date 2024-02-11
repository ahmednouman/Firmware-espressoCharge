
#ifndef SYSTEM_CONFIG_H_
#define SYSTEM_CONFIG_H_

//#define ENABLE_SYSTEM_LOG

#define HW_BUILD ((uint8_t) 0x08)
#define FW_VERSION  ((uint32_t) 0x01040000)

#define PD_SLAVE_ADDR   0x20
#define CHG_SLAVE_ADDR   0x6B
#define GAUGE_SLAVE_ADDR   0x55
#define EPROM_SLAVE_ADDR   0x50
#define OWN_ADDR   0x71

#define NUM_LEDS 3
#define LED1 TIM_CHANNEL_4
#define LED2 TIM_CHANNEL_2
#define LED3 TIM_CHANNEL_1
#define min_brightness 0
#define max_brightness 0xFFFF


#define PD_INTERRUPT  GPIO_PIN_NO_4
#define KNOCK_INTERRUPT  GPIO_PIN_NO_5

#define KNOCK_IT_PRIORITY  15

#define MSG_SIZE 70

#define APP_MODE  0
#define PROGRAM_PD_MODE  1
#define PROGRAM_GAUGE_MODE  2

typedef struct
{
	uint8_t SOC;
	uint8_t SOH;
	uint8_t remainingCapacity[2];
	uint8_t fullCapacity[2];
	uint8_t voltage[2];
	uint8_t current[2];
	uint8_t temperature[2];
	uint8_t timeToEmpty[2];
	uint8_t timeToFull[2];
}BatteryReport;

#endif /* SYSTEM_CONFIG_H_ */
