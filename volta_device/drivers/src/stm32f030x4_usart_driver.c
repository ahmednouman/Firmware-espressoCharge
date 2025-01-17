/*
 * Version: 0.1.1
 * Workflow version: 0.0.1
 *
 * stm32f030x4_usart_driver.c
 *
 * Created on: Jun 30, 2023
 * Author: Ahmed Ali
 * Company: espresso Displays
 * Project: Volta
 */

#include "stm32f030x4_usart_driver.h"
#include "stm32f030x4_rcc_driver.h"

uint8_t USART_GetFlagStatus(USART_RegDef_t *pUSARTx, uint8_t StatusFlagName)
{
	if (pUSARTx->SR & StatusFlagName)
		return FLAG_SET;

	return FLAG_RESET;
}


void USART_Init(USART_Handle_t *pUSARTHandle)
{
	uint32_t tempreg = 0;

	USART1_PCLK_EN();

	tempreg |= ((1 << USART_CR1_RE) | (1 << USART_CR1_TE));

	tempreg &= ~(1 << USART_CR1_M1);
	tempreg |= 0 << USART_CR1_M0;

	pUSARTHandle->pUSARTx->CR1 = tempreg;

	pUSARTHandle->pUSARTx->CR2 = 0;

	pUSARTHandle->pUSARTx->BRR = 0x45U;
}

void USART_SendData(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len)
{

	uint16_t *pdata;

	for(uint32_t i = 0 ; i < Len; i++)
	{
		while(! USART_GetFlagStatus(pUSARTHandle->pUSARTx,USART_FLAG_TXE));
		pUSARTHandle->pUSARTx->TDR = (*pTxBuffer  & (uint8_t)0xFF);
		pTxBuffer++;
	}

	while( ! USART_GetFlagStatus(pUSARTHandle->pUSARTx,USART_FLAG_TC));
}

uint8_t USART_SendDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len)
{
    uint8_t txstate = pUSARTHandle->TxBusyState;

    if(txstate != USART_BUSY_IN_TX)
    {
        pUSARTHandle->TxLen = Len;
        pUSARTHandle->pTxBuffer = pTxBuffer;
        pUSARTHandle->TxBusyState = USART_BUSY_IN_TX;

        pUSARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_TXEIE);
        pUSARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_TCIE);
    }

    return txstate;
}

void USART_ReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len)
{
	for(uint32_t i = 0 ; i < Len; i++)
	{
		while(! USART_GetFlagStatus(pUSARTHandle->pUSARTx,USART_FLAG_RXNE));

		*pRxBuffer = (uint8_t) (pUSARTHandle->pUSARTx->RDR  & (uint8_t)0xFF);
	    pRxBuffer++;
	}
}

uint8_t USART_ReceiveDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len)
{
    uint8_t rxstate = pUSARTHandle->RxBusyState;

    if(rxstate != USART_BUSY_IN_RX)
    {
        pUSARTHandle->RxLen = Len;
        pUSARTHandle->pRxBuffer = pRxBuffer;
        pUSARTHandle->RxBusyState = USART_BUSY_IN_RX;

        (void)pUSARTHandle->pUSARTx->RDR;

        pUSARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_RXNEIE);
    }

    return rxstate;
}

void USART_ClearFlag(USART_RegDef_t *pUSARTx, uint16_t StatusFlagName)
{
    pUSARTx->SR &= ~(StatusFlagName);
}

void USART_IRQHandling(USART_Handle_t *pUSARTHandle)
{
    uint32_t temp1, temp2, temp3;
    uint16_t *pdata;

    /*************************Check for TC flag ********************************************/
    temp1 = pUSARTHandle->pUSARTx->SR & (1 << USART_SR_TC);
    temp2 = pUSARTHandle->pUSARTx->CR1 & (1 << USART_CR1_TCIE);

    if(temp1 && temp2)
    {
        if(pUSARTHandle->TxBusyState == USART_BUSY_IN_TX)
        {
            if(!pUSARTHandle->TxLen)
            {
                pUSARTHandle->pUSARTx->ICR |= (1 << USART_ICR_TCCF);
                pUSARTHandle->pUSARTx->CR1 &= ~(1 << USART_CR1_TCIE);
                pUSARTHandle->TxBusyState = USART_READY;
                pUSARTHandle->pTxBuffer = NULL;
                pUSARTHandle->TxLen = 0;
                USART_ApplicationEventCallback(pUSARTHandle, USART_EVENT_TX_CMPLT);
            }
        }
    }

    /*************************Check for TXE flag ********************************************/
    temp1 = pUSARTHandle->pUSARTx->SR & (1 << USART_SR_TXE);
    temp2 = pUSARTHandle->pUSARTx->CR1 & (1 << USART_CR1_TXEIE);

    if(temp1 && temp2)
    {
        if(pUSARTHandle->TxBusyState == USART_BUSY_IN_TX)
        {
            if(pUSARTHandle->TxLen > 0)
            {
				pUSARTHandle->pUSARTx->TDR = (*pUSARTHandle->pTxBuffer & (uint8_t)0xFF);
				pUSARTHandle->pTxBuffer++;
				pUSARTHandle->TxLen -= 1;
            }
            if(pUSARTHandle->TxLen == 0)
            {
                pUSARTHandle->pUSARTx->CR1 &= ~(1 << USART_CR1_TXEIE);
            }
        }
    }

    /*************************Check for RXNE flag ********************************************/
    temp1 = pUSARTHandle->pUSARTx->SR & (1 << USART_SR_RXNE);
    temp2 = pUSARTHandle->pUSARTx->CR1 & (1 << USART_CR1_RXNEIE);

    if(temp1 && temp2)
    {
        if(pUSARTHandle->RxBusyState == USART_BUSY_IN_RX)
        {
            if(pUSARTHandle->RxLen > 0)
            {
				*pUSARTHandle->pRxBuffer = (uint8_t)(pUSARTHandle->pUSARTx->RDR & (uint8_t)0xFF);
				
				pUSARTHandle->pRxBuffer++;
				pUSARTHandle->RxLen -= 1;
            }

            if(!pUSARTHandle->RxLen)
            {
                pUSARTHandle->pUSARTx->CR1 &= ~(1 << USART_CR1_RXNEIE);
                pUSARTHandle->RxBusyState = USART_READY;
                USART_ApplicationEventCallback(pUSARTHandle, USART_EVENT_RX_CMPLT);
            }
        }
    }


    /*************************Check for IDLE detection flag ********************************************/
    temp1 = pUSARTHandle->pUSARTx->SR & (1 << USART_SR_IDLE);
    temp2 = pUSARTHandle->pUSARTx->CR1 & (1 << USART_CR1_IDLEIE);

    if(temp1 && temp2)
    {
        pUSARTHandle->pUSARTx->ICR |= (1 << USART_ICR_IDLECF);
        USART_ApplicationEventCallback(pUSARTHandle, USART_EVENT_IDLE);
    }

    /*************************Check for Overrun detection flag ********************************************/
    // temp1 = pUSARTHandle->pUSARTx->SR & USART_SR_ORE;
    // temp2 = pUSARTHandle->pUSARTx->CR1 & USART_CR1_RXNEIE;

    // if(temp1 && temp2)
    // {
    //     USART_ApplicationEventCallback(pUSARTHandle, USART_ERR_ORE);
    // }

    /*************************Check for Error Flag ********************************************/
    // temp2 = pUSARTHandle->pUSARTx->CR3 & (1 << USART_CR3_EIE);

    // if(temp2)
    // {
    //     temp1 = pUSARTHandle->pUSARTx->SR;
    //     if(temp1 & (1 << USART_SR_FE))
    //     {
    //         USART_ApplicationEventCallback(pUSARTHandle, USART_ERR_FE);
    //     }

    //     if(temp1 & (1 << USART_SR_NF))
    //     {
    //         USART_ApplicationEventCallback(pUSARTHandle, USART_ERR_NE);
    //     }

    //     if(temp1 & (1 << USART_SR_ORE))
    //     {
    //         USART_ApplicationEventCallback(pUSARTHandle, USART_ERR_ORE);
    //     }
    // }
}

__weak void USART_ApplicationEventCallback(USART_Handle_t *pUSARTHandle, uint8_t event)
{
    // Weak callback implementation, can be overridden by the user
}
