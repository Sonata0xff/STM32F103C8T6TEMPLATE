#include "NSCP.h"
#ifdef NSCP_API_EN

void NSCP_Sender_Init(NSCP_ConfigTypeDef* comm_conf)
{
	if (comm_conf == NSCP_NULL ||
			comm_conf->tmp_status != NSCP_ON) return;
	//init code.
	//rcc enable
	comm_conf->func_handle();
	//gpio init
	HAL_GPIO_Init(comm_conf->sda_gpio_handle, comm_conf->sda_handle);
	HAL_GPIO_WritePin(comm_conf->sda_gpio_handle, comm_conf->sda_handle->Pin, GPIO_PIN_RESET);
	//dma init
	HAL_DMA_Init(comm_conf->dma_handle);
	__HAL_LINKDMA(comm_conf->pwm_handle, hdma[comm_conf->tim_cc_id], *(comm_conf->dma_handle));
	//NVIC Init
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);
	HAL_NVIC_SetPriority(comm_conf->dma_ir_handle, 1, 1);
	HAL_NVIC_EnableIRQ(comm_conf->dma_ir_handle);
	//tim init
	HAL_TIM_PWM_Init(comm_conf->pwm_handle);
	HAL_TIM_PWM_ConfigChannel(comm_conf->pwm_handle, comm_conf->pwm_channel_handle, comm_conf->Channel);
	//init code end.
	comm_conf->tmp_status = NSCP_READY;
}

//NSCP data block init
void NSCP_Sender_Config_Init(NSCP_ConfigTypeDef* comm_conf)
{
	if (comm_conf == NSCP_NULL) return;
	//data init
	comm_conf->data = 0x00;
	//TIM_OC_InitTypeDef init
	comm_conf->pwm_channel_handle->OCMode = TIM_OCMODE_PWM1;
	comm_conf->pwm_channel_handle->Pulse = 0;
	comm_conf->pwm_channel_handle->OCPolarity = TIM_OCPOLARITY_HIGH;
	comm_conf->pwm_channel_handle->OCFastMode = TIM_OCFAST_DISABLE;
	//DMA_HandleTypeDef init
	comm_conf->dma_handle->Instance = comm_conf->dma_conf;
	comm_conf->dma_handle->Init.Direction = DMA_MEMORY_TO_PERIPH;
	comm_conf->dma_handle->Init.PeriphInc = DMA_PINC_DISABLE;
	comm_conf->dma_handle->Init.MemInc = DMA_MINC_ENABLE;
	comm_conf->dma_handle->Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	comm_conf->dma_handle->Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	comm_conf->dma_handle->Init.Mode = DMA_NORMAL;
	comm_conf->dma_handle->Init.Priority = DMA_PRIORITY_HIGH;
	//TIM_HandleTypeDef init
	comm_conf->pwm_handle->Instance = comm_conf->tim_conf;
	comm_conf->pwm_handle->Init.CounterMode = TIM_COUNTERMODE_UP;
	comm_conf->pwm_handle->Init.Period = comm_conf->period - 1;
	comm_conf->pwm_handle->Init.Prescaler = 36 - 1;//36 div, supposed to be 2 Mhz, 0.5us
	comm_conf->pwm_handle->hdma[comm_conf->tim_cc_id] = comm_conf->dma_handle;
	//GPIO_InitTypeDef init
	comm_conf->sda_handle->Pin = comm_conf->sda_pin;
	comm_conf->sda_handle->Mode = GPIO_MODE_AF_PP;
	comm_conf->sda_handle->Pull = GPIO_NOPULL;
	comm_conf->sda_handle->Speed = GPIO_SPEED_FREQ_HIGH;
	//var init
	Atom_Write(&comm_conf->send_lock, ATOM_VALUE_RESET);
	//status init
	comm_conf->tmp_status = NSCP_ON;
}

//NSCP sender load in datas. 
void NSCP_Sender_Load_Data(NSCP_ConfigTypeDef* comm_conf)
{
	if (comm_conf == NSCP_NULL ||
			comm_conf->tmp_status != NSCP_READY) return;
	uint16_t bit_mask = 0x01;
	for (unsigned char i = 0; i < NSCP_MAX_PACK_LEN; ++i) {
		comm_conf->duty_send_buffer[i] = comm_conf->data & bit_mask ? comm_conf->one_period : (comm_conf->period - comm_conf->one_period);
		bit_mask <<= 1;
	}
	comm_conf->duty_send_buffer[NSCP_MAX_PACK_LEN] = 0;
	comm_conf->tmp_status = NSCP_LOADED;
}

//NSCP check the transport is clear, which means the data can be sent.
//NSCP_TRANS_NO_CLEAR means receiver is still not ready, NSCP_TRANS_CLEAR means receiver is ready, transport is ready.
uint8_t NSCP_Sender_Wait_For_Trans_Ready(NSCP_ConfigTypeDef* comm_conf)
{
	if (comm_conf == NSCP_NULL ||
			comm_conf->tmp_status != NSCP_LOADED) return NSCP_TRANS_NO_CLEAR;
	//coding ...
	comm_conf->tmp_status = NSCP_TRANS_READY;
	return NSCP_TRANS_CLEAR;
}

//NSCP wait until the transport is clear, which means the data can be sent. Block-on wait.
void NSCP_Sender_Wait_For_Trans_Ready_Sync(NSCP_ConfigTypeDef* comm_conf)
{
	if (comm_conf == NSCP_NULL ||
			comm_conf->tmp_status != NSCP_LOADED) return;
	//coding ...
	comm_conf->tmp_status = NSCP_TRANS_READY;
	return;
}

//start the data transfer. async send.
void NSCP_Sender_Trans_Launch(NSCP_ConfigTypeDef* comm_conf)
{
	if (comm_conf == NSCP_NULL ||
			comm_conf->tmp_status != NSCP_TRANS_READY) return;
	Atom_Write(&comm_conf->send_lock, ATOM_VALUE_SET);
	HAL_TIM_PWM_Start_DMA(comm_conf->pwm_handle,
												comm_conf->Channel,
												(uint32_t*)comm_conf->duty_send_buffer,
												NSCP_MAX_PACK_LEN + 1);
	comm_conf->tmp_status = NSCP_TRANS_ON;
	return;
}

//NSCP check the trnas is fin.NSCP_TRANS_NO_FIN means not , NSCP_TRANS_FIN means fin.
uint8_t NSCP_Sender_Wait_For_Trans_Fin(NSCP_ConfigTypeDef* comm_conf)
{
	if (comm_conf == NSCP_NULL ||
			comm_conf->tmp_status != NSCP_TRANS_ON) return NSCP_TRANS_NO_FIN;
	if (Atom_Read(&comm_conf->send_lock) == ATOM_VALUE_RESET) {
		comm_conf->tmp_status = NSCP_TRANS_FIN;
		return NSCP_TRANS_FIN;
	} else return NSCP_TRANS_NO_FIN;
}

//NSCP wait until the trnas is fin. Block-on wait.
void NSCP_Sender_Wait_For_Trans_Fin_Sync(NSCP_ConfigTypeDef* comm_conf)
{
	if (comm_conf == NSCP_NULL ||
			comm_conf->tmp_status != NSCP_TRANS_ON) return;
	while(Atom_Read(&comm_conf->send_lock) == ATOM_VALUE_SET);
	comm_conf->tmp_status = NSCP_TRANS_FIN;
	return;
}

//NSCP handle finishing work after finish transport
void NSCP_Sender_Trans_Post_Handle(NSCP_ConfigTypeDef* comm_conf)
{
	if (comm_conf == NSCP_NULL ||
			comm_conf->tmp_status != NSCP_TRANS_FIN) return;
	Atom_Write(&comm_conf->send_lock, ATOM_VALUE_RESET);
	NSCP_Sender_Reset_Trans(comm_conf);
	return;
}

//NSCP reset the sender's transport.
void NSCP_Sender_Reset_Trans(NSCP_ConfigTypeDef* comm_conf)
{
	comm_conf->tmp_status = NSCP_READY;
	return;
}
#endif