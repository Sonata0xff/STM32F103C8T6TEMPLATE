#include "NSCP.h"
#include "OLED.h"
#ifdef NCP_API_EN

void NSCP_Sender_Init(NSCP_ConfigTypeDef* comm_conf)
{
	unsigned char title[] = "NSCP Call test";
	int size = 14;
	OLED_WriteIn_16x8String(0, 0, size, title);
}

//NSCP data block init
void NSCP_Sender_Config_Init(NSCP_ConfigTypeDef* comm_conf)
{
	
}

//NSCP sender load in datas. 
void NSCP_Sender_Load_Data(NSCP_ConfigTypeDef* comm_conf)
{
	
}

//NSCP transfer the data into dma format
void NSCP_Sender_Transfer_Data_to_Dma(NSCP_ConfigTypeDef* comm_conf)
{
	
}

//NSCP check the transport is clear, which means the data can be sent.
//NSCP_TRANS_NO_CLEAR means receiver is still not ready, NSCP_TRANS_CLEAR means receiver is ready, transport is ready.
uint8_t NSCP_Sender_Wait_For_Trans_Ready(NSCP_ConfigTypeDef* comm_conf)
{
	return NSCP_TRANS_CLEAR;
}

//NSCP wait until the transport is clear, which means the data can be sent. Block-on wait.
void NSCP_Sender_Wait_For_Trans_Ready_Sync(NSCP_ConfigTypeDef* comm_conf)
{
	
}

//start the data transfer. async send.
void NSCP_Sender_Trans_Launch(NSCP_ConfigTypeDef* comm_conf)
{
	
}

//NSCP check the trnas is fin.NSCP_TRANS_NO_FIN means not , NSCP_TRANS_FIN means fin.
uint8_t NSCP_Sender_Wait_For_Trans_Fin(NSCP_ConfigTypeDef* comm_conf)
{
	return NSCP_TRANS_FIN;
}

//NSCP wait until the trnas is fin. Block-on wait.
void NSCP_Sender_Wait_For_Trans_Fin_Sync(NSCP_ConfigTypeDef* comm_conf)
{
	
}

//NSCP handle finishing work after finish transport
void NSCP_Sender_Trans_Post_Handle(NSCP_ConfigTypeDef* comm_conf)
{
	
}

//NSCP reset the sender's transport.
void NSCP_Sender_Reset_Trans(NSCP_ConfigTypeDef* comm_conf)
{
	
}

#endif