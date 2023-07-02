#include <main.h>
#include<string.h>
#include "stm32f4xx_hal.h"

void SystemClockConfig( uint8_t clock_freq );
void GPIO_Init(void);
void Error_handler(void);
void TIMER2_Init(void);

TIM_HandleTypeDef htimer2;

int main(void)
{
	HAL_Init();

	SystemClockConfig(SYS_CLOCK_FREQ_50_MHZ);

	GPIO_Init();

    TIMER2_Init();

    while(1);

	return 0;
}


void SystemClockConfig( uint8_t clock_freq )
{
    RCC_OscInitTypeDef Osc_Init;
    RCC_ClkInitTypeDef Clk_Init;

    Osc_Init.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    Osc_Init.HSIState = RCC_HSI_ON;
    Osc_Init.HSICalibrationValue = 16;
    Osc_Init.PLL.PLLState = RCC_PLL_ON;
    Osc_Init.PLL.PLLSource = RCC_PLLSOURCE_HSI;

    switch(clock_freq)
    {
        case SYS_CLOCK_FREQ_50_MHZ:
        {
            Osc_Init.PLL.PLLM = 8;
            Osc_Init.PLL.PLLN = 50;
            Osc_Init.PLL.PLLP = RCC_PLLP_DIV2;
            Osc_Init.PLL.PLLQ = 2;

            Clk_Init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
            Clk_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
            Clk_Init.AHBCLKDivider = RCC_SYSCLK_DIV1;
            Clk_Init.APB1CLKDivider = RCC_HCLK_DIV2;
            Clk_Init.APB2CLKDivider = RCC_HCLK_DIV1;
            break;
        }
        case SYS_CLOCK_FREQ_84_MHZ:
        {
            Osc_Init.PLL.PLLM = 8;
            Osc_Init.PLL.PLLN = 84;
            Osc_Init.PLL.PLLP = RCC_PLLP_DIV2;
            Osc_Init.PLL.PLLQ = 2;

            Clk_Init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
            Clk_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
            Clk_Init.AHBCLKDivider = RCC_SYSCLK_DIV1;
            Clk_Init.APB1CLKDivider = RCC_HCLK_DIV2;
            Clk_Init.APB2CLKDivider = RCC_HCLK_DIV1;
            break;
        }
        case SYS_CLOCK_FREQ_120_MHZ:
        {
            Osc_Init.PLL.PLLM = 8;
            Osc_Init.PLL.PLLN = 120;
            Osc_Init.PLL.PLLP = RCC_PLLP_DIV2;
            Osc_Init.PLL.PLLQ = 2;

            Clk_Init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
            Clk_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
            Clk_Init.AHBCLKDivider = RCC_SYSCLK_DIV1;
            Clk_Init.APB1CLKDivider = RCC_HCLK_DIV4;
            Clk_Init.APB2CLKDivider = RCC_HCLK_DIV2;
            break;
        }
        default:
        {
            return;
        }

        if( HAL_RCC_OscConfig(&Osc_Init) != HAL_OK )
        {
            Error_handler();
        }

        if( HAL_RCC_ClockConfig(&Clk_Init, FLASH_LATENCY_2) != HAL_OK )
        {
            Error_handler();
        }

        /* Configure the sustick timer interrupt frequency (for every 1 ms) */
        uint32_t hclk_freq = HAL_RCC_GetHCLKFreq();
        HAL_SYSTICK_Config(hclk_freq / 1000);

        /* Configure the systick */
        HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

        /* SysTick_IRQn interrupt configuration */
        HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
    }
}

void GPIO_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef ledgpio;
	ledgpio.Pin = GPIO_PIN_5;
	ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
	ledgpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA,&ledgpio);
}

void TIMER2_Init(void)
{
    TIM_IC_InitTypeDef timer2IC_Config;

    htimer2.Instance = TIM2;
    htimer2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htimer2.Init.Period = 0xFFFFFFFF;
    htimer2.Init.Prescaler = 1;
    if ( HAL_TIM_IC_Init(&htimer2) != HAL_OK )
    {
        Error_handler();
    }

    timer2IC_Config.ICFilter = 0;
    timer2IC_Config.ICPolarity = TIM_ICPOLARITY_RISING;
    timer2IC_Config.ICPrescaler = TIM_ICPSC_DIV1;
    timer2IC_Config.ICSelection = TIM_ICSELECTION_DIRECTTI;
    if ( HAL_TIM_IC_ConfigChannel(&htimer2, &timer2IC_Config, TIM_CHANNEL_1) != HAL_OK )
    {
        Error_handler();
    }

}

void Error_handler(void)
{
	while(1);
}

