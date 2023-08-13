/*
 * main.c
 *
 *  Created on: Jul 9, 2023
 *      Author: Admin
 */

#include <main.h>
#include <string.h>

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
    TIM_OC_InitTypeDef tim2PWM_Config;
    htimer2.Instance = TIM2;
    htimer2.Init.Period = 10000-1;
    htimer2.Init.Prescaler = 4999;
    if ( HAL_OK != HAL_TIM_PWM_Init(&htimer2) )
    {
        Error_handler();
    }
    
    tim2PWM_Config.OCMode = TIM_OCMODE_PWM1;
    tim2PWM_Config.OCPolarity = TIM_OCPOLARITY_HIGH;
    
    /* This is important and needs to be worked out
     * to generate wanted pulse.
     */
    /**
     * @brief Pulse calculation:
     * Req.: 40% duty cycle PWM of 1 second period.
     * This means that the time between Update Events is 1 second. 
     * First we need to calculate what will be the value of ARR to get the period of 1 second.
     * Let's say ARR = 10k
     * Pulse = ARR x 40% = ARR x (40/100) = 4k;
     * Pulse number goes into CCR (Capture Compare Register) and when timer's counter reaches 4k, then it will toggle.
     * 
     * When counter value is less than CCR1(Pulse), the channel is HIGH (because of OCPolarity).
     * 
     */
    tim2PWM_Config.Pulse = (htimer2.Init.Period * 25) / 100;
    if ( HAL_OK != HAL_TIM_PWM_ConfigChannel(&htimer2, &tim2PWM_Config, TIM_CHANNEL_1) )
    {
        Error_handler();
    }

    tim2PWM_Config.Pulse = (htimer2.Init.Period * 40) / 100;
    if ( HAL_OK != HAL_TIM_PWM_ConfigChannel(&htimer2, &tim2PWM_Config, TIM_CHANNEL_2) )
    {
        Error_handler();
    }

    tim2PWM_Config.Pulse = (htimer2.Init.Period * 75) / 100;
    if ( HAL_OK != HAL_TIM_PWM_ConfigChannel(&htimer2, &tim2PWM_Config, TIM_CHANNEL_3) )
    {
        Error_handler();
    }
    
    tim2PWM_Config.Pulse = (htimer2.Init.Period * 90) / 100;
    if ( HAL_OK != HAL_TIM_PWM_ConfigChannel(&htimer2, &tim2PWM_Config, TIM_CHANNEL_4) )
    {
        Error_handler();
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
    huart2.Init.Mode = UART_MODE_TX_RX;
    if ( HAL_UART_Init(&huart2) != HAL_OK )
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

    switch(clock_freq)
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

    if ( HAL_OK != HAL_RCC_OscConfig(&Osc_Init) )
    {
        Error_handler();
    }

    if ( HAL_OK != HAL_RCC_ClockConfig(&Clock_Init, flash_latency) )
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

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{

}
