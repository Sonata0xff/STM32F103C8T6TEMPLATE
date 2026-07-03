#include "test.h"
#ifdef TEST_H
//----------------------------------------------------
#ifdef OLED_TEST
void OLED_Test()
{
	//datas
	char title[] = "hello,2026!";
	int size = 11;
	
	//init OLED
	IIC1_Init(0, 0);
	SetIIC_Comm_Mode(1);//Polling mode
	OLED_Init();
	OLED_TurnOn_Screen();
	OLED_Flash_Screen(0x00);
	
	//start test
	OLED_WriteIn_16x8String(0, 0, size, (unsigned char *)title);
	
	//stuck
	while(1);
}
#endif
//----------------------------------------------------
#endif