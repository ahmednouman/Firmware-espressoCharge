/*
 * Version: 0.1.1
 * Workflow version: 0.0.1
 *
 * file: generic_api.h
 *
 * Created on: Jun 30, 2023
 * Author: Ahmed Ali
 * Company: espresso Displays
 * Project: Volta
 */

#ifndef INC_GENERIC_API_H_
#define INC_GENERIC_API_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include "stm32f030x4_usart_driver.h"

/*
* Generic API's
*/
void delay(uint32_t usec);
void logmsg(uint8_t size, char *format, ...);
void encodeData(uint8_t* data, uint8_t len, uint8_t* encoded_data, uint8_t* encoded_len);
void int_to_str(uint32_t value, char *str, size_t size);
uint32_t convertBufferTo32Bit(const uint8_t *buffer, size_t length);
void serial_log(const char *msg);
void update_msg_and_log(char *msg, const char *label, int value);
void update_msg_and_log_with_byte(char *msg, const char *label, uint8_t value);
void msg_only_and_log(char *msg, const char *label);

uint8_t calculateXorChecksum(uint8_t* data, uint8_t len);

void suspend_systick_tim();
void resume_systick_tim();

#endif /* INC_GENERIC_API_H_ */
