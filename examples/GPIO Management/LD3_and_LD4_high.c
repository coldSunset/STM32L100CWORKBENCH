
#include "stm32l1xx_hal.h"

void SystemClock_Config(void);
//static MX_GPIO_Init(void);

int main()
{
	//SystemClock_Config();
	//MX_GPIO_Init();

	//intialise value of GPIO config registers
	volatile uint32_t* GPIOC_MODER = 0x00;
	volatile uint32_t* GPIOC_ODR   = 0x00;

	//write in address of GPIO config registers
	GPIOC_MODER = (uint32_t*)0x40020800;
	GPIOC_ODR   = (uint32_t*)(0x40020800 + 0x14);

	//enable AHB clock for Port C access
	RCC -> AHBENR |= 1 << 2 ;


	*GPIOC_MODER |= 0x05 << 16 ; // set I/O dir for PC8 and PC9
	*GPIOC_ODR   |= 0x03 << 8  ; // set PC8 and PC9 to high




	return 0;
}
