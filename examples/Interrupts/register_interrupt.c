

#include "stm32l100xc.h"
#include "lib_gpio.h"


int i;
uint32_t temp;
void EXTI0_IRQHandler(void)
{
	EXTI -> PR |= 1<<0;
	//GPIOC ->ODR |= 1 << 8;
	GPIOC -> ODR ^= 1<<8;
}

int main(void)
{

	// Configuring interrupts
	// configure PA0 as a rising interrupt -> turn on LED
		RCC -> AHBENR |= 1<<2;
		GPIOC -> MODER |= 1<<8*2;
	// configure interrupt mask
		EXTI -> IMR |= 1 << 0;
	// rising edge enabled
		EXTI -> RTSR |= 1 << 0;
	// SYS CONFIG EXTI Register
		SYSCFG -> EXTICR[0] |= 0x0 << 0;
		NVIC_EnableIRQ(EXTI0_IRQn);




	while (1)
  {



  }
  return 0;
}

/**
  * @brief System Clock Configuration
  * @retval None
  */


/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */


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
