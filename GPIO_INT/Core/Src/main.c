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


void SystemClock_Config(void);
static void MX_GPIO_Init(void);


int main(void)
{


  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();





    __disable_irq();    //   Disable Global Interrupt


    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;   // Enable clock access for PA
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;   // Enable clock access for PC
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;  // Enable clock access for SYSCFG

    GPIOA->MODER |= 0x0400;                // Set PA5 as ouput pin
    SYSCFG->EXTICR[3] = 0x0020;            // Select PC for EXTI13
    	  	  	  	  	  	  	  	  	   // SYSCFG4 but in raison for starting at 0 we take [3]

    EXTI->IMR  |= EXTI_IMR_MR13;  // Unmask EXTI13                 #0x2000
    EXTI->FTSR |= EXTI_FTSR_TR13; // Select Falling Edge Trigger   #0x2000

    NVIC_EnableIRQ(EXTI15_10_IRQn);

    __enable_irq();


  while (1) {}

}


void delay_ticks(unsigned ticks)
{
    SysTick->LOAD = ticks;
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;

    // COUNTFLAG is a bit that is set to 1 when counter reaches 0.
    // It's automatically cleared when read.
    while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0);
    SysTick->CTRL = 0;
}


static inline void delay_ms(unsigned ms)
{
    delay_ticks((ms * (84000000 / 8)) / 1000);
}


void EXTI15_10_IRQHandler(void)
{
	/*
	GPIOA->BSRR = 0x20;//GPIO_BSRR_BS5;  // Set the ODR bit #0x20
	delay_ms(200);
	GPIOA->BSRR = 0x00200000;//GPIO_BSRR_BR5; // Reset the ODR bit #0x00200000
	delay_ms(200);
	GPIOA->BSRR = GPIO_BSRR_BS5;   // Set the ODR bit #0x20
	delay_ms(200);
	GPIOA->BSRR = GPIO_BSRR_BR5;   // Reset the ODR bit #0x00200000
	delay_ms(200);
	EXTI->PR = EXTI_PR_PR13;       //Reset the Pending bit #0x2000;
	*/

	GPIOA->ODR ^= (1<<5);
	delay_ms(20);
	EXTI->PR = EXTI_PR_PR13;
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

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
