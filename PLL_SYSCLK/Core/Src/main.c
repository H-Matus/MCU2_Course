/*
 * main.c
 *
 *  Created on: Jun 25, 2023
 *      Author: Admin
 */

#include <stdint.h>
#include <string.h>
#include "main.h"
#include "stm32f4xx_hal.h"

#define FALSE   0
#define TRUE    1

void UART2_Init(void);
void Error_handler(void);

UART_HandleTypeDef huart2;

int main(void)
{
    RCC_OscInitTypeDef osc_init;
    RCC_ClkInitTypeDef clk_init;
    uint8_t msg[100];

	HAL_Init();

	UART2_Init();

    
    while(1) { };

	return 0;
}

void SystemClockConfig(void)
{

}

void UART2_Init(void)
{
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    if ( HAL_UART_Init(&huart2) != HAL_OK )
    {
        // There is a problem
        Error_handler();
    }
}

void Error_handler(void)
{
    while(1);
}
