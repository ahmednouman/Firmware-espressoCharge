#include "fuel_gauge.h"
#define SCALING_FACTOR 100

static void gauge_read_register(I2C_Handle_t *pI2CHandle, uint8_t reg_addr, uint8_t *buffer, uint8_t size);
static uint8_t calculateBatteryLevel(uint16_t voltage);

static uint8_t calculateBatteryLevel(uint16_t voltage) {
    uint32_t scaledVoltage = (uint32_t)voltage * SCALING_FACTOR;
    uint32_t numerator = scaledVoltage - (uint32_t)15500 * SCALING_FACTOR;
    uint32_t denominator = (uint32_t)21000 * SCALING_FACTOR - (uint32_t)15500 * SCALING_FACTOR;
    uint8_t batteryLevel = (uint8_t)((numerator * 100) / denominator);

    return batteryLevel;
}

void gauge_read_register(I2C_Handle_t *pI2CHandle, uint8_t reg_addr, uint8_t *buffer, uint8_t size)
{
   delay(200000);
   while(I2C_MasterSendDataIT(pI2CHandle, &reg_addr, 1, GAUGE_SLAVE_ADDR, I2C_ENABLE_SR) != I2C_READY);

   pI2CHandle->rxCmplt = RESET;
   while(I2C_MasterReceiveDataIT(pI2CHandle, buffer, size, GAUGE_SLAVE_ADDR, I2C_ENABLE_SR) != I2C_READY);

   while(pI2CHandle->rxCmplt != SET)
   {

   }
   pI2CHandle->rxCmplt = RESET;
}

void gauge_get_control_status(I2C_Handle_t *pI2CHandle, uint8_t* rcv_buf)
{
	uint8_t buf[3] = { 0x00, 0x00, 0x00 };

	while(I2C_MasterSendDataIT(pI2CHandle, &buf[0], 3, GAUGE_SLAVE_ADDR, I2C_ENABLE_SR) != I2C_READY);
	delay(200000);

	gauge_read_register(pI2CHandle, 0x00, rcv_buf, 2);
}

void get_bat_report(I2C_Handle_t *pI2CHandle, BatteryReport *batReport)
{

		get_bat_voltage(pI2CHandle, batReport);
		get_bat_current(pI2CHandle, batReport);
		get_bat_SOC(pI2CHandle, batReport);
		get_bat_SOH(pI2CHandle, batReport);
		get_bat_remaining_capacity(pI2CHandle, batReport);
		get_bat_full_capacity(pI2CHandle, batReport);
		get_bat_temperature(pI2CHandle, batReport);
		get_bat_time_to_empty(pI2CHandle, batReport);
		get_bat_time_to_full(pI2CHandle, batReport);
}

void get_bat_level(I2C_Handle_t *pI2CHandle, BatteryReport *batReport)
{

//	get_bat_voltage(pI2CHandle, batReport);
//	get_bat_current(pI2CHandle, batReport);
//	get_bat_SOC(pI2CHandle, batReport);
//	get_bat_SOH(pI2CHandle, batReport);
//	get_bat_remaining_capacity(pI2CHandle, batReport);
//	get_bat_full_capacity(pI2CHandle, batReport);
//	get_bat_temperature(pI2CHandle, batReport);
//	get_bat_time_to_empty(pI2CHandle, batReport);
//	get_bat_time_to_full(pI2CHandle, batReport);
}

void get_bat_SOC(I2C_Handle_t *pI2CHandle, BatteryReport *batReport)
{
	uint8_t rcv_buf[2];

	gauge_read_register(pI2CHandle, 0x02, &rcv_buf[0], 2);
    batReport->SOC =rcv_buf[0];
}

void get_bat_SOH(I2C_Handle_t *pI2CHandle, BatteryReport *batReport)
{
	uint8_t rcv_buf[2];

	gauge_read_register(pI2CHandle, 0x2E, &rcv_buf[0], 2);
    batReport->SOH =rcv_buf[0];
}

void get_bat_remaining_capacity(I2C_Handle_t *pI2CHandle, BatteryReport *batReport)
{
	uint8_t rcv_buf[2];

	gauge_read_register(pI2CHandle, 0x04, &rcv_buf[0], 2);
    batReport->remainingCapacity[0] =rcv_buf[0];
    batReport->remainingCapacity[1] =rcv_buf[1];
}

void get_bat_full_capacity(I2C_Handle_t *pI2CHandle, BatteryReport *batReport)
{
	uint8_t rcv_buf[2];

	gauge_read_register(pI2CHandle, 0x06, &rcv_buf[0], 2);
    batReport->fullCapacity[0] =rcv_buf[0];
    batReport->fullCapacity[1] =rcv_buf[1];
}

void get_bat_voltage(I2C_Handle_t *pI2CHandle, BatteryReport *batReport)
{
	uint8_t rcv_buf[2];

	gauge_read_register(pI2CHandle, 0x08, &rcv_buf[0], 2);
	batReport->voltage[0] = rcv_buf[0];
	batReport->voltage[1] = rcv_buf[1];

}

void get_bat_current(I2C_Handle_t *pI2CHandle, BatteryReport *batReport)
{
	uint8_t rcv_buf[2];

	gauge_read_register(pI2CHandle, 0x10, &rcv_buf[0], 2);
	batReport->current[0] = rcv_buf[0];
	batReport->current[1] = rcv_buf[1];
}

void get_bat_temperature(I2C_Handle_t *pI2CHandle, BatteryReport *batReport)
{
	uint8_t rcv_buf[2];

	gauge_read_register(pI2CHandle, 0x0C, &rcv_buf[0], 2);
    batReport->temperature[0] =rcv_buf[0];
    batReport->temperature[1] =rcv_buf[1];
}

void get_bat_time_to_empty(I2C_Handle_t *pI2CHandle, BatteryReport *batReport)
{
	uint8_t rcv_buf[2];

	gauge_read_register(pI2CHandle, 0x18, &rcv_buf[0], 2);
    batReport->timeToEmpty[0] =rcv_buf[0];
    batReport->timeToEmpty[1] =rcv_buf[1];
}

void get_bat_time_to_full(I2C_Handle_t *pI2CHandle, BatteryReport *batReport)
{
	uint8_t rcv_buf[2];

	gauge_read_register(pI2CHandle, 0x1A, &rcv_buf[0], 2);
    batReport->timeToFull[0] =rcv_buf[0];
    batReport->timeToFull[1] =rcv_buf[1];
}

void gauge_sealed(I2C_Handle_t *pI2CHandle)
{
	uint8_t buf[3] = { 0x00, 0x20, 0x00 };
	while(I2C_MasterSendDataIT(pI2CHandle, &buf[0], 3, GAUGE_SLAVE_ADDR, I2C_ENABLE_SR) != I2C_READY);
	delay(200000);

}

void gauge_unseal(I2C_Handle_t *pI2CHandle)
{
	uint8_t rcv_buffer[2];
	uint8_t buf[3] = { 0x00, 0x14, 0x04 };
	while(I2C_MasterSendDataIT(pI2CHandle, &buf[0], 3, GAUGE_SLAVE_ADDR, I2C_ENABLE_SR) != I2C_READY);
	delay(200000);
	buf[0] = 0x00;
	buf[1] = 0x72;
	buf[2] = 0x36;
	while(I2C_MasterSendDataIT(pI2CHandle, &buf[0], 3, GAUGE_SLAVE_ADDR, I2C_ENABLE_SR) != I2C_READY);
	delay(200000);
	memset(buf, 0, 3);
	while(I2C_MasterSendDataIT(pI2CHandle, &buf[0], 3, GAUGE_SLAVE_ADDR, I2C_ENABLE_SR) != I2C_READY);
	delay(200000);
	while(I2C_MasterSendDataIT(pI2CHandle, 0x00, 1, GAUGE_SLAVE_ADDR, I2C_ENABLE_SR) != I2C_READY);

	pI2CHandle->rxCmplt = RESET;
	while(I2C_MasterReceiveDataIT(pI2CHandle, rcv_buffer, 2, GAUGE_SLAVE_ADDR, I2C_ENABLE_SR) != I2C_READY);

	while(pI2CHandle->rxCmplt != SET)
	{

	}
	pI2CHandle->rxCmplt = RESET;
}

void gauge_reset(I2C_Handle_t *pI2CHandle)
{
	uint8_t buf[3] = { 0x00, 0x41, 0x00 };
	while(I2C_MasterSendDataIT(pI2CHandle, &buf[0], 3, GAUGE_SLAVE_ADDR, I2C_ENABLE_SR) != I2C_READY);
	delay(200000);
}

void gauge_full_sleep_enable(I2C_Handle_t *pI2CHandle)
{
	uint8_t buf[3] = { 0x00, 0x10, 0x00 };
	while(I2C_MasterSendDataIT(pI2CHandle, &buf[0], 3, GAUGE_SLAVE_ADDR, I2C_ENABLE_SR) != I2C_READY);
	delay(200000);
}

void gauge_get_device_type(I2C_Handle_t *pI2CHandle, uint8_t* rcv_buf)
{
	uint8_t buf[3] = { 0x00, 0x01, 0x00 };

	while(I2C_MasterSendDataIT(pI2CHandle, &buf[0], 3, GAUGE_SLAVE_ADDR, I2C_ENABLE_SR) != I2C_READY);
	delay(200000);

	gauge_read_register(pI2CHandle, 0x00, rcv_buf, 2);
}

void gauge_get_firmware_version(I2C_Handle_t *pI2CHandle, uint8_t* rcv_buf)
{
	uint8_t buf[3] = { 0x00, 0x02, 0x00 };

	while(I2C_MasterSendDataIT(pI2CHandle, &buf[0], 3, GAUGE_SLAVE_ADDR, I2C_ENABLE_SR) != I2C_READY);
	delay(200000);

	gauge_read_register(pI2CHandle, 0x00, rcv_buf, 2);
}

void gauge_flash_write_binary(I2C_Handle_t *pI2CHandle, uint8_t* buf, uint8_t len)
{
	uint8_t addr = buf[1];
	while(I2C_MasterSendDataIT(pI2CHandle, &buf[2], len, addr, I2C_ENABLE_SR) != I2C_READY);
}

bool gauge_flash_read_binary(I2C_Handle_t *pI2CHandle, uint8_t* buf, uint8_t len)
{
	uint8_t addr = buf[1];
	uint8_t reg_addr = buf[2];
	uint8_t read_size = buf[3];
	uint8_t buffer[read_size];

	while(I2C_MasterSendDataIT(pI2CHandle, &reg_addr, 1, addr, I2C_ENABLE_SR) != I2C_READY);

	pI2CHandle->rxCmplt = RESET;
	while(I2C_MasterReceiveDataIT(pI2CHandle, buffer, read_size, addr, I2C_ENABLE_SR) != I2C_READY);

	while(pI2CHandle->rxCmplt != SET)
	{

	}
	pI2CHandle->rxCmplt = RESET;

    for (uint8_t i = 0; i < read_size; i++)
    {
        if (buffer[i] != buf[i + 3])
        {
            return false;
        }
    }

    return true;
}



