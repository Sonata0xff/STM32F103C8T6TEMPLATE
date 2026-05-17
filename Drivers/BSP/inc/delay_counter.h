#include "bsp_config.h"

#ifdef DELAY_COUNTER_API_EN
#ifndef DELAY_COUNTER_H
#define DELAY_COUNTER_H
#include "stm32f1xx.h"                  // Device header
#include "stm32f1xx_hal.h"
#include "AtomVariable.h"
//The step is 1us
/*
The left tasks:
(1) nscp sender need to be retested.
(2) delay counter desgin
*/
typedef void (*tim_clock_init_func)(void);
typedef struct {
	TIM_TypeDef*				 	tim_conf;
	uint32_t 							period;
	tim_clock_init_func   rcc_init_func;
	IRQn_Type             ir_handle;
	
	TIM_HandleTypeDef*    htim;
} DelayCounter_ConfigTypeDef;

#define DC_NULL 0
#define DC_PRESCALER 72
#define DC_ZERO 0

//init a counter
void DC_Init(DelayCounter_ConfigTypeDef* conf);
//start count delay
void DC_Delay(DelayCounter_ConfigTypeDef* conf);


#endif
#endif