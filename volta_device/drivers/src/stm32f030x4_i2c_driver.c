/*
 * Version: 0.1.1
 * Workflow version: 0.0.1
 *
 * file: stm32f030x4_i2c_driver.c
 *
 * Created on: Jun 30, 2023
 * Author: Ahmed Ali
 * Company: espresso Displays
 * Project: Volta
 */

#include "stm32f030x4_i2c_driver.h"

static void I2C_ExecuteAddressPhaseWrite(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr);
static void I2C_ExecuteAddressPhaseRead(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr);

static void I2C_DisableInterrupts(I2C_Handle_t *pI2CHandle);

void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx)
{
	pI2Cx->CR2 |= ( 1 << I2C_CR2_START);
}

static void I2C_ExecuteAddressPhaseWrite(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr)
{
	SlaveAddr = SlaveAddr << 1;
	SlaveAddr &= ~(1); 
    pI2Cx->CR2 |= SlaveAddr;

	pI2Cx->CR2 &= ~( 1 << I2C_CR2_RD_WRN);
}

static void I2C_ExecuteAddressPhaseRead(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr)
{
	SlaveAddr = SlaveAddr << 1;
	SlaveAddr |= 1; 
	pI2Cx->CR2 |= SlaveAddr;

	pI2Cx->CR2 |= ( 1 << I2C_CR2_RD_WRN);
}

 void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx)
{
	pI2Cx->CR2 |= ( 1 << I2C_CR2_STOP);
}

void I2C_Init(I2C_Handle_t *pI2CHandle)
{
	uint32_t tempreg = 0 ;

	I2C1_PCLK_EN();

    pI2CHandle->pI2Cx->CR1 &= ~(1 << I2C_CR1_PE);

	pI2CHandle->pI2Cx->CR1 |= ( 1 << I2C_CR1_NOSTRETCH);

	pI2CHandle->pI2Cx->CR2 |= (1 << I2C_CR2_AUTOEND);

    pI2CHandle->pI2Cx->TIMINGR = 0x2000090E;
}

void I2C_DeInit(I2C_RegDef_t *pI2Cx)
{

}

uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx , uint32_t FlagName)
{
	if(pI2Cx->ISR & FlagName)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}

static void I2C_DisableInterrupts(I2C_Handle_t *pI2CHandle)
{
	pI2CHandle->pI2Cx->CR1 &= ~( 1 << I2C_CR1_TXIE);
	pI2CHandle->pI2Cx->CR1 &= ~( 1 << I2C_CR1_RXIE);
	pI2CHandle->pI2Cx->CR1 &= ~( 1 << I2C_CR1_STOPIE);
	pI2CHandle->pI2Cx->CR1 &= ~( 1 << I2C_CR1_TCIE);

	// pI2CHandle->pI2Cx->CR1 &= ~( 1 << I2C_CR1_ERRIE);
}

void I2C_CloseReceiveData(I2C_Handle_t *pI2CHandle)
{
	I2C_DisableInterrupts(pI2CHandle);

    pI2CHandle->TxRxState = I2C_READY;
	pI2CHandle->pRxBuffer = NULL;
	pI2CHandle->RxLen = 0;
	pI2CHandle->RxSize = 0;
}

void I2C_CloseSendData(I2C_Handle_t *pI2CHandle)
{
	I2C_DisableInterrupts(pI2CHandle);

	pI2CHandle->TxRxState = I2C_READY;
	pI2CHandle->pTxBuffer = NULL;
	pI2CHandle->TxLen = 0;
}


uint8_t I2C_MasterSendDataIT(I2C_Handle_t *pI2CHandle,uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t Sr)
{
	uint8_t busystate = pI2CHandle->TxRxState;
	if( (busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX))
	{
		pI2CHandle->pTxBuffer = pTxBuffer;
		pI2CHandle->TxLen = Len;
		pI2CHandle->TxRxState = I2C_BUSY_IN_TX;
		pI2CHandle->DevAddr = SlaveAddr;
//		pI2CHandle->Sr = Sr;
 
		pI2CHandle->pI2Cx->CR1 |= ( 1 << I2C_CR1_TXIE);
		pI2CHandle->pI2Cx->CR1 |= ( 1 << I2C_CR1_RXIE);
		pI2CHandle->pI2Cx->CR1 |= ( 1 << I2C_CR1_STOPIE);
		pI2CHandle->pI2Cx->CR1 |= ( 1 << I2C_CR1_TCIE);

		// pI2CHandle->pI2Cx->CR1 |= ( 1 << I2C_CR1_ERRIE);
			
		I2C_ExecuteAddressPhaseWrite(pI2CHandle->pI2Cx,SlaveAddr);

	    pI2CHandle->pI2Cx->CR2 |= (Len << 16);

		I2C_GenerateStartCondition(pI2CHandle->pI2Cx);
	}

	return busystate;
}

uint8_t I2C_MasterReceiveDataIT(I2C_Handle_t *pI2CHandle,uint8_t *pRxBuffer, uint8_t Len, uint8_t SlaveAddr,uint8_t Sr)
{
	uint8_t busystate = pI2CHandle->TxRxState;
	if( (busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX))
	{
		pI2CHandle->pRxBuffer = pRxBuffer;
		pI2CHandle->RxLen = Len;
		pI2CHandle->TxRxState = I2C_BUSY_IN_RX;
		pI2CHandle->RxSize = Len;
		pI2CHandle->DevAddr = SlaveAddr;
//		pI2CHandle->Sr = Sr;
		pI2CHandle->rxCmplt = 0;

		pI2CHandle->pI2Cx->CR1 |= ( 1 << I2C_CR1_TXIE);
		pI2CHandle->pI2Cx->CR1 |= ( 1 << I2C_CR1_RXIE);
		pI2CHandle->pI2Cx->CR1 |= ( 1 << I2C_CR1_STOPIE);
		pI2CHandle->pI2Cx->CR1 |= ( 1 << I2C_CR1_TCIE);

		// pI2CHandle->pI2Cx->CR1 |= ( 1 << I2C_CR1_ERRIE);

		I2C_ExecuteAddressPhaseRead(pI2CHandle->pI2Cx,SlaveAddr);

		pI2CHandle->pI2Cx->CR2 |= (Len << 16);
		
		I2C_GenerateStartCondition(pI2CHandle->pI2Cx);
	}

	return busystate;
}


void I2C_EV_IRQHandling(I2C_Handle_t *pI2CHandle)
{
	uint32_t temp1, temp2, temp3;

	temp3 = pI2CHandle->pI2Cx->ISR & ( 1 << I2C_SR1_STOPF);
	if(temp3)
	{
		pI2CHandle->pI2Cx->ICR |= ( 1 << I2C_ICR_STOPCF);

		pI2CHandle->pI2Cx->CR2 = 0x2000000U;

		I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_STOP);

		if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
		{
			I2C_CloseSendData(pI2CHandle);
		}else if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
		{
			pI2CHandle->rxCmplt = SET;
			I2C_CloseReceiveData(pI2CHandle);
		}
	}

	temp3 = pI2CHandle->pI2Cx->ISR & ( 1 << I2C_SR1_TXIS);
	if(temp3)
	{
		if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
		{
			if(pI2CHandle->TxLen > 0)
			{
				pI2CHandle->pI2Cx->TXDR = *(pI2CHandle->pTxBuffer);
				pI2CHandle->TxLen--;
				pI2CHandle->pTxBuffer++;
			}
		}
	}

	temp3 = pI2CHandle->pI2Cx->ISR & ( 1 << I2C_SR1_RXNE);
	if(temp3)
	{
		if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
		{
			if(pI2CHandle->RxSize > 0)
			{
				*pI2CHandle->pRxBuffer = pI2CHandle->pI2Cx->RXDR;
				pI2CHandle->pRxBuffer++;
				pI2CHandle->RxSize--;
			}
		}
	}

	// temp3 = pI2CHandle->pI2Cx->ISR & ( 1 << I2C_SR1_TC);
	// if(temp3)
	// {
	// 	I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

	// 	I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_TRNS_CMPLT);

	// 	if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
	// 	{
	// 		I2C_CloseSendData(pI2CHandle);
	// 	}else if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
	// 	{
	// 		pI2CHandle->rxCmplt = SET;
	// 		I2C_CloseReceiveData(pI2CHandle);
	// 	}
	// }

	// temp3 = (pI2CHandle->pI2Cx->ISR) & ( 1 << I2C_SR1_BERR);
	// if(temp3)
	// {
	// 	pI2CHandle->pI2Cx->ICR |= ( 1 << I2C_ICR_BERRCF);
	// 	I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_BERR);
	// }

	// temp3 = (pI2CHandle->pI2Cx->ISR) & ( 1 << I2C_SR1_ARLO);
	// if(temp3)
	// {
	// 	pI2CHandle->pI2Cx->ICR |= ( 1 << I2C_ICR_ARLOCF);
	// 	I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_ARLO);
	// }

	// temp3 = (pI2CHandle->pI2Cx->ISR) & ( 1 << I2C_SR1_OVR);
	// if(temp3)
	// {
	// 	pI2CHandle->pI2Cx->ICR |= ( 1 << I2C_ICR_OVRCF);
	// 	I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_OVR);
	// }

	// temp3 = (pI2CHandle->pI2Cx->ISR) & ( 1 << I2C_SR1_PECERR);
	// if(temp3)
	// {
	// 	pI2CHandle->pI2Cx->ICR |= ( 1 << I2C_ICR_PECERRCF);
	// 	I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_PECERR);
	// }

	// temp3 = (pI2CHandle->pI2Cx->ISR) & ( 1 << I2C_SR1_TIMEOUT);
	// if(temp3)
	// {
	// 	pI2CHandle->pI2Cx->ICR |= ( 1 << I2C_ICR_TIMEOUTCF);
	// 	I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_TIMEOUT);
	// }
}

void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t AppEv)
{
	if(AppEv == I2C_EV_TRNS_CMPLT)
	{
		if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
		{
			//rxCmplt = SET;
		}
	}

	// else if(AppEv == I2C_ERROR_BERR)
	// {
	//    if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
	//    {
	// 	I2C_CloseReceiveData(pI2CHandle);
	//    }else{
	// 	I2C_CloseSendData(pI2CHandle);
	//    }
	//    I2C_GenerateStopCondition(I2C1);
	//    while(1); 
	// }else if(AppEv == I2C_ERROR_ARLO)
	// {
	//    if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
	//    {
	// 	I2C_CloseReceiveData(pI2CHandle);
	//    }else{
	// 	I2C_CloseSendData(pI2CHandle);
	//    }
	//    I2C_GenerateStopCondition(I2C1);

	//    while(1); //or take any other action
	// }else if(AppEv == I2C_ERROR_OVR)
	// {
    //    if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
	//    {
	// 	I2C_CloseReceiveData(pI2CHandle);
	//    }else{
	// 	I2C_CloseSendData(pI2CHandle);
	//    }
	//    I2C_GenerateStopCondition(I2C1);

	//    while(1); //or take any other action
	// }else if(AppEv == I2C_ERROR_PECERR)
	// {
    //    if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
	//    {
	// 	I2C_CloseReceiveData(pI2CHandle);
	//    }else{
	// 	I2C_CloseSendData(pI2CHandle);
	//    }
	//    I2C_GenerateStopCondition(I2C1);

	//    while(1); //or take any other action
	// }else if(AppEv == I2C_ERROR_TIMEOUT)
	// {
    //    if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
	//    {
	// 	I2C_CloseReceiveData(pI2CHandle);
	//    }else{
	// 	I2C_CloseSendData(pI2CHandle);
	//    }
	//    I2C_GenerateStopCondition(I2C1);

	//    while(1); //or take any other action
	// }
}
