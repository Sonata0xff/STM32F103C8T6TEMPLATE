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

void TransNum2StringWOS(unsigned char val, char* res);

void TransI16_2_float(int16_t from_, float* to_, float bias);

void TransFloat_2_Str(float val, char * res);//1 singal + 4 int + 1 dot + 3 float = 9 size

void TransI16_2_Str(uint16_t from_, char* res);
#endif
#endif