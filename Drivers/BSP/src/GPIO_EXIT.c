#include "GPIO_EXIT.h"

#ifdef GPIO_EXIT_API_EN
/*
EXTI info:
PA8 exti
*/
//The handle func type

void (*EXit_Handle_Func)(void);

void Input_Port_IT_Init(Handler_Func handle_func)
{
	//Get handle func
	EXit_Handle_Func = handle_func;
	//GPIO Init
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef port_config = {
		.Mode = GPIO_MODE_IT_FALLING,
		.Pin = GPIO_PIN_8,
		.Pull = GPIO_PULLUP,
		.Speed = GPIO_SPEED_FREQ_HIGH
	};
	HAL_GPIO_Init(GPIOA, &port_config);
	//IT Init
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);
	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

//exti IT handle
void EXTI9_5_IRQHandler()
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == GPIO_PIN_8) EXit_Handle_Func();
}

#endif