#include "bsp_config.h"
#ifndef TEST_H
#define TEST_H
//----------------------------------------------------
/*
test case : nrf2401 communication
*/
#ifdef NRF2401_TEST
#include "stm32f1xx.h"                  // Device header
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_conf.h"
#include "IIC.h"
#include "OLED.h"
#include "ISPI.h"
#include "nrf2401.h"
#include "utils.h"
#include "GPIO_EXIT.h"

#ifdef NRF2401_SENDER_TEST
void NRF2401_Sender();
#endif

#ifdef NRF2401_RECEIVER_TEST
void NRF2401_Receiver();
#endif

#ifdef NRF_COMM_CASE1_SEND
void nrf_testcase1_send();
#endif

#ifdef NRF_COMM_CASE1_RECV
void nrf_testcase1_recv();
#endif
#endif
//----------------------------------------------------
/*
test case : timCounter
*/
#ifdef TIM_COUNTER_TEST
#include "stm32f1xx.h"                  // Device header
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_conf.h"
#include "timCounter.h"

#ifdef TIM_COUNTER_BASE_CASE1
void OneMsTest();
#endif

#endif
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
/*
test case : MPU
*/
#ifdef MPU_TEST
#include "stm32f1xx.h"                  // Device header
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_conf.h"
#include "OLED.h"
#include "IIC.h"
#include "MPU.h"
#include "utils.h"
void MPU_Gyro_Test();
void MPU_Accel_Test();
void MPU_Mag_Test();
#endif
//----------------------------------------------------
/*
test case : NSCP
*/
#ifdef NSCP_TEST
#include "stm32f1xx.h"                  // Device header
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_conf.h"
#include "NSCP.h"
void NSCP_Send_Init_Test();
void NSCP_Send_Load_Test();
void NSCP_Send_Full_Send_Test();
#endif
//----------------------------------------------------
/*
test case : delay counter
*/
#ifdef DELAY_COUNTER_TEST
#include "stm32f1xx.h"                  // Device header
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_conf.h"
#include "IIC.h"
#include "OLED.h"

//coding ...
void DC_Init_Test();
#endif
//----------------------------------------------------
#endif