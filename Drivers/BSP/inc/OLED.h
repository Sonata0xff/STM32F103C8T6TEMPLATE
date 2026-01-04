#include "stm32f1xx.h"                  // Device header
#include "stm32f1xx_hal.h"

#ifndef OLED_H
#define OLED_H
//v128 is column, range is (0~15). v8 is row, range is (0~3)
void OLED_Init(); // mainly  focus on screen setting
void OLED_TurnOn_Screen();
void OLED_TurnOff_Screen();
void OLED_Flash_Screen(unsigned char val);// flash the screen with value 'val'
void OLED_Write1Byte(unsigned char v128, unsigned char v8, unsigned char val);// write a colunm
void OLED_WriteIn_16x8Char(unsigned char v128, unsigned char v8, unsigned char val);// write a 16x8 size char
void OLED_WriteIn_16x8String(unsigned char v128, unsigned char v8, int len, unsigned char val[]); // write a string , each char's size is 16x8
#endif