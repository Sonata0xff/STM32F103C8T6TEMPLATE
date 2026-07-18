#include "esc.h"
#ifdef ESC_API_EN
//control variable
NSCP_ConfigTypeDef recv_handle;
ESC_Status esc_status = ESC_STATUS_ON;
ESC_ConfigTypeDef esc_config;
AtomVarType period_cache_lock;
uint16_t six_period_cache = LOW_STAB_PRD; //usr change, sys read, default is 5ms
uint16_t six_period = LOW_STAB_PRD; //sys change, sys read, default is 5ms

//---------------private func--------------
//nscp init func
void ESC_NSCP_Init()
{
	__HAL_RCC_DMA1_CLK_ENABLE();
	__HAL_RCC_TIM2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
}
void HAL_TIM_OC_MspInit(TIM_HandleTypeDef *htim)
{
	__HAL_RCC_TIM1_CLK_ENABLE();
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	__HAL_RCC_TIM1_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef pwm_port = {
		.Mode = GPIO_MODE_AF_PP,
		.Pull = GPIO_NOPULL,
		.Speed = GPIO_SPEED_FREQ_HIGH
	};
	//OCx init
	pwm_port.Pin = GPIO_PIN_8;
	HAL_GPIO_Init(GPIOA, &pwm_port);
	pwm_port.Pin = GPIO_PIN_9;
	HAL_GPIO_Init(GPIOA, &pwm_port);
	pwm_port.Pin = GPIO_PIN_10;
	HAL_GPIO_Init(GPIOA, &pwm_port);
	//OCxN init
	pwm_port.Pin = GPIO_PIN_13;
	HAL_GPIO_Init(GPIOB, &pwm_port);
	pwm_port.Pin = GPIO_PIN_14;
	HAL_GPIO_Init(GPIOB, &pwm_port);
	pwm_port.Pin = GPIO_PIN_15;
	HAL_GPIO_Init(GPIOB, &pwm_port);
	//NVIC Init
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);
	HAL_NVIC_SetPriority(TIM1_CC_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);
}

void DMA1_Channel2_IRQHandler()
{
	HAL_DMA_IRQHandler(&esc_config.nscp_dma_han);
	NSCP_Recv_Trans_Post_Handle(&recv_handle);
}
void TIM1_CC_IRQHandler()
{
	HAL_TIM_IRQHandler(&esc_config.out_tim_conf);
}

unsigned char tmp_motor_status = 0;
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
	//wait for coding ...
	
}
//power start running func.
void ESC_APP_On()
{
	//nscp config
	recv_handle.Channel = TIM_CHANNEL_2;
	recv_handle.one_period = ESC_NSCP_ONE_PRD;
	recv_handle.period = ESC_NSCP_PRD;
	recv_handle.sampling_period = ESC_NSCP_SAMP_PRD;
	recv_handle.sda_pin = ESC_NSCP_SDA_PIN;
	recv_handle.tim_cc_id = TIM_DMA_ID_CC2;
	recv_handle.dma_ir_handle = DMA1_Channel2_IRQn;
	recv_handle.dma_conf = DMA1_Channel2;
	recv_handle.tim_conf = TIM2;
	recv_handle.sda_gpio_handle = GPIOA;
	recv_handle.func_handle = ESC_NSCP_Init;
	
	recv_handle.dma_handle = &esc_config.nscp_dma_han;
	recv_handle.pwm_handle = &esc_config.nscp_pwm_conf;
	recv_handle.pwm_slave_handle = &esc_config.nscp_slav_conf;
	recv_handle.sda_handle = &esc_config.nscp_gpio_conf;
	
	//esc output config
	esc_config.out_tim_conf.Instance = TIM1;
	esc_config.out_tim_conf.Init.Prescaler = 72 - 1; //1Mhz, 1us
	esc_config.out_tim_conf.Init.CounterMode = TIM_COUNTERMODE_UP;
	esc_config.out_tim_conf.Init.Period = ESC_PRD - 1;// 0.5ms
	esc_config.out_tim_conf.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	
	esc_config.out_oc_conf.OCMode = TIM_OCMODE_TIMING;
	esc_config.out_oc_conf.Pulse = ESC_PRD - 5;//0.5ms 6 period
	
	esc_config.out_pwm_config.OCMode = TIM_OCMODE_PWM1;
	esc_config.out_pwm_config.Pulse = ESC_INI_PWM_PRD - 1; //1/2v = 5v 
	esc_config.out_pwm_config.OCIdleState = TIM_OCIDLESTATE_RESET;
	esc_config.out_pwm_config.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	esc_config.out_pwm_config.OCPolarity = TIM_OCPOLARITY_HIGH;
	esc_config.out_pwm_config.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	//atom init
	Atom_Write(&period_cache_lock, ATOM_VALUE_RESET);
}
//app env init. 
void ESC_APP_Init()
{
	//esc output init
	HAL_TIM_PWM_Init(&esc_config.out_tim_conf);
	//esc output ch1 init & start -- pwm
	HAL_TIM_PWM_ConfigChannel(&esc_config.out_tim_conf,
															&esc_config.out_pwm_config,
															TIM_CHANNEL_1);
	esc_config.out_tim_conf.Instance->CCMR1 &= ~(TIM_CCMR1_OC1PE);
	HAL_TIM_PWM_Start(&esc_config.out_tim_conf, TIM_CHANNEL_1);
	esc_config.out_tim_conf.Instance->CCER &= ~(TIM_CCER_CC1E);
	//esc output ch2 init & start -- pwm
	HAL_TIM_PWM_ConfigChannel(&esc_config.out_tim_conf,
															&esc_config.out_pwm_config,
															TIM_CHANNEL_2);
	esc_config.out_tim_conf.Instance->CCMR1 &= ~(TIM_CCMR1_OC2PE);
	HAL_TIM_PWM_Start(&esc_config.out_tim_conf, TIM_CHANNEL_2);
	esc_config.out_tim_conf.Instance->CCER &= ~(TIM_CCER_CC2E);
	//esc output ch3 init & start -- pwm
	HAL_TIM_PWM_ConfigChannel(&esc_config.out_tim_conf,
															&esc_config.out_pwm_config,
															TIM_CHANNEL_3);
	esc_config.out_tim_conf.Instance->CCMR2 &= ~(TIM_CCMR2_OC3PE);
	HAL_TIM_PWM_Start(&esc_config.out_tim_conf, TIM_CHANNEL_3);
	esc_config.out_tim_conf.Instance->CCER &= ~(TIM_CCER_CC3E);
	//esc output ch4 init & start -- tik clock
	HAL_TIM_OC_Init(&esc_config.out_tim_conf);
	HAL_TIM_OC_ConfigChannel(&esc_config.out_tim_conf,
													 &esc_config.out_oc_conf,
													 TIM_CHANNEL_4);
	esc_config.out_tim_conf.Instance->CCMR2 &= (~TIM_CCMR2_OC4PE);
	HAL_TIM_OC_Start_IT(&esc_config.out_tim_conf, TIM_CHANNEL_4);
	
	
	//nscp init & start
	NSCP_Recv_Config_Init(&recv_handle);
	NSCP_Recv_Init(&recv_handle);
	NSCP_Recv_Start(&recv_handle);
	//set status
	esc_status = ESC_STATUS_IDLE;
}

//trans the system from Idle to low speed stable
void ESC_APP_Trans_To_Ready()
{
	esc_status = ESC_STATIS_LOW_TRANS;
	//wait for coding ...
	esc_status = ESC_STATUS_LOW_STAB;
	#ifdef ESC_DEBUG_MODE
	ESC_ERROR check_result = ESC_TRANS_LOW_STAB_Check_func();
	#endif
}

//---------------public func---------------
ESC_ERROR ESC_APP_Main()
{
	//ESC device config
	ESC_APP_On();
	//ESC device init& start
	ESC_APP_Init();
	//stablize the motor speed.
	ESC_APP_Trans_To_Ready();
	//coding ...
	return ESC_ERR_NERR;
}

//---------------debug func----------------
#ifdef ESC_DEBUG_MODE
__weak ESC_ERROR ESC_TRANS_LOW_STAB_Check_func()
{
	return ESC_ERR_NERR;
}
#endif
#endif