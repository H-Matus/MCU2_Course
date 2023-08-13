/*
 * main.c
 *
 *  Created on: Jul 9, 2023
 *      Author: Admin
 */

#include <main.h>
#include <string.h>


void GPIO_Init(void);
void Error_handler(void);
void TIMER2_Init(void);
void UART2_Init(void);
void SystemClock_Config_HSE(uint8_t clock_freq);

TIM_HandleTypeDef htimer2;
UART_HandleTypeDef huart2;

uint32_t pulse1_value = 25000;  /* to produce 500Hz */
uint32_t pulse2_value = 12500;  /* to produce 1kHz */
uint32_t pulse3_value = 6250;   /* to produce 2kHz */
uint32_t pulse4_value = 3125;   /* to produce 4kHz */

uint32_t ccr_content;

int main(void)
{
    HAL_Init();
    SystemClock_Config_HSE(SYS_CLOCK_FREQ_50_MHZ);
    GPIO_Init();
    UART2_Init();
    TIMER2_Init();

    /* To start the timer: */
    if ( HAL_OK != HAL_TIM_OC_Start_IT(&htimer2, TIM_CHANNEL_1) )
    {
        Error_handler();
    }

    if ( HAL_OK != HAL_TIM_OC_Start_IT(&htimer2, TIM_CHANNEL_2) )
    {
        Error_handler();
    }

    if ( HAL_OK != HAL_TIM_OC_Start_IT(&htimer2, TIM_CHANNEL_3) )
    {
        Error_handler();
    }
    
    if ( HAL_OK != HAL_TIM_OC_Start_IT(&htimer2, TIM_CHANNEL_4) )
    {
        Error_handler();
    }

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
    TIM_OC_InitTypeDef tim2OC_Init;

    htimer2.Instance = TIM2;
    htimer2.Init.Period = 0xFFFFFFFF;
    htimer2.Init.Prescaler = 1;
    if ( HAL_OK != HAL_TIM_OC_Init(&htimer2) )
    {
        Error_handler();
    }

    tim2OC_Init.OCMode = TIM_OCMODE_TOGGLE;
    tim2OC_Init.OCPolarity = TIM_OCPOLARITY_HIGH;
    tim2OC_Init.Pulse = pulse1_value;
    if ( HAL_OK != HAL_TIM_OC_ConfigChannel(&htimer2, &tim2OC_Init, TIM_CHANNEL_1) )
    {
        Error_handler();
    }

    tim2OC_Init.Pulse = pulse2_value;
    if ( HAL_OK != HAL_TIM_OC_ConfigChannel(&htimer2, &tim2OC_Init, TIM_CHANNEL_2) )
    {
        Error_handler();
    }

    tim2OC_Init.Pulse = pulse3_value;
    if ( HAL_OK != HAL_TIM_OC_ConfigChannel(&htimer2, &tim2OC_Init, TIM_CHANNEL_3) )
    {
        Error_handler();
    }

    tim2OC_Init.Pulse = pulse4_value;
    if ( HAL_OK != HAL_TIM_OC_ConfigChannel(&htimer2, &tim2OC_Init, TIM_CHANNEL_4) )
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
    /* TIM3_CH1 toggling with frequency = 500Hz */
    if(HAL_TIM_ACTIVE_CHANNEL_1 == htim->Channel)
    {
        ccr_content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
        __HAL_TIM_SetCompare(htim, TIM_CHANNEL_1, (ccr_content + pulse1_value));

    }

    /* TIM3_CH2 toggling with frequency = 1kHz. */
    if(HAL_TIM_ACTIVE_CHANNEL_2 == htim->Channel)
    {
        ccr_content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
        __HAL_TIM_SetCompare(htim, TIM_CHANNEL_2, (ccr_content + pulse2_value));

    }

    /* TIM3_CH3 toggling with frequency = 2kHz. */
    if(HAL_TIM_ACTIVE_CHANNEL_3 == htim->Channel)
    {
        ccr_content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);
        __HAL_TIM_SetCompare(htim, TIM_CHANNEL_3, (ccr_content + pulse3_value));
 
    }

    /* TIM3_CH4 toggling with frequency = 4kHz. */
    if(HAL_TIM_ACTIVE_CHANNEL_4 == htim->Channel)
    {
        ccr_content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);
        __HAL_TIM_SetCompare(htim, TIM_CHANNEL_4, (ccr_content + pulse4_value));

    }
}
