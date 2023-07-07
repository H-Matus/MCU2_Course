#include <main.h>
#include <string.h>
#include <stdio.h>
#include "stm32f4xx_hal.h"

void SystemClockConfig( uint8_t clock_freq );
void GPIO_Init(void);
void UART2_Init(void);
void Error_handler(void);
void TIMER2_Init(void);
void LSE_Configuration(void);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);

UART_HandleTypeDef huart2;
TIM_HandleTypeDef htimer2;
uint32_t input_captures[2] = {0};
uint8_t count = 0;
uint8_t is_capture_done = FALSE;

int main(void)
{
    uint32_t capture_diff = 0;
    double timer2_cnt_freq = 0;
    double timer2_cnt_res = 0;
    double user_signal_time_period = 0;
    double user_signal_freq = 0;
    char user_msg[100];

	HAL_Init();

	SystemClockConfig(SYS_CLOCK_FREQ_50_MHZ);

	GPIO_Init();

	UART2_Init();

    TIMER2_Init();

    LSE_Configuration();

    while(1)
    {
        if(is_capture_done)
        {
            if(input_captures[1] > input_captures[0])
            {
                capture_diff = input_captures[1] - input_captures[0];
            }
            else
            {
                capture_diff = (0xFFFFFFFF - input_captures[0]) + input_captures[1];
            }

            /* After this we can calculate the time period of the applied input signal. */
            timer2_cnt_freq = (HAL_RCC_GetPCLK1Freq() * 2) / htimer2.Init.Prescaler;
            timer2_cnt_res = 1 / timer2_cnt_freq;
            user_signal_time_period = capture_diff * timer2_cnt_res;
            user_signal_freq = 1 / user_signal_time_period;

            sprintf(user_msg, "Frequency of the signal applied = %f\r\n", user_signal_freq);
            HAL_UART_Transmit(&huart2, user_msg, strlen(user_msg), HAL_MAX_DELAY);

            is_capture_done = FALSE;
        }
    }

	return 0;
}


void SystemClockConfig( uint8_t clock_freq )
{
    RCC_OscInitTypeDef Osc_Init;
    RCC_ClkInitTypeDef Clk_Init;

    Osc_Init.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSE;
    Osc_Init.HSIState = RCC_HSI_ON;
    Osc_Init.LSEState = RCC_LSE_ON;
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

void LSE_Configuration(void)
{
    /* Supplying the LSE oscillator via GPIO */
    HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_LSE, RCC_MCODIV_1);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if(!is_capture_done)
    {
        if(count == 1)
        {
            input_captures[0] = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1);
            count++;
        }
        else if(count == 2)
        {
            input_captures[1] = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1);
            count = 1;
            is_capture_done = TRUE;
        }
    }
}

void Error_handler(void)
{
	while(1);
}

