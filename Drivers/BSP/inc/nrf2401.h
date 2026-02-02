#include "bsp_config.h"

#ifdef NRF2401_API_EN
#ifndef NRF2401_H
#define NRF2401_H
#include "stm32f1xx.h"                  // Device header
#include "stm32f1xx_hal.h"
#include "ISPI.h"
#include "GPIO_EXIT.h"
#include "AtomVariable.h"
/*
Pip1 for data receive
Pip0 for data Ack
*/
void NRF2401_Init(int CS_Line);

//Start nrf2401, transport into standby-I mode.
void NRF2401_Start();

//stop nrf2401, transport into shutdown mode.
void NRF2401_Stop();

//transport to Send mode
void NRF2401_Send_Mode();
void NRF2401_Send(unsigned char* datas, unsigned char size);

//transport to Receive mode
void NRF2401_Recv_Mode();
void NRF2401_Recv();

void NRF2401_IRQ_Handler();
#endif
#endif