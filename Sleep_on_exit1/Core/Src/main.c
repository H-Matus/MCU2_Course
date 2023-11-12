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
extern uint8_t some_data[];


int main(void)
{
    HAL_Init();
    SystemClock_Config_HSE(SYS_CLOCK_FREQ_50_MHZ);
    GPIO_Init();
    UART2_Init();
    TIMER6_Init();

    HAL_PWR_EnableSleepOnExit();  //SCB->SCR |= (1 << 1);

    TIM6->SR = 0;

    HAL_TIM_Base_Start_IT(&htimer6);

    while(1);

    return 0;
}

void GPIO_AnalogConfig(void)
{
	GPIO_InitTypeDef GpioA;

	uint32_t gpio_pins = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;

	GpioA.Pin = gpio_pins;
	GpioA.Mode = GPIO_MODE_ANALOG;
	HAL_GPIO_Init(GPIOA, &GpioA);
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

    ledgpio.Pin = GPIO_PIN_12;
    ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
    ledgpio.Pull = GPIO_NOPULL;

    /* GPIO Init function */
    HAL_GPIO_Init(GPIOA, &ledgpio);
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
    huart2.Init.Mode = UART_MODE_TX_RX;
    if (HAL_UART_Init(&huart2) != HAL_OK)
    {
        Error_handler();
    }
}

void TIMER6_Init(void)
{
	htimer6.Instance = TIM6;
	htimer6.Init.Prescaler = 4999;
	htimer6.Init.Period = 32 - 1;
	if(HAL_TIM_Base_Init(&htimer6) != HAL_OK)
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

void LED_Manage_Output(uint8_t led_no)
{
    switch(led_no)
    {
        case 1:
            HAL_GPIO_WritePin(LED1_PORT, LED1_PIN_NO, GPIO_PIN_SET);
            HAL_GPIO_WritePin(LED2_PORT, LED2_PIN_NO, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LED3_PORT, LED3_PIN_NO, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LED4_PORT, LED4_PIN_NO, GPIO_PIN_RESET);
            break;
        case 2:
            HAL_GPIO_WritePin(LED1_PORT, LED1_PIN_NO, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LED2_PORT, LED2_PIN_NO, GPIO_PIN_SET);
            HAL_GPIO_WritePin(LED3_PORT, LED3_PIN_NO, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LED4_PORT, LED4_PIN_NO, GPIO_PIN_RESET);
            break;
        case 1:
            HAL_GPIO_WritePin(LED1_PORT, LED1_PIN_NO, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LED2_PORT, LED2_PIN_NO, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LED3_PORT, LED3_PIN_NO, GPIO_PIN_SET);
            HAL_GPIO_WritePin(LED4_PORT, LED4_PIN_NO, GPIO_PIN_RESET);
            break;
        case 1:
            HAL_GPIO_WritePin(LED1_PORT, LED1_PIN_NO, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LED2_PORT, LED2_PIN_NO, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LED3_PORT, LED3_PIN_NO, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LED4_PORT, LED4_PIN_NO, GPIO_PIN_SET);
            break;
        default:
            break;

    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (HAL_OK != HAL_UART_Transmit(&huart2, (uint8_t*)some_data, (uint16_t)strlen((char*)some_data), HAL_MAX_DELAY))
	{
		Error_handler();
	}
}
