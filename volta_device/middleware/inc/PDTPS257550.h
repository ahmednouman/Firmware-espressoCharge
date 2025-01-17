#ifndef INC_PDTPS257550_H_
#define INC_PDTPS257550_H_

#include<stdio.h>
#include<string.h>
#include "stm32f030x4.h"
#include "stm32f030x4_i2c_driver.h"
#include "generic_api.h"
#include "system_config.h"


extern uint8_t pd_cmd_status;
extern uint8_t pd_cmd_task_result;

extern uint8_t plugAction;
extern uint8_t power_direction;

/*
* Device Register Addresses
*/
#define PD_MODE     0x03
#define PD_TYPE     0x04
#define PD_CUSTUSE     0x06
#define PD_CMD1     0x08
#define PD_DATA1     0x09
#define PD_DEVICE_CAPABILITIES     0x0D
#define PD_VERSION     0x0F
#define PD_INT_EVENT1     0x14
#define PD_INT_MASK1     0x16
#define PD_INT_CLEAR1     0x18
#define PD_STATUS     0x1A
#define PD_POWER_PATH_STATUS     0x26
#define PD_PORT_CONTROL     0x29
#define PD_BOOT_STATUS     0x2D
#define PD_BUILD_DESCRIPTION     0x2E
#define PD_DEVICE_INFO     0x2F
#define PD_RX_SOURCE_CAPS     0x30
#define PD_RX_SINK_CAPS     0x31
#define PD_TX_SOURCE_CAPS     0x32
#define PD_TX_SINK_CAPS     0x33
#define PD_ACTIVE_CONTRACT_PDO     0x34
#define PD_ACTIVE_CONTACT_RDO     0x35
#define PD_POWER_STATUS     0x3F
#define PD_PD_STATUS     0x40
#define PD_TYPEC_STATE     0x69
#define PD_GPIO_STATUS     0x72
#define PD_SLEEP_CONFIG     (uint8_t)0x70

#define PD_MODE_APP 0
#define PD_MODE_PATCH 1
#define PD_MODE_BOOT 2

#define PD_INT_PlugInsertOrRemoval    3
#define PD_INT_PRSwapComplete    4
#define PD_INT_StatusUpdate    26
#define PD_INT_PDStatusUpdate    27
#define PD_INT_CMDComplete    30


#define PD_PR_SWAP_TO_SINK   0
#define PD_PR_SWAP_TO_SOURCE   1
#define PD_GET_SRC_CAPS   2
#define PD_PR_SWAP_TO_UFP   3
#define PD_RESET			4
#define PD_CLEAR_DEAD_FLAG  5

#define SOURCING 1
#define SINKING 0

#define PD_CMD_READY ((uint8_t) 0)
#define PD_CMD_IN_PROGRESS ((uint8_t) 1)

#define PD_CMD_EXECUTED        0
#define PD_INVALID_CMD      1

#define PD_TSK_CMPL_SUCC  0x0
#define PD_TSK_TIMEOUT      0x1
#define PD_TSK_REJECT       0x3
#define PD_TSK_LOCKED       0x4
#define PD_TSK_FAILED       0x5

#define SLEEP_MODE_ACTIVE   1
#define SLEEP_MODE_INACTIVE   0

#define PD_PDO_FORMAT_HEX   0
#define PD_PDO_FORMAT_STRING   1

/*
* bit fields of STATUS Register
*/
#define PD_PortRole     5
#define PD_PlugPresent     0

#define PD_NO_PLUG      0
#define PD_PLUG_CONNECTED    1

/*
* bit fields of POWER_PATH_STATUS Register
*/
#define PD_PowerSource     14

/*
* bit fields of BOOT_STATUS Register
*/
#define PD_PatchConfigSource     29

/*
* bit fields of ACTIVE_CONTRACT_PDO Register
*/
#define PD_ActivePDO     0

/*
* bit fields of ACTIVE_CONTRACT_PDO Register
*/
#define PD_PowerConnection     0
#define PD_SourceSink     1
#define PD_TypeCCurrent     2
#define PD_ChargerDetectStatus     4

/*
* bit fields of PD_STATUS Register
*/
#define PD_PortType    4
#define PD_SoftResetDetails    8
#define PD_HardResetDetails     16
#define PD_CErrorRecoveryDetails     22

/*
* bit fields of TYPEC_STATE Register
*/
#define PD_TypeCPortState    24

/*
* bit fields of PD_SLEEP_CONFIG Register
*/
#define SleepModeAllowed    0


/*
* APIs Prototypes
*/
void pd_read_register(I2C_Handle_t *pI2CHandle, uint8_t reg_addr, uint8_t *buffer, uint8_t size);
void pd_write_command(I2C_Handle_t *pI2CHandle, uint8_t reg_addr, uint8_t *data, uint8_t size);

uint8_t pd_4cc_command(I2C_Handle_t *pI2CHandle, uint8_t command);

uint8_t pd_get_plug_detect(I2C_Handle_t *pI2CHandle);
uint8_t pd_get_power_direction(I2C_Handle_t *pI2CHandle);

uint8_t pd_reset(I2C_Handle_t *pI2CHandle);
uint8_t pd_swap_to_source(I2C_Handle_t *pI2CHandle);
uint8_t pd_swap_to_sink(I2C_Handle_t *pI2CHandle);
uint8_t pd_swap_to_UFP(I2C_Handle_t *pI2CHandle);
uint8_t pd_clear_dead_flag(I2C_Handle_t *pI2CHandle);

uint8_t pd_get_dead_flag(I2C_Handle_t *pI2CHandle);
uint8_t pd_get_mode(I2C_Handle_t *pI2CHandle);

void pd_get_rx_source_caps(I2C_Handle_t *pI2CHandle, uint32_t* PDObuf, uint8_t* num);
void pd_get_rx_sink_caps(I2C_Handle_t *pI2CHandle, uint32_t* PDObuf, uint8_t* num);
// void pd_set_tx_source_caps(I2C_Handle_t *pI2CHandle);
// void pd_set_tx_sink_caps(I2C_Handle_t *pI2CHandle);

uint32_t pd_get_active_contract_pdo(I2C_Handle_t *pI2CHandle, uint8_t *pdo_buf);

uint8_t pd_get_sleep_config(I2C_Handle_t *pI2CHandle);
uint8_t pd_set_sleep_config(I2C_Handle_t *pI2CHandle, uint8_t EnorDi);


void pd_interrupt_mask(I2C_Handle_t *pI2CHandle);
void pd_interrupt_clear(I2C_Handle_t *pI2CHandle, uint8_t interr, uint8_t* int_buf);
void pd_interrupt_clear_all(I2C_Handle_t *pI2CHandle);
void pd_IRQHandling(I2C_Handle_t *pI2CHandle);

void pd_patch_eprom(I2C_Handle_t *pI2CHandle, uint8_t* buf, uint8_t len);
void pd_set_eeprom_pointer(I2C_Handle_t *pI2CHandle);

void pd_pbms(I2C_Handle_t *pI2CHandle);
#endif /* INC_PDTPS257550_H_ */
