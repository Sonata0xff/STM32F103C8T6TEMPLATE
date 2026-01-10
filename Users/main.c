#include "stm32f1xx.h"                  // Device header
#include "stm32f1xx_hal.h"
#include "IIC.h"
#include "OLED.h"
#include "ISPI.h"
#include "nrf2401.h"
int main()
{
	//data
	unsigned char val[] = {0xaa};
	int size = 1;
	//stand process
	HAL_Init();
	
	//IIC Init
	IIC1_Init(0, 0);
	
	//SPI init
	ISPI1_Init();
	
	//oled init
	OLED_Init();
	
	//send 1 byte
	ISPI1_SendBytes(val, size);
	
	//oled open
	unsigned char mk[] = "helooasdasdasdasdsd";
	OLED_TurnOn_Screen();
	OLED_Flash_Screen(0x00);
	OLED_WriteIn_16x8String(0,0,19, mk);
	
	
	while(1);
	return 0;
}