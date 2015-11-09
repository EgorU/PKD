/**
  ******************************************************************************
  * Проект для STM32F103RE
  * main.c
  * Работает
  * Инициализация переферии
  * Работа с клавиатурой
  * 15.05.2015
  ******************************************************************************
  */
#include "stm32f10x.h"
#include "stm32f10x_it.h"
/* Includes ------------------------------------------------------------------*/
#include "string.h"
#include "stdlib.h"
#include "math.h"
#include "main.h"
#include "stdio.h"
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "led.h"
#include "SystemConfiguration.h"
#include "stm32f10x_it.h"
#include "baza.h"
#include "keyboard.h"
#include "USART.h"
/* Private typedef -----------------------------------------------------------*/
//SD_CardInfo SDCardInfo;
USART_InitTypeDef USART_InitStructure;
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
struct parentsFlag Flag;
struct parentsCounter Counter;

 void Delay(uint32_t);
 void TimingDelayDecrement(void);
extern  void InitSystemConfiguration(void);
extern  void WriteData(uint8_t);
extern uint8_t exti;
__IO extern uint32_t delay;


int main(void)
{
    SystemInit(); // Инициализация
    InitSystemConfiguration(); // Инициализация
    
/*
    Приветствие
    DisplayClear(); // Очистка дисплея
    ReturnHome(); // Возврат курсора в начало строки
    WriteTxt(text_start0);
    MoveCursor_UpDown(DOWN);
    WriteTxt(text_start1);
 */
    
/*
    Установка стартовых значений
    */
    exti = 0;
    Flag.OutputToDisplay.All = 0;
    Flag.OutputToDisplay.OutputKeyboardNumber = 0;
    Flag.OutputToDisplay.OutputFromPKD = 0;
    Flag.OutputToDisplay.OutputOnlyEnter = 0;

////////////////////////////////////

    DisplayClear(); // Очистка дисплея
    ReturnHome(); // Возврат курсора в начало строки
    WriteTxt(text_PKD_0);

    while(1)
    {
        if (Flag.OutputToDisplay.All) AnalyzeEXTI();
    }  //End while
}

 void Delay(uint32_t time)
{
	delay = time;
	while(delay)
        {
            delay--;
            if (delay>1000000) delay = 100;
        }
}
#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
                        DisplayClear(); // Очистка дисплея
                        WaitBusyFlag();
                        ReturnHome(); // Возврат курсора в начало строки
                        WaitBusyFlag();
                        WriteTxt(text_FATALYTY);
                        WaitBusyFlag();
	while (1)
	{

	}

}
#endif
