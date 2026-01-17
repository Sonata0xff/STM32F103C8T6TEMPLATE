#include "bsp_config.h"

#ifdef NRF2401_API_EN
#include "nrf2401.h"
static int CSN_ORDER = 0;

void NRF2401_Init(int CS_Line)
{
	//init IRQ source
	
	//init CE source
	GPIO_InitTypeDef ce_config = {
		.Mode = GPIO_MODE_OUTPUT_PP,
		.Pin = GPIO_PIN_2,
		.Pull = GPIO_NOPULL,
		.Speed = GPIO_SPEED_FREQ_MEDIUM
	};
	HAL_GPIO_Init(GPIOA, &ce_config);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
	//init CSN
	CSN_ORDER = CS_Line;
}

void NRF2401_Start(unsigned char* res)
{
	//transform Shutdown to standby
	/*
	1.CSN pull down
	2.trans to standby
	*/
	unsigned char val[] = {0x00, 0x00}; // read reg 0x00 val
	int size = 2;
	ISPI1_SelectDevice(CSN_ORDER);
	//ISPI1_SendBytes(val, 2);
	ISPI1_SenRecBytes(val, res, size);
	ISPI_Comm_Block_Wait();
	
}

#endif