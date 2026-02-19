#include "stm32f1xx.h"                  // Device header
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_conf.h"
#include "utils.h"
#include "test.h"
#include "timCounter.h"
int main()
{
	//stand process
	Standard_process();
	//work
	OneMsTest();
	return 0;
}