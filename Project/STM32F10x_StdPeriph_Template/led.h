/*
 * led.h
 *
 *  Created on: 28.02.2013
 *      Author: Администратор
 */

#ifndef LED_H_
#define LED_H_
#endif /* LED_H_ */

#include "stm32f10x_gpio.h"
#include "stm32f10x.h"

#define RS   GPIO_Pin_5
#define RW   GPIO_Pin_4
#define E    GPIO_Pin_0
#define DB0  GPIO_Pin_9
#define DB1  GPIO_Pin_2
#define DB2  GPIO_Pin_10
#define DB3  GPIO_Pin_11
#define DB4  GPIO_Pin_12
#define DB5  GPIO_Pin_13
#define DB6  GPIO_Pin_14
#define DB7  GPIO_Pin_15
#define DATAPORT DB0 | DB1 | DB2 | DB3 | DB4 | DB5 | DB6 | DB7
#define BUSY GPIO_Pin_15

#define DB0_bit()  (GPIO_SetBits(GPIOB, DB0))
#define DB1_bit()  (GPIOB->BSRR |= DB1)
#define DB2_bit()  (GPIOB->BSRR |= DB2)
#define DB3_bit()  (GPIOB->BSRR |= DB3)
#define DB4_bit()  (GPIOB->BSRR |= DB4)
#define DB5_bit()  (GPIOB->BSRR |= DB5)
#define DB6_bit()  (GPIOB->BSRR |= DB6)
#define DB7_bit()  (GPIOB->BSRR |= DB7)

#define setRS() (GPIOC->ODR |= RS)
#define resetRS() (GPIOC->ODR &= ~ RS)
#define setRW() (GPIOC->ODR |= RW)
#define resetRW() (GPIOC->ODR &= ~ RW)
#define setEnable() (GPIOB->ODR |= E)
#define resetEnable() (GPIOB->ODR &= ~ E)
#define setdata(x) {\
	GPIOB->ODR &= ~(DB0 | DB1 | DB2 | DB3 | DB4 | DB5 | DB6 | DB7); GPIOB->ODR |= (((x & 0x02) << 1) | ((x & 0xFC) << 8) | ((x & 0x01) << 9));}
#define isbusy() (GPIOB->IDR & BUSY)

typedef enum
{
	FOUR_BITS = 0,
	EIGHT_BITS = 1
}data_length;

typedef enum
{
	ONE_LINE = 0,
	TWO_LINES = 1
}number_of_display_lines;

typedef enum
{
	CHARACTER_5x8 = 0,
	CHARACTER_5x10 = 1
}character_font_set;

typedef enum
{
	ENGLISH_JAPANESE_CHARACTER_FONT_TABLE = 0,
	WESTERN_EUROPEAN_CHARACTER_FONT_TABLE = 1,
	ENGLISH_RUSSIAN_CHARACTER_FONT_TABLE = 2
}font_table_selection;

typedef enum
{
	DISPLAY_OFF = 0,
	DISPLAY_ON = 1
}display_on_off;

typedef enum
{
	CURSOR_OFF = 0,
	CURSOR_ON = 1
}display_cursor;

typedef enum
{
	BLINK_OFF = 0,
	BLINK_ON = 1
}blink_on_off;

typedef enum
{
	DECREMENT_BIT = 0,
	INCREMENT_BIT = 1
}increment_decrement_bit;

typedef enum
{
	DO_NOT_SHIFT_DISPLAY = 0,
	SHIFT_DISPLAY = 1
}shift_entire_display;

typedef enum
{
	CURSOR_SHIFT = 0,
	DISPLAY_SHIFT = 1
}cursor_display;

typedef enum
{
	LEFT = 0,
	RIGHT = 1
}left_right;

typedef enum
{
	UP = 0,
	DOWN = 1
}up_down;


void WaitBusyFlag(void);
void PushIntoPort(uint8_t);
void WriteData(uint8_t);
void SetDataPortInInputMode(void);
void SetDataPortInOutputMode(void);
void ReturnHome(void);
void MoveCursor_ShiftDisplay(cursor_display, left_right);
void MoveCursor_UpDown(up_down);
