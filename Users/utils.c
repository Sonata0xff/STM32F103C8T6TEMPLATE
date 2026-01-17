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
	unsigned char upVal = ((val >> 4) & 0x0f);
	unsigned char downVal = (val & 0x0f);
	upVal = upVal >= 10 ? upVal - 10 + 'a' : upVal + '0';
	downVal = downVal >= 10 ? downVal - 10 + 'a' : downVal + '0';
	res[2] = upVal;
	res[3] = downVal;
	
}
#endif