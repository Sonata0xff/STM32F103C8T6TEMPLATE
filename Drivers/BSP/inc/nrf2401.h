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

enum CommStatus {
	CommStatus_Dummy = 0,
	CommStatus_Shutdown,
	CommStatus_Standby,
	CommStatus_Send,
	CommStatus_Receive
};

void NRF2401_Init(int CS_Line);

//Start nrf2401, transport into standby-I mode.
void NRF2401_Start();

//stop nrf2401, transport into shutdown mode.
void NRF2401_Stop();

//revert send / receive into standby-I
void NRF2401_Revert_Standby();

//transport to Send mode
void NRF2401_Send_Mode();
void NRF2401_Send(unsigned char* datas);
void NRF2401_Send_Block_Wait();
//return 1 means success
uint8_t NRF2401_Send_Wait();
void Send_Time_Out_Handle();
void Send_Fin_Handle();

//transport to Receive mode
void NRF2401_Recv_Mode();
void NRF2401_Recv_Block_Wait(unsigned char* datas);
//return 1 means success
uint8_t NRF2401_Recv_Wait(unsigned char* datas);
void NRF2401_Recv_Handle();

//print reg value
void NRF2401_Get_Reg(unsigned char addr, unsigned char *res);

void NRF2401_IRQ_Handler();
#endif
#endif