#include "bsp_config.h"

#ifdef GPIO_EXIT_API_EN
#ifndef GPIO_EXIT_H
#define GPIO_EXIT_H
#include "stm32f1xx.h"                  // Device header
#include "stm32f1xx_hal.h"

typedef void (* Handler_Func)(void);
void Input_Port_IT_Init(Handler_Func handle_func);

#endif
#endif