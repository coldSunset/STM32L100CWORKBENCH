

#include "stm32l100xc.h"
//#include "lib_gpio.h"
#include "stdlib.h"
#include "string.h"
#include "stdarg.h"
#include "main.h"
int i;
uint32_t temp;

void SystemClock_Config(void);
static void uartprintf(char* msg, ...);

int main(void)
{
	SystemClock_Config();
	//* USART Debugging
	// ENABLE CLCK FOR USART 1
	RCC -> APB2ENR |= RCC_APB2ENR_USART1EN;//|RCC_APB2LPENR_SYSCFGLPEN;
	// SET BAUD RATE USARTDIV= fck/(16*BAUDRATE)
	// = 32 MHz/(16*9600) = 208.333 D05
	USART1 -> BRR |= 0xd05;
	// ENABLE USART, TRANSMIT, ENABLE RECIEVE INTERRUPT
	USART1 -> CR1 |= USART_CR1_TE|USART_CR1_RE|USART_CR1_UE|USART_CR1_RXNEIE;// 0x200c;
	NVIC_EnableIRQ(USART1_IRQn);
	// Configure GPIO PA9 TX PA10 RX
	RCC -> AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA -> MODER |= GPIO_MODER_MODER9_1|GPIO_MODER_MODER10_1;//GPIO_MODER_MODER9_1|GPIO_MODER_MODER10_1;
	GPIOA -> OSPEEDR |= GPIO_OSPEEDER_OSPEEDR9_Msk|GPIO_OSPEEDER_OSPEEDR10_Msk;//GPIO_OSPEEDER_OSPEEDR9_Msk|GPIO_OSPEEDER_OSPEEDR9_Msk;
	GPIOA -> AFR[1] |=  7 << GPIO_AFRH_AFSEL9_Pos;
	GPIOA -> AFR[1] |=  7 << GPIO_AFRH_AFSEL10_Pos;

	uartprintf("This is a test message #%i\r\n", 1);
	while (1)
  {



  }
  return 0;
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

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */


/* USER CODE BEGIN 4 */
static void uartprintf(char* msg, ...)
{
	char buff[80];

	va_list args;
	va_start(args, msg);
	vsprintf(buff, msg, args);

	for(int i =0; i<strlen(buff); i++)
	{

		USART1 -> DR = (uint8_t)buff[i];
		while(!(USART1->SR & USART_SR_TXE));
	}
}

void USART1_IRQHandler(void)
{
	//check if data has been recieved
	if(USART1->SR & USART_SR_RXNE)
	{
		char temp  = USART1->DR; // fetch data
		USART1->DR = temp;
		while(!(USART1->SR & USART_SR_TXE));
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
