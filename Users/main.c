#include "stm32f1xx.h"                  // Device header
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_conf.h"
#include "IIC.h"
#include "OLED.h"
#include "ISPI.h"
#include "nrf2401.h"
#include "utils.h"
#include "GPIO_EXIT.h"

int testVal = 0;

void TestFunc()
{
	if (testVal >= 9) testVal = 0;
	else testVal++;
}

int main()
{
	//stand process
	Standard_process();
	
	//data
	uint32_t pin_group[1] = {GPIO_PIN_4};
	unsigned char res[2] = {0x00, 0xff};
	char tmp[4] = {'#', '#', '#', '#'};
	
	//SPI init
	ISPI1_Init();
	ISPI1_NSS_Init(GPIOA, pin_group, 1);
	
	//IIC Init
	IIC1_Init(0, 0);
	
	//OLED init
	OLED_Init();
	OLED_TurnOn_Screen();
	OLED_Flash_Screen(0x00);
	
	//GPIO_INPUT init
	Input_Port_IT_Init(TestFunc);
	
	//test code
	NRF2401_Init(0);
	NRF2401_Start(res);
	
	
	for (int i = 0; i < 2; i++) {
		TransNum2String(res[i], tmp);
		OLED_WriteIn_16x8String(i*4, 0, 4, (unsigned char*)tmp);
	}
	while(1);
	return 0;
}