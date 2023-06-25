/*
 * msp.c
 *
 *  Created on: Jun 25, 2023
 *      Author: Admin
 */

#include "msp.h"

void HAL_MspInit(void)
{
    /* Low level processor specific inits.*/
    /* 1. Set up the priority grouping of the ARM Cortex MX Processor. */
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

    /* 2. Enable required system exceptions of the ARM Cortex MX Processor. */
    SCB->SHCSR |= 0x7 << 16;

    /* 3. Configure the priority for system exceptions. */
    HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
    HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
    HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
}
