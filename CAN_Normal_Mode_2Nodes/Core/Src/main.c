/*
 * main.c
 *
 *  Created on: Jul 9, 2023
 *      Author: Admin
 */

#include <main.h>
#include <string.h>

UART_HandleTypeDef huart2;
CAN_HandleTypeDef hcan1;
TIM_HandleTypeDef htimer6;
CAN_RxHeaderTypeDef RxHeader;


int main(void)
{
    HAL_Init();
    SystemClock_Config_HSE(SYS_CLOCK_FREQ_50_MHZ);
    GPIO_Init();
    UART2_Init();
    CAN1_Init();

    CAN_Filter_Config();

    if( HAL_CAN_ActivateNotification(&hcan1, CAN_IT_TX_MAILBOX_EMPTY | CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_BUSOFF ) != HAL_OK )
    {
        Error_handler();
    }

    if( HAL_CAN_Start(&hcan1) != HAL_OK )
    {
        Error_handler();
    }

    CAN1_Tx();

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
	htimer6.Init.Period = 10000 - 1;
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

void CAN1_Init(void)
{
    hcan1.Instance = CAN1;
    hcan1.Init.mode = CAN_MODE_NORMAL;
    hcan1.Init.AutoBusOff = DISABLE;
    hcan1.Init.AutoRetransmission = ENABLE;
    hcan1.Init.AutoWakeup = DISABLE;
    hcan1.Init.ReceiveFifoLocked = DISABLE;
    hcan1.Init.TimeTriggeredMode = DISABLE;
    hcan1.Init.TransmitFifoPriority = DISABLE;

    // Settings related to CAN bit timings (bitrate = 500 kbps)
    hcan1.Init.Prescaler = 5;
    hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
    hcan1.Init.TimeSeg1 = CAN_BS1_8TQ;
    hcan1.Init.TimeSeg2 = CAN_BS2_1TQ;

    if( HAL_CAN_Init(&hcan1) != HAL_OK )
    {
        Error_handler();
    }
}

void CAN1_Tx(void)
{
    CAN_TxHeaderTypeDef TxHeader;
    uint32_t TxMailbox;

    uint8_t our_message[5] = {'H','E','L','L','O'};

    TxHeader.DLC = 5;
    TxHeader.StdId = 0x65D;
    TxHeader.IDE = CAN_ID_STD;
    TxHeader.RTR = CAN_RTR_DATA;

    if ( HAL_CAN_AddTxMessage(&hcan1, &TxHeader, our_message, &TxMailbox) != HAL_OK )
    {
        Error_handler();
    }
    
}

void CAN1_Rx(void)
{
}

void CAN_Filter_Config(void)
{
    CAN_FilterTypeDef can1_filter_config;

    can1_filter_config.FilterActivation = ENABLE;
    can1_filter_config.FilterBank = 0;
    can1_filter_config.FilterFIFOAssignment = CAN_RX_FIFO0;
    can1_filter_config.FilterIdHigh = 0;
    can1_filter_config.FilterIdLow = 0;
    can1_filter_config.FilterMaskIdHigh = 0;
    can1_filter_config.FilterMaskIdLow = 0;
    can1_filter_config.FilterMode = CAN_FILTERMODE_IDMASK;
    can1_filter_config.FilterScale = CAN_FILTERSCALE_32BIT;

    if( HAL_CAN_ConfigFilter(&hcan1, &can1_filter_config) != HAL_OK )
    {
        Error_handler();
    }
}

void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
    char msg[50];
    sprintf(msg, "Message transmitted: M0 \r\n");
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
}

void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan)
{
    char msg[50];
    sprintf(msg, "Message transmitted: M1 \r\n");
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
}

void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan)
{
    char msg[50];
    sprintf(msg, "Message transmitted: M2 \r\n");
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    uint8_t rcvd_msg[5];
    char msg[50];

    if( HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader, &rcvd_msg) != HAL_OK )
    {
        Error_handler();
    }

    if ( RxHeader.StdId == 0x65D && RxHeader.RTR == 0 )
    {
    	// This is data frame sent from n1 to n2
        LED_Manage_Output(rcvd_msg[0]);
        sprintf(msg, "Message transmitted: #%x\r\n", rcvd_msg[0]);
    }
    else if ( RxHeader.StdId == 0x651 && RxHeader.RTR == 1 )
    {
        // This is a remote frame sent by n1 to n2
        Send_response(RxHeader.StdId);
        return;
    }
    else if ( RxHeader.StdId == 0x651 && RxHeader.RTR == 0 )
    {
        // It's a reply ( data frame ) by n2 to n1
        sprintf(msg, "Reply Received : #%X\r\n", rcvd_msg[0] << 8 | rcvd_msg[1]);
    }

    sprintf(msg, "Message Received: %s\r\n", rcvd_msg);
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
}

void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
    char msg[50];
    sprintf(msg, "Error happened. \r\n");
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
}

void Send_response(uint32_t StdId)
{
    CAN_TxHeaderTypeDef TxHeader;

    uint32_t TxMailbox;

    uint8_t response[2] = {0xAB, 0xCD};

    TxHeader.DLC = 2;
    TxHeader.StdId = StdId;
    TxHeader.IDE = CAN_ID_STD;
    TxHeader.RTR = CAN_RTR_DATA;

    if ( HAL_CAN_AddTxMessage( &hcan1, &TxHeader, response, &TxMailbox ) != HAL_OK )
    {
        Error_handler();
    }

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

