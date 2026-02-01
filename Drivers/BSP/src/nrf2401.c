#include "bsp_config.h"

#ifdef NRF2401_API_EN
#include "nrf2401.h"
#include "OLED.h"
#include "utils.h"
static int CSN_ORDER = 0;

void NRF2401_Init(int CS_Line)
{
	//init IRQ source
	//Input_Port_IT_Init(NRF2401_IRQ_Handler);
	//init CE source
	GPIO_InitTypeDef ce_config = {
		.Mode = GPIO_MODE_OUTPUT_PP,
		.Pin = GPIO_PIN_2,
		.Pull = GPIO_NOPULL,
		.Speed = GPIO_SPEED_FREQ_HIGH
	};
	HAL_GPIO_Init(GPIOA, &ce_config);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
	//init CSN
	CSN_ORDER = CS_Line;
	HAL_Delay(100);
}

void NRF2401_Start()
{
	//data store variable
	unsigned char orders[] = {0x00, 0x00};
	unsigned char recData[] = {0x00, 0x00};
	
	//reset STATUS, first time STATUS may be wrong(FUCK YOU SI24R1!!!)
	ISPI1_SelectDevice(CSN_ORDER);
	ISPI1_SenRecBytes(orders, recData, 2);
	ISPI_Comm_Block_Wait();
	ISPI1_UnSelectDevice(CSN_ORDER);
	orders[0] = 0x27;
	orders[1] = recData[0];
	ISPI1_SelectDevice(CSN_ORDER);
	ISPI1_SendBytes(orders, 2);
	ISPI_Comm_Block_Wait();
	ISPI1_UnSelectDevice(CSN_ORDER);
	orders[0] = 0x00;
	orders[1] = 0x00;
	
	
	
	//read reg0
	ISPI1_SelectDevice(CSN_ORDER);
	ISPI1_SenRecBytes(orders, recData, 2);
	ISPI_Comm_Block_Wait();
	ISPI1_UnSelectDevice(CSN_ORDER);
	
	//Set PWR_UP to 1, transport into standby-I
	orders[0] = 0x20;
	orders[1] = (recData[1] | 0x02);
	ISPI1_SelectDevice(CSN_ORDER);
	ISPI1_SendBytes(orders, 2);
	ISPI_Comm_Block_Wait();
	ISPI1_UnSelectDevice(CSN_ORDER);
	
	HAL_Delay(2);
	
	//test func
	/*orders[0] = 0x00;
	orders[1] = 0x00;
	ISPI1_SelectDevice(CSN_ORDER);
	ISPI1_SenRecBytes(orders, recData, 2);
	ISPI_Comm_Block_Wait();
	ISPI1_UnSelectDevice(CSN_ORDER);
	char tmp[4] = {'#', '#', '#', '#'};
	for (int i = 0; i < 2; i++) {
		TransNum2String(recData[i], tmp);
		OLED_WriteIn_16x8String(i*4, 0, 4, (unsigned char*)tmp);
	}*/
}

void NRF2401_Stop()
{
	//Pull down CE
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
	
	//order variable
	unsigned char orders[] = {0x00, 0x00};
	unsigned char recData[] = {0x00, 0x00};

	//read reg0
	ISPI1_SelectDevice(CSN_ORDER);
	ISPI1_SenRecBytes(orders, recData, 2);
	ISPI_Comm_Block_Wait();
	ISPI1_UnSelectDevice(CSN_ORDER);
	
	//Set PWR_UP to 0, transport into shutdown mode.
	orders[0] = 0x20;
	orders[1] = (recData[1] & (~0x02));
	ISPI1_SelectDevice(CSN_ORDER);
	ISPI1_SendBytes(orders, 2);
	ISPI_Comm_Block_Wait();
	ISPI1_UnSelectDevice(CSN_ORDER);
	
	//test func
	/*orders[0] = 0x00;
	orders[1] = 0x00;
	ISPI1_SelectDevice(CSN_ORDER);
	ISPI1_SenRecBytes(orders, recData, 2);
	ISPI_Comm_Block_Wait();
	ISPI1_UnSelectDevice(CSN_ORDER);
	char tmp[4] = {'#', '#', '#', '#'};
	for (int i = 0; i < 2; i++) {
		TransNum2String(recData[i], tmp);
		OLED_WriteIn_16x8String(i*4, 0, 4, (unsigned char*)tmp);
	}*/
}

void NRF2401_IRQ_Handler()
{
	//wait for coding ...
}

#endif