
#ifndef INC_FUEL_GAUGE_H_
#define INC_FUEL_GAUGE_H_

#include <stdbool.h>
#include "initialise.h"

void gauge_get_control_status(I2C_Handle_t *pI2CHandle, uint8_t* rcv_buf);
void get_bat_report(I2C_Handle_t *pI2CHandle, BatteryReport *batReport);
void get_bat_level(I2C_Handle_t *pI2CHandle, BatteryReport *batReport);
void get_bat_voltage(I2C_Handle_t *pI2CHandle, BatteryReport *batReport);
void get_bat_current(I2C_Handle_t *pI2CHandle, BatteryReport *batReport);
void get_bat_SOC(I2C_Handle_t *pI2CHandle, BatteryReport *batReport);
void get_bat_SOH(I2C_Handle_t *pI2CHandle, BatteryReport *batReport);
void get_bat_remaining_capacity(I2C_Handle_t *pI2CHandle, BatteryReport *batReport);
void get_bat_full_capacity(I2C_Handle_t *pI2CHandle, BatteryReport *batReport);
void get_bat_temperature(I2C_Handle_t *pI2CHandle, BatteryReport *batReport);
void get_bat_time_to_empty(I2C_Handle_t *pI2CHandle, BatteryReport *batReport);
void get_bat_time_to_full(I2C_Handle_t *pI2CHandle, BatteryReport *batReport);
void gauge_get_device_type(I2C_Handle_t *pI2CHandle, uint8_t* rcv_buf);
void gauge_get_firmware_version(I2C_Handle_t *pI2CHandle, uint8_t* rcv_buf);

void gauge_sealed(I2C_Handle_t *pI2CHandle);
void gauge_unseal(I2C_Handle_t *pI2CHandle);
void gauge_reset(I2C_Handle_t *pI2CHandle);
void gauge_full_sleep_enable(I2C_Handle_t *pI2CHandle);

void gauge_flash_write_binary(I2C_Handle_t *pI2CHandle, uint8_t* buf, uint8_t len);
bool gauge_flash_read_binary(I2C_Handle_t *pI2CHandle, uint8_t* buf, uint8_t len);

#endif /* INC_FUEL_GAUGE_H_ */
