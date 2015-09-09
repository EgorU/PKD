/*
 * main.h
 *
 *  Created on: 28.02.2013
 *      Author: Администратор
 */

#ifndef MAIN_H_
#define MAIN_H_

//////////Адрес ПКД////////////////////
#define ADDRESS_PKD        1
///////////////////////////////////////
// Codes of the keyboard
#define NO_SYMBOL        0
#define CODE_ONE         177
#define CODE_TWO         178
#define CODE_THREE       180
#define CODE_FOUR        209
#define CODE_FIVE        210
#define CODE_SIX         212
#define CODE_SEVEN       113
#define CODE_EIGHT       114
#define CODE_NINE        116
#define CODE_ZERO        226
#define CODE_ARROW_LEFT  225
#define CODE_ARROW_RIGHT 228
#define CODE_STOP        184
#define CODE_CANCEL      216
#define CODE_MENU        120
#define CODE_ENTER       232
#define SYMBOL_SHOWN     255

#define SYMBOL_ZERO      48
#define SYMBOL_ONE       49
#define SYMBOL_TWO       50
#define SYMBOL_THREE     51
#define SYMBOL_FOUR      52
#define SYMBOL_FIVE      53
#define SYMBOL_SIX       54
#define SYMBOL_SEVEN     55
#define SYMBOL_EIGHT     56
#define SYMBOL_NINE      57
#define SYMBOL_EMPTY     32

#define ON     1
#define OFF    0

// Антидребезг
#define COUNTXWAIT       150



#define text_start0     "      Телекорт"
#define text_start1     "  ПКД 2015 готово"
#define text_FATALYTY   "FATALYTY ERROR"

#define text_PKD_0      " йПриложите карту"
#define text_PKD_1      "zаВведите зону  "
#define text_PKD_2      "1=On 2=Off"
#define text_PKD_3      "Подтвердите действие"

/***********_Базовые_Флаги_***************
 */
extern struct parentsFlag
{
    struct heirFlag1
    {
          uint8_t OutputFromPKD;
          uint8_t OutputFromKeyboard;
          uint8_t OutputAnalysis;
          uint8_t OutputKeyboardNumber;
          uint8_t OutputOnlyUnlock;
          uint8_t OutputOnlyEnter;
          uint8_t All;
    } OutputToDisplay;

    struct heirFlag2
    {
          uint8_t Find;
          uint8_t DirectManipulation;
          uint8_t Read;
    } AccessCard;


    struct heirFlag3
    {
          uint8_t ChangeData;
          uint8_t OperationFlashCard;
    } FlashCard;

    struct heirFlag4
    {
          uint8_t InPut;
          uint8_t OutPut;
    } SendData;

    struct heirFlag5
    {
          uint8_t ErrorInput;
          uint8_t ErrorRead;
          uint8_t ErrorWrite;
    } Error;
} Flag;

extern struct parentsCounter
{
    uint8_t DelSymbol;
    uint8_t WaitExti;
    uint8_t by15ms;
    uint8_t card_i;
} Counter;

#endif /* MAIN_H_ */
