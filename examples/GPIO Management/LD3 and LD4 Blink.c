
#include "stm32l1xx_hal.h"


void toggleOnOff(int on);
void configGPIO(void);

//intialise value of GPIO config registers
volatile uint32_t* GPIOC_MODER = 0x00;
volatile uint32_t* GPIOC_ODR   = 0x00;

int main()
{

	 HAL_Init();
	 configGPIO(); // PIN declaration and setting
	 int state = 0;
		while(1)
		{
			state = (!state);
			toggleOnOff(state);
			HAL_Delay(200);

		}

	return 0;
}


void configGPIO(void)
{

	//write in address of GPIO configure registers
	GPIOC_MODER = (uint32_t*)0x40020800;
	GPIOC_ODR   = (uint32_t*)(0x40020800 + 0x14);
	//enable AHB clock for Port C access
	RCC -> AHBENR |= 1 << 2 ;
	*GPIOC_MODER |= 0x05 << 16 ; // set I/O direction for PC8 and PC9


}

void toggleOnOff(int on)
{
	if(on == 1)
	{
		*GPIOC_ODR   |= 0x03 << 8  ;  // set PC8 and PC9 to high
	}
	else if(on != 1)
	{
		*GPIOC_ODR   &= 0x00 << 8  ; // set PC8 and PC9 to low
	}
}
