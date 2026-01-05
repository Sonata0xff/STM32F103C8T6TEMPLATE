#include "stm32f1xx.h"                  // Device header
#include "stm32f1xx_hal.h"
#include "nrf2401.h"
#include "IIC.h"
#include "OLED.h"
int main()
{
	//data
	unsigned char title[] = "hello,2026!";
	int size = 11;
	//stand process
	HAL_Init();
	
	//start IIC master sneder
	IIC1_Init(0, 0);
	
	OLED_Init();
	
	OLED_TurnOn_Screen();
	
	OLED_Flash_Screen(0x00);
	
	OLED_WriteIn_16x8String(3, 1, size, title);
	
	while(1);
	return 0;
}