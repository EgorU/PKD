
/*---------------------Клавиатура--------------------------------
 *      \       EXTI    BIT      |       LED1    LED2    LED3   |
 *                               |                              |
 *      1       0       PС3      |       0       0       1      |
 *      2       1       PС3      |       0       0       1      |
 *      del     2       PС3      |       0       0       0      |
 *      3       0       PA0      |       0       1       0      |
 *      4       1       PA0      |       0       1       0      |
 *      Off     2       PA0      |       1       1       0      |
 *      5       0       PA1      |       0       1       1      |
 *      6       1       PA1      |       0       1       1      |
 *      On      2       PA1      |       1       1       0      |
 *      7       0       PA2      |       1       0       0      |
 *      8       1       PA2      |       1       0       0      |
 *      Cl      2       PA2      |       0       0       0      |
 *      9       0       PA3      |       1       0       1      |
 *      0       1       PA3      |       1       0       1      |
 *      En      2       PA3      |       1       1       1      |
 *---------------------------------------------------------------
 *      EXTI_1  PC0     |
 *      EXTI_2  PC1     |
 *      EXTI_3  PC2     |
 *-----------------------
 */

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "led.h"
#include "SystemConfiguration.h"
#include "sdcard.h"
#include "baza.h"
#include "main.h"
#include "stm32f10x_it.h"

/* Private typedef -----------------------------------------------------------*/
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
GPIO_InitTypeDef GPIO_InitStructure;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t PrescalerValue = 0;

/* Private function prototypes -----------------------------------------------*/
void EXTIInit(uint32_t, FunctionalState, EXTIMode_TypeDef, EXTITrigger_TypeDef, int, uint8_t, uint8_t);
void InitLED(void);

extern void InitSystemConfiguration(void);
extern void InitPort(GPIO_TypeDef *, uint16_t, GPIOSpeed_TypeDef, GPIOMode_TypeDef);
void EnablePortsAndDevices(uint32_t, uint32_t, uint32_t);
void EnableRemap(uint32_t);
void SystickT(void);

inline extern void Timer_Configuration();
inline extern void Keyboard_Configuration();
inline extern void USART_Configuration();
inline extern void SD_Configuration();
inline extern void CardReder_Configuration();
inline extern void RCC_Configuration(void);
inline extern void GPIO_Configuration(void);
inline extern void NVIC_Configuration(void);
void SystickT(void);

 void InitSystemConfiguration(void) // инициализация ног портов
{

 	SystickT();

        /* System Clocks Configuration */
	EnablePortsAndDevices(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |
                              RCC_APB2Periph_AFIO /*| USARTy_GPIO_CLK */| RCC_APB2Periph_USART1,
                              RCC_APB1Periph_TIM2, 0);


        /* Enable GPIO clock */
	InitPort(GPIOC, GPIO_Pin_5 | GPIO_Pin_4, GPIO_Speed_10MHz, GPIO_Mode_Out_PP);
	InitPort(GPIOB, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11
			| GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15, GPIO_Speed_10MHz, GPIO_Mode_Out_PP);

    
       /* Displey Configuration */
	InitLED();

        /* Timer Configuration */
        Timer_Configuration();     

        /* Настройка клавиатуры */
        Keyboard_Configuration();    
        
        /* Настройка работы с картой доступа */
        CardReder_Configuration();
        
        /* Enable USARTy Clock */
        RCC_APB2PeriphClockCmd(USARTy_CLK, ENABLE); 

        /* Configure USART */
        USART_Configuration();

        /* NVIC Configuration */
        NVIC_Configuration();
        
        /* CD Configuration */
        SD_Configuration();
}
/**
  * @brief  настройка таймера 2 на период 0,01 секунда
  */
inline void Timer_Configuration()
{
    /* NVIC Configuration */
    NVIC_Configuration();

    PrescalerValue = (uint16_t) (SystemCoreClock / 12000000) - 1;

     /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 50001;
    TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
 
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    /* Prescaler configuration */
    TIM_PrescalerConfig(TIM2, PrescalerValue, TIM_PSCReloadMode_Immediate);

    /* Output Compare Timing Mode configuration: Channel1 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;//TIM_OCMode_Timing;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 1;// CCR1_Val;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM_OC1Init(TIM2, &TIM_OCInitStructure);

    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);

    // TIM IT enable 
    TIM_ITConfig(TIM2, TIM_IT_CC1/* | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4*/, ENABLE);


/*
 SysTick->CTRL=	SysTick_CTRL_CLKSOURCE_Msk |
                SysTick_CTRL_TICKINT_Msk   |
                SysTick_CTRL_ENABLE_Msk;*/
 //SysTick->LOAD = (SystemCoreClock/1000)-1;
 
 	if(SysTick_Config(SystemCoreClock / 100000)) // Failure check/ 160ticks ~10us
	{
	
	}
        
/* TIM2 enable counter */
    TIM_Cmd(TIM2, ENABLE);
}

/**
  * @brief  Настройка обмена по USART
  */
inline void USART_Configuration()
{
    USART_InitTypeDef USART_InitStructure;

    /* Enable the USART2 Pins Software Remapping */
    GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);  

    /* Configure USARTy Rx as input floating */
    Port(USARTy_GPIO, USARTy_RxPin, GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING);

    /* Configure USARTy Tx as alternate function push-pull */
    Port(USARTy_GPIO, USARTy_TxPin, GPIO_Speed_50MHz, GPIO_Mode_AF_PP);

    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    /* Configure USART*/
    USART_Init(USARTy, &USART_InitStructure);

    /* Enable USART Receive and Transmit interrupts */
    USART_ITConfig(USARTy, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USARTy, USART_IT_TXE, ENABLE);

    /* Enable the USARTy */
    USART_Cmd(USARTy, ENABLE);
}


/**
  * @brief  настройка CD Карты
  */
inline void SD_Configuration()
{
SD_CardInfo SDCardInfo;
 
    // Иницилизация карты
    SD_Init();
    
    // Получаем информацию о карте
    SD_GetCardInfo(&SDCardInfo);
    
    // Выбор карты и настройка режима работы
    SD_SelectDeselect((uint32_t) (SDCardInfo.RCA << 16));
    SD_SetDeviceMode(SD_POLLING_MODE);
 }

/**
  * @brief  Настройка клавиатуры
  */
void Keyboard_Configuration()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	InitPort(GPIOC, EXTI_0_BIT | EXTI_1_BIT | EXTI_2_BIT,                       GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING);

        EXTIInit(EXTI_Line0, ENABLE, EXTI_Mode_Interrupt, EXTI_Trigger_Rising,  EXTI0_IRQn, GPIO_PortSourceGPIOC, GPIO_PinSource0);
	EXTIInit(EXTI_Line1, ENABLE, EXTI_Mode_Interrupt, EXTI_Trigger_Rising,  EXTI1_IRQn, GPIO_PortSourceGPIOC, GPIO_PinSource1);
	EXTIInit(EXTI_Line2, ENABLE, EXTI_Mode_Interrupt, EXTI_Trigger_Rising,  EXTI2_IRQn, GPIO_PortSourceGPIOC, GPIO_PinSource2);

	Port(GPIOA, LINE2_BIT | LINE3_BIT | LINE4_BIT | LINE5_BIT,                  GPIO_Speed_50MHz, GPIO_Mode_Out_PP);
	Port(GPIOC, LINE1_BIT,                                                      GPIO_Speed_50MHz, GPIO_Mode_Out_PP);
  
}


/**
  * @brief  Настройка считывателя
  */
void CardReder_Configuration()
{
/*
Данные        D1      PA9
              D0      PA11
Светодиод     Red     PA10
              Green   PA8
*/
      /*GREEN, RED */
//	Port(GPIOA, RED | GREEN,        GPIO_Speed_50MHz, GPIO_Mode_Out_PP);

        /*EXTI_Line3, EXTI_Line4 */
	InitPort(GPIOA, EXTI_3_BIT | EXTI_4_BIT,                                    GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING);

        EXTIInit(EXTI_Line3, ENABLE, EXTI_Mode_Interrupt, EXTI_Trigger_Falling,  EXTI3_IRQn, GPIO_PortSourceGPIOA, GPIO_PinSource3);
	EXTIInit(EXTI_Line4, ENABLE, EXTI_Mode_Interrupt, EXTI_Trigger_Falling,  EXTI4_IRQn, GPIO_PortSourceGPIOA, GPIO_PinSource4);
}


/**
  * @brief  Configure the nested vectored interrupt controller.
  */
inline void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

   /* Configure the NVIC Preemption Priority Bits */  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
 
  /* Enable the USARTy Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USARTy_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);


  /* Enable the TIM2 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}

inline  void EnablePortsAndDevices(uint32_t ports, uint32_t apb1, uint32_t apb2)
{ // Активация системных шин
	RCC_APB2PeriphClockCmd(ports | apb2, ENABLE);
	RCC_APB1PeriphClockCmd(apb1, ENABLE);
}

inline  void EnableRemap(uint32_t remap)
{ // Активация Ремапа
	GPIO_PinRemapConfig(remap, ENABLE);
}

void InitPort(GPIO_TypeDef * port, uint16_t pin, GPIOSpeed_TypeDef speed, GPIOMode_TypeDef mode)
{ // Инициализация порта
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit( & GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = pin;
	GPIO_InitStructure.GPIO_Speed = speed;
	GPIO_InitStructure.GPIO_Mode = mode;
	GPIO_Init(port, & GPIO_InitStructure);
}

inline  void EXTIInit(uint32_t line, FunctionalState linecmd, EXTIMode_TypeDef mode, EXTITrigger_TypeDef trigger, int IRQn, uint8_t port, uint8_t pin)
{ // инициализация внешних прерываний
	EXTI_InitTypeDef   EXTI_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	GPIO_EXTILineConfig(port, pin);
	EXTI_InitStructure.EXTI_Line = line;
	EXTI_InitStructure.EXTI_Mode = mode;
	EXTI_InitStructure.EXTI_Trigger = trigger;
	EXTI_InitStructure.EXTI_LineCmd = linecmd;
	EXTI_Init( & EXTI_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( & NVIC_InitStructure);
}

void SystickT(void) // Инициализация функции для задержки
{
	if(SysTick_Config(SystemCoreClock / 100000)) // Failure check/ 160ticks ~10us
	{
		while(1)
		{
		}
	}
}

