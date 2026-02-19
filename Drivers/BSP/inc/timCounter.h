#include "bsp_config.h"

#ifdef TIM_COUNTER_API_EN
#ifndef TIM_COUNTER_H
#define TIM_COUNTER_H
#include "stm32f1xx.h"                  // Device header
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_conf.h"
#define NULL 0
/* CK_INT  = 72Mhz
suppose that the step is 10us.
The period is n x 10 us.
n10us is the gap or period
CounterHandleFunc is the handle func
10us = 0.1Mhz, prescalar = 720.
*/
typedef void (* CounterHandleFunc)(void);
void Tim2_Counter_Init(uint32_t n10us, CounterHandleFunc func);
void Tim2_Counter_Start();
void Tim2_Counter_Stop();


#endif
#endif