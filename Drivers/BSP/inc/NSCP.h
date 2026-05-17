#include "bsp_config.h"
#ifdef NSCP_API_EN
#ifndef NSCP_H
#define NSCP_H

#include "stm32f1xx.h"                  // Device header
#include "stm32f1xx_hal.h"
#include "AtomVariable.h"

typedef void (*rcc_clock_init_func)(void);

typedef enum {
	NSCP_ON = 0,
	NSCP_READY,
	NSCP_LOADED,
	NSCP_TRANS_READY,
	NSCP_TRANS_ON,
	NSCP_TRANS_FIN
} NscpCommStatus;

typedef struct {
	uint16_t data;
	
	uint32_t 							Channel;
	uint32_t							one_period;
	uint32_t 							period;
	uint32_t							sda_pin;
	uint16_t 							tim_cc_id;
	IRQn_Type 						dma_ir_handle;
	DMA_Channel_TypeDef* 	dma_conf;
	TIM_TypeDef*				 	tim_conf;
	GPIO_TypeDef*					sda_gpio_handle;
	rcc_clock_init_func 	func_handle;
	
	
	NscpCommStatus 				tmp_status;
	TIM_OC_InitTypeDef*		pwm_channel_handle;
	DMA_HandleTypeDef*		dma_handle;
	TIM_HandleTypeDef*		pwm_handle;
	GPIO_InitTypeDef*			sda_handle;
} NSCP_ConfigTypeDef;


#define NSCP_NULL 0
#define NSCP_MAX_PACK_LEN 11
#define NSCP_TRANS_CLEAR 1
#define NSCP_TRANS_NO_CLEAR 0
#define NSCP_TRANS_FIN 1
#define NSCP_TRANS_NO_FIN 0
#define NSCP_BIT_ONE 1


//NSCP data block init
//NSCP_ON
void NSCP_Sender_Config_Init(NSCP_ConfigTypeDef* comm_conf);
//NSCP sender init func.
//NSCP_ON -> NSCP_READY
void NSCP_Sender_Init(NSCP_ConfigTypeDef* comm_conf);
//NSCP sender load in datas.
//NSCP_READY -> NSCP_LOADED
void NSCP_Sender_Load_Data(NSCP_ConfigTypeDef* comm_conf);
//NSCP transfer the data into dma format
//NSCP check the transport is clear, which means the data can be sent.
//0 means receiver is still not ready, 1 means receiver is ready, transport is ready.
//NSCP_LOADED -> NSCP_TRANS_READY
uint8_t NSCP_Sender_Wait_For_Trans_Ready(NSCP_ConfigTypeDef* comm_conf);
//NSCP wait until the transport is clear, which means the data can be sent. Block-on wait.
//NSCP_LOADED -> NSCP_TRANS_READY
void NSCP_Sender_Wait_For_Trans_Ready_Sync(NSCP_ConfigTypeDef* comm_conf);
//start the data transfer. async send.
//NSCP_TRANS_READY -> NSCP_TRANS_ON
void NSCP_Sender_Trans_Launch(NSCP_ConfigTypeDef* comm_conf);
//NSCP check the trnas is fin.0 means not , 1 means fin.
//NSCP_TRANS_ON -> NSCP_TRANS_FIN
uint8_t NSCP_Sender_Wait_For_Trans_Fin(NSCP_ConfigTypeDef* comm_conf);
//NSCP wait until the trnas is fin. Block-on wait.
//NSCP_TRANS_ON -> NSCP_TRANS_FIN
void NSCP_Sender_Wait_For_Trans_Fin_Sync(NSCP_ConfigTypeDef* comm_conf);
//NSCP handle finishing work after finish transport
//NSCP_TRANS_FIN -> NSCP_TRANS_FIN
void NSCP_Sender_Trans_Post_Handle(NSCP_ConfigTypeDef* comm_conf);
//NSCP reset the sender's transport.
//NSCP_TRANS_FIN -> NSCP_READY
void NSCP_Sender_Reset_Trans(NSCP_ConfigTypeDef* comm_conf);

//functional method, get receive buffer
const uint16_t* NSCP_Get_Recv_Buf();
//functional method, get send buffer
const uint16_t* NSCP_Get_Send_Buf();
#endif
#endif