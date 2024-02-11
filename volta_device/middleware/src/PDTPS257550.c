/*
 * Version: 0.1.1
 * Workflow version: 0.0.1
 *
 * file: PDTPS257550.c
 *
 * Created on: Jun 30, 2023
 * Author: Ahmed Ali
 * Company: espresso Displays
 * Project: Volta
 */


#include "PDTPS257550.h"

static uint8_t pd_get_command_response(I2C_Handle_t *pI2CHandle);
static uint8_t get_task_result(I2C_Handle_t *pI2Handle);
static __vo uint8_t regData[5];

uint8_t plugAction = 0;
uint8_t power_direction = 2;

uint8_t pd_cmd_status = PD_CMD_READY;
uint8_t pd_cmd_task_result;

void pd_read_register(I2C_Handle_t *pI2CHandle, uint8_t reg_addr, uint8_t *buffer, uint8_t size)
{
   while(I2C_MasterSendDataIT(pI2CHandle, &reg_addr, 1, PD_SLAVE_ADDR, I2C_ENABLE_SR) != I2C_READY);

   pI2CHandle->rxCmplt = RESET;
   while(I2C_MasterReceiveDataIT(pI2CHandle, buffer, size, PD_SLAVE_ADDR, I2C_ENABLE_SR) != I2C_READY);

   while(pI2CHandle->rxCmplt != SET)
   {

   }
   pI2CHandle->rxCmplt = RESET;
}

void pd_write_command(I2C_Handle_t *pI2CHandle, uint8_t reg_addr, uint8_t *data, uint8_t size)
{

    while(I2C_MasterSendDataIT(pI2CHandle, data, size , PD_SLAVE_ADDR, I2C_ENABLE_SR) != I2C_READY);
    delay(200000);
}

static uint8_t pd_get_command_response(I2C_Handle_t *pI2CHandle)
{
   uint8_t rcv_buf[5];

   while(1)
   {
       delay(200000);
       pd_read_register(pI2CHandle, PD_CMD1, &rcv_buf[0], 5);
       if (rcv_buf[0] == 0x4 && rcv_buf[1] == 0x21 && rcv_buf[2] == 0x43 && rcv_buf[3] == 0x4D && rcv_buf[4] == 0x44)  //!CMD
       {
           return PD_INVALID_CMD;
       }else if(rcv_buf[0] == 0x4 && rcv_buf[1] == 0x00 && rcv_buf[2] == 0x00 && rcv_buf[3] == 0x00 && rcv_buf[4] == 0x00)
       {
           return PD_CMD_EXECUTED;
       }
   }
   return PD_INVALID_CMD;
}

static uint8_t get_task_result(I2C_Handle_t *pI2CHandle)
{
	uint8_t task_result;
	uint8_t command_execute = pd_get_command_response(pI2CHandle);
	if(command_execute == PD_CMD_EXECUTED)
	{
		uint8_t rcv_buf[2];
	    pd_read_register(pI2CHandle, PD_DATA1, &rcv_buf[0], 2);
	    task_result = rcv_buf[1] & 0x0F;
	    return task_result;
	}
}

static uint8_t pd_get_command_responseIT(I2C_Handle_t *pI2CHandle)
{
   uint8_t rcv_buf[5];
   pd_read_register(pI2CHandle, PD_CMD1, &rcv_buf[0], 5);
   if (rcv_buf[0] == 0x4 && rcv_buf[1] == 0x21 && rcv_buf[2] == 0x43 && rcv_buf[3] == 0x4D && rcv_buf[4] == 0x44)  //!CMD
   {
       return PD_INVALID_CMD;
   }else if(rcv_buf[0] == 0x4 && rcv_buf[1] == 0x00 && rcv_buf[2] == 0x00 && rcv_buf[3] == 0x00 && rcv_buf[4] == 0x00)
   {
       return PD_CMD_EXECUTED;
   }
   return PD_INVALID_CMD;
}

uint8_t pd_4cc_command(I2C_Handle_t *pI2CHandle, uint8_t command)
{
    uint8_t cmd[5];
    uint8_t command_execute, task_result;

    if(command == PD_PR_SWAP_TO_SINK)
    {
       cmd[0] = PD_CMD1;
       cmd[1] = 0x04;
       cmd[2] = 0x53;
       cmd[3] = 0x57;
       cmd[4] = 0x53;
       cmd[5] = 0x6B;

       pd_write_command(pI2CHandle, PD_CMD1, &cmd[0], 6);

       task_result = get_task_result(pI2CHandle);

       return task_result;
    }else if(command == PD_PR_SWAP_TO_SOURCE)
    {
       cmd[0] = PD_CMD1;
       cmd[1] = 0x04;
       cmd[2] = 0x53;
       cmd[3] = 0x57;
       cmd[4] = 0x53;
       cmd[5] = 0x72;

       pd_write_command(pI2CHandle, PD_CMD1, &cmd[0], 6);

       task_result = get_task_result(pI2CHandle);

       return task_result;
    }else if(command == PD_GET_SRC_CAPS)
    {
        cmd[0] = PD_CMD1;
        cmd[1] = 0x04;
        cmd[2] = 0x47;
        cmd[3] = 0x53;
        cmd[4] = 0x72;
        cmd[5] = 0x43;

        pd_write_command(pI2CHandle, PD_CMD1, &cmd[0], 6);

        task_result = get_task_result(pI2CHandle);

        return task_result;
    }else if(command == PD_PR_SWAP_TO_UFP)
    {
        cmd[0] = PD_CMD1;
        cmd[1] = 0x04;
        cmd[2] = 0x53;
        cmd[3] = 0x57;
        cmd[4] = 0x55;
        cmd[5] = 0x46;

        pd_write_command(pI2CHandle, PD_CMD1, &cmd[0], 6);

        task_result = get_task_result(pI2CHandle);

        return task_result;
    }else if(command == PD_CLEAR_DEAD_FLAG)
        {
            cmd[0] = PD_CMD1;
            cmd[1] = 0x04;
            cmd[2] = 0x44;
            cmd[3] = 0x42;
            cmd[4] = 0x66;
            cmd[5] = 0x67;

            pd_write_command(pI2CHandle, PD_CMD1, &cmd[0], 6);

            task_result = get_task_result(pI2CHandle);

            return task_result;
    }else if(command == PD_RESET)
       {
        cmd[0] = PD_CMD1;
        cmd[1] = 0x04;
        cmd[2] = 0x47;
        cmd[3] = 0x41;
        cmd[4] = 0x49;
        cmd[5] = 0x44;

        pd_write_command(pI2CHandle, PD_CMD1, &cmd[0], 6);

//        task_result = get_task_result(pI2CHandle);

        return 0;
    }
    return PD_TSK_FAILED;
}

uint8_t pd_reset(I2C_Handle_t *pI2CHandle)
{
	uint8_t res = pd_4cc_command(pI2CHandle, PD_RESET);
	return res;
}

uint8_t pd_get_plug_detect(I2C_Handle_t *pI2CHandle)
{
    pd_read_register(pI2CHandle, PD_STATUS, &regData[0], 5);
    return (regData[1] & 0x1);
}

uint8_t pd_get_power_direction(I2C_Handle_t *pI2CHandle)
{
    pd_read_register(pI2CHandle, PD_STATUS, &regData[0], 5);
    return ( (regData[1] >> 5) & 0x1);
}

uint8_t pd_swap_to_source(I2C_Handle_t *pI2CHandle)
{
    uint8_t res = pd_4cc_command(pI2CHandle, PD_PR_SWAP_TO_SOURCE);
    return res;
}

uint8_t pd_swap_to_sink(I2C_Handle_t *pI2CHandle)
{
    uint8_t res = pd_4cc_command(pI2CHandle, PD_PR_SWAP_TO_SINK);
    return res;
}

uint8_t pd_swap_to_UFP(I2C_Handle_t *pI2CHandle)
{
	 uint8_t res = pd_4cc_command(pI2CHandle, PD_PR_SWAP_TO_UFP);
	 return res;
}

uint8_t pd_clear_dead_flag(I2C_Handle_t *pI2CHandle)
{
	 uint8_t res = pd_4cc_command(pI2CHandle, PD_CLEAR_DEAD_FLAG);
	 return res;
}

uint8_t pd_get_dead_flag(I2C_Handle_t *pI2CHandle)
{
	uint8_t buf[6];
	__vo uint8_t flag_val = 0xFF;
	pd_read_register(pI2CHandle, PD_POWER_PATH_STATUS, &buf[0], 6);
	flag_val = (buf[5] >> 6) & 0x03;
	if(flag_val == 1){
		flag_val = 0;
	}else if(flag_val == 2){
		flag_val = 1;
	}

	return flag_val;
}

uint8_t pd_get_mode(I2C_Handle_t *pI2CHandle)
{
	__vo uint8_t pd_mode = 0xFF;
	pd_read_register(pI2CHandle, PD_MODE, &regData[0], 5);
    if(regData[1] == 0x41 && regData[2] == 0x50 && regData[3] == 0x50 && regData[4] == 0x20){
    	pd_mode = PD_MODE_APP;
    }else if(regData[1] == 0x50 && regData[2] == 0x54 && regData[3] == 0x43 && regData[4] == 0x48){
    	pd_mode = PD_MODE_PATCH;
    }else if(regData[1] == 0x42 && regData[2] == 0x4F && regData[3] == 0x4F && regData[4] == 0x54){
    	pd_mode = PD_MODE_BOOT;
    }
    return pd_mode;
}

void pd_get_rx_source_caps(I2C_Handle_t *pI2CHandle, uint32_t* PDObuf,  uint8_t* num)
{
    uint8_t buf[30];
    uint8_t numPDOs;
    pd_read_register(pI2CHandle, PD_RX_SOURCE_CAPS, &buf[0], 30);

    numPDOs = buf[1] & 0x7;
    *num = numPDOs;

    uint32_t PDOs[numPDOs]; 
    for(uint8_t i=0; i < numPDOs; i++){
        PDOs[i] = (uint32_t)((buf[2 + i * 4 + 3] << 24) |
                            (buf[2 + i * 4 + 2] << 16) |
                            (buf[2 + i * 4 + 1] << 8) |
                            (buf[2 + i * 4 + 0]));
    }

    memcpy(PDObuf, PDOs, numPDOs * sizeof(uint32_t));
}

void pd_get_rx_sink_caps(I2C_Handle_t *pI2CHandle, uint32_t* PDObuf, uint8_t* num)
{
    uint8_t buf[30];
    uint8_t numPDOs;
    pd_read_register(pI2CHandle, PD_RX_SINK_CAPS, &buf[0], 30);

    numPDOs = buf[1] & 0x7;
    *num = numPDOs;

    uint32_t PDOs[numPDOs]; 
    for(uint8_t i=0; i < numPDOs; i++){
        PDOs[i] = ((uint32_t)buf[i * 4 + 3] << 24) |
                  ((uint32_t)buf[i * 4 + 2] << 16) |
                  ((uint32_t)buf[i * 4 + 1] << 8) |
                  ((uint32_t)buf[i * 4 + 0]);
    }

    memcpy(PDObuf, PDOs, numPDOs * sizeof(uint32_t));
}

// void pd_set_tx_source_caps(I2C_Handle_t *pI2CHandle)
// {

// }

// void pd_set_tx_sink_caps(I2C_Handle_t *pI2CHandle)
// {
    
// }

uint32_t pd_get_active_contract_pdo(I2C_Handle_t *pI2CHandle, uint8_t *pdo_buf)
{
//    uint8_t buf[5];
    uint32_t activePDO;
    pd_read_register(pI2CHandle, PD_ACTIVE_CONTRACT_PDO, &pdo_buf[0], 5);
    
    activePDO = ((uint32_t)pdo_buf[4] << 24) |
                ((uint32_t)pdo_buf[3] << 16) |
                ((uint32_t)pdo_buf[2] << 8) |
                ((uint32_t)pdo_buf[1]);
    return activePDO;
}

uint8_t pd_get_sleep_config(I2C_Handle_t *pI2CHandle)
{
    uint8_t buf[2];
    pd_read_register(pI2CHandle, PD_SLEEP_CONFIG, &buf, 2);
    return (buf[1] & 0x1);
}

uint8_t pd_set_sleep_config(I2C_Handle_t *pI2CHandle, uint8_t EnorDi)
{
    uint8_t result;
    uint8_t buf[3] = {PD_SLEEP_CONFIG, 0x1, EnorDi};
    pd_write_command(pI2CHandle, PD_SLEEP_CONFIG, &buf[0], 3);

    result = pd_get_sleep_config(pI2CHandle);
    return result;
}

void pd_interrupt_mask(I2C_Handle_t *pI2CHandle)
{
    uint8_t buf[13] = {PD_INT_MASK1, 11};
    memset(&buf[2], 0x0, 11);
    buf[2] = 0x8;
    pd_write_command(pI2CHandle, PD_SLEEP_CONFIG, &buf[0], 13);

}

void pd_interrupt_clear(I2C_Handle_t *pI2CHandle, uint8_t interr, uint8_t* int_buf)
{
    uint8_t buf[13] = {PD_INT_CLEAR1, 11};
    memset(&buf[2], 0x0, 11);

    if(interr == PD_INT_PlugInsertOrRemoval)
    {
        buf[2] = 0x8;
        pd_write_command(pI2CHandle, PD_INT_CLEAR1, &buf[0], 13);

    }else if(interr == PD_INT_PRSwapComplete)
    {
        buf[2] = 0x10;
        pd_write_command(pI2CHandle, PD_INT_CLEAR1, &buf[0], 13);

    }
}

void pd_interrupt_clear_all(I2C_Handle_t *pI2CHandle)
{
    uint8_t buf[13] = {PD_INT_CLEAR1, 11};
    memset(&buf[2], 0xFF, 11);
    pd_write_command(pI2CHandle, PD_INT_CLEAR1, &buf[0], 13);

}

void pd_IRQHandling(I2C_Handle_t *pI2CHandle)
{
    uint8_t buf[12];
    pd_read_register(pI2CHandle, PD_INT_EVENT1, &buf[0], 12);

    if( (buf[1] >> PD_INT_PlugInsertOrRemoval) & 0x1){
        pd_interrupt_clear(pI2CHandle, PD_INT_PlugInsertOrRemoval, &buf[0]);
        delay(200000);
        plugAction = pd_get_plug_detect(pI2CHandle);
        delay(200000);
        if(plugAction == PD_PLUG_CONNECTED){
        	power_direction = pd_get_power_direction(pI2CHandle);
        	if(power_direction == SOURCING){
        		delay(200000);
        		pd_swap_to_UFP(pI2CHandle);
        	}else if(power_direction == SINKING){

        	}
        }else{
            power_direction = 2;
        }
    }
}

void pd_patch_eprom(I2C_Handle_t *pI2CHandle, uint8_t* buf, uint8_t len)
{
	while(I2C_MasterSendDataIT(pI2CHandle, buf, len, EPROM_SLAVE_ADDR, I2C_ENABLE_SR) != I2C_READY);
}

void pd_set_eeprom_pointer(I2C_Handle_t *pI2CHandle)
{
	uint8_t buf[2] = {0x80, 0x00};
	while(I2C_MasterSendDataIT(pI2CHandle, buf, 2, EPROM_SLAVE_ADDR, I2C_ENABLE_SR) != I2C_READY);
	delay(200000);
}

void pd_pbms(I2C_Handle_t *pI2CHandle)
{
	uint8_t cmd[5];

	cmd[0] = PD_CMD1;
	cmd[1] = 0x04;
	cmd[2] = 0x50;
	cmd[3] = 0x42;
	cmd[4] = 0x4D;
	cmd[5] = 0x73;

    pd_write_command(pI2CHandle, PD_CMD1, &cmd[0], 6);
}

