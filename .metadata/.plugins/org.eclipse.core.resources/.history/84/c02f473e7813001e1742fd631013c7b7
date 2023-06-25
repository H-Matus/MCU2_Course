/*
 * msp.c
 *
 *  Created on: Jun 25, 2023
 *      Author: Admin
 */

#include "msp.h"

/**
 * @brief   Starting low level hardware specific inits. 
 * 
 */
void HAL_MspInit(void)
{
    /* Low level processor specific inits.*/
    /* 1. Set up the priority grouping of the ARM Cortex MX Processor. */
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

    /* 2. Enable required system exceptions of the ARM Cortex MX Processor. */
    SCB->SHCSR |= 0x7 << 16;

    /* 3. Configure the priority for system exceptions. */
    HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
    HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
    HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
}

/**
 * @brief       Initialising low level hardware for UART.
 * @attention   When the peripheral is not working as expected, then this function should
 *              be the first debug spot. Most people do mistakes in configuring the low
 *              level peripheral settings.
 * 
 * @param       huart 
 */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef gpio_uart;

    /* 1. Enable the clock for the USART2 peripheral. */
    /* Take a look at the HAL_RCC header file. */
    __HAL_RCC_USART2_CLK_ENABLE();

    /* 2. Do the pin muxing configurations. */

    /* UART2 TX: */
    /* Pin muxing is different for every MCU. */
    /* Identify pin packs first in the MCU datasheet. */
    gpio_uart.Pin = GPIO_PIN_2;
    gpio_uart.Mode = GPIO_MODE_AF_PP;
    gpio_uart.Pull = GPIO_PULLUP; /* In most communication protocols, the GPIOs need to be pulled up. */ 
    gpio_uart.Speed = GPIO_SPEED_FREQ_LOW;
    gpio_uart.Alternate = GPIO_AF7_USART2; // UART2 TX
    HAL_GPIO_Init(GPIOA, &gpio_uart);

    /* UART2 RX: */
    gpio_uart.Pin = GPIO_PIN_3; // UART2 RX
    HAL_GPIO_Init(GPIOA, &gpio_uart);

    /* 3. Enable the IRQ and set up the priority. */
    /* Interrupt specific. */
    HAL_NVIC_EnableIRQ(USART2_IRQn);
    HAL_NVIC_SetPriority(USART2_IRQn, 15, 0);

}