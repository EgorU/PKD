/**
  ******************************************************************************
  *stm32f10x_it.c
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "SystemConfiguration.h"
#include "main.h"
#include "led.h"
#include "USART.h"
#include "baza.h"

/* Private typedef -----------------------------------------------------------*/
//typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t exti;
extern uint8_t FANTI_DREB_KNOPKA;       //флаг антидребезга

uint16_t countTime = 0;
uint16_t L1 = 0;
uint16_t L2 = 0;
uint16_t L3 = 0;
uint16_t L4 = 0;
uint16_t L5 = 0;
extern __IO uint16_t CCR1_Val;
extern __IO uint16_t CCR2_Val;
extern __IO uint16_t CCR3_Val;
extern __IO uint16_t CCR4_Val;
uint32_t __IO delay;
__IO uint8_t TxCounter;
__IO uint8_t RxCounter;

uint8_t TxBuffer[255];
uint8_t RxBuffer[255];

uint8_t NbrOfDataToTransfer;

uint32_t snCard[25];

extern uint32_t readBuffer[basaSise];
extern inline void TimingDelayDecrement(void);
extern void SystickT(void);
 void AnalyzeEXTI(void);

extern void ReadCD(uint8_t , uint32_t* , uint16_t );

//uint8_t exti = 0;

/** @addtogroup STM32F10x_StdPeriph_Examples
 * @{
 */

/** @addtogroup USART_DMA_Interrupt
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
 * @brief  This function handles NMI exception.
 * @param  None
 * @retval None
 */
void NMI_Handler(void)
{
}

/**
 * @brief  This function handles Hard Fault exception.
 * @param  None
 * @retval None
 */
void HardFault_Handler(void)
{
	/* Go to infinite loop when Hard Fault exception occurs */
	while(1)
	{
	}
}

/**
 * @brief  This function handles Memory Manage exception.
 * @param  None
 * @retval None
 */
void MemManage_Handler(void)
{
	/* Go to infinite loop when Memory Manage exception occurs */
	while(1)
	{
	}
}

/**
 * @brief  This function handles Bus Fault exception.
 * @param  None
 * @retval None
 */
void BusFault_Handler(void)
{
	/* Go to infinite loop when Bus Fault exception occurs */
	while(1)
	{
	}
}

/**
 * @brief  This function handles Usage Fault exception.
 * @param  None
 * @retval None
 */
void UsageFault_Handler(void)
{
	/* Go to infinite loop when Usage Fault exception occurs */
	while(1)
	{
	}
}

/**
 * @brief  This function handles SVCall exception.
 * @param  None
 * @retval None
 */
void SVC_Handler(void)
{
}

/**
 * @brief  This function handles Debug Monitor exception.
 * @param  None
 * @retval None
 */
void DebugMon_Handler(void)
{
}

/**
 * @brief  This function handles PendSV_Handler exception.
 * @param  None
 * @retval None
 */
void PendSV_Handler(void)
{
}

/**
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 */
void SysTick_Handler(void)
{
	if(delay > 0) delay--;
}

/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/

/**
 * @brief  This function handles USART1 global interrupt request.
 * @param  None
 * @retval None
 */
void USARTy_IRQHandler(void)
{
    if(USART_GetITStatus(USARTy, USART_IT_RXNE) != RESET)
    {
        USART_SendData(USARTy, USART_ReceiveData(USARTy));
//        RxBuffer[RxCounter++] = USART_ReceiveData(USART);

        USART_ClearITPendingBit(USARTy, USART_IT_RXNE );

 //       ReadUSART();
    }

  if(USART_GetITStatus(USARTy, USART_IT_TXE) != RESET)
  {   
    /* Write one byte to the transmit data register */
    USART_SendData(USARTy, TxBuffer[TxCounter++]);

    if(TxCounter == NbrOfDataToTransfer)
    {
      /* Disable the USARTy Transmit interrupt */
      USART_ITConfig(USARTy, USART_IT_TXE, DISABLE);
    }    
  }
}

/**
  * @brief  This function handles TIM2 global interrupt request.
  * @param  Каждые 0,01 секунду
  * @retval None
  */
void TIM2_IRQHandler(void) 
{
    if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)        TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
    countTime++;
    if(countTime == 3)  //       0.03 с
    {
        LINE2_On();        LINE1_Off();        L2 = ON;        L1 = OFF;
    }
    if(countTime == 6)  //       0.06с
    {
        LINE3_On();        LINE2_Off();        L3 = ON;        L2 = OFF;
    }
    if(countTime == 9)  //       0.09 с
    {
        LINE4_On();        LINE3_Off();        L4 = ON;        L3 = OFF;
    }
    if(countTime == 12)  //       0.12 с
    {
        LINE5_On();        LINE4_Off();        L5 = ON;        L4 = OFF;
    }
    if(countTime == 15)  //       0.15 с
    {
        LINE1_On();        LINE5_Off();        L1 = ON;        L5 = OFF; 
    }
    if(countTime == 15)  //       0.15 с
    {
        if (Counter.by15ms == 4) 
        {
            if(Flag.AccessCard.Read)   
            {   
                CDcard();
                Flag.AccessCard.Read = OFF;
                Flag.OutputToDisplay.OutputKeyboardNumber = ON;
                Flag.OutputToDisplay.All = ON;
                Flag.OutputToDisplay.OutputOnlyEnter = ON;
                DisplayClear(); // Очистка дисплея
                ReturnHome(); // Возврат курсора в начало строки
                WriteTxt(text_PKD_1);
                for (Counter.card_i = 0; Counter.card_i <25; Counter.card_i++) snCard[Counter.card_i] = 0x00; //Стриание данных о считанной карты
                Flag.AccessCard.Find = ON;
//                ReadCD(0, readBuffer, basaELEM);
            }
            exti = 0;
            Counter.by15ms = 0;
        }
        Counter.by15ms++;
        if (Counter.WaitExti == 100) {Counter.by15ms=0;   FANTI_DREB_KNOPKA = 0;}
        Counter.WaitExti++;
        countTime = 0;
    }
}

/**
 * @brief  This function handles USART3 global interrupt request.
 * @param  None
 * @retval None
 */
void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		/* Read one byte from the receive data register */
	}
}

void EXTI0_IRQHandler(void) // Обработчик прерываний первой линии
{
    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
        Counter.by15ms = 0;
        EXTI_ClearITPendingBit(EXTI_Line0);
        EXTI_ClearITPendingBit(EXTI_Line1);
        EXTI_ClearITPendingBit(EXTI_Line2);
//        LED1_On();
        if(exti == 0)
        {
            if (L1)         {exti = CODE_ONE;       return;}
            if (L2)         {exti = CODE_THREE;     return;}
            if (L3)         {exti = CODE_FIVE;      return;}
            if (L4)         {exti = CODE_SEVEN;     return;}
            if (L5)         {exti = CODE_NINE;      return;}
        }
    }
}
void EXTI1_IRQHandler(void) // второй линии
{
    if(EXTI_GetITStatus(EXTI_Line1) != RESET)
    {
        Counter.by15ms = 0;
        EXTI_ClearITPendingBit(EXTI_Line0);
        EXTI_ClearITPendingBit(EXTI_Line1);
        EXTI_ClearITPendingBit(EXTI_Line2);
//        LED2_On();
        if(exti == 0)
        {
            if (L1)         {exti = CODE_TWO;       return;}
            if (L2)         {exti = CODE_FOUR;      return;}
            if (L3)         {exti = CODE_SIX;       return;}
            if (L4)         {exti = CODE_EIGHT;     return;}
            if (L5)         {exti = CODE_ZERO;      return;}
        }
    }
}
void EXTI2_IRQHandler(void) // третьей линии
{
    if(EXTI_GetITStatus(EXTI_Line2) != RESET)
    {
        Counter.by15ms = 0;
        EXTI_ClearITPendingBit(EXTI_Line0);
        EXTI_ClearITPendingBit(EXTI_Line1);
        EXTI_ClearITPendingBit(EXTI_Line2);
//        LED3_On();
        if(exti == 0)
        {
            if (L1)         {exti = CODE_ARROW_LEFT;        return;}
            if (L2)         {exti = CODE_CANCEL;            return;}
            if (L3)         {exti = CODE_MENU;              return;}
            if (L4)         {exti = CODE_STOP;              return;}
            if (L5)         {exti = CODE_ENTER;             return;}

        }
    }
}

void EXTI3_IRQHandler(void) // Data 0
{
    Flag.AccessCard.Read = ON;
    Counter.by15ms = 0;
    if(EXTI_GetITStatus(EXTI_Line3) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line3);
        snCard[Counter.card_i++] = 0x01;
    }
}
void EXTI4_IRQHandler(void) // Data 1
{
    Flag.AccessCard.Read = ON;
    Counter.by15ms = 0;
    if(EXTI_GetITStatus(EXTI_Line4) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line4);
        snCard[Counter.card_i++] = 0x00;
    }
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
 * @brief  This function handles PPP interrupt request.
 * @param  None
 * @retval None
 */
/*void PPP_IRQHandler(void)
 {
 }*/

/**
 * @}
 */

/**
 * @}
 */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
