#include "bsp_config.h"

#ifdef DELAY_COUNTER_API_EN
#ifndef DELAY_COUNTER_H
#define DELAY_COUNTER_H
#include "stm32f1xx.h"                  // Device header
#include "stm32f1xx_hal.h"
#include "AtomVariable.h"
//The step is 1us
/*
The counter at least is 7us 
*/

typedef void (*tim_clock_init_func)(void);
typedef void (*tim_up_handle)(void);
typedef struct {
	//user conf
	TIM_TypeDef*				 	tim_conf;
	uint32_t 							period;
	tim_clock_init_func   rcc_init_func;
	IRQn_Type             ir_handle;
	tim_up_handle					tim_handle_func;
	
	//sys conf
	AtomVarType           timer_lock;
	TIM_HandleTypeDef*    htim;
} DelayCounter_ConfigTypeDef;

#define DC_NULL 0
#define DC_PRESCALER 72
#define DC_ZERO 0
#define DC_READY 0
#define DC_BUZY 1
#define DC_BIAS 6

//init a counter
void DC_Init(DelayCounter_ConfigTypeDef* conf);
//start count delay
void DC_Delay(DelayCounter_ConfigTypeDef* conf);

//wait for ready
void DC_Wait_Sync(DelayCounter_ConfigTypeDef* conf);
//check if ready
//1 means not ready, 0 means ready
uint8_t DC_Wait(DelayCounter_ConfigTypeDef* conf);

//interrupt func
void DC_IRQ_Handle_Func(DelayCounter_ConfigTypeDef* conf);


#endif
#endif