/*
 * main.c
 *
 *  Created on: Jul 9, 2023
 *      Author: Admin
 */

#include <string.h>
#include "stm32f4xx_hal.h"
#include "main.h"

void GPIO_Init(void);
void Error_handler(void);
void TIMER2_Init(void);
void UART2_Init(void);
void SystemClock_Config_HSE(uint8_t clock_freq);

TIM_HandleTypeDef htimer2;
UART_HandleTypeDef huart2;

int main(void)
{
    HAL_Init();
    SystemClock_Config_HSE(SYS_CLOCK_FREQ_50_MHZ);
    GPIO_Init();
    UART2_Init();
    TIMER2_Init();

    while(1);

    return 0;
}

void GPIO_Init(void)
{
    /* Enable AHB1 peripheral clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* Define the structure of GPIO */
    GPIO_InitTypeDef ledgpio;
    ledgpio.Pin = GPIO_PIN_5;
    ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
    ledgpio.Pull = GPIO_NOPULL;
    
    /* GPIO Init function */
    HAL_GPIO_Init(GPIOA, &ledgpio);

}

void Error_handler(void)
{
    while(1)
    {

    }
}

void TIMER2_Init(void)
{

}

void UART2_Init(void)
{
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    if ( HAL_UART_Init(&huart2) != HAL_OK )
    {
        Error_handler();
    }

}

void SystemClock_ConfigHSE(uint8_t clock_freq)
{

}