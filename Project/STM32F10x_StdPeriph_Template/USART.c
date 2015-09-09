#include "stm32f10x_it.h"
#include "led.h"
#include "SystemConfiguration.h"
#include "main.h"
#include "USART.h"


/*******************************************************************************
 *                              Обмен по USART
 *******************************************************************************
 *      RxBuffer[]      - принимает данные по USART
 *      RxCounter       - Его личный счётчик
 *      TxBuffer[]      - Отправляет данные по USART
 *      TxCounter       - Его личный счётчик
 *
 *      RxMessage[]     - Записывает принятые сообщения
 *      
 */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t RXcount = 0;
uint8_t RXcountMessage = 0;
uint8_t RxMessage[USARTbufSise];

 
 uint8_t messegeTipe;            

 
 
 /* Private function prototypes -----------------------------------------------*/
void TxMessage(uint8_t, uint8_t, uint8_t, uint8_t,
            uint32_t, uint8_t, uint16_t, uint8_t, uint16_t,
            uint8_t, uint8_t, uint16_t, uint8_t*);
void TxMessageConf(uint8_t, uint8_t, uint8_t*);
/* Private functions ---------------------------------------------------------*/

void ReadUSART()
{
    uint8_t RXadr = RxCounter - 0x20;
    uint8_t RXLast = 0x00;
    if(ADDRESS_PKD == RxBuffer[RXadr])
    {
        if(RxCounter < 0x20)
        {
            RXadr = USARTbufSise - RxCounter;
        }
        if((RXadr + RxBuffer[RXadr + 1] + 1) > USARTbufSise) 
            RXLast = RXadr + RxBuffer[RXadr + 1] + 1 - USARTbufSise;
        else 
            RXLast = (RXadr + RxBuffer[RXadr + 1] + 1);
        if(RxBuffer[RXLast] == 0xFF)
        {
            while (RXcount < (RxBuffer[RXadr + 1] + 2))
            {
                if (RXadr == USARTbufSise)
                    RXadr = 0x00;
                RxMessage[RXcount] = RxBuffer[RXadr + RXcount];
                RXcount++;
            }
            RXcountMessage++;
        }
    }
}

void WriteUSART(uint8_t siseMsg)
{
    TxCounter = 0x00;
    while(TxCounter < siseMsg)
    {
        USART_SendData(USARTy, TxBuffer[TxCounter++]);
    }
}


void firstSendUSART()
{
    uint8_t ggg = 0x02;
    TxBuffer[0x00] = 1;
    TxBuffer[0x01] = 0x10;
    while(ggg < 0x10)
    {
        ggg++;
        TxBuffer[ggg] = ggg;
    }
    TxBuffer[0x11] = 0xFF;
    WriteUSART(0x12);
    ggg = 0x00;
        while(ggg < 0x20)
    {
        TxBuffer[ggg] = 0;
        ggg++;
    }
}

void ReadWrite()
{
    uint8_t ggg = 0x00;
    uint8_t ggg2 = 0x00;
    ggg2 = RXcountMessage;
    if (RXcountMessage>5)
    while(RXcountMessage)
    {
        while(ggg < (RxMessage[ggg2 - RXcountMessage + 1] + 1))
        {
            TxBuffer[ggg] = RxMessage[ggg];
            ggg++;
        }
        ggg = 0x00;
        RXcountMessage--;
 //       WriteUSART(0x14);
    }
    
}


void TxMessage(uint8_t adres, uint8_t gr_com, uint8_t com, uint8_t pkd,
            uint32_t comanda, uint8_t card1_gr, uint16_t card1_num, uint8_t card2_gr, uint16_t card2_num,
            uint8_t zone, uint8_t onoff, uint16_t pas, uint8_t* time)
{
    uint8_t ggg = 0x00;

    TxBuffer[0x00] = adres;
    TxBuffer[0x01] = 0x19;
    TxBuffer[0x02] = gr_com;
    TxBuffer[0x03] = com;
    TxBuffer[0x04] = 0x92;
    TxBuffer[0x05] = pkd;
    TxBuffer[0x06] = comanda;
    TxBuffer[0x07] = comanda;
    TxBuffer[0x08] = comanda;
    TxBuffer[0x09] = card1_gr;
    TxBuffer[0x0A] = card1_num;
    TxBuffer[0x0B] = card1_num;
    TxBuffer[0x0C] = card2_gr;
    TxBuffer[0x0D] = card2_num;
    TxBuffer[0x0E] = card2_num;
    TxBuffer[0x0F] = zone;
    TxBuffer[0x10] = onoff;
    TxBuffer[0x11] = pas;
    TxBuffer[0x12] = pas;
    TxBuffer[0x13] = 0xFF;
    TxBuffer[0x14] = 0xFE;
    TxBuffer[0x15] = time[0];
    TxBuffer[0x16] = time[1];
    TxBuffer[0x17] = time[2];
    TxBuffer[0x18] = time[3];
    TxBuffer[0x19] = time[4];
    TxBuffer[0x20] = time[5];

 //   WriteUSART(0x21);
    ggg = 0x00;
        while(ggg < 0x22)
    {
        TxBuffer[ggg] = 0;
        ggg++;
    }
}


void TxMessageConf(uint8_t adres, uint8_t pkd, uint8_t* time)
{
    uint8_t ggg = 0x00;
    uint8_t siseMsg = 0x00;

    switch (messegeTipe)
    {
        case NoCardLBD_1:
              siseMsg = 0x0B;
              TxBuffer[0x02] = 0x03;
              TxBuffer[0x03] = 0x92;
              TxBuffer[0x04] = 0x84;
              TxBuffer[0x05] = 0x01;
              TxBuffer[0x06] = time[0];
              TxBuffer[0x07] = time[1];
              TxBuffer[0x08] = time[2];
              TxBuffer[0x09] = time[3];
              TxBuffer[0x0A] = time[4];
              TxBuffer[0x0B] = time[5];
              break;
  
        case NaborError_1:
              siseMsg = 0x0B;
              TxBuffer[0x02] = 0x03;
              TxBuffer[0x03] = 0x92;
              TxBuffer[0x04] = 0xA4;
              TxBuffer[0x05] = 0xE4;
              TxBuffer[0x06] = time[0];
              TxBuffer[0x07] = time[1];
              TxBuffer[0x08] = time[2];
              TxBuffer[0x09] = time[3];
              TxBuffer[0x0A] = time[4];
              TxBuffer[0x0B] = time[5];
              break;
  
        case PussError_1:
              siseMsg = 0x0B;
              TxBuffer[0x02] = 0x03;
              TxBuffer[0x03] = 0x92;
              TxBuffer[0x04] = 0xA4;
              TxBuffer[0x05] = 0x00;
              TxBuffer[0x06] = time[0];
              TxBuffer[0x07] = time[1];
              TxBuffer[0x08] = time[2];
              TxBuffer[0x09] = time[3];
              TxBuffer[0x0A] = time[4];
              TxBuffer[0x0B] = time[5];
             break;
  
        case NotWorkWithZone_1:
              siseMsg = 0x0B;
              TxBuffer[0x02] = 0x03;
              TxBuffer[0x03] = 0x92;
              TxBuffer[0x04] = 0x84;
              TxBuffer[0x05] = 0x00;
              TxBuffer[0x06] = time[0];
              TxBuffer[0x07] = time[1];
              TxBuffer[0x08] = time[2];
              TxBuffer[0x09] = time[3];
              TxBuffer[0x0A] = time[4];
              TxBuffer[0x0B] = time[5];
             break;
  
        case NoCard1_LBD_2:
              siseMsg = 0x0B;
              TxBuffer[0x02] = 0x03;
              TxBuffer[0x03] = 0x92;
              TxBuffer[0x04] = 0xC4;
              TxBuffer[0x05] = 0x01;
              TxBuffer[0x06] = time[0];
              TxBuffer[0x07] = time[1];
              TxBuffer[0x08] = time[2];
              TxBuffer[0x09] = time[3];
              TxBuffer[0x0A] = time[4];
              TxBuffer[0x0B] = time[5];
              break;
  
        case NoCard2_LBD_2:
              siseMsg = 0x0B;
              TxBuffer[0x02] = 0x03;
              TxBuffer[0x03] = 0x92;
              TxBuffer[0x04] = 0xC4;
              TxBuffer[0x05] = 0x10;
              TxBuffer[0x06] = time[0];
              TxBuffer[0x07] = time[1];
              TxBuffer[0x08] = time[2];
              TxBuffer[0x09] = time[3];
              TxBuffer[0x0A] = time[4];
              TxBuffer[0x0B] = time[5];
              break;
  
        case NaborError_2:
              siseMsg = 0x0B;
              TxBuffer[0x02] = 0x03;
              TxBuffer[0x03] = 0x92;
              TxBuffer[0x04] = 0xC4;
              TxBuffer[0x05] = 0x01;
              TxBuffer[0x06] = time[0];
              TxBuffer[0x07] = time[1];
              TxBuffer[0x08] = time[2];
              TxBuffer[0x09] = time[3];
              TxBuffer[0x0A] = time[4];
              TxBuffer[0x0B] = time[5];
              break;
  
        case PussError_2:
              siseMsg = 0x0B;
              TxBuffer[0x02] = 0x03;
              TxBuffer[0x03] = 0x92;
              TxBuffer[0x04] = 0xE4;
              TxBuffer[0x05] = 0x00;
              TxBuffer[0x06] = time[0];
              TxBuffer[0x07] = time[1];
              TxBuffer[0x08] = time[2];
              TxBuffer[0x09] = time[3];
              TxBuffer[0x0A] = time[4];
              TxBuffer[0x0B] = time[5];
              break;
  
        case SameCard_2:
              siseMsg = 0x0B;
              TxBuffer[0x02] = 0x03;
              TxBuffer[0x03] = 0x92;
              TxBuffer[0x04] = 0xC4;
              TxBuffer[0x05] = 0x80;
              TxBuffer[0x06] = time[0];
              TxBuffer[0x07] = time[1];
              TxBuffer[0x08] = time[2];
              TxBuffer[0x09] = time[3];
              TxBuffer[0x0A] = time[4];
              TxBuffer[0x0B] = time[5];
             break;
  
        case TimeDelayOff_2:
              siseMsg = 0x0B;
              TxBuffer[0x02] = 0x03;
              TxBuffer[0x03] = 0x92;
              TxBuffer[0x04] = 0xC4;
              TxBuffer[0x05] = 0x80;
              TxBuffer[0x06] = time[0];
              TxBuffer[0x07] = time[1];
              TxBuffer[0x08] = time[2];
              TxBuffer[0x09] = time[3];
              TxBuffer[0x0A] = time[4];
              TxBuffer[0x0B] = time[5];
              break;

        case PKD_On:
              siseMsg = 0x0C;
              TxBuffer[0x02] = 0x03;
              TxBuffer[0x03] = 0x92;
              TxBuffer[0x04] = 0xFD;
              TxBuffer[0x05] = 0x01;
              TxBuffer[0x06] = 0x00;
              TxBuffer[0x07] = time[0];
              TxBuffer[0x08] = time[1];
              TxBuffer[0x09] = time[2];
              TxBuffer[0x0A] = time[3];
              TxBuffer[0x0B] = time[4];
              TxBuffer[0x0C] = time[5];
              break;
  
        case PKD_Off:
              siseMsg = 0x0C;
              TxBuffer[0x02] = 0x03;
              TxBuffer[0x03] = 0x92;
              TxBuffer[0x04] = 0x90;
              TxBuffer[0x05] = 0x01;
              TxBuffer[0x06] = 0x04;
              TxBuffer[0x07] = time[0];
              TxBuffer[0x08] = time[1];
              TxBuffer[0x09] = time[2];
              TxBuffer[0x0A] = time[3];
              TxBuffer[0x0B] = time[4];
              TxBuffer[0x0C] = time[5];
              break;
  
        case Ok:
              siseMsg = 0x0C;
              TxBuffer[0x02] = 0x90;
              TxBuffer[0x03] = 0x92;
              TxBuffer[0x04] = 0x01;
              TxBuffer[0x05] = 0x21;
              TxBuffer[0x05] = 0x4F;
              TxBuffer[0x05] = 0x4B;
              TxBuffer[0x06] = time[0];
              TxBuffer[0x07] = time[1];
              TxBuffer[0x08] = time[2];
              TxBuffer[0x09] = time[3];
              TxBuffer[0x0A] = time[4];
              TxBuffer[0x0B] = time[5];
              break;
  
    }

    TxBuffer[0x00] = adres;
    TxBuffer[0x01] = siseMsg;


//    WriteUSART(siseMsg + 1);
    ggg = 0x00;
        while(ggg < (siseMsg + 2))
    {
        TxBuffer[ggg] = 0;
        ggg++;
    }
}