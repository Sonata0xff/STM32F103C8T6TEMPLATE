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
#include "utils.h"
#include "NSCP.h"
#include "OLED.h"
void NSCP_Send_Init_Test();
void NSCP_Send_Load_Test();
void NSCP_Send_Full_Send_Test();
void NSCP_Send_Full_Send_Test2();
void NSCP_Send_Full_Send_Test3();

void NSCP_Recv_Init_Test(); //receiver test

//---------------------merge test----------------------------------
void Merge_Test_Send();
void Merge_Test_Recv();
//---------------------merge test end------------------------------


#endif
//----------------------------------------------------
/*
test case : delay counter
*/
#ifdef DELAY_COUNTER_TEST
#include "stm32f1xx.h"                  // Device header
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_conf.h"
#include "delay_counter.h"
#include "IIC.h"
#include "OLED.h"

//coding ...
void DC_COUNT_Test();
void DC_Repeat_Count_Test1();
void DC_Repeat_Count_Test2();
void DC_Repeat_Count_Test3();
void DC_Repeat_Count_Test4(); // nscp fake env test1
void DC_Repeat_Count_Test5(); // nscp fake env test2
#endif
//----------------------------------------------------
#endif