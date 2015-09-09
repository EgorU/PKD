/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H
#define __USART_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

//////////////Типы команд 
#define NoCardLBD_1             1
#define NaborError_1            2
#define PussError_1             3
#define NotWorkWithZone_1       4
#define NoCard1_LBD_2           5
#define NoCard2_LBD_2           6
#define NaborError_2            7
#define PussError_2             8
#define SameCard_2              9
#define TimeDelayOff_2          0
#define PKD_On                  11
#define PKD_Off                 12
#define Ok                      13

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#define USARTbufSise 255
extern uint8_t TxBuffer[USARTbufSise];
extern uint8_t RxBuffer[USARTbufSise];


void ReadUSART();
void WriteUSART(uint8_t);
void firstSendUSART();
void ReadWrite();

#endif /* USART_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/