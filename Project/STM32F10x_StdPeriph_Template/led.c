/*
  ******************************************************************************
  * Дисплей
  *
  ******************************************************************************
  *  Контакт    |    Значение   |       Пин     |     Шлейф     |
  *-------------+---------------+---------------+---------------+
  *     1       |       VCC     |       +5V     |       -       |
  *     2       |       VDD     |       0V      |       -       |
  *     3       |       -       |       -       |       -       |
  *     4       |       RS      |       PC4     |       -       |
  *     5       |       RW      |       PC5     |       -       |
  *     6       |       E       |       PB0     |       1       |
  *     7       |       DB0     |       PB1     |       2       |
  *     8       |       DB1     |       PB2     |       3       |
  *     9       |       DB2     |       PB10    |       4       |
  *     10      |       DB3     |       PB11    |       5       |
  *     11      |       DB4     |       PB12    |       6       |
  *     12      |       DB5     |       PB13    |       7       |
  *     13      |       DB6     |       PB14    |       8       |
  *     14      |       DB7     |       PB15    |       9       |
  *     15      |       -       |       -       |       -       |
  *     16      |       -       |       -       |       -       |
  ******************************************************************************
 */

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "led.h"
#include "SystemConfiguration.h"
#include "keyboard.h"

#include "string.h"
#include "stdlib.h"
#include "math.h"
#include "stdio.h"
#include "main.h"


uint8_t alphabet[] = {"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюяABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghigklmnopqrstuvwxyz 0123456789"};
uint8_t alphabet2[] = {
  0xE3,0xA0,0x42,0xA1,0xE0,0x45,0xA2,0xA3,0xA4,0xA5,0xA6,0x4B,0xA7,0x4C,0x48,0x4F,0xA8,0x50,0x43,0x54,0xA9,0xAA,0x58,0xE1,0xAB,0xAC,0xE2,0xAD,0xAE,0xC4,0xAF,0xB0,0xB1,  //Русский заглавные
  0x61,0xB2,0xB3,0xB4,0xE3,0x65,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0x6F,0xBE,0x70,0x63,0xBF,0x79,0xE4,0x78,0xE5,0xC0,0xC1,0xE6,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,  //Русский прописные
  0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A, //Английский заглавный
  0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A, //Английский прописной
  0x20,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39};   // " 0123456789"

uint8_t position;

void InitLED(void);
static  void FunctionSet(data_length, number_of_display_lines, character_font_set, font_table_selection);
void WaitBusyFlag(void);
static  void DisplayOnOffControl(display_on_off, display_cursor, blink_on_off);
void DisplayClear(void);
void ReturnHome(void);
static  void EntryModeSet(increment_decrement_bit, shift_entire_display);
static  void PushIntoPort(uint8_t);
void WriteData(uint8_t);
static  void SetDataPortInInputMode(void);
static  void SetDataPortInOutputMode(void);
//static  void SetCGRAMAddress(uint8_t);
//static  void SetDDRAMAddress(uint8_t);
void MoveCursor_ShiftDisplay(cursor_display, left_right);
void MoveCursor_UpDown(up_down);

extern  void InitPort(GPIO_TypeDef *, uint16_t, GPIOSpeed_TypeDef, GPIOMode_TypeDef);

extern  void Delay(uint32_t);

void InitLED(void) // Инициализация дисплея
{
        Delay(100);
	resetEnable(); // Сброс флага Enable
	resetRW(); // Сброс флага RW
	resetRS(); // Сброс флага RS
        FunctionSet(EIGHT_BITS, TWO_LINES, CHARACTER_5x8, ENGLISH_RUSSIAN_CHARACTER_FONT_TABLE); // Задание режима работы дисплея
        //0x0011 1010
	WaitBusyFlag(); // Ожидание завершения команды
	DisplayOnOffControl(DISPLAY_OFF, CURSOR_OFF, BLINK_OFF); //Выключение дисплея
	WaitBusyFlag();
	DisplayOnOffControl(DISPLAY_ON, CURSOR_ON, BLINK_OFF); // Включение дисплея
	WaitBusyFlag();
	DisplayClear(); // Очистка дисплея
	WaitBusyFlag();
	EntryModeSet(INCREMENT_BIT, DO_NOT_SHIFT_DISPLAY); // Задание режима работы курсора дисплея
	WaitBusyFlag();
	ReturnHome(); // Возврат курсора в начало строки
	WaitBusyFlag();
}

static  void FunctionSet(data_length length, number_of_display_lines display_lines, character_font_set font_set, font_table_selection font_table)
{
	uint8_t data = 0;
	data = (uint8_t)font_table + ((uint8_t)font_set << 2) + ((uint8_t)display_lines << 3) + ((uint8_t)length << 4) + (1 << 5);
	PushIntoPort(data);
}

static  void DisplayOnOffControl(display_on_off on_off, display_cursor cursor, blink_on_off blink)
{
	uint8_t data = 0;
	data = (uint8_t)blink + ((uint8_t)cursor << 1) + ((uint8_t)on_off << 2) + (1 << 3);
	PushIntoPort(data);
}

 void WaitBusyFlag(void)
{
	resetRS();
	setRW();
	SetDataPortInInputMode();
        Delay(100);
	while(isbusy())
	{
		resetEnable();
		Delay(100);
		setEnable();
	}
	Delay(100);
	resetRW();
	SetDataPortInOutputMode();
}

 void DisplayClear(void)
{
	PushIntoPort(1);
        position = 0;
}

 void ReturnHome(void)
{
	PushIntoPort(2);
}

static  void EntryModeSet(increment_decrement_bit inc_dec_bit, shift_entire_display shift_disp)
{
	uint8_t data = 0;
	data = (uint8_t)shift_disp + (((uint8_t)inc_dec_bit) << 1) + (1 << 2);
	PushIntoPort(data);
}

static  void PushIntoPort(uint8_t data)
{
	resetEnable();
	setdata(0);
//	Delay(100);
	setEnable();
	setdata(data);
//	Delay(100);
	resetEnable();
}

 void WriteData(uint8_t data)
{
    uint8_t Nsymbol = 0x00;
    uint8_t Cposition = 0x00;
    uint8_t Nposition = 0x00;
    Nposition = position;
    ReturnHome(); // Возврат курсора в начало строки
    while(Cposition<(Nposition))
    {
        MoveCursor_ShiftDisplay(CURSOR_SHIFT,RIGHT);
        Cposition++;
    }
    
    while(Nsymbol<sizeof(alphabet))
    {
        if(data==alphabet[Nsymbol])
        {
            data=alphabet2[Nsymbol];
            break;
        }
        Nsymbol++;
    }
    Nsymbol = 0x00;
    
    setRS();
    resetRW();
    PushIntoPort(data);
    WaitBusyFlag();
    position++;

}

static  void SetDataPortInInputMode(void) // Установка порта в режим приёма
{
	//GPIOB->CRL &= 0xfffff00f;
	//GPIOB->CRH &= 0x000000ff;
	InitPort(GPIOB, DATAPORT, GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING);
}

static  void SetDataPortInOutputMode(void) // Установка порта в режим передачи
{
	//GPIOB->CRL |= 0x00000330;
	//GPIOB->CRH |= 0x33333300;
	InitPort(GPIOB, DATAPORT, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);
}
/*
 void SetCGRAMAddress(uint8_t address)
{
	uint8_t data = address + (1 << 6);
	PushIntoPort(data);
}

 void SetDDRAMAddress(uint8_t address)
{
	uint8_t data = address + (1 << 7);
	PushIntoPort(data);
}*/

 void MoveCursor_ShiftDisplay(cursor_display cur_disp, left_right lef_rig) // Управление режимами работы курсора
{
	uint8_t data = (((uint8_t)lef_rig) << 2) + (((uint8_t)cur_disp) << 3) + (1 << 4);
        if (lef_rig == LEFT) position --;
	PushIntoPort(data);
}

void MoveCursor_UpDown(up_down updown)
{
    uint8_t R = 0x00;
    ReturnHome(); // Возврат курсора в начало строки
    while(R<(64))
    {
        MoveCursor_ShiftDisplay(CURSOR_SHIFT,RIGHT);
        R++;
    }
    position = 64;
}
void WriteTxt(uint8_t* text) // Инициализация дисплея
{
    for (uint8_t symbol = 0x00;text[symbol];symbol++)
    {
        WriteData(text[symbol]);
    }
}

