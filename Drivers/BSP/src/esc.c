#include "esc.h"
#ifdef ESC_API_EN
//control variable
NSCP_ConfigTypeDef recv_handle;
ESC_Status esc_status = ESC_STATUS_ON;
ESC_ConfigTypeDef esc_config;
AtomVarType period_lock;
uint16_t six_period = LOW_STAB_PRD; //usr change, sys read, default is 20ms
uint16_t pwm_high_prd = ESC_INI_PWM_PRD;//sys change, sys read, default is 8us of 42us
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
	__HAL_RCC_DMA1_CLK_ENABLE();
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
	
	//dma init
	HAL_DMA_Init(&esc_config.out_dma_conf);
	__HAL_LINKDMA((&esc_config.out_tim_conf), hdma[TIM_DMA_ID_UPDATE], (esc_config.out_dma_conf));
	
	//NVIC Init
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);
	HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 1, 1);
	HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
}

void DMA1_Channel2_IRQHandler()
{
	HAL_DMA_IRQHandler(&esc_config.nscp_dma_han);
	NSCP_Recv_Trans_Post_Handle(&recv_handle);
}
void DMA1_Channel5_IRQHandler()
{
	HAL_DMA_IRQHandler(&esc_config.out_dma_conf);
}
uint16_t six_map[6] = {0x03C0, 0x030C, 0x003C, 0x0C30, 0x0C03, 0x00C3};
void HAL_ECS_DMAPeriodElapsedCplt(struct __DMA_HandleTypeDef * hdma)
{
	TIM1->RCR = six_period - 1;
	esc_config.out_tim_conf.Instance->CCR1 = ESC_PRD - pwm_high_prd - 1;
	esc_config.out_tim_conf.Instance->CCR2 = ESC_PRD - pwm_high_prd - 1;
	esc_config.out_tim_conf.Instance->CCR3 = ESC_PRD - pwm_high_prd - 1;
}

HAL_StatusTypeDef HAL_ECS_TIM_Base_Start_DMA(TIM_HandleTypeDef *htim, const uint32_t *pData, uint16_t Length)
{
	uint32_t tmpsmcr;

  /* Check the parameters */
  assert_param(IS_TIM_DMA_INSTANCE(htim->Instance));

  /* Set the TIM state */
  if (htim->State == HAL_TIM_STATE_BUSY)
  {
    return HAL_BUSY;
  }
  else if (htim->State == HAL_TIM_STATE_READY)
  {
    if ((pData == NULL) || (Length == 0U))
    {
      return HAL_ERROR;
    }
    else
    {
      htim->State = HAL_TIM_STATE_BUSY;
    }
  }
  else
  {
    return HAL_ERROR;
  }

  /* Set the DMA Period elapsed callbacks */
  htim->hdma[TIM_DMA_ID_UPDATE]->XferCpltCallback = HAL_ECS_DMAPeriodElapsedCplt;
  htim->hdma[TIM_DMA_ID_UPDATE]->XferHalfCpltCallback = NULL;

  /* Set the DMA error callback */
  htim->hdma[TIM_DMA_ID_UPDATE]->XferErrorCallback = TIM_DMAError ;

  /* Enable the DMA channel */
  if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_UPDATE], (uint32_t)pData, (uint32_t)&esc_config.out_tim_conf.Instance->CCER,
                       Length) != HAL_OK)
  {
		__HAL_DMA_DISABLE_IT(htim->hdma[TIM_DMA_ID_UPDATE], DMA_IT_HT);
    /* Return error status */
    return HAL_ERROR;
  }

  /* Enable the TIM Update DMA request */
  __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_UPDATE);

  /* Enable the Peripheral, except in trigger mode where enable is automatically done with trigger */
  if (IS_TIM_SLAVE_INSTANCE(htim->Instance))
  {
    tmpsmcr = htim->Instance->SMCR & TIM_SMCR_SMS;
    if (!IS_TIM_SLAVEMODE_TRIGGER_ENABLED(tmpsmcr))
    {
      __HAL_TIM_ENABLE(htim);
    }
  }
  else
  {
    __HAL_TIM_ENABLE(htim);
  }

  /* Return function status */
  return HAL_OK;
}

HAL_StatusTypeDef HAL_ECS_TIM_Base_Stop_DMA(TIM_HandleTypeDef *htim)
{
	/* Check the parameters */
  assert_param(IS_TIM_DMA_INSTANCE(htim->Instance));

  /* Disable the TIM Update DMA request */
  __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_UPDATE);

  (void)HAL_DMA_Abort_IT(htim->hdma[TIM_DMA_ID_UPDATE]);

  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);

  /* Set the TIM state */
  htim->State = HAL_TIM_STATE_READY;

  /* Return function status */
  return HAL_OK;
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
	esc_config.out_tim_conf.Init.Period = ESC_PRD - 1;// 42us
	esc_config.out_tim_conf.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	esc_config.out_tim_conf.Init.RepetitionCounter = LOW_STAB_PRD - 1; //20ms
	esc_config.out_tim_conf.hdma[TIM_DMA_ID_UPDATE] = &esc_config.out_dma_conf;
	
	esc_config.out_dma_conf.Instance = DMA1_Channel5;
	esc_config.out_dma_conf.Init.Direction = DMA_MEMORY_TO_PERIPH;
	esc_config.out_dma_conf.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	esc_config.out_dma_conf.Init.MemInc = DMA_MINC_ENABLE;
	esc_config.out_dma_conf.Init.Mode = DMA_CIRCULAR;
	esc_config.out_dma_conf.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	esc_config.out_dma_conf.Init.PeriphInc = DMA_PINC_DISABLE;
	esc_config.out_dma_conf.Init.Priority = DMA_PRIORITY_HIGH;
	
	
	esc_config.out_pwm_config.OCMode = TIM_OCMODE_PWM1;
	esc_config.out_pwm_config.Pulse = ESC_PRD - ESC_INI_PWM_PRD - 1; //8us
	esc_config.out_pwm_config.OCIdleState = TIM_OCIDLESTATE_RESET;
	esc_config.out_pwm_config.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	esc_config.out_pwm_config.OCPolarity = TIM_OCPOLARITY_HIGH;
	esc_config.out_pwm_config.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	//atom init
	Atom_Write(&period_lock, ATOM_VALUE_RESET);
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
	//start the tik
	HAL_ECS_TIM_Base_Start_DMA(&esc_config.out_tim_conf, (uint32_t*)six_map, 6);
	//fake wait
	for(int i = 1; i < 10000; i++);
	//coding ...
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
	while(1);
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