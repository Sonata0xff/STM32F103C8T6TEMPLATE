#include "IIC.h"

static const uint32_t clk_freq = 200000; //200kHz
static const uint32_t masterAddr = 0; // OwnAddress


//basic iic info
I2C_HandleTypeDef iic_config = {
		.Instance = I2C1,
		.Init.ClockSpeed = clk_freq,
		.Init.DutyCycle = I2C_DUTYCYCLE_2,
		.Init.OwnAddress1 = masterAddr,
		.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT,
		.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE,
		.Init.OwnAddress2 = masterAddr,
		.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE,
		.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE,
		.State = HAL_I2C_STATE_RESET
};

void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
	//CLOCK int
	__HAL_RCC_GPIOB_CLK_ENABLE();
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
}

// it func
void I2C1_EV_IRQHandler()
{
	HAL_I2C_EV_IRQHandler(&iic_config);
}
void I2C1_ER_IRQHandler()
{
	HAL_I2C_ER_IRQHandler(&iic_config);
}

HAL_StatusTypeDef IIC_Init()
{
	//CLOCK int
	__HAL_RCC_I2C1_CLK_ENABLE();
	//I2C init
	return HAL_I2C_Init(&iic_config);
}
