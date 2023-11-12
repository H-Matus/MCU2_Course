/*
 * it.c
 *
 *  Created on: Aug 13, 2023
 *      Author: Admin
 */

#include <main.h>

extern CAN_HandleTypeDef hcan1;
extern TIM_HandleTypeDef htimer6;

void SysTick_Handler(void)
{
    HAL_IncTick();
    HAL_SYSTICK_IRQHandler();
}

void TIM6_DAC_IRQHandler(void)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);
	HAL_TIM_IRQHandler(&htimer6);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
}

void EXTI15_10_IRQHandler(void)
{
	HAL_TIM_Base_Start_IT(&htimer6);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
}
