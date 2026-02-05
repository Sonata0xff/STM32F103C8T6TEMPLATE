#include "bsp_config.h"
#ifdef UTILS_API_EN
#ifndef UTILS_H
#define UTILS_H

#include "stm32f1xx.h"                  // Device header
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_conf.h"
#include "start_up.h"


void Standard_process();

void TransNum2String(unsigned char val, char* res);
#endif
#endif