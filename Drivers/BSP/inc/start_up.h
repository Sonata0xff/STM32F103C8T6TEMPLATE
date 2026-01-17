#include "bsp_config.h"
#ifdef START_UP_API_EN
#ifndef START_UP_H
#define START_UP_H

#include "stm32f1xx.h"                  // Device header
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_conf.h"


void SystemClock_Config(void);

#endif
#endif