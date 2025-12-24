#include "stm32f1xx.h"                  // Device header
#include "stm32f1xx_hal.h"
#include "nrf2401.h"
#include "IIC.h"
int main()
{
	//standard process
	HAL_Init();
	//standard process end
	
	//test code
	testFunc();
	//test code end
	HAL_StatusTypeDef ret = IIC_Init();
	if (ret == HAL_OK) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
	} else {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
	}
	while(1);
	return 0;
}