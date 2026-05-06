#include "stm32f1xx.h"                  // Device header
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_conf.h"
#include "utils.h"
#include "test.h"
#include "OLED.h"
#include "IIC.h"

TIM_OC_InitTypeDef pwm_ch_conf = {
		.OCMode = TIM_OCMODE_PWM1,
		.Pulse = 0, // 2 pulse
		.OCPolarity = TIM_OCPOLARITY_HIGH,
		.OCFastMode = TIM_OCFAST_DISABLE
};

DMA_HandleTypeDef dma_han = {
	.Instance = DMA1_Channel7,
	.Init.Direction = DMA_MEMORY_TO_PERIPH,
	.Init.PeriphInc = DMA_PINC_DISABLE,
	.Init.MemInc = DMA_MINC_ENABLE,
	.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD,
	.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD,
	.Init.Mode = DMA_NORMAL,
	.Init.Priority = DMA_PRIORITY_HIGH
};

TIM_HandleTypeDef pwm_conf = {
		.Instance = TIM2,
		.Init.CounterMode = TIM_COUNTERMODE_UP,
		.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1,
		.Init.Period = 10 - 1, // 5 us
		.Init.Prescaler = 36 - 1, //36 div, supposed to be 2 Mhz, 0.5us
		.hdma[TIM_DMA_ID_CC2] = &dma_han
};

uint16_t dutyCArry[4] = {2, 8, 2, 8};

int main()
{
	//stand process
	Standard_process();
	
	IIC1_Init(0, 0);
	OLED_Init();
	
	OLED_TurnOn_Screen();
	
	OLED_Flash_Screen(0x00);
	
	//work code
	//pwm init
	HAL_TIM_PWM_Init(&pwm_conf);
	HAL_TIM_PWM_ConfigChannel(&pwm_conf, &pwm_ch_conf, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start_DMA(&pwm_conf, TIM_CHANNEL_2, (uint32_t*)dutyCArry, 4);
	while(1);
	//work code end
	return 0;
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	__HAL_RCC_DMA1_CLK_ENABLE();
	__HAL_RCC_TIM2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef gpio_conf = {
		.Pin = GPIO_PIN_1,
		.Mode = GPIO_MODE_AF_PP,
		.Pull = GPIO_NOPULL,
		.Speed = GPIO_SPEED_FREQ_HIGH
	}; //PA1 for TIM2_CH2
	HAL_GPIO_Init(GPIOA, &gpio_conf);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
	//dma init
	HAL_DMA_Init(&dma_han);
	__HAL_LINKDMA(htim, hdma[TIM_DMA_ID_CC2], dma_han);
	//NVIC Init
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);
	HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 1, 1);
	HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);
}

void DMA1_Channel7_IRQHandler()
{
	HAL_DMA_IRQHandler(&dma_han);
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Instance == TIM2)
  {
    HAL_TIM_PWM_Stop_DMA(&pwm_conf, TIM_CHANNEL_2);
  }
}
