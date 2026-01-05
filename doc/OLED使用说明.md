该文档主要用于说明OLED的API使用：
1.OLE屏幕的API使用了硬件IIC1的通信，其中stm32作为master与OLED进行交互。
2.下方样例代码说明一种基本使用。
3.如果需要使用OLED的API则需要确保bsp_config.h中定义宏IIC_API_EN与OLED_API_EN以此让相关模块代码参与编译，否则将会产生静态检查与编译报错。
4.OLED选用128x64大小，1306作为硬件驱动的一块显示屏。

样例代码：在屏幕中打印“hello,2026!”字符串
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