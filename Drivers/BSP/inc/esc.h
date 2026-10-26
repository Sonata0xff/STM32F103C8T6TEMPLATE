#include "bsp_config.h"

#ifdef ESC_API_EN
#ifndef ESC_H
#define ESC_H

#include "stm32f1xx.h"                  // Device header
#include "stm32f1xx_hal.h"
#include "NSCP.h"

#define LOW_STAB_PRD 476 // 20ms
#define ESC_PRD 42 //42us
#define ESC_INI_PWM_PRD 5 // 5~8us
#define ESC_NSCP_ONE_PRD 4 //2us
#define ESC_NSCP_PRD 5 //2.5us
#define ESC_NSCP_SAMP_PRD 2//1us
#define ESC_NSCP_SDA_PIN GPIO_PIN_6 //sda line

typedef enum {
	ESC_STATUS_ON,
	ESC_STATUS_IDLE,
	ESC_STATIS_LOW_TRANS,
	ESC_STATUS_LOW_STAB,
	ESC_STATUS_TRANS
} ESC_Status;

typedef enum {
	ESC_ERR_NERR,
} ESC_ERROR;


//inner instruction
typedef struct {
	DMA_HandleTypeDef nscp_dma_han;
	TIM_HandleTypeDef nscp_pwm_conf;
	GPIO_InitTypeDef nscp_gpio_conf;
	TIM_SlaveConfigTypeDef nscp_slav_conf;
	TIM_HandleTypeDef out_tim_conf;
	DMA_HandleTypeDef out_dma_conf;
	TIM_OC_InitTypeDef out_pwm_config;
} ESC_ConfigTypeDef;

//inner func
void HAL_ECS_DMAPeriodElapsedCplt(struct __DMA_HandleTypeDef * hdma);
HAL_StatusTypeDef HAL_ECS_TIM_Base_Start_DMA(TIM_HandleTypeDef *htim, const uint32_t *pData, uint16_t Length);
HAL_StatusTypeDef HAL_ECS_TIM_Base_Stop_DMA(TIM_HandleTypeDef *htim);
//esc app enterance.
ESC_ERROR ESC_APP_Main();

#ifdef ESC_DEBUG_MODE
ESC_ERROR ESC_TRANS_LOW_STAB_Check_func();
#endif

#endif
#endif