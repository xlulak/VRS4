/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#include "assignment.h"

void SystemClock_Config(void);
uint8_t check_button_state(GPIO_TypeDef* PORT, uint8_t PIN);

uint8_t switch_state = 0;

int main(void)
{
  /*Default system setup*/
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  SystemClock_Config();


  /*
   * TASK - configure MCU peripherals so that button triggers external interrupt - EXTI.
   * Button must be connected to the GPIO port B, pin 4.
   * LED must be connected to the GPIO port A and its pin 4.
   *
   * Adjust values of macros defined in "assignment.h".
   * Implement function "checkButtonState" declared in "assignment.h".
   */


  /* Configure external interrupt - EXTI*/

  /*set EXTI source PA3*/ // ->PB4
   SYSCFG->EXTICR[0] &= ~(0xFU << 12U);	//keby nejde tak exticr[1]

   //Enable interrupt from EXTI line 4
   EXTI->IMR |= EXTI_IMR_MR4;

   //Set EXTI trigger to falling edge
   EXTI->RTSR &= ~(EXTI_IMR_MR4);
   EXTI->FTSR |= EXTI_IMR_MR4;

  /* Configure GPIOB-4 pin as an input pin - button */
  RCC->AHBENR |= RCC_AHBENR_GPIOBEN;		//enabled clock
  GPIOB->MODER &= ~(GPIO_MODER_MODER4);		//moder reset -> INPUT
  GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR4);		//transistors
  GPIOB->PUPDR |= GPIO_PUPDR_PUPDR4_0;


  /* Configure GPIOA-4 pin as an output pin - LED */
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN;			//clock enabled
  GPIOA->MODER &= ~(GPIO_MODER_MODER4);			//reset
  GPIOA->MODER |= GPIO_MODER_MODER4_0;			//outputmode
  GPIOA->OTYPER &= ~(GPIO_OTYPER_OT_4);			//otyper 4 in A set to 1
  GPIOA->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR4); 	//set speed of output register
  GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR4);			//no pull up/pull down

	  //type your code for GPIO configuration here:


  while (1)
  {
	  if(switch_state)
	  {
		  GPIOA->BSRR |= GPIO_BSRR_BS_4;
		  for(uint16_t i=0; i<0xFF00; i++){}
		  GPIOA->BRR |= GPIO_BRR_BR_4;
		  for(uint16_t i=0; i<0xFF00; i++){}
	  }
	  else
	  {
		  GPIOA->BRR |= GPIO_BRR_BR_4;
	  }
  }

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);

  if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_0)
  {
  Error_Handler();  
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {
    
  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {
  
  }
  LL_Init1msTick(8000000);
  LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
  LL_SetSystemCoreClock(8000000);
}


uint8_t checkButtonState(GPIO_TypeDef* PORT, uint8_t PIN, uint8_t edge, uint8_t samples_window, uint8_t samples_required)
{
	 /*  Function checks if the button was pressed or EXTI detection was false - positive.
	 *  @input_param_1 - PORT: GPIO port connected to button.
	 *  @input_param_2 - PIN: GPIO pin connected to button.
	 *  @input_param_3 - edge: EXTI trigger edge. Defines what is the input GPIO idle state after the button is pressed.
	 *  					   TRIGGER_RISE - input is expected to be "1" after EXTI was triggered.
	 *  					   TRIGGER_FALL - input is expected to be "0" after EXTI was triggered.
	 *  @input_param_4 - samples_window: How many samples are checked (input port is read) after EXTI is triggered.
	 *  @input_param_5 - samples_required: How many samples in row are required to be in the idle state. */

	int pocitadlo = 0;

	for (int i=0;i<samples_window;i++)
	{
		if (edge == 1)					//ak je edge == 1 tak vlastne je to FALL hrana, cize pojdeme hladat 0
		{
			if (!BUTTON_GET_STATE)
			{
				pocitadlo++;
			}
			else
			{
				pocitadlo = 0;
			}
			if (pocitadlo == samples_required)
			{
				return 1;
			}
		}

		else if (edge == 0)				//ak je edge == 0 tak je to RISE hrana, cize pojdeme hladat 1
		{
			if (BUTTON_GET_STATE)
			{
				pocitadlo++;
			}
			else
			{
				pocitadlo = 0;
			}
			if (pocitadlo == samples_required)
			{
				return 1;
			}
		}
	}
return 0;
}

void EXTI4_IRQHandler(void)
{
	if(checkButtonState(GPIO_PORT_BUTTON,
						GPIO_PIN_BUTTON,
						BUTTON_EXTI_TRIGGER,
						BUTTON_EXTI_SAMPLES_WINDOW,
						BUTTON_EXTI_SAMPLES_REQUIRED))
	{
		switch_state ^= 1;
	}

	/* Clear EXTI4 pending register flag */

	EXTI->PR |= (EXTI_PR_PIF4);
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
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
