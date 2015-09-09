#include "stm32f10x.h"
#include "main.h"
#include "stm32f10x_rcc.h"
#include "led.h"
#include "SystemConfiguration.h"
#include "stm32f10x_it.h"
#include "baza.h"
#include "keyboard.h"





uint8_t FANTI_DREB_KNOPKA;       //флаг антидребезга
extern uint8_t number = 0;




uint8_t ASCIItoTable(uint8_t* string)
{
/*  for(uint8_t sds = 0;sds<sizeof(alphabet);sds++)
    if(string[sds]==alphabet[sds])
      stringNew[sds]=alphabet2[sds];*/
  return 1;
}

void Keyboard()
{
               DisplayClear(); // Очистка дисплея
                ReturnHome(); // Возврат курсора в начало строки
                WriteTxt(text_PKD_0);
     Flag.OutputToDisplay.OutputFromKeyboard = OFF;
    if((exti != NO_SYMBOL) && (exti != SYMBOL_SHOWN) ) 
        {
            if (FANTI_DREB_KNOPKA)//флаг антидребезга
            {
                Counter.WaitExti++;
                if(Counter.WaitExti < COUNTXWAIT)
                {
                    FANTI_DREB_KNOPKA = 0;
                    exti = SYMBOL_SHOWN;
                    return;
                }
            }
AnalyzeEXTI();
                DisplayClear(); // Очистка дисплея
                ReturnHome(); // Возврат курсора в начало строки
                WriteTxt(text_PKD_0);
            if((Flag.OutputToDisplay.OutputOnlyUnlock) && ((exti == CODE_CANCEL) || (exti == CODE_MENU))) //Вывод блокировок
                {}
            else if((Flag.OutputToDisplay.OutputKeyboardNumber) && ((exti != CODE_CANCEL) && (exti != CODE_MENU))) //Вывод цифр и управл ими
                {AnalyzeEXTI();}
        }
}



void AnalyzeEXTI() // Анализ нажатий кнопок на клавиатуре
{
    FANTI_DREB_KNOPKA = 1;
    switch(exti)
    {
        case CODE_ONE:
            if (Flag.OutputToDisplay.OutputKeyboardNumber)
            {
                number = number*10 + 1;
                exti = SYMBOL_SHOWN;
                WriteData(SYMBOL_ONE);
//                Flag.OutputToDisplay.OutputKeyboardNumber = OFF;
                break;
            }
            else break;
                
        case CODE_TWO:
            if (Flag.OutputToDisplay.OutputKeyboardNumber)
            {
                number = number*10 + 2;
                exti = SYMBOL_SHOWN;
                WriteData(SYMBOL_TWO);
//                Flag.OutputToDisplay.OutputKeyboardNumber = OFF;
                break;
            }
            else break;
                
        case CODE_THREE:
            if (Flag.OutputToDisplay.OutputKeyboardNumber)
            {
                number = number*10 + 3;
                exti = SYMBOL_SHOWN;
                WriteData(SYMBOL_THREE);
//                Flag.OutputToDisplay.OutputKeyboardNumber = OFF;
                break;
            }
            else break;
                
        case CODE_FOUR:
            if (Flag.OutputToDisplay.OutputKeyboardNumber)
            {
                number = number*10 + 4;
                exti = SYMBOL_SHOWN;
                WriteData(SYMBOL_FOUR);
//                Flag.OutputToDisplay.OutputKeyboardNumber = OFF;
                break;
            }
            else break;
                
        case CODE_FIVE:
            if (Flag.OutputToDisplay.OutputKeyboardNumber)
            {
                number = number*10 + 5;
                exti = SYMBOL_SHOWN;
                WriteData(SYMBOL_FIVE);
//                Flag.OutputToDisplay.OutputKeyboardNumber = OFF;
                break;
            }
            else break;
                
        case CODE_SIX:
            if (Flag.OutputToDisplay.OutputKeyboardNumber)
            {
                number = number*10 + 6;
                exti = SYMBOL_SHOWN;
                WriteData(SYMBOL_SIX);
//                Flag.OutputToDisplay.OutputKeyboardNumber = OFF;
                break;
            }
            else break;
                
        case CODE_SEVEN:
            if (Flag.OutputToDisplay.OutputKeyboardNumber)
            {
                number = number*10 + 7;
                exti = SYMBOL_SHOWN;
                WriteData(SYMBOL_SEVEN);
//                Flag.OutputToDisplay.OutputKeyboardNumber = OFF;
                break;
            }
            else break;
                
        case CODE_EIGHT:
            if (Flag.OutputToDisplay.OutputKeyboardNumber)
            {
                number = number*10 + 8;
                exti = SYMBOL_SHOWN;
                WriteData(SYMBOL_EIGHT);
//                Flag.OutputToDisplay.OutputKeyboardNumber = OFF;
                break;
            }
            else break;
                
        case CODE_NINE:
            if (Flag.OutputToDisplay.OutputKeyboardNumber)
            {
                number = number*10 + 9;
                exti = SYMBOL_SHOWN;
                WriteData(SYMBOL_NINE);
//                Flag.OutputToDisplay.OutputKeyboardNumber = OFF;
                break;
            }
            else break;
                
        case CODE_ZERO:
            if (Flag.OutputToDisplay.OutputKeyboardNumber)
            {
                number = number*10 + 0;
                exti = SYMBOL_SHOWN;
                WriteData(SYMBOL_ZERO);
//                Flag.OutputToDisplay.OutputKeyboardNumber = OFF;
                break;
            }
            else break;
                
        case CODE_STOP:
            if (Flag.OutputToDisplay.OutputFromPKD)
            {
                exti = SYMBOL_SHOWN;
                DisplayClear(); // Очистка дисплея
                ReturnHome(); // Возврат курсора в начало строки
                Flag.OutputToDisplay.OutputFromPKD = OFF;
                break;
            }
            else break;
               
        case CODE_ARROW_LEFT:
            if (Flag.OutputToDisplay.OutputKeyboardNumber)
            {
                exti = SYMBOL_SHOWN;
                MoveCursor_ShiftDisplay(CURSOR_SHIFT, LEFT);
                WriteData(SYMBOL_EMPTY);
                MoveCursor_ShiftDisplay(CURSOR_SHIFT, LEFT);
//                Flag.OutputToDisplay.OutputKeyboardNumber = OFF;
                break;
            }
            else break;
              
        case CODE_ARROW_RIGHT:
            if (Flag.OutputToDisplay.OutputKeyboardNumber)
            {
                exti = SYMBOL_SHOWN;
                MoveCursor_ShiftDisplay(CURSOR_SHIFT, RIGHT);
                WriteData(SYMBOL_EMPTY);
                MoveCursor_ShiftDisplay(CURSOR_SHIFT, RIGHT);
//                Flag.OutputToDisplay.OutputKeyboardNumber = OFF;
                break;
            }
            else break;
              
        case CODE_ENTER:
            if (Flag.OutputToDisplay.OutputOnlyEnter)
            {
                exti = SYMBOL_SHOWN;
                MoveCursor_ShiftDisplay(DISPLAY_SHIFT, RIGHT);
                Flag.OutputToDisplay.OutputOnlyEnter = OFF;
                Flag.OutputToDisplay.OutputKeyboardNumber = OFF;
                break;
            }
            else break;
              
        case CODE_CANCEL:
            if (Flag.OutputToDisplay.OutputOnlyUnlock)
            {
                exti = SYMBOL_SHOWN;
                DisplayClear(); // Очистка дисплея
                ReturnHome(); // Возврат курсора в начало строки
                WriteTxt(text_PKD_0);
                WaitBusyFlag();
                Flag.OutputToDisplay.OutputOnlyUnlock = OFF;
                Flag.OutputToDisplay.OutputOnlyEnter = ON;
                break;
            }
            else break;
             
        case CODE_MENU:
            if (Flag.OutputToDisplay.OutputOnlyUnlock)
            {
                exti = SYMBOL_SHOWN;
                Flag.OutputToDisplay.OutputOnlyUnlock = OFF;
            }
            else break;
    }
}

