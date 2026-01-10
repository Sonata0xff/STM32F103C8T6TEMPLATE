#include "bsp_config.h"
#ifdef ISPI_API_EN
#ifndef ISPI_H
#define ISPI_H

#include "stm32f1xx.h"                  // Device header
#include "stm32f1xx_hal.h"

void ISPI1_Init();

HAL_StatusTypeDef ISPI1_SendBytes(unsigned char* val, int size);

HAL_StatusTypeDef ISPI1_SenRecBytes(unsigned char* sendPacks, unsigned char* recvPacks, int size);

HAL_StatusTypeDef ISPI1_RecvBytes(unsigned char* recvPacks, int size);


void ISPI1_NSS_Init(GPIO_TypeDef* gpioG, uint32_t* Pin, unsigned char size);

void ISPI1_SelectDevice(int pos);

void ISPI1_UnSelectDevice(int pos);

char ISPI1_GetDeviceStatus(int pos);

void ISPI_Comm_Block_Wait();
#endif
#endif