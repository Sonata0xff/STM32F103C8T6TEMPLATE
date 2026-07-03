#include "utils.h"
#ifdef UTILS_API_EN

void Standard_process()
{
	//clock init & HAL_Init
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

void TransNum2StringWOS(unsigned char val, char* res)
{
	unsigned char upVal = ((val >> 4) & 0x0f);
	unsigned char downVal = (val & 0x0f);
	upVal = upVal >= 10 ? upVal - 10 + 'a' : upVal + '0';
	downVal = downVal >= 10 ? downVal - 10 + 'a' : downVal + '0';
	res[0] = upVal;
	res[1] = downVal;
}

void TransI16_2_float(int16_t from_, float* to_, float bias)
{
	*to_ = from_ / bias;
}

void TransFloat_2_Str(float val, char * res)
{
	char i,j,k;
	for (i = 0; i < 9; ++i) res[i] = '0';
	if (val < 0) {
		res[0] = '-';
		val *= -1.0f;
	} else {
		res[0] = '+';
	}
	res[5] = '.';
	int fourSize = val;
	float tmp2 = fourSize;
	val -= tmp2;
	val *= 1000.0f;
	int threeSize = val;
	char pivot = 4;
	for (j = 0; j < 4; ++j) {
		if (fourSize == 0) break;
		res[pivot--] = (fourSize % 10 + '0');
		fourSize /= 10;
	}
	pivot = 8;
	for (k = 0; k < 3; ++k) {
		if (threeSize == 0) break;
		res[pivot--] = (threeSize % 10 + '0');
		threeSize /= 10;
	}
}

void TransI16_2_Str(uint16_t from_, char* res)
{
	uint16_t model = 0xf000;
	uint16_t tmp;
	res[0] = '0';
	res[1] = 'x';
	char result;
	for (unsigned char i = 0; i < 4; i++) {
		tmp = ((from_ & model) >> ((3 - i) * 4));
		if (tmp > 9) result = tmp - 10 + 'a';
		else result = tmp + '0';
		res[i + 2] = result;
		model >>= 4;
	}
}
#endif