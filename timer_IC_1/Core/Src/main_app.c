#include<string.h>
#include "stm32f4xx_hal.h"
#include "main_app.h"

void SystemClockConfig( uint8_t clock_freq );
void GPIO_Init(void);
void Error_handler(void);

TIM_HandleTypeDef htimer2;

int main(void)
{
	HAL_Init();

	SystemClockConfig();

	GPIO_Init();

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
            osc_init.PLL.PLLM = 8;
            osc_init.PLL.PLLN = 50;
            osc_init.PLL.PLLP = RCC_PLLP_DIV2;
            osc_init.PLL.PLLQ = 2;
            osc_init.PLL.PLLR = 2;

            clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
            clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
            clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
            clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
            clk_init.APB2CLKDivider = RCC_HCLK_DIV1;
            break;
        }
        case SYS_CLOCK_FREQ_84_MHZ:
        {
            osc_init.PLL.PLLM = 8;
            osc_init.PLL.PLLN = 84;
            osc_init.PLL.PLLP = RCC_PLLP_DIV2;
            osc_init.PLL.PLLQ = 2;
            osc_init.PLL.PLLR = 2;

            clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
            clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
            clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
            clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
            clk_init.APB2CLKDivider = RCC_HCLK_DIV1;
            break;
        }
        case SYS_CLOCK_FREQ_120_MHZ:
        {
            osc_init.PLL.PLLM = 8;
            osc_init.PLL.PLLN = 120;
            osc_init.PLL.PLLP = RCC_PLLP_DIV2;
            osc_init.PLL.PLLQ = 2;
            osc_init.PLL.PLLR = 2;

            clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
            clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
            clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
            clk_init.APB1CLKDivider = RCC_HCLK_DIV4;
            clk_init.APB2CLKDivider = RCC_HCLK_DIV2;
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

        /* SysTick_IRQn interrupot configuration */
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

void Error_handler(void)
{
	while(1);
}

