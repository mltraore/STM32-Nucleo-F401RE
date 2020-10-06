#include "main.h"


// PC0-PC7 --> D0 - D7
// PB5 -> RS
// PB6 -> R/W
// PB7 -> EN

#define RS  0x20   //PB5
#define RW  0x40   //PB6
#define EN  0x80   //PB7


void SystemClock_Config(void);
static void MX_GPIO_Init(void);

void GPIO_Init(void);
void LCD_command(unsigned char command);
void LCD_data(char data);

void LCD_Init(void);
void delay_ticks(unsigned ticks);
static inline void delay_ms(unsigned ms);


int main(void)
{

  HAL_Init();
  SystemClock_Config();
 // MX_GPIO_Init();
  LCD_Init();

  while (1)
  {
    /* USER CODE END WHILE */

	 LCD_data('M');
	 LCD_data('O');
	 LCD_data('H');
	 LCD_data('A');
	 LCD_data('M');
	 LCD_data('E');
	 LCD_data('D');
	 LCD_command(1);  // Clear string

	 delay_ms(300);

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

void GPIO_Init(void)
{
   RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
   GPIOB->MODER |= 0x5400;  // PB5..7 in output mode
   GPIOB->BSRR   = 0x00C; // Set EN and RW low
   //RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
   GPIOC->MODER |= 0x00005555;  //PC0..7  in output mode
}


void LCD_Init(void)
{
	GPIO_Init();
    delay_ms(30);
    LCD_command(0x30);
    delay_ms(10);
    LCD_command(0x30);
    delay_ms(1);
    LCD_command(0x30);

    LCD_command(0x38);   // set 8-bit data mode2-line,5x7 font
    LCD_command(0x06);   // move cursor right
    LCD_command(0x01);   // Clear screen
    LCD_command(0x0F);   // turn display, blink cursor


}



void LCD_command(unsigned char command)
{
  GPIOB->BSRR = (RS|RW) << 16;  // rs= 0:instruction,  r/w=0: writing
  GPIOC->ODR  = command;
  GPIOB->BSRR = EN;
  delay_ms(0);
  GPIOB->BSRR = EN << 16;
}

void LCD_data(char data)
{
  GPIOB->BSRR = RS; //|(RW<<16); // rs=1,  r/w=0
  GPIOB->BSRR = (RW<<16);
  GPIOC->ODR  = data;
  GPIOB->BSRR = EN;
  delay_ms(0);
  GPIOB->BSRR = (EN<<16);
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
