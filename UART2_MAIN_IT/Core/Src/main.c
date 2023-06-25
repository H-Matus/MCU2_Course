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

void SystemClockConfig(void);
void UART2_Init(void);
void Error_handler(void);
uint8_t convert_to_capital(uint8_t);

UART_HandleTypeDef huart2;

char *user_data = "The application is running.\r\n";
uint8_t data_buffer[100];
uint8_t recvd_data;
uint32_t count = 0;
uint8_t reception_complete = FALSE;

int main(void)
{
	HAL_Init();
	SystemClockConfig();
	UART2_Init();

    uint16_t len_of_data = strlen(user_data);
    
    if( HAL_UART_Transmit(&huart2, (uint8_t*)user_data, len_of_data, HAL_MAX_DELAY) != HAL_OK )
    {
        Error_handler();
    }

    while(reception_complete != TRUE)
    {
        HAL_UART_Receive_IT(&huart2, &recvd_data, 1);
    }
    
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

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(recvd_data == '\r')
    {
        reception_complete = TRUE;
        data_buffer[count++] = '\r';
        HAL_UART_Transmit(&huart2, data_buffer, count, HAL_MAX_DELAY);
    }
    else
    {
        data_buffer[count++] = recvd_data;
    }
}

uint8_t convert_to_capital(uint8_t data)
{
    /* ASCII manipulations: */
    /* Checking if the letter is lowercase: */
    if( data >= 'a' && data <= 'z' )
    {
        data = data - ('a' - 'A');
    }

    return data;
}
