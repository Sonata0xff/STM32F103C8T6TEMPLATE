#include "stm32f1xx.h"                  // Device header
#include "stm32f1xx_hal.h"


#ifndef IIC_H
#define IIC_H
//IIC1 parameters:
//1.clkFreq : SCL frequency ,Hz. Default is 200000, means 200KHz
//2.selfAddr : self master address, default is 0. 
HAL_StatusTypeDef IIC1_Init(uint32_t clkFreq, uint32_t selfAddr);
HAL_StatusTypeDef IIC1Send1Byte(char* value, int size, uint16_t addr);
#endif