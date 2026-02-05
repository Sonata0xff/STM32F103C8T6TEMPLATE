#include "bsp_config.h"
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
#endif
//----------------------------------------------------