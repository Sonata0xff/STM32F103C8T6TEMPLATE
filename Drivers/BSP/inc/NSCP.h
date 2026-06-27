#include "bsp_config.h"
#ifdef NSCP_API_EN
#ifndef NSCP_H
#define NSCP_H

#include "stm32f1xx.h"                  // Device header
#include "stm32f1xx_hal.h"
#include "AtomVariable.h"
#include "delay_counter.h"

typedef void (*rcc_clock_init_func)(void);

#define NSCP_MAX_PACK_LEN 11

typedef enum {
	NSCP_ON = 0,
	NSCP_READY = 1,
	NSCP_LOADED = 2,
	NSCP_TRANS_READY = 3,
	NSCP_TRANS_ON = 4,
	NSCP_TRANS_FIN = 5,
	NSCP_LISTEN_ON = 6,
	NSCP_LISTEN_OFF = 7
} NscpCommStatus;

typedef struct {
	//send data
	uint16_t data;
	
	//usr conf
	uint32_t 							Channel; //sender
	uint32_t							one_period; //sender
	uint32_t 							period; //sender & recv
	uint32_t              sampling_period; //recv
	uint32_t							sda_pin; //sender & recv
	uint16_t 							tim_cc_id; //sender
	IRQn_Type 						dma_ir_handle; //sender & recv
	DMA_Channel_TypeDef* 	dma_conf; //sender & recv
	TIM_TypeDef*				 	tim_conf; //sender & recv
	GPIO_TypeDef*					sda_gpio_handle; //sender & recv
	rcc_clock_init_func 	func_handle; //sender & recv
	DelayCounter_ConfigTypeDef* gap_timer; //sender
	
	//sys conf
	uint16_t							data_cache;//recv
	uint16_t              duty_send_buffer[NSCP_MAX_PACK_LEN + 1]; //sender
	uint16_t              duty_recv_buffer[NSCP_MAX_PACK_LEN]; //recv
	AtomVarType           send_lock; //sender
	NscpCommStatus 				tmp_status; //sender & recv
	TIM_OC_InitTypeDef*		pwm_channel_handle; //sender
	DMA_HandleTypeDef*		dma_handle; //sender & recv
	TIM_HandleTypeDef*		pwm_handle; //sender & recv
	TIM_SlaveConfigTypeDef* pwm_slave_handle; //sender & recv
	GPIO_InitTypeDef*			sda_handle; //sender & recv
} NSCP_ConfigTypeDef;

#define NSCP_NULL 0
#define NSCP_TRANS_CLEAR 1
#define NSCP_TRANS_NO_CLEAR 0
#define TRANS_FIN 1
#define TRANS_NO_FIN 0
#define NSCP_BIT_ONE 1
#define NSCP_DELAY_GAP 8 //8us

/*
NSCP Sender API
*/
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


/*
NSCP recv API
*/
//comm_conf Init
// -> NSCP_ON
void NSCP_Recv_Config_Init(NSCP_ConfigTypeDef* comm_conf);

//recv first init
//NSCP_ON -> NSCP_READY
void NSCP_Recv_Init(NSCP_ConfigTypeDef* comm_conf);

//recv start receive
//NSCP_READY -> NSCP_LISTEN_ON
void NSCP_Recv_Start(NSCP_ConfigTypeDef* comm_conf);

//recv receive post handle
//NSCP_LISTEN_ON -> NSCP_LISTEN_OFF
void NSCP_Recv_Trans_Post_Handle(NSCP_ConfigTypeDef* comm_conf);

//recv Status change
//NSCP_LISTEN_OFF -> NSCP_READY -> NSCP_LISTEN_ON
//NSCP_LISTEN_OFF -> NSCP_TRANS_FIN
void NSCP_Recv_Trans_Change(NSCP_ConfigTypeDef* comm_conf);

//recv get comm result.
void NSCP_Recv_Get(NSCP_ConfigTypeDef* comm_conf);

//user get the data, double cache async reading.
uint16_t NSCP_Recv_Get_Data(NSCP_ConfigTypeDef* comm_conf);

//user stop nscp recv trans loop async. This is a direct shut down.
void NSCP_Recv_Abort(NSCP_ConfigTypeDef* comm_conf);

/*
left job
1.rerecive test writing.
2.merge test writing.
*/

#endif
#endif