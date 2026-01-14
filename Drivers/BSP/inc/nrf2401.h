#include "bsp_config.h"

#ifdef NRF2401_API_EN
#include "stm32f1xx.h"                  // Device header
#include "stm32f1xx_hal.h"
#include "ISPI.h"

#ifndef NRF2401_H
#define NRF2401_H

void NRF2401_Init(int CS_Line);

void NRF2401_Start(unsigned char* res);

#endif
#endif