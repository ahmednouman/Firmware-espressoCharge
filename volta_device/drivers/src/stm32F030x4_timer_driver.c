
#include "stm32F030x4_timer_driver.h"



void TIM_Base_SetConfig(TIM_TypeDef *TIMx, const TIM_Base_InitTypeDef *Structure)
{
    uint32_t tmpcr1;
    tmpcr1 = TIMx->CR1;

    tmpcr1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS);
    tmpcr1 |= Structure->CounterMode;

    tmpcr1 &= ~TIM_CR1_CKD;
    tmpcr1 |= (uint32_t)Structure->ClockDivision;

    MODIFY_REG(tmpcr1, TIM_CR1_ARPE, Structure->AutoReloadPreload);
    TIMx->CR1 = tmpcr1;

    TIMx->ARR = (uint32_t)Structure->Period;
    TIMx->PSC = Structure->Prescaler;

    TIMx->EGR = TIM_EGR_UG;
}

void TIM_OC1_SetConfig(TIM_TypeDef *TIMx, const TIM_OC_InitTypeDef *OC_Config)
{
    uint32_t tmpccmrx;
    uint32_t tmpccer;
    uint32_t tmpcr2;

    TIMx->CCER &= ~TIM_CCER_CC1E;
    tmpccer = TIMx->CCER;
    tmpcr2 = TIMx->CR2;
    tmpccmrx = TIMx->CCMR1;

    tmpccmrx &= ~TIM_CCMR1_OC1M;
    tmpccmrx &= ~TIM_CCMR1_CC1S;
    tmpccmrx |= OC_Config->OCMode;

    tmpccer &= ~TIM_CCER_CC1P;
    tmpccer |= OC_Config->OCPolarity;

    TIMx->CR2 = tmpcr2;
    TIMx->CCMR1 = tmpccmrx;
    TIMx->CCR1 = OC_Config->Pulse;
    TIMx->CCER = tmpccer;
}
void TIM_OC2_SetConfig(TIM_TypeDef *TIMx, const TIM_OC_InitTypeDef *OC_Config)
{
    uint32_t tmpccmrx;
    uint32_t tmpccer;
    uint32_t tmpcr2;

    TIMx->CCER &= ~TIM_CCER_CC2E;
    tmpccer = TIMx->CCER;
    tmpcr2 = TIMx->CR2;
    tmpccmrx = TIMx->CCMR1;

    tmpccmrx &= ~TIM_CCMR1_OC2M;
    tmpccmrx &= ~TIM_CCMR1_CC2S;
    tmpccmrx |= (OC_Config->OCMode << 8U);

    tmpccer &= ~TIM_CCER_CC2P;
    tmpccer |= (OC_Config->OCPolarity << 4U);

    TIMx->CR2 = tmpcr2;
    TIMx->CCMR1 = tmpccmrx;
    TIMx->CCR2 = OC_Config->Pulse;
    TIMx->CCER = tmpccer;
}

void TIM_OC3_SetConfig(TIM_TypeDef *TIMx, const TIM_OC_InitTypeDef *OC_Config)
{
    uint32_t tmpccmrx;
    uint32_t tmpccer;
    uint32_t tmpcr2;

    TIMx->CCER &= ~TIM_CCER_CC3E;
    tmpccer = TIMx->CCER;
    tmpcr2 = TIMx->CR2;
    tmpccmrx = TIMx->CCMR2;

    tmpccmrx &= ~TIM_CCMR2_OC3M;
    tmpccmrx &= ~TIM_CCMR2_CC3S;
    tmpccmrx |= OC_Config->OCMode;

    tmpccer &= ~TIM_CCER_CC3P;
    tmpccer |= (OC_Config->OCPolarity << 8U);

    TIMx->CR2 = tmpcr2;
    TIMx->CCMR2 = tmpccmrx;
    TIMx->CCR3 = OC_Config->Pulse;
    TIMx->CCER = tmpccer;
}

void TIM_OC4_SetConfig(TIM_TypeDef *TIMx, const TIM_OC_InitTypeDef *OC_Config)
{
    uint32_t tmpccmrx;
    uint32_t tmpccer;
    uint32_t tmpcr2;

    TIMx->CCER &= ~TIM_CCER_CC4E;
    tmpccer = TIMx->CCER;
    tmpcr2 = TIMx->CR2;
    tmpccmrx = TIMx->CCMR2;

    tmpccmrx &= ~TIM_CCMR2_OC4M;
    tmpccmrx &= ~TIM_CCMR2_CC4S;
    tmpccmrx |= (OC_Config->OCMode << 8U);

    tmpccer &= ~TIM_CCER_CC4P;
    tmpccer |= (OC_Config->OCPolarity << 12U);

    TIMx->CR2 = tmpcr2;
    TIMx->CCMR2 = tmpccmrx;
    TIMx->CCR4 = OC_Config->Pulse;
    TIMx->CCER = tmpccer;
}


void TIM_PWM_ConfigChannel(TIM_HandleTypeDef *htim, const TIM_OC_InitTypeDef *sConfig, uint32_t Channel)
{
    switch (Channel)
    {
        case TIM_CHANNEL_1:
        {
            TIM_OC1_SetConfig(htim->Instance, sConfig);
            htim->Instance->CCMR1 |= TIM_CCMR1_OC1PE;
            htim->Instance->CCMR1 &= ~TIM_CCMR1_OC1FE;
            htim->Instance->CCMR1 |= sConfig->OCFastMode;
            break;
        }
        case TIM_CHANNEL_2:
        {
            TIM_OC2_SetConfig(htim->Instance, sConfig);
            htim->Instance->CCMR1 |= TIM_CCMR1_OC2PE;
            htim->Instance->CCMR1 &= ~TIM_CCMR1_OC2FE;
            htim->Instance->CCMR1 |= sConfig->OCFastMode << 8U;
            break;
        }
        case TIM_CHANNEL_3:
        {
            TIM_OC3_SetConfig(htim->Instance, sConfig);
            htim->Instance->CCMR2 |= TIM_CCMR2_OC3PE;
            htim->Instance->CCMR2 &= ~TIM_CCMR2_OC3FE;
            htim->Instance->CCMR2 |= sConfig->OCFastMode;
            break;
        }
        case TIM_CHANNEL_4:
        {
            TIM_OC4_SetConfig(htim->Instance, sConfig);
            htim->Instance->CCMR2 |= TIM_CCMR2_OC4PE;
            htim->Instance->CCMR2 &= ~TIM_CCMR2_OC4FE;
            htim->Instance->CCMR2 |= sConfig->OCFastMode << 8U;
            break;
        }
        default:
            break;
    }
}

void TIM_CCxChannelCmd(TIM_TypeDef *TIMx, uint32_t Channel, uint32_t ChannelState)
{
    uint32_t tmp;
    tmp = TIM_CCER_CC1E << (Channel & 0x1FU);
    TIMx->CCER &= ~tmp;
    TIMx->CCER |= (uint32_t)(ChannelState << (Channel & 0x1FU));
}

void TIM_PWM_Start(TIM_HandleTypeDef *htim, uint32_t Channel)
{
    uint32_t tmpsmcr;
    TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_ENABLE);
    tmpsmcr = htim->Instance->SMCR & TIM_SMCR_SMS;
    __HAL_TIM_ENABLE(htim);
}
