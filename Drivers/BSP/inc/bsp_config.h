//This config file is used to controll if the bsp mode is compiled.
//for example, if define OLED_API_EN, then the code about oled will be compiled.
#ifndef BSP_CONFIG_H
#define BSP_CONFIG_H

#define IIC_API_EN
#define OLED_API_EN
//#define ISPI_API_EN
//#define NRF2401_API_EN
#define UTILS_API_EN
#define START_UP_API_EN
//#define GPIO_EXIT_API_EN
#define ATOM_VAR_API_EN
//#define TIM_COUNTER_API_EN
//#define MPU_API_EN
#define NSCP_API_EN
#define DELAY_COUNTER_API_EN
#define ESC_API_EN
//#define NRF2401_TEST
//#define NRF2401_RECEIVER_TEST
//#define NRF2401_SENDER_TEST
//#define NRF_COMM_CASE1_SEND
//#define NRF_COMM_CASE1_RECV
//#define TIM_COUNTER_TEST
//#define TIM_COUNTER_BASE_CASE1
//#define OLED_TEST
//#define MPU_TEST
//#define NSCP_TEST
//#define DELAY_COUNTER_TEST
#define ESC_TEST
#endif