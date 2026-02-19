#include "timCounter.h"

#ifdef TIM_COUNTER_API_EN
#define DEFAULT_PRE 720 //72Mhz / 720 = 0.1Mhz = 100Khz, means period step = 10us
#define DEFAULT_PERIOD 100 //100 x 10us = 1ms
#define DEFAULT_CHANNEL_ZERO 0 //unuse var

//task ptr
CounterHandleFunc tim2Func = NULL;

//tim2 info
TIM_HandleTypeDef tim2_info = {
	.Instance = TIM2,
	.Init.CounterMode = TIM_COUNTERMODE_UP,
	.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1,
	.Init.Period = DEFAULT_PERIOD,
	.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE,
	.Init.Prescaler = DEFAULT_PRE,
	.State = HAL_TIM_STATE_RESET
};

void Tim2_Counter_Init(uint32_t n10us, CounterHandleFunc func)
{
	//handle func init
	tim2Func = func;
	
	//TIM2 Init
	__HAL_RCC_TIM2_CLK_ENABLE();
	if (n10us == 0 || n10us > 0xffff) n10us = DEFAULT_PERIOD;
	tim2_info.Init.Period = n10us;
	HAL_TIM_Base_Init(&tim2_info);
	tim2_info.Instance->SR = 0;
	tim2_info.Instance->CNT = 0;
	
}
void Tim2_Counter_Start()
{
	HAL_TIM_Base_Start_IT(&tim2_info);
}
void Tim2_Counter_Stop()
{
	HAL_TIM_Base_Stop_IT(&tim2_info);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	//NVIC Init
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);
	HAL_NVIC_SetPriority(TIM2_IRQn, 3, 3);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

//IT handle
void TIM2_IRQHandler()
{
	//IT handle
	tim2_info.Instance->CR1 &= ~TIM_CR1_CEN;
	HAL_TIM_IRQHandler(&tim2_info);
	tim2Func();
	tim2_info.Instance->CR1 |= TIM_CR1_CEN;
}

#endif