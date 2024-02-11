/*
 * Version: 0.1.1
 * Workflow version: 0.0.1
 *
 * file: stm32f030x4_i2c_driver.h
 *
 * Created on: Jun 30, 2023
 * Author: Ahmed Ali
 * Company: espresso Displays
 * Project: Volta
 */

#ifndef INC_STM32F030X4_I2C_DRIVER_H_
#define INC_STM32F030X4_I2C_DRIVER_H_

#include "stm32f030x4.h"


/*
 * Configuration structure for I2Cx peripheral
 */
// typedef struct
// {
// 	// uint32_t I2C_SCLSpeed;
// 	// uint8_t  I2C_DeviceAddress;
// 	// uint8_t  I2C_AckControl;
// 	// uint8_t  I2C_FMDutyCycle;

// }I2C_Config_t;

/*
 *Handle structure for I2Cx peripheral
 */
typedef struct
{
	I2C_RegDef_t 	*pI2Cx;
	// I2C_Config_t 	I2C_Config;
	uint8_t 		*pTxBuffer; 
	uint8_t 		*pRxBuffer;	
	uint32_t 		TxLen;		
	uint32_t 		RxLen;		
	uint8_t 		TxRxState;	
	uint8_t 		DevAddr;	
    uint32_t        RxSize;		
    // uint8_t         Sr;
	uint8_t			rxCmplt;		
}I2C_Handle_t;


/*
 * I2C application states
 */
#define I2C_READY 					0
#define I2C_BUSY_IN_RX 				1
#define I2C_BUSY_IN_TX 				2

/*
 * @I2C_SCLSpeed
 */
#define I2C_SCL_SPEED_SM 	100000


/*
 * I2C related status flags definitions
 */
#define I2C_FLAG_TXE   		( 1 << I2C_SR1_TXE)
#define I2C_FLAG_TXIS		( 1 << I2C_SR1_TXIS)
#define I2C_FLAG_RXNE   	( 1 << I2C_SR1_RXNE)
#define I2C_FLAG_OVR  		( 1 << I2C_SR1_OVR)
#define I2C_FLAG_ARLO 		( 1 << I2C_SR1_ARLO)
#define I2C_FLAG_BERR 		( 1 << I2C_SR1_BERR)
#define I2C_FLAG_STOPF 		( 1 << I2C_SR1_STOPF)
#define I2C_FLAG_ADD10 		( 1 << I2C_SR1_ADD10)
#define I2C_FLAG_ADDR 		( 1 << I2C_SR1_ADDR)
#define I2C_FLAG_TIMEOUT 	( 1 << I2C_SR1_TIMEOUT)
#define I2C_FLAG_TC			( 1 << I2C_SR1_TC)

#define I2C_DISABLE_SR  	RESET
#define I2C_ENABLE_SR   	SET

/*
 *  I2C application events macros
 */
#define	I2C_EV_TRNS_CMPLT		0
#define I2C_EV_STOP				1
#define I2C_ERROR_BERR			2
#define I2C_ERROR_ARLO			3
#define I2C_ERROR_OVR			4
#define I2C_ERROR_PECERR		5
#define I2C_ERROR_TIMEOUT		6


/*
 * Init and De-init
 */
void I2C_Init(I2C_Handle_t *pI2CHandle);
void I2C_DeInit(I2C_RegDef_t *pI2Cx);


/*
 * Data Send and Receive
 */
uint8_t I2C_MasterSendDataIT(I2C_Handle_t *pI2CHandle,uint8_t *pTxbuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t Sr);
uint8_t I2C_MasterReceiveDataIT(I2C_Handle_t *pI2CHandle,uint8_t *pRxBuffer, uint8_t Len, uint8_t SlaveAddr,uint8_t Sr);

/*
 * IRQ Configuration and ISR handling
 */

void I2C_EV_IRQHandling(I2C_Handle_t *pI2CHandle);

void I2C_CloseReceiveData(I2C_Handle_t *pI2CHandle);
void I2C_CloseSendData(I2C_Handle_t *pI2CHandle);



/*
 * Other Peripheral Control APIs
 */
uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx , uint32_t FlagName);
void  I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx);
void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx);

/*
* Application callback
*/
void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t AppEv);

#endif /* INC_STM32F030X4_I2C_DRIVER_H_ */
