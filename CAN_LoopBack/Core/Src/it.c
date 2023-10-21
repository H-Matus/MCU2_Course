/*
 * it.c
 *
 *  Created on: Aug 13, 2023
 *      Author: Admin
 */

#include <main.h>

void SysTick_Handler(void)
{
    HAL_IncTick();
    HAL_SYSTICK_IRQHandler();
}
