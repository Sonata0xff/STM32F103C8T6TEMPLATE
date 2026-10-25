#include "stm32f1xx.h"                  // Device header
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_conf.h"
#include "utils.h"
#include "test.h"
#include "OLED.h"
#include "IIC.h"

int main()
{
	//stand process
	Standard_process();

	//work code
	ESC_Test_Init();
	/*__HAL_RCC_GPIOC_CLK_ENABLE();
	GPIO_InitTypeDef led_conf = {
		.Pin = GPIO_PIN_13,
		.Mode = GPIO_MODE_OUTPUT_PP,
		.Pull = GPIO_NOPULL,
		.Speed = GPIO_SPEED_FREQ_HIGH
	};
	HAL_GPIO_Init(GPIOC, &led_conf);
	while (1) {
		HAL_Delay(500);
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
	}*/
	
	return 0;
}