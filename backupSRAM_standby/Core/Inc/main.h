/*
 * main.h
 *
 *  Created on: Aug 13, 2023
 *      Author: Admin
 */

#ifndef INC_MAIN_H_
#define INC_MAIN_H_

#include "stm32f4xx_hal.h"

#define SYS_CLOCK_FREQ_50_MHZ 	50
#define SYS_CLOCK_FREQ_84_MHZ   84
#define SYS_CLOCK_FREQ_120_MHZ  120

#define LED1_PORT GPIOC
#define LED2_PORT GPIOC
#define LED3_PORT GPIOB
#define LED4_PORT GPIOC

#define LED1_PIN_NO GPIO_PIN_9
#define LED2_PIN_NO GPIO_PIN_8
#define LED3_PIN_NO GPIO_PIN_8
#define LED4_PIN_NO GPIO_PIN_6

/**
 * @brief 
 * 
 */
void GPIO_Init(void);

/**
 * @brief
 */
void GPIO_AnalogConfig(void);

/**
 * @brief 
 * 
 */
void Error_handler(void);

/**
 * @brief 
 * 
 */
void UART2_Init(void);

/**
 * @brief 
 * 
 * @param clock_freq 
 */
void SystemClock_Config_HSE(uint8_t clock_freq);

#endif /* INC_MAIN_H_ */
