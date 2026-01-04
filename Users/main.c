#include "stm32f1xx.h"                  // Device header
#include "stm32f1xx_hal.h"
#include "nrf2401.h"
#include "IIC.h"
#include "OLED.h"
int main()
{
	//stand process
	HAL_Init();
	
	//start IIC master sneder
	IIC1_Init(0, 0);
	
	
	OLED_Init();
	
	
	OLED_TurnOn_Screen();
	
	HAL_Delay(1000);
	
	OLED_Flash_Screen(0x00);
	
	while(1);
	return 0;
}