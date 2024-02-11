/*
 * Version: 0.1.1
 * Workflow version: 0.0.1
 *
 * stm32f030x4_usart_driver.h
 *
 * Created on: Jun 30, 2023
 * Author: Ahmed Ali
 * Company: espresso Displays
 * Project: Volta
 */

#ifndef INC_STM32F030X4_USART_DRIVER_H_
#define INC_STM32F030X4_USART_DRIVER_H_

#include "stm32f030x4.h"

/*
 * Configuration structure for USARTx peripheral
 */
typedef struct
{
	uint8_t USART_Mode;
	uint32_t USART_Baud;
	// uint8_t USART_NoOfStopBits;
	// uint8_t USART_WordLength;
	// uint8_t USART_ParityControl;
	// uint8_t USART_HWFlowControl;
}USART_Config_t;

/*
 * Handle structure for USARTx peripheral
 */
typedef struct
{
	USART_RegDef_t *pUSARTx;
	USART_Config_t   USART_Config;
	uint8_t *pTxBuffer;
	uint8_t *pRxBuffer;
	uint32_t TxLen;
	uint32_t RxLen;
	uint8_t TxBusyState;
	uint8_t RxBusyState;
}USART_Handle_t;

#define USART_STD_BAUD_9600					9600
#define USART_STD_BAUD_115200 				115200

/*
 * USART flags
 */

#define USART_FLAG_TXE 			( 1 << USART_SR_TXE)
#define USART_FLAG_RXNE 		( 1 << USART_SR_RXNE)
#define USART_FLAG_TC 			( 1 << USART_SR_TC)

/*
 * Application states
 */
#define USART_BUSY_IN_RX 1
#define USART_BUSY_IN_TX 2
#define USART_READY 0


#define USART_EVENT_TX_CMPLT  0
#define	USART_EVENT_RX_CMPLT  1
#define	USART_EVENT_IDLE      2
#define	USART_EVENT_CTS       3
#define	USART_EVENT_PE        4
#define	USART_ERR_FE     	  5
#define	USART_ERR_NE    	  6
#define	USART_ERR_ORE    	  7


/*
 * Init and De-init
 */
void USART_Init(USART_Handle_t *pUSARTHandle);
void USART_DeInit(USART_Handle_t *pUSARTHandle);

/*
 * Data Send and Receive
 */
void USART_ReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len);
void USART_SendData(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len);
uint8_t USART_SendDataIT(USART_Handle_t *pUSARTHandle,uint8_t *pTxBuffer, uint32_t Len);
uint8_t USART_ReceiveDataIT(USART_Handle_t *pUSARTHandle,uint8_t *pRxBuffer, uint32_t Len);

/*
 * IRQ Configuration and ISR handling
 */
void USART_IRQHandling(USART_Handle_t *pUSARTHandle);

/*
 * Other Peripheral Control APIs
 */

uint8_t USART_GetFlagStatus(USART_RegDef_t *pUSARTx, uint8_t StatusFlagName);
void USART_ClearFlag(USART_RegDef_t *pUSARTx, uint16_t StatusFlagName);
void USART_PeripheralControl(USART_RegDef_t *pUSARTx, uint8_t EnOrDi);

/*
 * Application Callbacks
 */
void USART_ApplicationEventCallback(USART_Handle_t *pUSARTHandle,uint8_t ApEv);

#endif /* INC_STM32F030X4_USART_DRIVER_H_ */
