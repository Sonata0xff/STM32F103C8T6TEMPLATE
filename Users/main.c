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
	HAL_StatusTypeDef ret = IIC1_Init(0, 0);
	if (ret == HAL_OK) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
	} else {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
	}
	char a[] = {0x00, 0xae, 0x20, 0x02, 0x81, 0xff, 0xda, 0x12, 0xb0, 0x00, 0x10, 0x8d, 0x14, 0xaf};
	int a_size = 14;
	uint16_t addr = 0x0078;
	ret = IIC1Send1Byte(a, a_size, addr);
	
	if (ret == HAL_OK) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
	} else {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
	}
	while(1);
	return 0;
}