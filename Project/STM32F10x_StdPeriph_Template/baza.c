/**
  ******************************************************************************
  * @file    SDIO/sdcard.c 
  * @author  MCD Application Team
  * @version V3.1.2
  * @date    09/28/2009
  * @brief   This file provides all the SD Card driver firmware functions.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "sdcard.h"
#include "baza.h"
#include "SystemConfiguration.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"
#include "stdio.h"
#include "main.h"
/*      Запись базы производится в таблицу
 *      +----------------basaELEM[19]---------------------------------------------------+
 *      |               |     0      |   1   |   2   |    3  |   4   |   5   |  ...  |   18  |
 *      basa[1024]------+------------+-------+-------+-------+-------+-------+-------+-------+
 *      |               | Семейство  |   №   |Пароль |   зоны                                | 
 *      |---------------+------------+-------+-------+---------------------------------------+
 *      |               |            |       |       |                                       |
 *      Зоны разрешённые владельцу жанной карты
 *      Байты   |       Зоны
 *      3..6    |       0..31
 *      7..10   |       32..63
 *      11..14  |       64..95
 *      15..18  |       96..127
 *        
 *      Таблица отправки команды в ЦУ
 *      +----------------basaELEM[19]-----------------------------------------------------------------------------------------------------------------------------------+
 *      |               |   0   |   1   |   2   |    3  |   4   |   5   |    6  |   7   |   8   |   9   |  10   |   11  |   12  |   13  |   14  |  15   |   16  |   17  |
 *      basa[1024]------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
 *      |               |  Адр  |  Ч б  | Гр ком|  Ком  |     Статус события    |       Карта № 1       |       Карта № 2       |  К1   |  К2   |   К3  |   К4  |   К5  | 
 *      |-----------------------+-----------------------+-----------------------+-----------------------+-----------------------+-------+-------+-------+-------+-------+
 *      |                       |                       |                       |                       |                       |       |       |       |       |       |
 *      
 *      K1      Номер зоны
 *      K2      Код операции (0х00 - снять с охраны, 0х01 - поставить на охрану)
 *      K3, K4  Проль (Пример пароль 1234 К3 = 12, Л4 = 34)
 *      K5      0xFF
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/



/* Private variables ---------------------------------------------------------*/
struct card AccessCard;
struct cardPass PasswordCard;


// Массивы входных и выходных данных и переменная для хранения данных нашей карте
uint32_t writeBuffer[basaSise];
uint32_t readBuffer[basaSise];
uint32_t BaseStaff[basaCARDS][basaELEM]; // В данном массиве basaCARDS всегда стоит в 1 что бы не дублировать запись в контроллер и на карту
uint8_t zone[MAXzon];
uint8_t arhivZone[MAXzonArhiv];
uint8_t card[basaELEM]; //карта

uint16_t lastCARDS;     //Номер Карты
uint16_t counterELEM;   //Данные выбранной карты

uint16_t i;           //Cчётчик карт
uint16_t j;           //Счётчик даных карты

uint16_t counter;       //Простой счётчик

/* Private function prototypes -----------------------------------------------*/
void Zon(uint32_t, uint32_t, uint32_t, uint32_t);       // Извлекает состояние 128 зон
void WriteAllBase();                                    // Записывает всю базу карт
void WriteOneCardBase();                                // Записывает 1-у карту последним номером
void DeleteAllBase();                                   // Удаляет всю базу карт
void DeleteOneCardBase(uint8_t);                        // Удаляет 1-у заданную карту

void ReadSD(uint8_t adr, uint32_t* write, uint16_t size);
void WriteSD(uint8_t adr, uint32_t* write, uint16_t size);

/* Private functions ---------------------------------------------------------*/

/*      Записывает на SD карту базу карт
 *      Карта, пароль, зоны
 */
void WriteAllBase()
{
    for(i=0;i<basaCARDS;i++)
    {
        BaseStaff[i][0] = AccessCard.Family;
        BaseStaff[i][1] = AccessCard.NumCard;
        BaseStaff[i][2] = PasswordCard.Pas;
        for(j=3;j<basaELEM;j++)
        {
                BaseStaff[i][j] = zone[j];
        }
        WriteSD((i * basaELEM), BaseStaff[1], basaELEM);
    }
}

/*      Записывает на SD 1 карту
 */
void WriteOneCardBase()
{
    i = lastCARDS;
    lastCARDS++;
    BaseStaff[0][0] = AccessCard.Family;
    BaseStaff[0][1] = AccessCard.NumCard;
    BaseStaff[0][2] = PasswordCard.Pas;
    for(j=3;j<basaELEM;j++)
    {   
        BaseStaff[0][j] = zone[j];
    }
    WriteSD((i * basaELEM), BaseStaff[0], basaELEM);

}

/*     Поиск карты
 */
void FaindCardBase()
{
    i = lastCARDS;
    lastCARDS++;
    for(i=0;i<basaCARDS;i++)
    {
        if ((BaseStaff[i][0] == AccessCard.Family) &&
            (BaseStaff[i][1] == AccessCard.NumCardHi) &&
            (BaseStaff[i][2] == AccessCard.NumCardLo))
        for(j=3;j<basaELEM;j++)
        {   
                BaseStaff[i][j] = zone[j];
        }
        break;
    }
    BaseStaff[i][0] = AccessCard.Family;
    BaseStaff[i][1] = AccessCard.NumCardHi;
    BaseStaff[i][2] = AccessCard.NumCardLo;
    BaseStaff[i][3] = PasswordCard.PasHi;
    BaseStaff[i][4] = PasswordCard.PasLo;
    for(j=5;j<basaELEM;j++)
    {   
            BaseStaff[i][j] = zone[j];
    }
}

/*      Записывает на SD карту базу карт
 *      Карта, пароль, зоны
 */
void DeleteAllBase()
{
    for(i=0;i<basaCARDS;i++)
    {
        for(j=0;j<basaELEM;j++)
        {   
                BaseStaff[i][j] = 0x00;
        }
        WriteSD((i * basaELEM), BaseStaff[i], basaELEM);
    }
}

/*      Записывает на SD 1 карту
 */
void DeleteOneCardBase(uint8_t DelCard)
{
    i = DelCard;
    for(j=0;j<basaELEM;j++)
    {   
        BaseStaff[i][j] = 0x00;
    }
    WriteSD((i * basaELEM), BaseStaff[i], basaELEM);
}

void WriteSD(uint8_t adr, uint32_t* write, uint16_t size)
{
   SD_WriteBlock(adr, write, size);
}

void ReadSD(uint8_t adr, uint32_t* readBuffer, uint16_t size)
{
    Flag.AccessCard.Find = OFF;
    WriteOneCardBase();
    SD_ReadBlock(adr, readBuffer, size);  
}

/**@brief  Преобразует записывает 1 в буфер зон, если зона активна
  */
void Zon(uint32_t zon1, uint32_t zon2, uint32_t zon3, uint32_t zon4)
{
    uint32_t TemporaryZones;
    TemporaryZones = zon1;
    counter = 0;
    while (TemporaryZones > 1)
    {
      zone[counter++] = (TemporaryZones - (TemporaryZones / 2) * 2);
      TemporaryZones = TemporaryZones / 2;
    }
    if (TemporaryZones != 0)        zone[counter] = 1;

    counter = 32;
    TemporaryZones = zon2;
    while (TemporaryZones > 1)
    {
        zone[counter++] = (TemporaryZones - (TemporaryZones / 2) * 2);
        TemporaryZones = TemporaryZones / 2;
    }
    if (TemporaryZones != 0)        zone[counter] = 1;

    counter = 64;
    TemporaryZones = zon3;
    while (TemporaryZones > 1)
    {
        zone[counter++] = (TemporaryZones - (TemporaryZones / 2) * 2);
        TemporaryZones = TemporaryZones / 2;
    }
    if (TemporaryZones != 0)        zone[counter] = 1;

    counter = 96;
    TemporaryZones = zon4;
    while (TemporaryZones > 1)
    {
        zone[counter++] = (TemporaryZones - (TemporaryZones / 2) * 2);
        TemporaryZones = TemporaryZones / 2;
    }
    if (TemporaryZones != 0)        zone[counter] = 1;
 }    
