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
#include <stdio.h>
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void USART2_Init(void);
int USART2_Write(int ch);
int data;
double voltage;
double celsius;

int main(void)
{

  HAL_Init();
  SystemClock_Config();
  //MX_GPIO_Init();


  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

  TIM2->PSC   = 1;
  TIM2->ARR   = 42000000 - 1;
  //TIM2->CCR1  = 0;
  TIM2->CCMR1 = 0x6800;
  TIM2->CCR2  = 50 -1 ;        // Preload value
  TIM2->CNT   = 0;
  TIM2->CR1   |= TIM_CR1_CEN;
  TIM2->CCER   = 0x0010;       //Enable Channel 2


  //ADC Set up

  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
  ADC->CCR |= 0x00800000;
  ADC->CCR &= ~0x00400000;  // VBATE must be disabled for temp sensor
  ADC1->SMPR1 = 0x04000000; // Sampling time minimum 10us
  ADC1->SQR3  = 18;         // temp sensor is on 18. channel

  ADC1->CR2 = 0x13000000;     // External trigger enable for regular channels Rising Edge
  	  	  	  	  	  	  	  // 0011: Timer 2 CC2 event
  ADC1->CR2 |= ADC_CR2_ADON;

  USART2_Init();

  //printf("STM32 Temperature \r\n");

  while (1)
  {
    /* USER CODE END WHILE */
	  //-specs=nosys.specs -specs=nano.specs
	  while(!(ADC1->SR)){}
	   data = ADC1->DR;
	   voltage = (double)data/4095*3.3;
	   celsius = (voltage - 0.76)/0.0025 + 25;
	   printf("data = %d \t\t temp = %.2f\r\n",data,celsius);
	   HAL_Delay(2000);
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

void USART2_Init(void)
{

   //Enable Clocks
   RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;     // Enable GPIOA   Clock
   RCC->APB1ENR |= RCC_APB1ENR_USART2EN;    // Enable USART2  Clock

   //Configure Pins [AF]
   GPIOA->MODER  |= 0x0020;       // Set PA2 to AF
   GPIOA->AFR[0] |= 0x0700;       // Enable AF7 for USART2 [PA2]

   //Configure USART2
   USART2->BRR  = 0x1117;         // 9600bps   @42Mhz
   USART2->CR1  = USART_CR1_TE;   // Enable  TX
   USART2->CR1 |= USART_CR1_UE;   // Enable USART
}

/*
struct __FILE{int handle;};
FILE __stdout={1};

int fputc(int c, FILE *f)
{
	return USART2_Write(c);
}
*/

int __io_putchar(int ch) {
	return USART2_Write(ch);
}

int USART2_Write(int ch)
{
   while(!(USART2->SR & 0x0080)){}
   USART2->DR = ch;
   return ch;
}

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

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

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
