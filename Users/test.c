#include "test.h"
//----------------------------------------------------
#ifdef NRF2401_TEST
#ifdef NRF2401_SENDER_TEST

void PrintAndCheckReg()
{
	int pos = 0;
	int line = 0;
	//orders
	unsigned char recData[4];
	char valueStr[2];
	//show STATUS and CONFIG
	NRF2401_Get_Reg(0x00, recData, 2);
	TransNum2StringWOS(recData[0], valueStr);
	OLED_WriteIn_16x8String(pos, line, 2, (unsigned char *)valueStr);
	pos += 3;

	TransNum2StringWOS(recData[1], valueStr);
	OLED_WriteIn_16x8String(pos, line, 2, (unsigned char *)valueStr);
	pos += 3;
	
	//show ENAA
	NRF2401_Get_Reg(0x01, recData, 2);
	TransNum2StringWOS(recData[1], valueStr);
	OLED_WriteIn_16x8String(pos, line, 2, (unsigned char *)valueStr);
	pos += 3;
	
	//show EN_RXADDR
	NRF2401_Get_Reg(0x02, recData, 2);
	TransNum2StringWOS(recData[1], valueStr);
	OLED_WriteIn_16x8String(pos, line, 2, (unsigned char *)valueStr);
	pos += 3;
	
	//show SETUP_AW
	NRF2401_Get_Reg(0x03, recData, 2);
	TransNum2StringWOS(recData[1], valueStr);
	OLED_WriteIn_16x8String(pos, line, 2, (unsigned char *)valueStr);
	pos += 3;
	
	//show SETUP_RETR
	NRF2401_Get_Reg(0x04, recData, 2);
	TransNum2StringWOS(recData[1], valueStr);
	OLED_WriteIn_16x8String(pos, line, 2, (unsigned char *)valueStr);
	pos += 3;
	
	pos = 2;
	
	//show FIFO_STATUS
	NRF2401_Get_Reg(0x17, recData, 2);
	TransNum2StringWOS(recData[1], valueStr);
	OLED_WriteIn_16x8String(pos, line + 1, 2, (unsigned char *)valueStr);
	pos += 3;
	
	//show FEATURE
	NRF2401_Get_Reg(0x1d, recData, 2);
	TransNum2StringWOS(recData[1], valueStr);
	OLED_WriteIn_16x8String(pos, line + 1, 2, (unsigned char *)valueStr);
	pos += 3;
	
	//show channel0 addr
	NRF2401_Get_Reg(0x0a, recData, 4);
	for (unsigned char i = 0; i < 3; i++) {
		TransNum2StringWOS(recData[i + 1], valueStr);
		OLED_WriteIn_16x8String(pos, line + 1, 2, (unsigned char *)valueStr);
		pos += 3;
	}
	pos = 0;
	
	//show send addr
	NRF2401_Get_Reg(0x10, recData, 4);
	for (unsigned char i = 0; i < 3; i++) {
		TransNum2StringWOS(recData[i + 1], valueStr);
		OLED_WriteIn_16x8String(pos, line + 2, 2, (unsigned char *)valueStr);
		pos += 3;
	}
	
	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == GPIO_PIN_RESET) OLED_WriteIn_16x8Char(pos, line + 2, '0');
	else OLED_WriteIn_16x8Char(pos, line + 2, '1');
	
}

void NRF2401_Sender()
{
	//test data
	char send_bytes[2] = {'F', 'U'};
	char ans[] = "OK";
	uint8_t res = 0;
	
	//NSS line Group
	uint32_t pin_group[1] = {GPIO_PIN_4};
	
	//communication protocol init
	ISPI1_Init();
	ISPI1_NSS_Init(GPIOA, pin_group, 1);
	//IIC Init
	IIC1_Init(0, 0);
	
	//Hardware: OLED init
	OLED_Init();
	OLED_TurnOn_Screen();
	OLED_Flash_Screen(0x00);
	//Hardware: NRF2401 init
	NRF2401_Init(0);
	
	//test code start----------------------------
	//OLED_WriteIn_16x8String(0, 0, 8, (unsigned char *)title);
	NRF2401_Start();
	NRF2401_Send_Mode();
	//PrintAndCheckReg();
	NRF2401_Send((unsigned char*)send_bytes);
	NRF2401_Send_Block_Wait();
	OLED_WriteIn_16x8String(0, 0, 2, (unsigned char*)ans);
	if (GetTimeOutRes() == 0) OLED_WriteIn_16x8Char(0,1, 'F');
	else OLED_WriteIn_16x8Char(0,1, 'T');
	//main loop
	while (1);
	//test code stop----------------------------
}
#endif

#ifdef NRF2401_RECEIVER_TEST

void PrintAndCheckReg()
{
	int pos = 0;
	int line = 0;
	//orders
	unsigned char recData[4];
	char valueStr[2];
	//show STATUS and CONFIG
	NRF2401_Get_Reg(0x00, recData, 2);
	TransNum2StringWOS(recData[0], valueStr);
	OLED_WriteIn_16x8String(pos, line, 2, (unsigned char *)valueStr);
	pos += 3;

	TransNum2StringWOS(recData[1], valueStr);
	OLED_WriteIn_16x8String(pos, line, 2, (unsigned char *)valueStr);
	pos += 3;
	
	//show ENAA
	NRF2401_Get_Reg(0x01, recData, 2);
	TransNum2StringWOS(recData[1], valueStr);
	OLED_WriteIn_16x8String(pos, line, 2, (unsigned char *)valueStr);
	pos += 3;
	
	//show EN_RXADDR
	NRF2401_Get_Reg(0x02, recData, 2);
	TransNum2StringWOS(recData[1], valueStr);
	OLED_WriteIn_16x8String(pos, line, 2, (unsigned char *)valueStr);
	pos += 3;
	
	//show SETUP_AW
	NRF2401_Get_Reg(0x03, recData, 2);
	TransNum2StringWOS(recData[1], valueStr);
	OLED_WriteIn_16x8String(pos, line, 2, (unsigned char *)valueStr);
	pos += 3;
	
	//show SETUP_RETR
	NRF2401_Get_Reg(0x04, recData, 2);
	TransNum2StringWOS(recData[1], valueStr);
	OLED_WriteIn_16x8String(pos, line, 2, (unsigned char *)valueStr);
	pos += 3;
	
	pos = 2;
	
	//show FIFO_STATUS
	NRF2401_Get_Reg(0x17, recData, 2);
	TransNum2StringWOS(recData[1], valueStr);
	OLED_WriteIn_16x8String(pos, line + 1, 2, (unsigned char *)valueStr);
	pos += 3;
	
	//show FEATURE
	NRF2401_Get_Reg(0x1d, recData, 2);
	TransNum2StringWOS(recData[1], valueStr);
	OLED_WriteIn_16x8String(pos, line + 1, 2, (unsigned char *)valueStr);
	pos += 3;
	
	//show channel0 addr
	NRF2401_Get_Reg(0x0a, recData, 4);
	for (unsigned char i = 0; i < 3; i++) {
		TransNum2StringWOS(recData[i + 1], valueStr);
		OLED_WriteIn_16x8String(pos, line + 1, 2, (unsigned char *)valueStr);
		pos += 3;
	}
	pos = 0;
	
	//show send addr
	NRF2401_Get_Reg(0x10, recData, 4);
	for (unsigned char i = 0; i < 3; i++) {
		TransNum2StringWOS(recData[i + 1], valueStr);
		OLED_WriteIn_16x8String(pos, line + 2, 2, (unsigned char *)valueStr);
		pos += 3;
	}
	
	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == GPIO_PIN_RESET) OLED_WriteIn_16x8Char(pos, line + 2, '0');
	else OLED_WriteIn_16x8Char(pos, line + 2, '1');
}

void NRF2401_Receiver()
{
	//test data
	char title[] = "Receive:";
	char recv_bytes[4] = {'#', '#', ' ', ' '};
	uint8_t res = 0;
	
	//NSS line Group
	uint32_t pin_group[1] = {GPIO_PIN_4};
	
	//communication protocol init
	ISPI1_Init();
	ISPI1_NSS_Init(GPIOA, pin_group, 1);
	//IIC Init
	IIC1_Init(0, 0);
	
	//Hardware: OLED init
	OLED_Init();
	OLED_TurnOn_Screen();
	OLED_Flash_Screen(0x00);
	//Hardware: NRF2401 init
	NRF2401_Init(0);
	
	//test code start----------------------------
	//OLED_WriteIn_16x8String(0, 0, 8, (unsigned char *)title);
	NRF2401_Start();
	NRF2401_Recv_Mode();
	//PrintAndCheckReg();
	NRF2401_Recv_Block_Wait((unsigned char*)recv_bytes);
	OLED_WriteIn_16x8String(0, 0, 4, (unsigned char*)recv_bytes);
	//main loop
	while (1);
	
	//test code stop----------------------------
}
#endif

int comm_len = 4;

#ifdef NRF_COMM_CASE1_SEND
void nrf_testcase1_send()
{
	//test data
	char title[] = "Sending...";
	char title2[] = "Send Fin:";
	char title3[] = "ACK";
	char title4[] = "ERR";
	char datas[] = "Happy new year, 2026!!!!";
	int dataLen = 24; 
	
	uint8_t res = 0;
	
	//OLED init
	IIC1_Init(0, 0);
	OLED_Init();
	OLED_TurnOn_Screen();
	OLED_Flash_Screen(0x00);
	
	//nrf2401 init
	uint32_t pin_group[1] = {GPIO_PIN_4};
	ISPI1_Init();
	ISPI1_NSS_Init(GPIOA, pin_group, 1);
	NRF2401_Init(0);
	NRF2401_Set_Comm_Data_Len(comm_len);
	
	//test code
	OLED_WriteIn_16x8String(0, 0, 10, (unsigned char*)title);
	NRF2401_Start();
	NRF2401_Send_Mode();
	for (int i = 0; i < dataLen / comm_len; i++) {
		NRF2401_Send((unsigned char *)(&datas[i * comm_len]));
		NRF2401_Send_Block_Wait();
		res |= GetTimeOutRes();
		HAL_Delay(3);
	}
	NRF2401_Revert_Standby();
	OLED_WriteIn_16x8String(0, 1, 9, (unsigned char*)title2);
	if (res != 0) {
		OLED_WriteIn_16x8String(9, 1, 3, (unsigned char*)title4);
	} else {
		OLED_WriteIn_16x8String(9, 1, 3, (unsigned char*)title3);
	}
	
	
	//finish
	while(1);
}

#endif

#ifdef NRF_COMM_CASE1_RECV
void nrf_testcase1_recv()
{
	//test data
	char title[] = "Waiting...";
	char recv_datas[comm_len];
	int x = 0, y = 1;
	
	//OLED init
	IIC1_Init(0, 0);
	OLED_Init();
	OLED_TurnOn_Screen();
	OLED_Flash_Screen(0x00);
	
	//nrf2401 init
	uint32_t pin_group[1] = {GPIO_PIN_4};
	ISPI1_Init();
	ISPI1_NSS_Init(GPIOA, pin_group, 1);
	NRF2401_Init(0);
	NRF2401_Set_Comm_Data_Len(comm_len);
	
	//test code
	OLED_WriteIn_16x8String(0, 0, 10, (unsigned char*)title);
	NRF2401_Start();
	NRF2401_Recv_Mode();
	while(1) {
		NRF2401_Recv_Block_Wait((unsigned char*)recv_datas);
		if(x == 16) {
			x = 0;
			++y;
		}
		OLED_WriteIn_16x8String(x, y, comm_len, (unsigned char *)recv_datas);
		x += comm_len;
	}
}
#endif
#endif
//----------------------------------------------------
#ifdef TIM_COUNTER_TEST

#ifdef TIM_COUNTER_BASE_CASE1
void timCounterOutput()
{
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_12);
}

void OneMsTest()
{
	//init GPIO
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef test_port = {
		.Mode = GPIO_MODE_OUTPUT_PP,
		.Pin = GPIO_PIN_12,
		.Speed = GPIO_SPEED_FREQ_HIGH
	};
	HAL_GPIO_Init(GPIOA, &test_port);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);
	
	//delay
	HAL_Delay(1);
	
	//init timCounter2 1ms period
	Tim2_Counter_Init(100, timCounterOutput);
	
	//start test
	Tim2_Counter_Start();
	
	//stuck loop
	while(1);
}
#endif
#endif
//----------------------------------------------------
#ifdef OLED_TEST
void OLED_Test()
{
	//datas
	char title[] = "hello,2026!";
	int size = 11;
	
	//init OLED
	IIC1_Init(0, 0);
	SetIIC_Comm_Mode(1);//Polling mode
	OLED_Init();
	OLED_TurnOn_Screen();
	OLED_Flash_Screen(0x00);
	
	//start test
	OLED_WriteIn_16x8String(0, 0, size, (unsigned char *)title);
	
	//stuck
	while(1);
}
#endif
//----------------------------------------------------
#ifdef MPU_TEST
void MPU_Gyro_Test()
{
	//datas
	float gyro[3];
	for (char i = 0; i < 3; ++i) gyro[i] = 0.0f;
	
	
	//Init comm protocol
	IIC1_Init(0, 0);
	SetIIC_Comm_Mode(1);//Polling mode
	char value_float[9];//4 int + 3 float
	
	//OLED Init
	OLED_Init();
	OLED_TurnOn_Screen();
	OLED_Flash_Screen(0x00);
	
	//start test
	MPU_Init();
	MPU_Start();
	MPU_System_Calibration();
	while(1) {
		MPU_Read_Gyro();
		MPU_Get_Gyro(gyro);
		TransFloat_2_Str(gyro[0], value_float);
		OLED_WriteIn_16x8String(0, 0, 9, (unsigned char*)value_float);
		TransFloat_2_Str(gyro[1], value_float);
		OLED_WriteIn_16x8String(0, 1, 9, (unsigned char*)value_float);
		TransFloat_2_Str(gyro[2], value_float);
		OLED_WriteIn_16x8String(0, 2, 9, (unsigned char*)value_float);
		HAL_Delay(250);
	}
}

void MPU_Accel_Test()
{
	//datas
	float accel[3];
	for (char i = 0; i < 3; ++i) accel[i] = 0.0f;
	
	
	//Init comm protocol
	IIC1_Init(0, 0);
	SetIIC_Comm_Mode(1);//Polling mode
	char value_float[9];//4 int + 3 float
	
	//OLED Init
	OLED_Init();
	OLED_TurnOn_Screen();
	OLED_Flash_Screen(0x00);
	
	//start test
	MPU_Init();
	MPU_Start();
	MPU_System_Calibration();
	while(1) {
		MPU_Read_Accel();
		MPU_Get_Accel(accel);
		TransFloat_2_Str(accel[0], value_float);
		OLED_WriteIn_16x8String(0, 0, 9, (unsigned char*)value_float);
		TransFloat_2_Str(accel[1], value_float);
		OLED_WriteIn_16x8String(0, 1, 9, (unsigned char*)value_float);
		TransFloat_2_Str(accel[2], value_float);
		OLED_WriteIn_16x8String(0, 2, 9, (unsigned char*)value_float);
		HAL_Delay(250);
	}
}
#endif
//----------------------------------------------------
