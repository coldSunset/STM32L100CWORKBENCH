
#include "stm32l1xx.h"
//#include "stm32l1xx_hal_rcc.h"



int main()
{
	volatile uint32_t *GPIOA_MODER = 0x0;
	volatile uint32_t *GPIOA_ODR = 0x0;

	GPIOA_MODER = (uint32_t*)0x40020000 ; // address of GPIOA -> MODER register
	GPIOA_ODR = (uint32_t*)(0x40020000 + 0x14); // address of GPIOA -> ODR register

	// this ensure that the peripheral is enabled and connected to the AHB1 bus
	RCC -> AHBENR |= 1 ;

	*GPIOA_MODER = *GPIOA_MODER | 0x1<<2; // I/O direction mode for PA1
	*GPIOA_ODR = *GPIOA_ODR | 1<<1; // SET PA1 to high

	while(1);




	return 0;
}
