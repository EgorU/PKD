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
/*      ������ ���� ������������ � �������
 *      +----------------basaELEM[19]---------------------------------------------------+
 *      |               |     0      |   1   |   2   |    3  |   4   |   5   |  ...  |   18  |
 *      basa[1024]------+------------+-------+-------+-------+-------+-------+-------+-------+
 *      |               | ���������  |   �   |������ |   ����                                | 
 *      |---------------+------------+-------+-------+---------------------------------------+
 *      |               |            |       |       |                                       |
 *      ���� ����������� ��������� ������ �����
 *      �����   |       ����
 *      3..6    |       0..31
 *      7..10   |       32..63
 *      11..14  |       64..95
 *      15..18  |       96..127
 *        
 *      ������� �������� ������� � ��
 *      +----------------basaELEM[19]-----------------------------------------------------------------------------------------------------------------------------------+
 *      |               |   0   |   1   |   2   |    3  |   4   |   5   |    6  |   7   |   8   |   9   |  10   |   11  |   12  |   13  |   14  |  15   |   16  |   17  |
 *      basa[1024]------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
 *      |               |  ���  |  � �  | �� ���|  ���  |     ������ �������    |       ����� � 1       |       ����� � 2       |  �1   |  �2   |   �3  |   �4  |   �5  | 
 *      |-----------------------+-----------------------+-----------------------+-----------------------+-----------------------+-------+-------+-------+-------+-------+
 *      |                       |                       |                       |                       |                       |       |       |       |       |       |
 *      
 *      K1      ����� ����
 *      K2      ��� �������� (0�00 - ����� � ������, 0�01 - ��������� �� ������)
 *      K3, K4  ����� (������ ������ 1234 �3 = 12, �4 = 34)
 *      K5      0xFF
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/



/* Private variables ---------------------------------------------------------*/
struct card AccessCard;
struct cardPass PasswordCard;


// ������� ������� � �������� ������ � ���������� ��� �������� ������ ����� �����
uint32_t writeBuffer[basaSise];
uint32_t readBuffer[basaSise];
uint32_t BaseStaff[basaCARDS][basaELEM]; // � ������ ������� basaCARDS ������ ����� � 1 ��� �� �� ����������� ������ � ���������� � �� �����
uint8_t zone[MAXzon];
uint8_t arhivZone[MAXzonArhiv];
uint8_t card[basaELEM]; //�����

uint16_t lastCARDS;     //����� �����
uint16_t counterELEM;   //������ ��������� �����

uint16_t i;           //C������ ����
uint16_t j;           //������� ����� �����

uint16_t counter;       //������� �������

/* Private function prototypes -----------------------------------------------*/
void Zon(uint32_t, uint32_t, uint32_t, uint32_t);       // ��������� ��������� 128 ���
void WriteAllBase();                                    // ���������� ��� ���� ����
void WriteOneCardBase();                                // ���������� 1-� ����� ��������� �������
void DeleteAllBase();                                   // ������� ��� ���� ����
void DeleteOneCardBase(uint8_t);                        // ������� 1-� �������� �����

void ReadSD(uint8_t adr, uint32_t* write, uint16_t size);
void WriteSD(uint8_t adr, uint32_t* write, uint16_t size);

/* Private functions ---------------------------------------------------------*/

/*      ���������� �� SD ����� ���� ����
 *      �����, ������, ����
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

/*      ���������� �� SD 1 �����
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

/*     ����� �����
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

/*      ���������� �� SD ����� ���� ����
 *      �����, ������, ����
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

/*      ���������� �� SD 1 �����
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

/**@brief  ����������� ���������� 1 � ����� ���, ���� ���� �������
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
