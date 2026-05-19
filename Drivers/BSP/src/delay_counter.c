#include "delay_counter.h"
#ifdef DELAY_COUNTER_API_EN

void DC_Init(DelayCounter_ConfigTypeDef* conf)
{
	if (conf == DC_NULL) return;
	//rcc init
	conf->rcc_init_func();
	//tim init
	conf->htim->Instance = conf->tim_conf;
	conf->htim->Init.Prescaler = DC_PRESCALER - 1; // 1Mhz
	conf->htim->Init.CounterMode = TIM_COUNTERMODE_UP;
	conf->htim->Init.Period = DC_ZERO;
	conf->htim->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	conf->htim->Init.RepetitionCounter = DC_ZERO;
	conf->htim->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(conf->htim);
	//nvic init
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);
	HAL_NVIC_SetPriority(conf->ir_handle, 1, 1);
	HAL_NVIC_EnableIRQ(conf->ir_handle);
	//var init
	Atom_Write(&conf->timer_lock, ATOM_VALUE_RESET);
}

void DC_Delay(DelayCounter_ConfigTypeDef* conf)
{
	if (conf == DC_NULL ||
			DC_Wait(conf) == DC_BUZY) return;
	conf->htim->Instance->SR = DC_ZERO;
	conf->htim->Instance->CNT = DC_ZERO;
	conf->tim_conf->ARR = conf->period - DC_BIAS;
	Atom_Write(&conf->timer_lock, ATOM_VALUE_SET);
	HAL_TIM_Base_Start_IT(conf->htim);
}

//wait for ready
void DC_Wait_Sync(DelayCounter_ConfigTypeDef* conf)
{
	__DSB();
	while (DC_Wait(conf) == DC_BUZY);
	return;
}
//check if ready
//1 means not ready, 0 means ready
uint8_t DC_Wait(DelayCounter_ConfigTypeDef* conf)
{
	__DSB();
	if (conf == DC_NULL) return DC_BUZY;
	if (Atom_Read(&conf->timer_lock) == ATOM_VALUE_RESET)
		return DC_READY;
	else
		return DC_BUZY;
}

//interrupt funcs
void DC_IRQ_Handle_Func(DelayCounter_ConfigTypeDef* conf)
{
	conf->tim_handle_func();
	Atom_Write(&conf->timer_lock, ATOM_VALUE_RESET);
	HAL_TIM_IRQHandler(conf->htim);
}
#endif