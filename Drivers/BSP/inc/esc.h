#include "bsp_config.h"

#ifdef ESC_API_EN
#ifndef ESC_H
#define ESC_H

#include "stm32f1xx.h"                  // Device header
#include "stm32f1xx_hal.h"
#include "NSCP.h"

#define LOW_STAB_PRD 10 // 0.5 * 10 = 5ms

typedef enum {
	ESC_STATUS_IDLE,
	ESC_STATIS_LOW_TRANS,
	ESC_STATUS_LOW_STAB,
	ESC_STATUS_TRANS
} ESC_Status;

typedef enum {
	ESC_ERR_NERR,
} ESC_ERROR;

typedef struct {
	//usr config
	
	//sys config
	
} ESC_ConfigTypeDef;

//esc app enterance.
ESC_ERROR ESC_APP_Main();

#ifdef ESC_DEBUG_MODE
ESC_ERROR ESC_TRANS_LOW_STAB_Check_func();
#endif

#endif
#endif