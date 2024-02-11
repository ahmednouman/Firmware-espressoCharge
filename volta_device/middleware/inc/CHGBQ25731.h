
#ifndef INC_CHGBQ25731_H_
#define INC_CHGBQ25731_H_

#include<stdio.h>
#include<string.h>
#include "stm32f030x4.h"
#include "stm32f030x4_i2c_driver.h"
#include "generic_api.h"
#include "system_config.h"

#define NORMAL 0
#define LOW_POWER 1

void chg_write_command(I2C_Handle_t *pI2CHandle, uint8_t reg_addr, uint8_t *data, uint8_t size);
void chg_read_register(I2C_Handle_t *pI2CHandle, uint8_t reg_addr, uint8_t *buffer, uint8_t size);

uint8_t chg_set_ic_power_mode(I2C_Handle_t *pI2CHandle, uint8_t mode);
void force_charge_off(I2C_Handle_t *pI2CHandle);
void chg_disable_otg(I2C_Handle_t *pI2CHandle);
void chg_enable_ddc_all(I2C_Handle_t *pI2CHandle);
void chg_disable_ddc_all(I2C_Handle_t *pI2CHandle);
uint8_t chg_get_ic_power_mode(I2C_Handle_t *pI2CHandle);

void chg_get_ManufacturerID(I2C_Handle_t *pI2CHandle, uint8_t* buf);
void chg_get_DeviceID(I2C_Handle_t *pI2CHandle, uint8_t* buf);

void chg_get_ChargeOption0(I2C_Handle_t *pI2CHandle, uint8_t* buf);
void chg_get_ChargeCurrent(I2C_Handle_t *pI2CHandle, uint8_t* buf);
void chg_get_ChargeVoltage(I2C_Handle_t *pI2CHandle, uint8_t* buf);
void chg_get_OTGVoltage(I2C_Handle_t *pI2CHandle, uint8_t* buf);
void chg_get_OTGCurrent(I2C_Handle_t *pI2CHandle, uint8_t* buf);
void chg_get_ChargerStatus(I2C_Handle_t *pI2CHandle, uint8_t* buf);
void chg_get_ProchotStatus(I2C_Handle_t *pI2CHandle, uint8_t* buf);
void chg_get_ADCVBUS_PSYS(I2C_Handle_t *pI2CHandle, uint8_t* buf);
void chg_get_ADCIBAT(I2C_Handle_t *pI2CHandle, uint8_t* buf);
void chg_get_ADCVSYS_VBAT(I2C_Handle_t *pI2CHandle, uint8_t* buf);
void chg_get_ADCOption(I2C_Handle_t *pI2CHandle, uint8_t* buf);

void chg_set_ADCOption(I2C_Handle_t *pI2CHandle, uint8_t* val);


#endif /* INC_CHGBQ25731_H_ */
