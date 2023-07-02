/*
 * msp.c
 *
 *  Created on: 2 Jul 2023
 *      Author: Admin
 */



#include <main.h>

void HAL_MspInit(void)
{
 //Here will do low level processor specific inits.
	//1. Set up the priority grouping of the arm cortex mx processor
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	//2. Enable the required system exceptions of the arm cortex mx processor
	SCB->SHCSR |= 0x7 << 16; //usage fault, memory fault and bus fault system exceptions

	//3. configure the priority for the system exceptions
	HAL_NVIC_SetPriority(MemoryManagement_IRQn,0,0);
	HAL_NVIC_SetPriority(BusFault_IRQn,0,0);
	HAL_NVIC_SetPriority(UsageFault_IRQn,0,0);
}

void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htimer)
{
    GPIO_InitTypeDef tim2ch1_gpio;

    /* 1. Enable the clock for timer 2: */
    __HAL_RCC_TIM2_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* 2. Configure a gpio to behave as timer2 channel 1 */
    tim2ch1_gpio.Pin = GPIO_PIN_0;
    tim2ch1_gpio.Mode = GPIO_MODE_AF_PP;
    tim2ch1_gpio.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(GPIOA, &tim2ch1_gpio);
    
    /* 3. nvic settings for IRQ */
    HAL_NVIC_SetPriority(TIM2_IRQn, 15, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);

}