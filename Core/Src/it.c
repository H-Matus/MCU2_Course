/*
 * it.c
 *
 *  Created on: Jun 25, 2023
 *      Author: Admin
 */

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();

}
