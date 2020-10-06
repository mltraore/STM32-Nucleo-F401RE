/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include "stm32f4xx.h"
#include <inttypes.h>

int main(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODER_MODER5_0;

	/*for(;;)
	{
		GPIOA->ODR |= 1<<5;
		GPIOA->ODR |= ~(1<<5);
	}*/

	//uint32_t value = GPIOA->IDR;
	//printf("****************%"PRIu32"*************************************\n",value);

	for(;;)
	{
		GPIOA->BSRRL |= 1<<5;
		GPIOA->BSRRL &= ~(1<<5);
	}
}
