/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "ringbuffer.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define WELCOME_MSG "Welcome to the STMDISCO management console\r\n"
#define MAIN_MENU   "Select the option you are interested in:\r\n\t1. Toggle LD3 and LD4 LED\r\n\t2. Read USER BUTTON status\r\n\t3. Clear screen and print this message "
#define PROMPT "\r\n> "
#define TEST "Hello"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
__IO ITStatus UartReady = SET;
char readBuf[1];
RingBuffer txBuf;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void SystemClock_Config(void);
void printWelcomeMessage(void);
uint8_t processUserInput(uint8_t opt);
uint8_t readUserInput(void);
void printTestMessage(void);
uint8_t UART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t len);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	uint8_t opt = 0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  RingBuffer_Init(&txBuf);
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  printMessage:
  	  printWelcomeMessage();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	   //printTestMessage();
    /* USER CODE END WHILE */

	    opt = readUserInput();
	    if(opt > 0)
	    {
	      processUserInput(opt);
	      if(opt == 3)
	      {
	        goto printMessage;
	    }
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLL_DIV3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void printWelcomeMessage(void)
{
	UART_Transmit(&huart1, (uint8_t*)"\033[0;0H", strlen("\033[0;0H"));
	UART_Transmit(&huart1, (uint8_t*)"\033[2J", strlen("\033[2J"));
	UART_Transmit(&huart1, (uint8_t*)WELCOME_MSG, strlen(WELCOME_MSG));
	UART_Transmit(&huart1, (uint8_t*)MAIN_MENU, strlen(MAIN_MENU));
	UART_Transmit(&huart1, (uint8_t*)PROMPT, strlen(PROMPT));
}

void printTestMessage(void)
{
	UART_Transmit(&huart1, (uint8_t*)"\033[0;0H", strlen("\033[0;0H"));
	UART_Transmit(&huart1, (uint8_t*)"\033[2J", strlen("\033[2J"));
	UART_Transmit(&huart1, (uint8_t*)TEST, strlen(TEST));
}

uint8_t UART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t len)
{
	if(HAL_UART_Transmit_IT(huart, pData, len) != HAL_OK)
	{
		if(RingBuffer_Write(&txBuf, pData, len) != RING_BUFFER_OK)
			return 0;
	}
	return 1;
}

uint8_t readUserInput(void) {
	int8_t retVal = -1;

	if(UartReady == SET) {
	UartReady = RESET;
	HAL_UART_Receive_IT(&huart1, (uint8_t*)readBuf, 1);
	retVal = atoi(readBuf);
	}
	return retVal;
}


uint8_t processUserInput(uint8_t opt) {
	char msg[30];

	if(!opt || opt > 3)
		return 0;

	sprintf(msg, "%d", opt);
	UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg));

	switch(opt) {
	case 1:
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8);
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_9);
		UART_Transmit(&huart1, (uint8_t*)"\r\n", strlen("\r\n"));
		sprintf(msg, "LEDs ARE %s\r\n", HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9) == GPIO_PIN_RESET ? "OFF" : "ON");
		UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg));
		break;
	case 2:
		sprintf(msg, "\r\nUSER BUTTON status: %s\r\n", HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET ? "RELEASED" : "PRESSED");
		UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg));
		break;
	case 3:
		return 2;
	};

	return 1;
}
 void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart1)
 {
 /* Set transmission flag: transfer complete*/
 UartReady = SET;
 }

 void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart1)
 {
	 uint8_t txData = txBuf.tail;
	 if(RingBuffer_GetDataLength(&txBuf) > 0)
	 	{
		 RingBuffer_Read(&txBuf, &txData, 1);
		 HAL_UART_Transmit_IT(huart1, &txData, 1);
	 	}
 }

 void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
 {
	 if(huart->ErrorCode == HAL_UART_ERROR_ORE)
		 {
		 	 HAL_UART_Receive_IT(&huart1, readBuf, 1);
		 }
 }
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
