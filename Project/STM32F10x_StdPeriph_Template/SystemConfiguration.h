
#ifndef Sys_Conf
#define Sys_Conf

/* Define to prevent recursive inclusion -------------------------------------*/
/*      ����������
 *      ���������� �����
 */
#define EXTI_0_BIT      GPIO_Pin_0      // PC0
#define EXTI_1_BIT      GPIO_Pin_1      // PC1
#define EXTI_2_BIT      GPIO_Pin_2      // PC2
#define LINE1_BIT       GPIO_Pin_3      // PC3
#define LINE2_BIT       GPIO_Pin_0      // PA0
#define LINE3_BIT       GPIO_Pin_1      // PA1
#define LINE4_BIT       GPIO_Pin_2      // PA2
#define LINE5_BIT       GPIO_Pin_7      // PA7
///////////////////////////END

/*      ����������
 *      ���������� ������
 */
#define LINE1_On()        (GPIOC -> ODR |= LINE1_BIT)   // �������� ������� 1-� �����  "1"  "2"  "del"
#define LINE2_On()        (GPIOA -> BSRR = LINE2_BIT)   // �������� ������� 2-� �����  "3"  "4"  "Off"
#define LINE3_On()        (GPIOA -> BSRR = LINE3_BIT)   // �������� ������� 3-� �����  "5"  "6"  "On "
#define LINE4_On()        (GPIOA -> BSRR = LINE4_BIT)   // �������� ������� 4-� �����  "7"  "8"  "Cl "
#define LINE5_On()        (GPIOA -> ODR |= LINE5_BIT)   // �������� ������� 5-� �����  "9"  "0"  "En "

#define LINE1_Off()       (GPIOC -> BRR  = LINE1_BIT)  // ��������� ������� 1-� �����  "1"  "2"  "del"
#define LINE2_Off()       (GPIOA -> BRR  = LINE2_BIT)  // ��������� ������� 2-� �����  "3"  "4"  "Off"
#define LINE3_Off()       (GPIOA -> BRR  = LINE3_BIT)  // ��������� ������� 3-� �����  "5"  "6"  "On "
#define LINE4_Off()       (GPIOA -> BRR  = LINE4_BIT)  // ��������� ������� 4-� �����  "7"  "8"  "Cl "
#define LINE5_Off()       (GPIOA -> BRR  = LINE5_BIT)  // ��������� ������� 5-� �����  "9"  "0"  "En "
//////////////////////////END

/*      ���������� LED1, LED2, LED3  ���� ������*/
 
#define LED_1              GPIO_Pin_4      // PA4
#define LED_2              GPIO_Pin_5      // PA5
#define LED_3              GPIO_Pin_6      // PA6

#define LED1_On()        /* (GPIOA -> BSRR = LED_1) */  // �������� ��������� 1
#define LED2_On()        /* (GPIOA -> BSRR = LED_2) */  // �������� ��������� 2
#define LED3_On()        /* (GPIOA -> BSRR = LED_3) */  // �������� ��������� 3

#define LED1_Off()       /* (GPIOA -> BRR  = LED_1) */ // ��������� ��������� 1
#define LED2_Off()       /* (GPIOA -> BRR  = LED_2) */ // ��������� ��������� 2
#define LED3_Off()       /* (GPIOA -> BRR  = LED_3) */ // ��������� ��������� 3
//////////////////////////END


/* C���������� */
/*      ���������� RED, GREEN
 */
#define RED                GPIO_Pin_10      // PA10
#define GREEN              GPIO_Pin_8       // P�8

#define RED_On()          (GPIOA -> BSRR = RED  )   // �������� ��������� 1
#define GREEN_On()        (GPIOA -> BSRR = GREEN)   // �������� ��������� 2

#define RED_Off()         (GPIOA -> BRR  = RED  )  // ��������� ��������� 1
#define GREEN_Off()       (GPIOA -> BRR  = GREEN)  // ��������� ��������� 2
//////////////////////////END

/*      ������ D0 D1
 */
#define EXTI_3_BIT      GPIO_Pin_3      // PA11
#define EXTI_4_BIT      GPIO_Pin_4      // P�9
//////////////////////////END


#define Port(port, pin, speed, mode)                    \
	GPIO_StructInit( & GPIO_InitStructure);         \
	GPIO_InitStructure.GPIO_Pin = pin;              \
	GPIO_InitStructure.GPIO_Speed = speed;          \
	GPIO_InitStructure.GPIO_Mode = mode;            \
	GPIO_Init(port, & GPIO_InitStructure);



/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
extern uint32_t snCard[25];
extern uint8_t sn;

/* Exported macro ------------------------------------------------------------*/

void WriteTxt(uint8_t*);


/* Exported functions ------------------------------------------------------- */
void WaitBusyFlag(void);
void DisplayClear(void);

void EXTIInit(uint32_t, FunctionalState, EXTIMode_TypeDef, EXTITrigger_TypeDef, int, uint8_t, uint8_t);
void InitLED(void);

extern void InitSystemConfiguration(void);
extern void InitPort(GPIO_TypeDef *, uint16_t, GPIOSpeed_TypeDef, GPIOMode_TypeDef);
void EnablePortsAndDevices(uint32_t, uint32_t, uint32_t);
void EnableRemap(uint32_t);

extern void Timer_Configuration();
extern void RCC_Configuration(void);
extern void GPIO_Configuration(void);
extern void NVIC_Configuration(void);
void print(const char* );

#endif /* Sys_Conf */

