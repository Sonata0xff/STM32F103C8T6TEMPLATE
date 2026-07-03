#include "utils.h"
#include "stm32f1xx.h"                  // Device header
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_conf.h"
int main()
{
	Standard_process();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef io_conf = {
		.Mode = GPIO_MODE_OUTPUT_PP,
		.Pin = GPIO_PIN_1,
		.Pull = GPIO_NOPULL,
		.Speed = GPIO_SPEED_FREQ_HIGH
	};
	HAL_GPIO_Init(GPIOA, &io_conf);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
	while(1) {
		HAL_Delay(500);
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
	};
	return 0;
}