#include "IIC.h"
static const uint32_t clk_freq = 300000; //300kHz
static const uint32_t masterAddr = 0; // OwnAddress

void IIC_Init()
{
	//CLOCK int
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_I2C1_CLK_ENABLE();
	//GPIO init
	__HAL_AFIO_REMAP_I2C1_ENABLE();
	GPIO_InitTypeDef sclConfig = {
		.Mode = GPIO_MODE_AF_OD,
		.Pin = GPIO_PIN_6,
		.Pull = GPIO_NOPULL,
		.Speed = GPIO_SPEED_FREQ_MEDIUM
	};
	GPIO_InitTypeDef sdaConfig = {
		.Mode = GPIO_MODE_AF_OD,
		.Pin = GPIO_PIN_7,
		.Pull = GPIO_NOPULL,
		.Speed = GPIO_SPEED_FREQ_MEDIUM
	};
	HAL_GPIO_Init(GPIOB, &sclConfig);
	HAL_GPIO_Init(GPIOB, &sdaConfig);
	//NVIC init
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);
	HAL_NVIC_SetPriority(I2C1_EV_IRQn, 0, 0);
	HAL_NVIC_SetPriority(I2C1_ER_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
	HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
	//I2C init
	//coding...
}
