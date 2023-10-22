/*
 * msp.c
 *
 *  Created on: Aug 13, 2023
 *      Author: Admin
 */

#include <main.h>

void HAL_MspInit(void)
{
    /* Low level processor specific inits. */
    /* 1. Set up the priority grouping of the arm cortex mx processor. */
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

    /* 2. Enable the required system exceptions of the arm cortex mx processor. */
    SCB->SHCSR |= 0x7 << 16; /* Usage fault, memory fault and bus fault system exceptions. */

    /* 3. Configure the priority for the system exceptions. */
    HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
    HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
    HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    /* Low level USART2 inits. */
    GPIO_InitTypeDef gpio_uart;

    /* 1. Enable the clock for the USART2 peripheral as well as for GPIOA peripheral. */
    __HAL_RCC_USART2_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* 2. Do the pin muxing configs. */
    gpio_uart.Pin = GPIO_PIN_2;
    gpio_uart.Mode = GPIO_MODE_AF_PP;
    gpio_uart.Pull = GPIO_PULLUP;
    gpio_uart.Speed = GPIO_SPEED_FREQ_LOW;
    gpio_uart.Alternate = GPIO_AF7_USART2; /* UART2_TX */
    HAL_GPIO_Init(GPIOA, &gpio_uart);

    gpio_uart.Pin = GPIO_PIN_3; /* UART2_RX */
    HAL_GPIO_Init(GPIOA, &gpio_uart);

    /* 3. Enable the IRQ and set up the priority (NVIC settings). */
    HAL_NVIC_EnableIRQ(USART2_IRQn);
    HAL_NVIC_SetPriority(USART2_IRQn, 15, 0);
}

void HAL_CAN_MspInit(CAN_HandleTypeDef *hcan)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    __HAL_RCC_CAN1_CLK_ENABLE();

    /**
     * PA11 -> CAN1_RX
     * PA12 -> CAN1_TX 
     * 
     */
    GPIO_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(CAN1_TX_IRQn, 15, 0);
    HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 15, 0);
    HAL_NVIC_SetPriority(CAN1_RX1_IRQn, 15, 0);
    HAL_NVIC_SetPriority(CAN1_SCE_IRQn, 15, 0);

    HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
    HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);
    HAL_NVIC_EnableIRQ(CAN1_SCE_IRQn);

}