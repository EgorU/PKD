/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F10x_IT_H
#define __STM32F10x_IT_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "math.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/*
     #define USART                   USART1
  #define USART_GPIO              GPIOA
  #define USART_CLK               RCC_APB2Periph_USART1
  #define USART_GPIO_CLK          RCC_APB2Periph_GPIOA
  #define USART_RxPin             GPIO_Pin_10
  #define USART_TxPin             GPIO_Pin_9
  #define USART_IRQn              USART1_IRQn
  #define USART_IRQHandler        USART1_IRQHandler
   */
   
     #define USARTy                   USART1
  #define USARTy_GPIO              GPIOA
  #define USARTy_CLK               RCC_APB2Periph_USART1
  #define USARTy_GPIO_CLK          RCC_APB2Periph_GPIOA
  #define USARTy_RxPin             GPIO_Pin_10
  #define USARTy_TxPin             GPIO_Pin_9
  #define USARTy_IRQn              USART1_IRQn
  #define USARTy_IRQHandler        USART1_IRQHandler
  

  #define CDcard()    AccessCard.Family    = pow((snCard[1] * 2),7) + pow((snCard[2] * 2),6) + pow((snCard[3] * 2),5) + pow((snCard[4] * 2),4) + pow((snCard[5] * 2),3) + pow((snCard[6] * 2),2) + pow((snCard[7] * 2),1) + snCard[8];  \
                      AccessCard.NumCard   = pow((snCard[9] * 2),7) + pow((snCard[10] * 2),6) + pow((snCard[11] * 2),5) + pow((snCard[12] * 2),4) + pow((snCard[13] * 2),3) + pow((snCard[14] * 2),2) + pow((snCard[15] * 2),1) + snCard[16]; \
                      PasswordCard.Pas     = pow((snCard[17] * 2),7) + pow((snCard[18] * 2),6) + pow((snCard[19] * 2),5) + pow((snCard[20] * 2),4) + pow((snCard[21] * 2),3) + pow((snCard[22] * 2),2) + pow((snCard[23] * 2),1) + snCard[24];\
                      AccessCard.FamilyHi  = pow((snCard[1] * 2),3) + pow((snCard[2] * 2),2) + pow((snCard[3] * 2),1) + snCard[4];  \
                      AccessCard.FamilyLo  = pow((snCard[5] * 2),3) + pow((snCard[6] * 2),2) + pow((snCard[7] * 2),1) + snCard[8];  \
                      AccessCard.NumCardHi = pow((snCard[9] * 2),3) + pow((snCard[10] * 2),2) + pow((snCard[11] * 2),1) + snCard[12]; \
                      AccessCard.NumCardLo = pow((snCard[13] * 2),3) + pow((snCard[14] * 2),2) + pow((snCard[15] * 2),1) + snCard[16]; \
                      PasswordCard.PasHi   = pow((snCard[17] * 2),3) + pow((snCard[18] * 2),2) + pow((snCard[19] * 2),1) + snCard[20];\
                      PasswordCard.PasLo   = pow((snCard[21] * 2),3) + pow((snCard[22] * 2),2) + pow((snCard[23] * 2),1) + snCard[24];

   
extern __IO uint8_t TxCounter;
extern __IO uint8_t RxCounter;


/* Exported functions ------------------------------------------------------- */
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void TIM2_IRQHandler(void);
void EXTI3_IRQHandler(void);
void EXTI4_IRQHandler(void);


#ifdef __cplusplus
}
#endif

#endif /* __STM32F10x_IT_H */