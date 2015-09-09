
#ifndef Baza
#define Baza
/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include "sdcard.h"
#include "SystemConfiguration.h"

/* Define to prevent recursive inclusion -------------------------------------*/

#define basaSise        1024
#define basaCARDS       30
#define basaELEM        20
#define MAXzon          128
#define MAXzonArhiv     16
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define HiLo16(x,y)      x * 0x100 + y
#define HiLo32(x,y)      x * 0x10000 + y

extern struct card
{
      uint8_t Family;
      uint8_t FamilyHi;
      uint8_t FamilyLo;
      uint8_t NumCard;
      uint8_t NumCardHi;
      uint8_t NumCardLo;
} AccessCard;

/*      Пароль
 */ 
extern struct cardPass
{
      uint8_t Pas;
      uint8_t PasHi;
      uint8_t PasLo;
} PasswordCard;

/* Exported functions ------------------------------------------------------- */
extern uint8_t exti;



#endif /* Sys_Conf */
