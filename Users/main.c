#include "stm32f1xx.h"                  // Device header
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_conf.h"
#include "utils.h"
#include "test.h"
#include "OLED.h"
#include "IIC.h"
int main()
{
	//stand process
	Standard_process();
	
	IIC1_Init(0, 0);
	OLED_Init();
	
	OLED_TurnOn_Screen();
	
	OLED_Flash_Screen(0x00);
	
	//work
	NSCP_Test();
	
	return 0;
}