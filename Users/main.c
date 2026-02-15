#include "stm32f1xx.h"                  // Device header
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_conf.h"
#include "IIC.h"
#include "OLED.h"
#include "ISPI.h"
#include "nrf2401.h"
#include "utils.h"
#include "GPIO_EXIT.h"
#include "test.h"

int main()
{
	//stand process
	Standard_process();
	//work
	nrf_testcase1_send();
	//nrf_testcase1_recv();
	return 0;
}