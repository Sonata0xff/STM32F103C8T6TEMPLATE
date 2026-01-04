#include "OLED.h"
#include "IIC.h"
uint16_t OLED_addr = 0x0078;
unsigned char ScreenOn = 0; // 0 means off

void OLED_Init() // mainly  focus on screen setting
{
	int startCommandsSize = 14;
	char startCommands[] = {
		0x00,//send controll byte
		0xae,// turn off screeen
		0x20,//set DDR addressing mode
		0x02,//set to page mode
		0x81,//set Contrast
		0xff,//set contrast to 0xff
		0xda,//set cross row output
		0x12,//set sequential output
		0xb0,//set start page order
		0x00,//set low 4bits of start col
		0x10,//set high 4bits of start col
		0x8d,//standard process 1 before start
		0x14,//standard process 2 before start
		0xaf
	};
	IIC1SendBytes(startCommands, startCommandsSize, OLED_addr);
	while(1);
	return;
}
void OLED_TurnOn_Screen()
{
	if (ScreenOn == 1) return;
	int commandsSize = 2;
	char commands[] = {
		0x00,//send controll byte
		0xaf// turn on command
	};
	HAL_StatusTypeDef ret = IIC1SendBytes(commands, commandsSize, OLED_addr);
	if (ret == HAL_OK) ScreenOn = 1;
}
void OLED_TurnOff_Screen()
{
	if (ScreenOn == 0) return;
	int commandsSize = 2;
	char commands[] = {
		0x00,//send controll byte
		0xae// turn off command
	};
	HAL_StatusTypeDef ret = IIC1SendBytes(commands, commandsSize, OLED_addr);
	if (ret == HAL_OK) ScreenOn = 0;
}
void OLED_Flash_Screen(unsigned char val)// flash the screen with value 'val'
{
	if (ScreenOn == 0) return;
	for (unsigned char i = 0xb0; i <= 0xb7; ++i) {
		int commands1Size = 4;
		char commands1[] = {
			0x00,
			i,
			0x00,
			0x10,
		};
		IIC1SendBytes(commands1, commands1Size, OLED_addr);
		int command2Size = 129;
		char commmands2[command2Size];
		commmands2[0] = 0x40;
		for (int j = 0; j < 128; ++j) commmands2[j+1] = val;
		IIC1SendBytes(commmands2, command2Size, OLED_addr);
	}
}
void OLED_Write1Byte(unsigned char v128, unsigned char v64, unsigned char val)// write a colunm
{
	
}
void OLED_WriteIn_16x8Char(unsigned char v128, unsigned char v8, unsigned char val[])// write a 16x8 size char
{
	
}
void OLED_WriteIn_16x8String(unsigned char v128, unsigned char v8, int len, unsigned char val[]) // write a string , each char's size is 16x8
{
	
}