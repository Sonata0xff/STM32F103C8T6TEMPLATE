#include "IIC.h"

#ifdef IIC_API_EN
//I2C1 is sneder, I2C2 is receiver.

static const uint32_t clk_freq = 200000; //default clock speed 200kHz
static const uint32_t self_addr = 0; //default self address SelfAddress
static uint8_t I2C1_SEND_FIN = 0;// not 0 means send finished.


//basic iic info
I2C_HandleTypeDef iic1_config = {
		.Instance = I2C1,
		.Init.ClockSpeed = clk_freq,
		.Init.DutyCycle = I2C_DUTYCYCLE_2,
		.Init.OwnAddress1 = self_addr,
		.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT,
		.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE,
		.Init.OwnAddress2 = self_addr,
		.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE,
		.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE,
		.State = HAL_I2C_STATE_RESET,
		.Mode = HAL_I2C_MODE_MASTER
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
	HAL_I2C_EV_IRQHandler(&iic1_config);
}
void I2C1_ER_IRQHandler()
{
	HAL_I2C_ER_IRQHandler(&iic1_config);
}

HAL_StatusTypeDef IIC1_Init(uint32_t clkFreq, uint32_t selfAddr)
{
	//CLOCK int
	__HAL_RCC_I2C1_CLK_ENABLE();
	//I2C init
	iic1_config.Init.ClockSpeed = clkFreq == 0 ? clk_freq : clkFreq;
	iic1_config.Init.OwnAddress1 = selfAddr == 0 ? self_addr : selfAddr;
	return HAL_I2C_Init(&iic1_config);
}

HAL_StatusTypeDef IIC1SendBytes(char* value, int size, uint16_t addr)
{
	return HAL_I2C_Master_Transmit_IT(&iic1_config, addr, (unsigned char*)value, size);
}
//This will only be activated by I2C1 master send finish.
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	I2C1_SEND_FIN = 1;
}

void IIC1_Send_Block_Wait()
{
	while(I2C1_SEND_FIN == 0);
	if (I2C1_SEND_FIN != 0) I2C1_SEND_FIN = 0;
}

#endif
