#include "delay_counter.h"
#ifdef DELAY_COUNTER_API_EN
//coding ...

TIM_HandleTypeDef htim = {
		.Instance = TIM4,
		.Init.Prescaler = 72 - 1, // 1Mhz
		.Init.CounterMode = TIM_COUNTERMODE_UP,
		.Init.Period = 2 - 1,//2us
		.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1,
		.Init.RepetitionCounter = 0,
		.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE
};

void timer_Init()
{
	HAL_TIM_Base_Init(&htim);
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);
	HAL_TIM_Base_Start_IT(&htim);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	__HAL_RCC_TIM4_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef gpio_conf = {
		.Pin = GPIO_PIN_6,
		.Mode = GPIO_MODE_OUTPUT_PP,
		.Pull = GPIO_NOPULL,
		.Speed = GPIO_SPEED_FREQ_HIGH
	};
	HAL_GPIO_Init(GPIOA, &gpio_conf);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);
	HAL_NVIC_SetPriority(TIM4_IRQn, 1, 1);
	HAL_NVIC_EnableIRQ(TIM4_IRQn);
}

void TIM4_IRQHandler()
{
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);
	HAL_TIM_IRQHandler(&htim);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	HAL_TIM_Base_Stop_IT(htim);
}

#endif