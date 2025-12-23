#include "nrf2401.h"

void testFunc()
{
	GPIO_InitTypeDef gpio = {
		.Pin = GPIO_PIN_6,
		.Mode = GPIO_MODE_OUTPUT_PP,
		.Pull = GPIO_NOPULL,
		.Speed = GPIO_SPEED_FREQ_MEDIUM
	};
	__HAL_RCC_GPIOA_CLK_ENABLE();
	HAL_GPIO_Init(GPIOA, &gpio);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
	while(1) {
		HAL_Delay(500);
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);
	}
}