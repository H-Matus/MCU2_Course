/*
 * main.c
 *
 *  Created on: Jul 9, 2023
 *      Author: Admin
 */

#include <main.h>
#include <string.h>

UART_HandleTypeDef huart2;
TIM_HandleTypeDef htimer6;
RTC_HandleTypeDef hrtc;
extern uint8_t some_data[];

void printmsg(char *format, ...)
{
	char str[80];

	// Extract the argument list using VA apis
	va_list args;
	va_start(args, format);
	vsprintf(str, format, args);
	HAL_UART_Transmit(&huart2, (uint8_t *)str, strlen(str), HAL_MAX_DELAY);
	va_end(args);

}


int main(void)
{
    HAL_Init();
    SystemClock_Config_HSE(SYS_CLOCK_FREQ_50_MHZ);
    GPIO_Init();
    UART2_Init();

    RTC_CalendarConfig();

    while(1)
    {

    }

    return 0;
}

void GPIO_Init(void)
{
    /* Enable peripheral clock */
	__HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /* Define the structure of GPIO for button */
    GPIO_InitTypeDef buttongpio, ledgpio;
    ledgpio.Pin = GPIO_PIN_5;
    ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
    ledgpio.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &ledgpio);

    buttongpio.Pin = GPIO_PIN_13;
    buttongpio.Mode = GPIO_MODE_IT_FALLING;
    buttongpio.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &buttongpio);

    HAL_NVIC_SetPriority(EXTI_15_10_IRQn, 15, 0);
    HAL_NVIC_EnableIRQ(EXTI_15_10_IRQn);

}

void RTC_Init(void)
{
	hrtc.Instance = RTC;
	hrtc.Init.HourFormat = RTC_HOURFORMAT_12;
	hrtc.Init.AsynchPrediv = 0x7F;
	hrtc.Init.SynchPrediv = 0xFF;
	hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_LOW;
	hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;

	if(HAL_OK != HAL_RTC_Init(&hrtc))
	{
		Error_handler();
	}
}

void RTC_CalendarConfig(void)
{
	RTC_TimeTypeDef RTC_TimeInit;

	RTC_TimeInit.Hours = 12;
	RTC_TimeInit.Minutes = 11;
	RTC_TimeInit.Seconds = 10;
	RTC_TimeInit.TimeFormat = RTC_HOURFORMAT12_PM;


	if(HAL_OK != HAL_RTC_SetTime(&hrtc, &RTC_TimeInit, RTC_FORMAT_BIN))
	{
		Error_handler();
	}

	RTC_DateTypeDef RTC_DateInit;

	RTC_DateInit.Date = 12;
	RTC_DateInit.Month = RTC_MONTH_JUNE;
	RTC_DateInit.Year = 23;
	RTC_DateInit.WeekDay = RTC_WEEKDAY_SUNDAY;

	if(HAL_OK != HAL_RTC_SetDate(&hrtc, &RTC_DateInit, RTC_FORMAT_BIN))
	{
		Error_handler();
	}

}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	RTC_DateTypeDef RTC_DateRead;
	RTC_TimeTypeDef RTC_TimeRead;

	if(HAL_OK != HAL_RTC_GetDate(&hrtc, &RTC_DateRead, RTC_FORMAT_BIN))
	{
		Error_handler();
	}

	if(HAL_OK != HAL_RTC_GetTime(&hrtc, &RTC_TimeRead, RTC_FORMAT_BIN))
	{
		Error_handler();
	}

	if(HAL_OK != HAL_UART_Transmit(&huart2, (uint8_t *)str, strlen(str), HAL_MAX_DELAY))
	{
		Error_handler();
	}
}

void Error_handler(void)
{
    while (1)
    {
    }
}

void UART2_Init(void)
{
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.Mode = UART_MODE_TX_TX;
    if (HAL_UART_Init(&huart2) != HAL_OK)
    {
        Error_handler();
    }
}

void SystemClock_Config_HSE(uint8_t clock_freq)
{
    RCC_OscInitTypeDef Osc_Init;
    RCC_ClkInitTypeDef Clock_Init;
    uint8_t flash_latency = 0;

    Osc_Init.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSE;
    Osc_Init.HSEState = RCC_HSE_ON;
    Osc_Init.HSIState = RCC_HSI_ON;
    Osc_Init.LSEState = RCC_LSE_ON;
    Osc_Init.PLL.PLLState = RCC_PLL_ON;
    Osc_Init.PLL.PLLSource = RCC_PLLSOURCE_HSE;

    switch (clock_freq)
    {
    case SYS_CLOCK_FREQ_50_MHZ:
        Osc_Init.PLL.PLLM = 4;
        Osc_Init.PLL.PLLN = 50;
        Osc_Init.PLL.PLLP = RCC_PLLP_DIV2;
        Osc_Init.PLL.PLLQ = 2;
        Clock_Init.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
        Clock_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
        Clock_Init.AHBCLKDivider = RCC_SYSCLK_DIV1;
        Clock_Init.APB1CLKDivider = RCC_HCLK_DIV2;
        Clock_Init.APB2CLKDivider = RCC_HCLK_DIV1;
        flash_latency = 1;
        break;
    case SYS_CLOCK_FREQ_84_MHZ:
        Osc_Init.PLL.PLLM = 4;
        Osc_Init.PLL.PLLN = 120;
        Osc_Init.PLL.PLLP = RCC_PLLP_DIV2;
        Osc_Init.PLL.PLLQ = 2;
        Clock_Init.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_SYSCLK;
        Clock_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
        Clock_Init.AHBCLKDivider = RCC_SYSCLK_DIV1;
        Clock_Init.APB1CLKDivider = RCC_HCLK_DIV2;
        Clock_Init.APB2CLKDivider = RCC_HCLK_DIV1;
        flash_latency = 2;
        break;
    case SYS_CLOCK_FREQ_120_MHZ:
        Osc_Init.PLL.PLLM = 4;
        Osc_Init.PLL.PLLN = 120;
        Osc_Init.PLL.PLLP = RCC_PLLP_DIV2;
        Osc_Init.PLL.PLLQ = 2;
        Clock_Init.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_SYSCLK;
        Clock_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
        Clock_Init.AHBCLKDivider = RCC_SYSCLK_DIV1;
        Clock_Init.APB1CLKDivider = RCC_HCLK_DIV4;
        Clock_Init.APB2CLKDivider = RCC_HCLK_DIV2;
        flash_latency = 3;
        break;
    default:
        break;
    }

    if (HAL_OK != HAL_RCC_OscConfig(&Osc_Init))
    {
        Error_handler();
    }

    if (HAL_OK != HAL_RCC_ClockConfig(&Clock_Init, flash_latency))
    {
        Error_handler();
    }

    /* Configure the systick timer interrupt frequency (for every 1 ms). */
    uint32_t hclk_freq = HAL_RCC_GetHCLKFreq();
    HAL_SYSTICK_Config(hclk_freq / 1000);

    /* Configure the Systick */
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    /* SysTick_IRQn priority configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

