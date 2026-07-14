#include "stm32f1xx.h"                  // Device header
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_conf.h"
#include "utils.h"
#include "test.h"
#include "OLED.h"
#include "IIC.h"

int main()
{
	//stand process
	Standard_process();

	//work code
	ESC_Test_Init();
	return 0;
}