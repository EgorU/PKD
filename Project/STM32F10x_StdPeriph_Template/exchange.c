/**
  ******************************************************************************
  * @file    SDIO/sdcard.c 
  * @author  MCD Application Team
  * @version V3.1.2
  * @date    09/28/2009
  * @brief   This file provides all the SD Card driver firmware functions.
  ******************************************************************************
  * @copy
  * Предназначен для 
  *     -       Передачи/Приёма сообщений в ЦУ
  */ 

/* Includes ------------------------------------------------------------------*/
#include "exchange.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "led.h"
#include "SystemConfiguration.h"
#include "sdcard.h"
#include "baza.h"
#include "stm32f10x_it.h"
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
