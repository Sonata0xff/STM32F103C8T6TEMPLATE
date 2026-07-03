#include "utils.h"
#include "stm32f1xx.h"                  // Device header
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_conf.h"
#include "bsp_config.h"
#include "test.h"
#include "FreeRTOS.h"
int main()
{
	Standard_process();
	
	OLED_Test();
	return 0;
}