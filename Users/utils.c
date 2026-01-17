#include "utils.h"
#ifdef UTILS_API_EN

void Standard_process()
{
	//HAL_Init
	HAL_Init();
	//clock init
	SystemClock_Config();
}

void TransNum2String(unsigned char val, char* res)
{
	res[0] = '0';
	res[1] = 'x';
	res[2] = (((val >> 4) & 0x0f) + '0');
	res[3] = ((val & 0x0f) + '0');
	
}
#endif