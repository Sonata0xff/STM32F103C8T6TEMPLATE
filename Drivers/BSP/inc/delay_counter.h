#include "bsp_config.h"

#ifdef DELAY_COUNTER_API_EN
#ifndef DELAY_COUNTER_H
#define DELAY_COUNTER_H
#include "stm32f1xx.h"                  // Device header
#include "stm32f1xx_hal.h"

void timer_Init();


#endif
#endif