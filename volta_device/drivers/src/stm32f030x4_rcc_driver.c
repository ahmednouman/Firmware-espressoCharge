/*
 * Version: 0.1.1
 * Workflow version: 0.0.1
 *
 * stm32f030x4_rcc_driver.c
 *
 * Created on: Jun 30, 2023
 * Author: Ahmed Ali
 * Company: espresso Displays
 * Project: Volta
 */

#include "stm32f030x4_rcc_driver.h"

uint16_t AHB_PreScaler[8] = {2,4,8,16,64,128,256,512};
uint8_t APB1_PreScaler[4] = { 2, 4 , 8, 16};

uint32_t RCC_GetPCLKValue(void)
{
	uint32_t SystemClock = 0, tmp, pclk;
	uint8_t clk_src = ( RCC->CFGR >> 2) & 0x3;

	uint8_t ahbp, apb2p;

	if(clk_src == 0)
	{
		SystemClock = 8000000;
	}else
	{
		SystemClock = 8000000;
	}
	tmp = (RCC->CFGR >> 4 ) & 0xF;

	if(tmp < 0x08)
	{
		ahbp = 1;
	}else
	{
       ahbp = AHB_PreScaler[tmp-8];
	}

	tmp = (RCC->CFGR >> 8 ) & 0x7;
	if(tmp < 0x04)
	{
		apb2p = 1;
	}else
	{
		apb2p = APB1_PreScaler[tmp-4];
	}

	pclk = (SystemClock / ahbp )/ apb2p;

	return pclk;
}

uint32_t  RCC_GetPLLOutputClock()
{

	return 0;
}
