/*
 * Version: 0.1.1
 * Workflow version: 0.0.1
 *
 * file: generic_api.c
 *
 * Created on: Jun 30, 2023
 * Author: Ahmed Ali
 * Company: espresso Displays
 * Project: Volta
 */

#include "generic_api.h"

uint8_t calculateXorChecksum(uint8_t* data, uint8_t len);
void uint8_to_str(uint8_t value, char *str);

extern usart1_handle;

void delay(uint32_t usec){
	volatile uint32_t count = ((usec * 16) /4) / 4;
	for(volatile int i=0 ; i < count; ++i ){
		 count--;
	}
}

//void logmsg(uint8_t size, char *format, ...)
//{
//	char str[size];
//
//	va_list args;
//	va_start(args, format);
//	vsprintf(str, format, args);
//	while( USART_SendDataIT(&usart1_handle,(uint8_t*)str, strlen(str)) != USART_READY);
//	delay(3000000);
//	va_end(args);
//}

uint8_t calculateXorChecksum(uint8_t* data, uint8_t len) {
    uint8_t checksum = 0;

    for (uint8_t i = 0; i < len; i++) {
        checksum ^= data[i];
    }

    return checksum;
}

void encodeData(uint8_t* data, uint8_t len, uint8_t* encoded_data, uint8_t* encoded_len)
{
    uint8_t checksum = calculateXorChecksum(data, len);
    data[len] = checksum;
    len++;

    uint8_t *encode = encoded_data;
    uint8_t *codep = encode++;
    uint8_t code = 1;

    for (uint8_t i = 0; i < len; ++i)
    {
        if (data[i])
        {
            *encode++ = data[i];
            ++code;
        }

        if (!data[i] || code == 0xFF)
        {
            *codep = code;
            code = 1;
            codep = encode;
            if (!data[i] || (i < len - 1))
                ++encode;
        }
    }

    *codep = code;
    *encode++ = 0;
    *encoded_len = (uint8_t)(encode - encoded_data);
}

void int_to_str(uint32_t value, char *str, size_t size) {
    char buffer[15];
    int index = 0;

    do {
        buffer[index++] = '0' + (value % 10);
        value /= 10;
    } while (value > 0 && index < sizeof(buffer));

    for (int i = 0; i < index; ++i) {
        str[i] = buffer[index - i - 1];
    }

    str[index] = '\0';
}

uint32_t convertBufferTo32Bit(const uint8_t *buffer, size_t length) {
    if (length == 0 || length > 4) {
        return 0;
    }

    uint32_t result = 0;

    for (size_t i = 0; i < length; i++) {
    	result |= ((uint32_t)buffer[i] << (8 * i));
    }

    return result;
}

void uint8_to_str(uint8_t value, char *str) {
    int idx = 0;
    do {
        str[idx++] = '0' + (value % 10);
        value /= 10;
    } while (value > 0);

    int start = 0;
    int end = idx - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        ++start;
        --end;
    }
    str[idx] = '\0';
}

void serial_log(const char *msg) {
    while (USART_SendDataIT(&usart1_handle, msg, strlen(msg)) != USART_READY);
    delay(5000);
}

void update_msg_and_log(char *msg, const char *label, int value) {
    char value_str[15];
    strcpy(msg, label);
    int_to_str(value, value_str, sizeof(value_str));
    for (size_t i = strlen(value_str); i < sizeof(value_str); ++i) {
        value_str[i] = '\0';
    }
    strcat(msg, value_str);
    strcat(msg, "\r\n");
    serial_log(msg);
}

void update_msg_and_log_with_byte(char *msg, const char *label, uint8_t value) {
	char value_str[4];
	uint8_to_str(value, value_str);

	strcpy(msg, label);
	strcat(msg, value_str);
	strcat(msg, "\r\n");

    serial_log(msg);
}

void msg_only_and_log(char *msg, const char *label) {
    strcpy(msg, label);
    strcat(msg, "\r\n");
    serial_log(msg);
}

void suspend_systick_tim()
{
    CLEAR_BIT(SysTick->CTRL,SysTick_CTRL_TICKINT_Msk);
}

void resume_systick_tim()
{
    SET_BIT(SysTick->CTRL,SysTick_CTRL_TICKINT_Msk);
}