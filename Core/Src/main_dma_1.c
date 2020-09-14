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
#include "dma.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
DMA_HandleTypeDef hdma_uart1_tx;
DMA_Channel_TypeDef dma_channel_4;
DMA_InitTypeDef dma1;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
extern UART_HandleTypeDef huart1;

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
char *msg = "Hello STM32 Lovers! This message is transferred in DMA Mode.\r\n";

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

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

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  //  __IO uint32_t *DMA_CCR4 =0x0;
  //  __IO uint32_t *DMA_CNDTR4 =0x0;
  //  __IO uint32_t *DMA_CPAR4 =0x0;
  //  __IO uint32_t *DMA_CMAR4 =0x0;
  //  DMA_CCR4 = (uint32_t*)(0x400263FF + 0x044);
  //  DMA_CNDTR4 = (uint32_t*)(0x400263FF + 0x048);
  //  DMA_CPAR4 = (uint32_t*)(0x400263FF + 0x04C);
  //  DMA_CMAR4 = (uint32_t*)(0x400263FF + 0x050);
  //  dma_channel_4.CCR = DMA_CCR4;
  //  dma_channel_4.CNDTR = DMA_CNDTR4;
  //  dma_channel_4.CPAR = DMA_CPAR4;
  //  dma_channel_4.CMAR = DMA_CMAR4;
  //
  //  dma1.Direction = DMA_MEMORY_TO_PERIPH;
  //  dma1.PeriphInc = DMA_PINC_DISABLE;
  //  dma1.MemInc = DMA_MINC_ENABLE;
  //  dma1.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  //  dma1.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  //  dma1.Mode = DMA_NORMAL;
  //  dma1.Priority = DMA_PRIORITY_LOW;
  //
  //  hdma_uart1_tx.Instance = &dma_channel_4;
  //  hdma_uart1_tx.Init = dma1;
  //  hdma_uart1_tx.Lock =
  //  HAL_DMA_Init(&hdma_usart2_tx);
    //-------------------
    //__IO uint32_t *uart1 =0x0;
   // uart1 = (uint32_t)(0x4001 3800);
    //dma type def init

    hdma_uart1_tx.Instance = DMA1_Channel4;
    hdma_uart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_uart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_uart1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_uart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_uart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_uart1_tx.Init.Mode = DMA_NORMAL;
    hdma_uart1_tx.Init.Priority = DMA_PRIORITY_LOW;

    HAL_DMA_Init(&hdma_uart1_tx);

    __HAL_LINKDMA(&huart1, hdmatx, hdma_uart1_tx);
    HAL_DMA_Start(&hdma_uart1_tx, (uint32_t)msg, (uint32_t)&huart1.Instance->DR, strlen(msg));

    //Enable UART in DMA mode
    huart1.Instance->CR3 |= USART_CR3_DMAT;
    //Wait for transfer complete
    HAL_DMA_PollForTransfer(&hdma_uart1_tx, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY);
    //Disable UART DMA mode
    huart1.Instance->CR3 &= ~USART_CR3_DMAT;
    //Turn LD2 ON
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */

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
