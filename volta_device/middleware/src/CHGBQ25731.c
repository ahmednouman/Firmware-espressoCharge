#include "CHGBQ25731.h"



//void chg_write_command(I2C_Handle_t *pI2CHandle, uint8_t reg_addr, uint8_t *data, uint8_t size)
//{
//
//	uint8_t m[4];
//	m[0] = 0x02;
//	m[1] = 0x80;
//	m[2] = 0x03;
//	m[3] = 0x00;
//    while(I2C_MasterSendDataIT(pI2CHandle, &m[0], 4 , CHG_SLAVE_ADDR, I2C_ENABLE_SR) != I2C_READY);
//    delay(2000000);
//}


//void chg_setup_charge_power(I2C_Handle_t *pI2CHandle)
//{
////	delay(200000);
////	uint8_t data[8] = { 0x02, 0x80, 0x03, 0x00, 0x04, 0x00, 0x05, 0x52};
//	uint8_t data[2] = {0};
//	data[0] = 0x02;
//	data[1] = 0x80;
//
//	chrg_write_command(pI2CHandle, 0,  &data[0], 2);
////	while(I2C_MasterSendDataIT(pI2CHandle, &data[0], 2 , CHG_SLAVE_ADDR, I2C_ENABLE_SR) != I2C_READY);
////	delay(200000);
////	while(I2C_MasterSendDataIT(pI2CHandle, &data[0], 2 , CHG_SLAVE_ADDR, I2C_ENABLE_SR) != I2C_READY);
//////	delay(200000);
////	while(I2C_MasterSendDataIT(pI2CHandle, &data[0], 2 , CHG_SLAVE_ADDR, I2C_ENABLE_SR) != I2C_READY);
//////	delay(200000);
////	while(I2C_MasterSendDataIT(pI2CHandle, &data[0], 2 , CHG_SLAVE_ADDR, I2C_ENABLE_SR) != I2C_READY);
////	delay(200000);
//}

void chg_read_register(I2C_Handle_t *pI2CHandle, uint8_t reg_addr, uint8_t *buffer, uint8_t size)
{
   while(I2C_MasterSendDataIT(pI2CHandle, &reg_addr, 1, CHG_SLAVE_ADDR, I2C_ENABLE_SR) != I2C_READY);

   pI2CHandle->rxCmplt = RESET;
   while(I2C_MasterReceiveDataIT(pI2CHandle, buffer, size, CHG_SLAVE_ADDR, I2C_ENABLE_SR) != I2C_READY);

   while(pI2CHandle->rxCmplt != SET)
   {

   }
   pI2CHandle->rxCmplt = RESET;
}

void chg_write_command(I2C_Handle_t *pI2CHandle, uint8_t reg_addr, uint8_t *data, uint8_t size)
{

    while(I2C_MasterSendDataIT(pI2CHandle, data, size , CHG_SLAVE_ADDR, I2C_ENABLE_SR) != I2C_READY);
    delay(200000);
}

uint8_t chg_set_ic_power_mode(I2C_Handle_t *pI2CHandle, uint8_t mode)
{
    uint8_t pwr_mode = 0;
    uint8_t reg_val;
    uint8_t buf[2] = { 0x01, 0x00 };

    chg_read_register(pI2CHandle, 0x01, &pwr_mode, 1);

    if (mode == LOW_POWER) {
        reg_val = pwr_mode | (1 << 7);
    } else if (mode == NORMAL) {
        reg_val = pwr_mode & ~(1 << 7);
    }

    buf[1] = reg_val;

    chg_write_command(pI2CHandle, 0x01, buf, 2);

    return reg_val;
}

void force_charge_off(I2C_Handle_t *pI2CHandle)
{
	uint8_t chrgOption1 = 0;
	uint8_t buf[2] = { 0x30, 0x00 };

	chg_read_register(pI2CHandle, 0x30, &chrgOption1, 1);
	buf[1] = chrgOption1 | (1 << 3);
	chg_write_command(pI2CHandle, 0x30, buf, 3);
	return;
}

void chg_disable_otg(I2C_Handle_t *pI2CHandle)
{

	uint8_t buf[3] = { 0x34, 0x34, 0x4 };

	chg_write_command(pI2CHandle, 0x34, buf, 3);
}

void chg_enable_ddc_all(I2C_Handle_t *pI2CHandle)
{
	chg_set_ic_power_mode(pI2CHandle, NORMAL);

	uint8_t buf[3] = { 0x3A, 0xFF, 0xFF };

	chg_write_command(pI2CHandle, 0x3A, buf, 3);
}

void chg_disable_ddc_all(I2C_Handle_t *pI2CHandle)
{
	chg_set_ic_power_mode(pI2CHandle, LOW_POWER);

	uint8_t buf[3] = { 0x3A, 0x00, 0x00 };

	chg_write_command(pI2CHandle, 0x3A, buf, 3);
}

uint8_t chg_get_ic_power_mode(I2C_Handle_t *pI2CHandle)
{
	uint8_t pwr_mode = 0;
	uint8_t bit_val;
	chg_read_register(pI2CHandle, 0x1, &pwr_mode, 1);
	bit_val = (pwr_mode >> 7) & 0x1;
	return bit_val;
}

void chg_get_ManufacturerID(I2C_Handle_t *pI2CHandle, uint8_t* buf)
{
	chg_read_register(pI2CHandle, 0x2E, buf, 1);
}

void chg_get_DeviceID(I2C_Handle_t *pI2CHandle, uint8_t* buf)
{
	chg_read_register(pI2CHandle, 0x2F, buf, 1);
}

void chg_get_ChargeOption0(I2C_Handle_t *pI2CHandle, uint8_t* buf)
{
	chg_read_register(pI2CHandle, 0x00, buf, 2);
}

void chg_get_ChargeCurrent(I2C_Handle_t *pI2CHandle, uint8_t* buf)
{
	chg_read_register(pI2CHandle, 0x02, buf, 2);
}

void chg_get_ChargeVoltage(I2C_Handle_t *pI2CHandle, uint8_t* buf)
{
	chg_read_register(pI2CHandle, 0x04, buf, 2);
}

void chg_get_OTGVoltage(I2C_Handle_t *pI2CHandle, uint8_t* buf)
{
	chg_read_register(pI2CHandle, 0x06, buf, 2);
}

void chg_get_OTGCurrent(I2C_Handle_t *pI2CHandle, uint8_t* buf)
{
	chg_read_register(pI2CHandle, 0x08, buf, 2);
}

void chg_get_ChargerStatus(I2C_Handle_t *pI2CHandle, uint8_t* buf)
{
	chg_read_register(pI2CHandle, 0x20, buf, 2);
}

void chg_get_ProchotStatus(I2C_Handle_t *pI2CHandle, uint8_t* buf)
{
	chg_read_register(pI2CHandle, 0x22, buf, 2);
}

void chg_get_ADCVBUS_PSYS(I2C_Handle_t *pI2CHandle, uint8_t* buf)
{
	chg_read_register(pI2CHandle, 0x26, buf, 2);
}

void chg_get_ADCIBAT(I2C_Handle_t *pI2CHandle, uint8_t* buf)
{
	chg_read_register(pI2CHandle, 0x28, buf, 2);
}

void chg_get_ADCVSYS_VBAT(I2C_Handle_t *pI2CHandle, uint8_t* buf)
{
	chg_read_register(pI2CHandle, 0x2C, buf, 2);
}

void chg_get_ADCOption(I2C_Handle_t *pI2CHandle, uint8_t* buf)
{
	chg_read_register(pI2CHandle, 0x3A, buf, 2);
}

void chg_set_ADCOption(I2C_Handle_t *pI2CHandle, uint8_t* val)
{
	uint8_t buf[3] = {0x3A, val[0], val[1] };
	chg_write_command(pI2CHandle, 0x3A, buf, 2);
}



