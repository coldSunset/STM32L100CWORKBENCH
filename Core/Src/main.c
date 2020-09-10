
#include "stm32l1xx_hal.h"
#include "gpio.h"
#include "stm32l100xc.h"

	 //interrupt handler
	 void EXTI0_IRQHandler(void)
	 {
		 __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
		 HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8);
	 }

int main()
{
	 GPIO_InitTypeDef GPIO_InitStruct;

	 HAL_Init();
	 // Important don't forget clock enable!
	 __HAL_RCC_GPIOC_CLK_ENABLE();
	 __HAL_RCC_GPIOA_CLK_ENABLE();

	 //define Blue Led
	 GPIO_InitStruct.Pin = GPIO_PIN_8;
	 GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	 GPIO_InitStruct.Pull = GPIO_NOPULL;
	 GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	 HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	 // define USER BUTTON (B1) = PA0
	 GPIO_InitStruct.Pin = GPIO_PIN_0;
	 GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	 GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	 GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	 HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	 // enable interrupt on EXTI0 line
	 HAL_NVIC_EnableIRQ(EXTI0_IRQn);



	 while(1)
	 {

	 }


	return 0;
}



