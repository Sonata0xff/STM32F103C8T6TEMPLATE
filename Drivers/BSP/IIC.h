#include "stm32f1xx.h"                  // Device header
#include "stm32f1xx_hal.h"
#include "bsp_config.h"

#ifdef IIC_API_EN
#ifndef IIC_H
#define IIC_H
#include "AtomVariable.h"
//IIC1 parameters:
//1.clkFreq : SCL frequency ,Hz. Default is 200000, means 200KHz
//2.selfAddr : self master address, default is 0.
void SetIIC_Comm_Mode(uint8_t mode);
HAL_StatusTypeDef IIC1_Init(uint32_t clkFreq, uint32_t selfAddr);
HAL_StatusTypeDef IIC1SendBytes(char* value, int size, uint16_t addr);
HAL_StatusTypeDef IIC1ReadSlaveReg(char* value, int size, uint16_t addr, uint16_t regAddr);
HAL_StatusTypeDef IIC1WriteSlaveReg(char* value, int size, uint16_t addr, uint16_t regAddr);
//block wait until IIC1 finish sending.
void IIC1_Send_Block_Wait();
#endif
#endif