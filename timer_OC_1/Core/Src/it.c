/*
 * it.c
 *
 *  Created on: Jul 9, 2023
 *      Author: Admin
 */

#include <main.h>

void SysTick_Handler(void)
{
    HAL_IncTick();
    HAL_SYSTICK_IRQHandler();
}

void TIM2_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&htimer2);
}