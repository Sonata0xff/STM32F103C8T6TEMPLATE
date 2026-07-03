#include "bsp_config.h"
#ifndef TEST_H
#define TEST_H
//----------------------------------------------------
/*
test case : OLED
*/
#ifdef OLED_TEST
#include "stm32f1xx.h"                  // Device header
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_conf.h"
#include "OLED.h"
#include "IIC.h"
void OLED_Test();
#endif
//----------------------------------------------------
#endif