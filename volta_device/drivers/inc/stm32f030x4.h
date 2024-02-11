/*
 * Version: 0.1.1
 * Workflow version: 0.0.1
 *
 * file: stm32f030x4.h
 *
 * Created on: Jun 30, 2023
 * Author: Ahmed Ali
 * Company: espresso Displays
 * Project: Volta
 */

#ifndef INC_STM32F030X4_H_
#define INC_STM32F030X4_H_

#include<stddef.h>
#include<stdint.h>

#define __vo volatile
#define __IO __vo
#define __weak __attribute__((weak))

#define __WFI()                             __asm volatile ("wfi")

#define __WFE()                             __asm volatile ("wfe")



/********************************** Processor Specific Details **********************************/
/*
 * ARM Cortex Mx Processor NVIC ISERx register Addresses
 */
#define NVIC_ISER0          ( (__vo uint32_t*)0xE000E100 )

/*
 * ARM Cortex Mx Processor NVIC ICERx register Addresses
 */
#define NVIC_ICER0 			((__vo uint32_t*)0xE000E180)

/*
 * ARM Cortex Mx Processor Priority Register Address Calculation
 */
#define NVIC_PR_BASE_ADDR 	((__vo uint32_t*)0xE000E400)

/*
 * ARM Cortex Mx Processor number of priority bits implemented in Priority Register
 */
#define NO_PR_BITS_IMPLEMENTED  4


#define SCS_BASEADDR            (0xE000E000UL)                            /*!< System Control Space Base Address */
#define SysTick_BASEADDR        (SCS_BASEADDR +  0x0010UL)                    /*!< SysTick Base Address */
#define SCB_BASE            (SCS_BASEADDR +  0x0D00UL)

/*
 * base addresses of Flash and SRAM memories
 */

#define FLASH_BASEADDR						0x08000000U
#define SRAM_BASEADDR						0x20000000U        //SRAM is 4 Kbytes


/*
 * AHBx and APBx Bus Peripheral base addresses
 */

#define PERIPH_BASEADDR 						0x40000000U
#define APB1PERIPH_BASEADDR					    PERIPH_BASEADDR
#define AHB1PERIPH_BASEADDR						0x40020000U
#define AHB2PERIPH_BASEADDR						0x48000000U

/*
 * Base addresses of peripherals on AHB1 bus
 */

#define RCC_BASEADDR                     (AHB1PERIPH_BASEADDR + 0x1000)

/*
 * Base addresses of peripherals on APB1 bus
 */

#define I2C1_BASEADDR						(APB1PERIPH_BASEADDR + 0x5400)
#define USART1_BASEADDR						(0x40013800U)
#define EXTI_BASEADDR						(0x40010400U)
#define SYSCFG_BASEADDR        				(0x40010000U)
#define TIM3_BASEADDR					 (APB1PERIPH_BASEADDR + 0x00000400UL)
#define IWDG_BASEADDR             (APB1PERIPH_BASEADDR + 0x00003000UL)
#define PWR_BASE              (APB1PERIPH_BASEADDR + 0x00007000UL)

/*
 * Base addresses of peripherals on AHB2 bus
 */

#define GPIOA_BASEADDR                   (AHB2PERIPH_BASEADDR + 0x0000)
#define GPIOB_BASEADDR                   (AHB2PERIPH_BASEADDR + 0x0400)




/**********************************peripheral register definition structures **********************************/

/*
 * peripheral register definition structure for GPIO
 */

typedef struct
{
	__vo uint32_t MODER;                       
	__vo uint32_t OTYPER;                       
	__vo uint32_t OSPEEDR;
	__vo uint32_t PUPDR;
	__vo uint32_t IDR;
	__vo uint32_t ODR;
	__vo uint32_t BSRR;
	__vo uint32_t LCKR;
	__vo uint32_t AFR[2];	
    __vo uint32_t BRR;				 
}GPIO_RegDef_t;


/*
 * peripheral register definition structure for RCC
 */
typedef struct
{
  __vo uint32_t CR;                										
  __vo uint32_t CFGR;          
  __vo uint32_t CIR;    
  __vo uint32_t APB2RSTR;    
  __vo uint32_t APB1RSTR;    
  __vo uint32_t AHBENR;
  __vo uint32_t APB2ENR;
  __vo uint32_t APB1ENR;
  __vo uint32_t BDCR;
  __vo uint32_t CSR;
  __vo uint32_t AHBRSTR;
  __vo uint32_t CFGR2;
  __vo uint32_t CFGR3;
  __vo uint32_t CR2;
} RCC_RegDef_t;

/*
 * peripheral register definition structure for EXTI
 */
 typedef struct
{
  __vo uint32_t IMR;      
  __vo uint32_t EMR;      
  __vo uint32_t RTSR;     
  __vo uint32_t FTSR;     
  __vo uint32_t SWIER;    
  __vo uint32_t PR;       
} EXTI_RegDef_t;

/*
 * peripheral register definition structure for SYSCFG
 */
typedef struct
{
	__vo uint32_t CFGR1;                
	__vo uint32_t EXTICR[4];    
	__vo uint32_t CFGR2;         
} SYSCFG_RegDef_t;

/*
 * peripheral register definition structure for I2C
 */
typedef struct
{
  __vo uint32_t CR1;        
  __vo uint32_t CR2;       
  __vo uint32_t OAR1;      
  __vo uint32_t OAR2;       
  __vo uint32_t TIMINGR;         
  __vo uint32_t TIMEOUTR;       
  __vo uint32_t ISR;        
  __vo uint32_t ICR;        
  __vo uint32_t PECR;     
  __vo uint32_t RXDR;       
  __vo uint32_t TXDR;  
}I2C_RegDef_t;

/*
 * peripheral register definition structure for USART
 */
typedef struct
{
    __vo uint32_t CR1;  
    __vo uint32_t CR2;    
	__vo uint32_t CR3;   
    __vo uint32_t BRR;  
    uint32_t reserved0;
    __vo uint32_t PTOR;  
    __vo uint32_t RQR;  
	__vo uint32_t SR;   
    __vo uint32_t ICR;    
	__vo uint32_t RDR;    
    __vo uint32_t TDR;             
} USART_RegDef_t;

typedef struct
{
	__vo uint32_t CR1;
	__vo uint32_t CR2;
	__vo uint32_t SMCR;
	__vo uint32_t DIER;
	__vo uint32_t SR;
	__vo uint32_t EGR;
	__vo uint32_t CCMR1;
	__vo uint32_t CCMR2;
	__vo uint32_t CCER;
	__vo uint32_t CNT;
	__vo uint32_t PSC;
	__vo uint32_t ARR;
	__vo uint32_t RCR;
	__vo uint32_t CCR1;
	__vo uint32_t CCR2;
	__vo uint32_t CCR3;
	__vo uint32_t CCR4;
	__vo uint32_t BDTR;
	__vo uint32_t DCR;
	__vo uint32_t DMAR;
	__vo uint32_t OR;
} TIM_TypeDef;


typedef struct
{
  __IO uint32_t KR;  
  __IO uint32_t PR;   
  __IO uint32_t RLR;  
  __IO uint32_t SR;   
  __IO uint32_t WINR; 
} IWDG_TypeDef;



typedef struct
{
  __vo uint32_t Prescaler; 
  __vo uint32_t Reload;   
  __vo uint32_t Window;    

} IWDG_InitTypeDef;

typedef struct
{
  IWDG_TypeDef *Instance;

  IWDG_InitTypeDef Init;
} IWDG_HandleTypeDef;

typedef struct
{
  __vo uint32_t CR;   
  __vo uint32_t CSR;  
} PWR_TypeDef;



/**
  \brief  Structure type to access the System Control Block (SCB).
 */
typedef struct
{
	__vo  uint32_t CPUID;                  /*!< Offset: 0x000 (R/ )  CPUID Base Register */
	__vo uint32_t ICSR;                   /*!< Offset: 0x004 (R/W)  Interrupt Control and State Register */
        uint32_t RESERVED0;
    __vo uint32_t AIRCR;                  /*!< Offset: 0x00C (R/W)  Application Interrupt and Reset Control Register */
        __vo uint32_t SCR;                    /*!< Offset: 0x010 (R/W)  System Control Register */
        __vo uint32_t CCR;                    /*!< Offset: 0x014 (R/W)  Configuration Control Register */
        uint32_t RESERVED1;
        __vo uint32_t SHP[2U];                /*!< Offset: 0x01C (R/W)  System Handlers Priority Registers. [0] is RESERVED */
        __vo uint32_t SHCSR;                  /*!< Offset: 0x024 (R/W)  System Handler Control and State Register */
} SCB__RegDef_t;


/**
  \brief  Structure type to access the System Timer (SysTick).
 */
typedef struct
{
	__vo uint32_t CTRL;                   /*!< Offset: 0x000 (R/W)  SysTick Control and Status Register */
	__vo uint32_t LOAD;                   /*!< Offset: 0x004 (R/W)  SysTick Reload Value Register */
	__vo uint32_t VAL;                    /*!< Offset: 0x008 (R/W)  SysTick Current Value Register */
	__vo  uint32_t CALIB;                  /*!< Offset: 0x00C (R/ )  SysTick Calibration Register */
} SysTick__RegDef_t;

/*
 * peripheral definitions ( Peripheral base addresses typecasted to xxx_RegDef_t)
 */
#define GPIOA  				((GPIO_RegDef_t*)GPIOA_BASEADDR)
#define GPIOB  				((GPIO_RegDef_t*)GPIOB_BASEADDR)
#define TIM3                ((TIM_TypeDef *) TIM3_BASEADDR)

#define IWDG                ((IWDG_TypeDef *) IWDG_BASEADDR)

#define RCC 				((RCC_RegDef_t*)RCC_BASEADDR)
#define EXTI				((EXTI_RegDef_t*)EXTI_BASEADDR)
#define SYSCFG				((SYSCFG_RegDef_t*)SYSCFG_BASEADDR)

#define I2C1  				((I2C_RegDef_t*)I2C1_BASEADDR)

#define USART1  			((USART_RegDef_t*)USART1_BASEADDR)


#define SCB                 ((SCB__RegDef_t      *)     SCB_BASE      )
#define SysTick             ((SysTick__RegDef_t   *)     SysTick_BASEADDR  )   

#define PWR                 ((PWR_TypeDef *) PWR_BASE)
/*
 * Clock Enable Macros for GPIOx peripherals
 */
#define GPIOA_PCLK_EN()    	(RCC->AHBENR |= (1 << 17))
#define GPIOB_PCLK_EN()		(RCC->AHBENR |= (1 << 18))
#define TIM3_PCLK_EN()		(RCC->APB1ENR |= (1 << 1))

/*
 * Clock Enable Macros for I2Cx peripherals
 */
#define I2C1_PCLK_EN()     (RCC->APB1ENR |= (1 << 21))

 /*
 * Clock Enable Macros for USARTx peripherals
 */
#define USART1_PCLK_EN() (RCC->APB2ENR |= (1 << 14))

 /*
 * Clock Enable Macros for SYSCFG peripherals
 */
#define SYSCFG_PCLK_EN() (RCC->APB2ENR |= (1 << 0))

#define PWR_PCLK_EN()  (RCC->APB1ENR |= (1 << 28))
/*
 * Clock DISABLE macros for GPIOx peripheral
 */
#define GPIOA_PCLK_DI()		( RCC->AHBENR &= ~(1 << 17) )
#define GPIOB_PCLK_DI()		( RCC->AHBENR &= ~(1 << 18) )
#define TIM3_PCLK_DI()		(RCC->APB1ENR &= ~(1 << 1))

/*
 * Clock DISABLE macros for I2Cx peripheral
 */
#define I2C1_PCLK_DI() (RCC->APB1ENR &= ~(1 << 21))
#define USART1_PCLK_DI() (RCC->APB2ENR &= ~(1 << 14))
#define PWR_PCLK_DI()  (RCC->APB1ENR &= ~(1 << 28))
/*
 * This macro returns a code( between 0 to 1) for a given GPIO base address(x)
 */
#define GPIO_BASEADDR_TO_CODE(x)      ( (x == GPIOA)?0:\
                                        (x == GPIOB)?1:0)

/*
 * IRQ(Interrupt Request) Numbers 
 */

#define IRQ_NO_EXTI0_1 		5
#define IRQ_NO_EXTI2_3 		6
#define IRQ_NO_EXTI4_15 	7
#define IRQ_NO_I2C1       23
#define IRQ_NO_USART1     27
#define TIM3_IRQn			16

/*
 * Generric Macros
 */
#define ENABLE 				1
#define DISABLE 			0
#define ON          0
#define OFF          1
#define SET 				ENABLE
#define RESET 				DISABLE
#define GPIO_PIN_SET        SET
#define GPIO_PIN_RESET      RESET
#define FLAG_RESET         RESET
#define FLAG_SET 			SET


/******************************************************************************************
 *Bit position definitions of I2C peripheral
 ******************************************************************************************/
/*
* Bit position definitions I2C_CR1
*/
#define I2C_CR1_PE						    0
#define I2C_CR1_TXIE						  1
#define I2C_CR1_RXIE						  2
#define I2C_CR1_ADDRIE						3
#define I2C_CR1_NACKIE						4
#define I2C_CR1_STOPIE						5
#define I2C_CR1_TCIE						  6
#define I2C_CR1_ERRIE						  7
#define I2C_CR1_NOSTRETCH  				17

/*
* Bit position definitions I2C_CR2
*/
#define I2C_CR2_START 					13
#define I2C_CR2_STOP  				 	14
#define I2C_CR2_ADDMODE    			11
#define I2C_CR2_ADD0    				0
#define I2C_CR2_AUTOEND         25
#define I2C_CR2_RD_WRN          10

/*
 * Bit position definitions I2C_OAR1
 */

 /*
 * Bit position definitions I2C_OAR2
 */

/*
 * Bit position definitions I2C_SR1
 */
#define I2C_SR1_ADDR 				 	3
#define I2C_SR1_STOPF 					5
#define I2C_SR1_RXNE 					2
#define I2C_SR1_TXE 					0
#define I2C_SR1_TXIS 					1
#define I2C_SR1_NACKF 					4
#define I2C_SR1_TC 					    6
#define I2C_SR1_TCR 					    7
#define I2C_SR1_BERR 					8
#define I2C_SR1_ARLO 					9
#define I2C_SR1_OVR 					10
#define I2C_SR1_TIMEOUT 				12
#define I2C_SR1_BUSY 				    15
#define I2C_SR1_PECERR 				    11            

/*
 * Bit position definitions I2C_ICR
 */
#define I2C_ICR_ADDRCF 					3
#define I2C_ICR_NACKCF 					4
#define I2C_ICR_STOPCF 					5
#define I2C_ICR_BERRCF 				  8
#define I2C_ICR_ARLOCF 					9
#define I2C_ICR_OVRCF 					10
#define I2C_ICR_PECERRCF 				11
#define I2C_ICR_TIMEOUTCF 			12
#define I2C_ICR_ALERTCF 				13


/*
 * Bit position definitions USART_CR1
 */
#define USART_CR1_MME					  13
#define USART_CR1_CMIE					14
#define USART_CR1_RTOIE					26
#define USART_CR1_RWU 					1
#define USART_CR1_RE  					2
#define USART_CR1_TE 					  3
#define USART_CR1_IDLEIE 				4
#define USART_CR1_RXNEIE  			5
#define USART_CR1_TCIE					6
#define USART_CR1_TXEIE					7
#define USART_CR1_PEIE 					8
#define USART_CR1_PS 					  9
#define USART_CR1_PCE 					10
#define USART_CR1_WAKE  				11
#define USART_CR1_M0 					  12
#define USART_CR1_M1 					  28
#define USART_CR1_UE 					  0
#define USART_CR1_OVER8  				15

/*
 * Bit position definitions USART_CR2
 */
#define USART_CR2_LBCL   				8
#define USART_CR2_CPHA   				9
#define USART_CR2_CPOL   				10
#define USART_CR2_STOP   				12
#define USART_CR2_CLKEN   			11
#define USART_CR2_SWAP     			15

/*
 * Bit position definitions USART_CR3
 */
#define USART_CR3_EIE   				0
#define USART_CR3_HDSEL   			3
#define USART_CR3_DMAR  				6
#define USART_CR3_DMAT   				7
#define USART_CR3_RTSE   				8
#define USART_CR3_CTSE   				9
#define USART_CR3_CTSIE   			10
#define USART_CR3_ONEBIT   			11

/*
 * Bit position definitions USART_SR
 */

#define USART_SR_PE        				0
#define USART_SR_FE        				1
#define USART_SR_NF        				2
#define USART_SR_ORE       				3
#define USART_SR_IDLE       			4
#define USART_SR_RXNE        			5
#define USART_SR_TC        				6
#define USART_SR_TXE        			7
#define USART_SR_CTSIF        		9
#define USART_SR_CTS        			10

/*
 * Bit position definitions USART_ICR
 */
#define USART_ICR_TCCF        		6
#define USART_ICR_CTSCF        		9
#define USART_ICR_IDLECF        	4

/*****************************************************************************/
/*                                                                           */
/*                        Independent WATCHDOG (IWDG)                        */
/*                                                                           */
/*****************************************************************************/
/*******************  Bit definition for IWDG_KR register  *******************/
#define IWDG_KR_KEY_Pos      (0U)                                              
#define IWDG_KR_KEY_Msk      (0xFFFFUL << IWDG_KR_KEY_Pos)                      /*!< 0x0000FFFF */
#define IWDG_KR_KEY          IWDG_KR_KEY_Msk                                   /*!< Key value (write only, read 0000h) */

/*******************  Bit definition for IWDG_PR register  *******************/
#define IWDG_PR_PR_Pos       (0U)                                              
#define IWDG_PR_PR_Msk       (0x7UL << IWDG_PR_PR_Pos)                          /*!< 0x00000007 */
#define IWDG_PR_PR           IWDG_PR_PR_Msk                                    /*!< PR[2:0] (Prescaler divider) */
#define IWDG_PR_PR_0         (0x1UL << IWDG_PR_PR_Pos)                          /*!< 0x01 */
#define IWDG_PR_PR_1         (0x2UL << IWDG_PR_PR_Pos)                          /*!< 0x02 */
#define IWDG_PR_PR_2         (0x4UL << IWDG_PR_PR_Pos)                          /*!< 0x04 */

/*******************  Bit definition for IWDG_RLR register  ******************/
#define IWDG_RLR_RL_Pos      (0U)                                              
#define IWDG_RLR_RL_Msk      (0xFFFUL << IWDG_RLR_RL_Pos)                       /*!< 0x00000FFF */
#define IWDG_RLR_RL          IWDG_RLR_RL_Msk                                   /*!< Watchdog counter reload value */

/*******************  Bit definition for IWDG_SR register  *******************/
#define IWDG_SR_PVU_Pos      (0U)                                              
#define IWDG_SR_PVU_Msk      (0x1UL << IWDG_SR_PVU_Pos)                         /*!< 0x00000001 */
#define IWDG_SR_PVU          IWDG_SR_PVU_Msk                                   /*!< Watchdog prescaler value update */
#define IWDG_SR_RVU_Pos      (1U)                                              
#define IWDG_SR_RVU_Msk      (0x1UL << IWDG_SR_RVU_Pos)                         /*!< 0x00000002 */
#define IWDG_SR_RVU          IWDG_SR_RVU_Msk                                   /*!< Watchdog counter reload value update */
#define IWDG_SR_WVU_Pos      (2U)                                              
#define IWDG_SR_WVU_Msk      (0x1UL << IWDG_SR_WVU_Pos)                         /*!< 0x00000004 */
#define IWDG_SR_WVU          IWDG_SR_WVU_Msk                                   /*!< Watchdog counter window value update */

/*******************  Bit definition for IWDG_KR register  *******************/
#define IWDG_WINR_WIN_Pos    (0U)                                              
#define IWDG_WINR_WIN_Msk    (0xFFFUL << IWDG_WINR_WIN_Pos)                     /*!< 0x00000FFF */
#define IWDG_WINR_WIN        IWDG_WINR_WIN_Msk                                 /*!< Watchdog counter window value */

/** @defgroup IWDG_Prescaler IWDG Prescaler
  * @{
  */
#define IWDG_PRESCALER_4                0x00000000u                                     /*!< IWDG prescaler set to 4   */
#define IWDG_PRESCALER_8                IWDG_PR_PR_0                                    /*!< IWDG prescaler set to 8   */
#define IWDG_PRESCALER_16               IWDG_PR_PR_1                                    /*!< IWDG prescaler set to 16  */
#define IWDG_PRESCALER_32               (IWDG_PR_PR_1 | IWDG_PR_PR_0)                   /*!< IWDG prescaler set to 32  */
#define IWDG_PRESCALER_64               IWDG_PR_PR_2                                    /*!< IWDG prescaler set to 64  */
#define IWDG_PRESCALER_128              (IWDG_PR_PR_2 | IWDG_PR_PR_0)                   /*!< IWDG prescaler set to 128 */
#define IWDG_PRESCALER_256              (IWDG_PR_PR_2 | IWDG_PR_PR_1)                   /*!< IWDG prescaler set to 256 */
/**
  * @}
  */

/** @defgroup IWDG_Window_option IWDG Window option
  * @{
  */
#define IWDG_WINDOW_DISABLE             IWDG_WINR_WIN

/**
  * @brief  IWDG Key Register BitMask
  */
#define IWDG_KEY_RELOAD                 0x0000AAAAu
#define IWDG_KEY_ENABLE                 0x0000CCCCu
#define IWDG_KEY_WRITE_ACCESS_ENABLE    0x00005555u
#define IWDG_KEY_WRITE_ACCESS_DISABLE   0x00000000u

#define IWDG_KERNEL_UPDATE_FLAGS        (IWDG_SR_WVU | IWDG_SR_RVU | IWDG_SR_PVU)


#define SysTick_CTRL_TICKINT_Pos            1U                                        
#define SysTick_CTRL_TICKINT_Msk           (1UL << SysTick_CTRL_TICKINT_Pos) 


#define SCB_SCR_SLEEPDEEP_Pos               2                                            
#define SCB_SCR_SLEEPDEEP_Msk              (1UL << SCB_SCR_SLEEPDEEP_Pos)  

#define CR_DS_MASK               ((uint32_t)0xFFFFF3FC)
#define CR_PLS_MASK              ((uint32_t)0xFFFFFF1F)
#define CR_VOS_MASK              ((uint32_t)0xFFFF3FFF)


#define SET_BIT(REG, BIT)     ((REG) |= (BIT))

#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))

#define READ_BIT(REG, BIT)    ((REG) & (BIT))

#define CLEAR_REG(REG)        ((REG) = (0x0))

#define WRITE_REG(REG, VAL)   ((REG) = (VAL))

#define READ_REG(REG)         ((REG))

#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))


#include "stm32f030x4_gpio_driver.h"

#endif /* INC_STM32F030X4_H_ */
