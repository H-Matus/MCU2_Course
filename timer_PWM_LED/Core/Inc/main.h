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

/**
 * @brief 
 * 
 */
void GPIO_Init(void);

/**
 * @brief 
 * 
 */
void Error_handler(void);

/**
 * @brief 
 * 
 */
void TIMER2_Init(void);

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
