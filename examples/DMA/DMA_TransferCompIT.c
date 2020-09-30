

#include "stm32l100xc.h"
//#include "lib_gpio.h"
#include "main.h"
#include "string.h"

#define timedelay 100

int i;
uint32_t temp;

char buff[2000]; //empty buffer

void SystemClock_Config(void);
void uartprintf(char* msg, ...);
void toggle_twice(void);
void usart_send(char* msg);

int main(void)
{
	SystemClock_Config();

	/* GPIO CONFIG TEST
	RCC -> AHBENR |= 1 << 2;

	// Test GPIO Config
	GPIO_Config GPIOConfig;
	GPIOConfig.Pin = GPIO_PIN_8;
	GPIOConfig.Mode = OUTPUT_MODE;
	GPIOConfig.OutType = OUT_PP;
	GPIOConfig.Speed = MEDIUM_SPEED;
	GPIOConfig.Pull = NO_PUPD;
	ConfigGpio(GPIOC, &GPIOConfig);

	//Test GPIO Toggle

	ToggleGpio(GPIOC, GPIO_PIN_8);
	while(i<100000){i++;}
	ToggleGpio(GPIOC, GPIO_PIN_8);

	// Test GPIO Write
	GpioWrite(GPIOC, GPIO_PIN_8, SET);
	//while(i<1000){i++;}
	GpioWrite(GPIOC, GPIO_PIN_8, RESET);
	*/

	/* INTERUPT CONFIGURATION
	// configure Interrupts on PA0 and PB0 (same EXTI line)
		RCC -> AHBENR |= 1<<2;
	// Blue led and green led
		GPIOC -> MODER |= (1<<8*2) | (1<<9*2);
	// configure interrupt mask on EXTI line 0
		EXTI -> IMR |= 1 << 0;
	// rising edge enabled on line EXTI line 0
		EXTI -> RTSR |= 1 << 0;
	// SYS CONFIG EXTI Register
		SYSCFG -> EXTICR[0] |= 0x0 << 0;
		NVIC_EnableIRQ(EXTI0_IRQn);
	*/

	//* USART Debugging
	// ENABLE CLCK FOR USART 1
	RCC -> APB2ENR |= RCC_APB2ENR_USART1EN;//|RCC_APB2LPENR_SYSCFGLPEN;
	// SET BAUD RATE USARTDIV= fck/(16*BAUDRATE)
	// = 32 MHz/(16*9600) = 208.333 D05
	USART1 -> BRR |= 0xd05;
	// ENABLE USART, TRANSMIT, ENABLE RECIEVE INTERRUPT
	USART1 -> CR1 |= USART_CR1_TE|USART_CR1_RE|USART_CR1_UE|USART_CR1_RXNEIE;
	NVIC_EnableIRQ(USART1_IRQn);
	// Configure GPIO PA9 TX PA10 RX
	RCC -> AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA -> MODER |= GPIO_MODER_MODER9_1|GPIO_MODER_MODER10_1;;
	GPIOA -> OSPEEDR |= GPIO_OSPEEDER_OSPEEDR9_Msk|GPIO_OSPEEDER_OSPEEDR10_Msk;;
	GPIOA -> AFR[1] |=  7 << GPIO_AFRH_AFSEL9_Pos;
	GPIOA -> AFR[1] |=  7 << GPIO_AFRH_AFSEL10_Pos;
	// enable DMA1 on Channel 4
	// enable DMA for transmission on UART1
	RCC -> AHBENR |= RCC_AHBENR_DMA1EN_Msk;
	USART1 -> CR3 |= USART_CR3_DMAT;
	// Configuration register settings,
	DMA1_Channel4 -> CCR |= DMA_CCR_MINC&(~DMA_CCR_CIRC)|DMA_CCR_DIR;
	// ENABLE TRANSFER COMPLETE IT
	DMA1_Channel4 -> CCR |= DMA_CCR_TCIE;
	DMA1_Channel4 -> CNDTR |= 2000;
	DMA1_Channel4 -> CPAR |= (uint32_t)&(USART1->DR);
	DMA1_Channel4 -> CMAR |= (uint32_t)(buff);
	NVIC_EnableIRQ(DMA1_Channel4_IRQn);
	//ENABLE PC8 and PC9 (blue and green leds)
	RCC -> AHBENR|= RCC_AHBENR_GPIOCEN;
	GPIOC -> MODER |= GPIO_MODER_MODER8_0|GPIO_MODER_MODER9_0;
	GPIOC->BSRR |= GPIO_BSRR_BR_9; //Start PC9 in reset mode
	uartprintf("This is a test message #%i\r\n", 1);

	// fill buffer
	for(int i=0; i<2000; i++)
	{
		buff[i] = 'X';
	}
	DMA1_Channel4 -> CCR |=DMA_CCR_EN;
	while (1)
  {
		toggle_twice();

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


void USART1_IRQHandler(void)
{
	//check if data has been recieved
	if(USART1->SR & USART_SR_RXNE)
	{
		char temp  = USART1->DR; // fetch data
		USART1->DR = temp;
		while(!(USART1->SR & USART_SR_TC));
	}
}

void DMA1_Channel4_IRQHandler(void)
{
	//check which ISR has been asserted
	if(DMA1 -> ISR & DMA_ISR_TCIF4)
	{
		DMA1-> IFCR |= DMA_IFCR_CTCIF4;
		GPIOC->BSRR |= GPIO_BSRR_BS_9; // set green LED
	}
}

void usart_send(char* msg)
{
	for(int i=0; i<strlen(msg); i++)
	{
		USART1->DR = msg[i]; //transmit data
		while(!(USART1->SR & USART_SR_TXE));// wait until shift
											//register clears
	}
}

void toggle_twice(void)
{
	GPIOC-> BSRR |=  GPIO_BSRR_BS_8;
	HAL_Delay(timedelay);
	GPIOC->BSRR |= GPIO_BSRR_BR_8;
	HAL_Delay(timedelay);
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
