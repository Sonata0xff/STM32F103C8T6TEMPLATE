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

void MPU_Mag_Test()
{
	//datas
	float magnet[3];
	for (char i = 0; i < 3; ++i) magnet[i] = 0.0f;
	
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
		MPU_Read_Magnet();
		MPU_Get_Magnet(magnet);
		TransFloat_2_Str(magnet[0], value_float);
		OLED_WriteIn_16x8String(0, 0, 9, (unsigned char*)value_float);
		TransFloat_2_Str(magnet[1], value_float);
		OLED_WriteIn_16x8String(0, 1, 9, (unsigned char*)value_float);
		TransFloat_2_Str(magnet[2], value_float);
		OLED_WriteIn_16x8String(0, 2, 9, (unsigned char*)value_float);
		HAL_Delay(250);
		
	}
	/*IIC1ReadSlaveReg(mag, 3, 0x003c, 0x00);
	IIC1_Send_Block_Wait();
	
	for (char i = 0; i < 3; ++i) {
		TransNum2StringWOS(mag[i], val);
		OLED_WriteIn_16x8String(i * 3, 0, 3, (unsigned char*)val);
	}
	while(1);*/
}
#endif
//----------------------------------------------------
#ifdef NSCP_TEST
#define TEST_NULL 0

TIM_OC_InitTypeDef pwm_ch_conf;
DMA_HandleTypeDef dma_han;
TIM_HandleTypeDef pwm_conf;
GPIO_InitTypeDef gpio_conf;
TIM_SlaveConfigTypeDef slav_conf;
typedef void (*post_handle_func)(NSCP_ConfigTypeDef*);
post_handle_func post_func_ptr;

void empty_post_handle(NSCP_ConfigTypeDef* conf)
{
	(void)conf;
}

//gap timer config
void rcc_init_func()
{
	__HAL_RCC_TIM4_CLK_ENABLE();
}

TIM_HandleTypeDef htmi_nscp_gap_conf;

DelayCounter_ConfigTypeDef gap_timer = {
	.tim_conf = TIM4,
	.rcc_init_func = rcc_init_func,
	.ir_handle = TIM4_IRQn,
	.tim_handle_func = DC_NULL,
	.htim = &htmi_nscp_gap_conf
};

void TIM4_IRQHandler()
{
	DC_IRQ_Handle_Func(&gap_timer);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance != TIM4) return;
	HAL_TIM_Base_Stop_IT(htim);
}
//gap timer config end

void rcc_init()
{
	__HAL_RCC_DMA1_CLK_ENABLE();
	__HAL_RCC_TIM2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
}

NSCP_ConfigTypeDef nscp_init = {
		.Channel = TIM_CHANNEL_2,
		.one_period = 4,
		.period = 5,//2.5us
		.sampling_period = 2, //1us
		.sda_pin = GPIO_PIN_1,
		.tim_cc_id = TIM_DMA_ID_CC2,
		//.dma_ir_handle = DMA1_Channel7_IRQn,
		.dma_ir_handle = DMA1_Channel2_IRQn,
		//.dma_conf = DMA1_Channel7,
		.dma_conf = DMA1_Channel2,
		.tim_conf = TIM2,
		.sda_gpio_handle = GPIOA,
		.func_handle = rcc_init,
		.gap_timer = &gap_timer,
		
		.pwm_channel_handle = &pwm_ch_conf,
		.dma_handle = &dma_han,
		.pwm_handle = &pwm_conf,
		.pwm_slave_handle = &slav_conf,
		.sda_handle = &gpio_conf
};

void NSCP_Send_Init_Test()
{
	post_func_ptr = empty_post_handle;
	uint16_t dutyCArry[5] = {1, 4, 1, 4, 0};
	NSCP_Sender_Config_Init(&nscp_init);
	NSCP_Sender_Init(&nscp_init);
	HAL_TIM_PWM_Start_DMA(&pwm_conf, TIM_CHANNEL_2, (uint32_t*)dutyCArry, 5);
	while(1);
}

void NSCP_Send_Load_Test()
{
	post_func_ptr = empty_post_handle;
	NSCP_Sender_Config_Init(&nscp_init);
	NSCP_Sender_Init(&nscp_init);
	nscp_init.data = 0x555;
	NSCP_Sender_Load_Data(&nscp_init);
	HAL_TIM_PWM_Start_DMA(&pwm_conf, nscp_init.Channel,
												(uint32_t*)nscp_init.duty_send_buffer, NSCP_MAX_PACK_LEN + 1);
	while(1);
}

void NSCP_Send_Full_Send_Test()
{
	post_func_ptr = NSCP_Sender_Trans_Post_Handle;
	NSCP_Sender_Config_Init(&nscp_init);
	NSCP_Sender_Init(&nscp_init);
	HAL_Delay(30000);
	nscp_init.data = 0x555;
	NSCP_Sender_Load_Data(&nscp_init);
	NSCP_Sender_Wait_For_Trans_Ready_Sync(&nscp_init);
	NSCP_Sender_Trans_Launch(&nscp_init);
	while (NSCP_Sender_Wait_For_Trans_Fin(&nscp_init) == TRANS_NO_FIN) {
		//HAL_Delay(1);
	}
	while(1);
}

void NSCP_Send_Full_Send_Test2()
{
	post_func_ptr = NSCP_Sender_Trans_Post_Handle;
	NSCP_Sender_Config_Init(&nscp_init);
	NSCP_Sender_Init(&nscp_init);
	
	GPIO_InitTypeDef check_port = {
		.Pin = GPIO_PIN_6,
		.Mode = GPIO_MODE_OUTPUT_PP,
		.Pull = GPIO_NOPULL,
		.Speed = GPIO_SPEED_FREQ_HIGH
	};
	HAL_GPIO_Init(GPIOA, &check_port);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
	nscp_init.data = 0x555;
	NSCP_Sender_Load_Data(&nscp_init);
	NSCP_Sender_Wait_For_Trans_Ready_Sync(&nscp_init);
	NSCP_Sender_Trans_Launch(&nscp_init);
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);
	NSCP_Sender_Wait_For_Trans_Fin_Sync(&nscp_init);
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);
	while(1);
}

void NSCP_Send_Full_Send_Test3()
{
	post_func_ptr = NSCP_Sender_Trans_Post_Handle;
	NSCP_Sender_Config_Init(&nscp_init);
	NSCP_Sender_Init(&nscp_init);
	
	GPIO_InitTypeDef check_port = {
		.Pin = GPIO_PIN_6,
		.Mode = GPIO_MODE_OUTPUT_PP,
		.Pull = GPIO_NOPULL,
		.Speed = GPIO_SPEED_FREQ_HIGH
	};
	HAL_GPIO_Init(GPIOA, &check_port);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
	nscp_init.data = 0x555;
	NSCP_Sender_Load_Data(&nscp_init);
	NSCP_Sender_Wait_For_Trans_Ready_Sync(&nscp_init);
	NSCP_Sender_Trans_Launch(&nscp_init);
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);
	NSCP_Sender_Wait_For_Trans_Fin_Sync(&nscp_init);
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);
	nscp_init.data = 0x7ff;
	NSCP_Sender_Load_Data(&nscp_init);
	NSCP_Sender_Wait_For_Trans_Ready_Sync(&nscp_init);
	NSCP_Sender_Trans_Launch(&nscp_init);
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);
	NSCP_Sender_Wait_For_Trans_Fin_Sync(&nscp_init);
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);
	nscp_init.data = 0x000;
	NSCP_Sender_Load_Data(&nscp_init);
	NSCP_Sender_Wait_For_Trans_Ready_Sync(&nscp_init);
	NSCP_Sender_Trans_Launch(&nscp_init);
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);
	NSCP_Sender_Wait_For_Trans_Fin_Sync(&nscp_init);
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);
	while(1);
}
//test area----------------------------------------------------
DMA_HandleTypeDef rx_dma_hd = {
	.Instance = DMA1_Channel6,
	.Init.Direction = DMA_MEMORY_TO_MEMORY,
	.Init.PeriphInc = DMA_PINC_DISABLE,
	.Init.MemInc = DMA_MINC_ENABLE,
	.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD,
	.Init.MemDataAlignment = DMA_PDATAALIGN_HALFWORD,
	.Init.Mode = DMA_NORMAL,
	.Init.Priority = DMA_PRIORITY_HIGH
};
TIM_HandleTypeDef rx_timer_hd = {
	.Instance = TIM3,
	.Init.Prescaler = 36 - 1,
	.Init.CounterMode = TIM_COUNTERMODE_UP,
	.Init.Period = 500 - 1,
	.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1,
	.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE
};
//TIM3_CH2 -- PA7
TIM_SlaveConfigTypeDef slave_conf_hd = {
	.SlaveMode = TIM_SLAVEMODE_TRIGGER,
	.InputTrigger = TIM_TS_TI2FP2,
	.TriggerPolarity = TIM_INPUTCHANNELPOLARITY_RISING,
	.TriggerFilter = 0x0
};
int countSize = 0;
void TIM3_IRQHandler()
{
	HAL_TIM_IRQHandler(&rx_timer_hd);
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);
	if (countSize < 4) {
		countSize++;
	} else {
		HAL_TIM_Base_Stop_IT(&rx_timer_hd);
	}
}

void GPIO_DMA_FUNC_TEST()
{
	int size = 16;
	uint16_t tmp;
	char title[16] = {0};
	//init OLED
	IIC1_Init(0, 0);
	SetIIC_Comm_Mode(1);//Polling mode
	OLED_Init();
	OLED_TurnOn_Screen();
	OLED_Flash_Screen(0x00);
	//dma init
	__HAL_RCC_DMA1_CLK_ENABLE();
	HAL_DMA_Init(&rx_dma_hd);
	
	//gpio init
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef io_config = {
		.Pin = GPIO_PIN_6,
		.Mode = GPIO_MODE_OUTPUT_PP,
		.Speed = GPIO_SPEED_FREQ_HIGH,
		.Pull = GPIO_NOPULL
	};
	HAL_GPIO_Init(GPIOA, &io_config);
	
	//test code
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
	HAL_DMA_Start(&rx_dma_hd, (uint32_t)(&GPIOA->ODR), (uint32_t)(&tmp), 1);
	for (int i = 0; i < size; i++) {
		if (((1 << i) & tmp) == 0) title[i] = '0';
		else title[i] = '1';
	}
	OLED_WriteIn_16x8String(0, 0, 16, (unsigned char*)title);
}

void TIMER_FUNC_TEST()
{
	__HAL_RCC_TIM3_CLK_ENABLE();
	//gpio init
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef io_config = {
		.Pin = GPIO_PIN_6,
		.Mode = GPIO_MODE_OUTPUT_PP,
		.Speed = GPIO_SPEED_FREQ_HIGH,
		.Pull = GPIO_NOPULL
	};
	HAL_GPIO_Init(GPIOA, &io_config);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
	//NVIC Init
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);
	HAL_NVIC_SetPriority(TIM3_IRQn, 1, 1);
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
	//timer init
	HAL_TIM_Base_Init(&rx_timer_hd);
	//test code
	countSize = 0;
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);
	rx_timer_hd.Instance->CNT = 300 - 1;//300 - 1;
	__HAL_TIM_CLEAR_IT(&rx_timer_hd, TIM_IT_UPDATE);
	HAL_TIM_Base_Start_IT(&rx_timer_hd);
}

void TRIGGER_FUNC_TEST()
{
	__HAL_RCC_TIM3_CLK_ENABLE();
	
	//gpio init
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef io_config = {
		.Pin = GPIO_PIN_6,
		.Mode = GPIO_MODE_OUTPUT_PP,
		.Speed = GPIO_SPEED_FREQ_HIGH,
		.Pull = GPIO_NOPULL
	};
	HAL_GPIO_Init(GPIOA, &io_config);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
	GPIO_InitTypeDef GPIO_EX_conf = {
		.Pin = GPIO_PIN_7,
		.Mode = GPIO_MODE_AF_INPUT,
		.Pull = GPIO_NOPULL,
		.Speed = GPIO_SPEED_FREQ_HIGH
	};
	HAL_GPIO_Init(GPIOA, &GPIO_EX_conf);
	//NVIC Init
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);
	HAL_NVIC_SetPriority(TIM3_IRQn, 1, 1);
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
	
	//timer init
	HAL_TIM_Base_Init(&rx_timer_hd);
	//timer slave mode init
	HAL_TIM_SlaveConfigSynchro(&rx_timer_hd, &slave_conf_hd);
	
	//test code
	countSize = 0;
	rx_timer_hd.Instance->CNT = 300 - 1;//300 - 1;
	__HAL_TIM_CLEAR_IT(&rx_timer_hd, TIM_IT_UPDATE);
	HAL_TIM_Base_Start_IT(&rx_timer_hd);
	while(1);
}

void NSCP_Recv_Init_Test()
{
	HAL_Delay(1000);
	//datas
	int size = 16;
	char title[16] = {0};
	uint16_t tmp;
	uint16_t std_res = 0x05;
	
	//init OLED
	IIC1_Init(0, 0);
	SetIIC_Comm_Mode(1);//Polling mode
	OLED_Init();
	OLED_TurnOn_Screen();
	OLED_Flash_Screen(0x00);
	
	
	/*gpio-dma test
	GPIO_DMA_FUNC_TEST();
	*/
	
	/*
	timer-test
	TIMER_FUNC_TEST();
	*/
	
	/*
	external-triggier-test
	TRIGGER_FUNC_TEST();
	*/
	NSCP_Recv_Config_Init(&nscp_init);
	NSCP_Recv_Init(&nscp_init);
	NSCP_Recv_Start(&nscp_init);
	while (nscp_init.tmp_status != NSCP_TRANS_FIN);
	for (unsigned char i = 0; i < 11; i++) {
		if (((1 << i) & nscp_init.data) != 0) title[i] = '1';
		else title[i] = '0';
	}
	OLED_WriteIn_16x8String(0, 0, 11, (unsigned char*)title);
	//stuck
	while(1);
}
//test area end----------------------------------------------------

//---------------------merge test----------------------------------

#define PACKS_NUM 2000

NSCP_ConfigTypeDef merge_send_handle = {
		.Channel = TIM_CHANNEL_2,
		.one_period = 4,
		.period = 5,//2.5us
		.sampling_period = 2, //1us
		.sda_pin = GPIO_PIN_1,
		.tim_cc_id = TIM_DMA_ID_CC2,
		.dma_ir_handle = DMA1_Channel7_IRQn,
		.dma_conf = DMA1_Channel7,
		.tim_conf = TIM2,
		.sda_gpio_handle = GPIOA,
		.func_handle = rcc_init,
		.gap_timer = &gap_timer,
		
		.pwm_channel_handle = &pwm_ch_conf,
		.dma_handle = &dma_han,
		.pwm_handle = &pwm_conf,
		.pwm_slave_handle = &slav_conf,
		.sda_handle = &gpio_conf
};

void Merge_Test_Send()
{
	
	//init OLED
	IIC1_Init(0, 0);
	SetIIC_Comm_Mode(1);//Polling mode
	OLED_Init();
	OLED_TurnOn_Screen();
	OLED_Flash_Screen(0x00);
	char title1[] = "Sending...";
	char title2[] = "Fin";
	post_func_ptr = NSCP_Sender_Trans_Post_Handle;
	NSCP_Sender_Config_Init(&merge_send_handle);
	NSCP_Sender_Init(&merge_send_handle);
	HAL_Delay(10000);
	merge_send_handle.data = 0x0001;
	OLED_WriteIn_16x8String(0, 0, 10, (unsigned char*)title1);
	for (int i = 0; i < PACKS_NUM; i++) {
		NSCP_Sender_Load_Data(&merge_send_handle);
		NSCP_Sender_Wait_For_Trans_Ready_Sync(&merge_send_handle);
		NSCP_Sender_Trans_Launch(&merge_send_handle);
		NSCP_Sender_Wait_For_Trans_Fin_Sync(&merge_send_handle);
		merge_send_handle.data += 1;
	}
	OLED_WriteIn_16x8String(10, 0, 3, (unsigned char*)title2);
	while(1);
}

NSCP_ConfigTypeDef merge_recv_handle = {
		.Channel = TIM_CHANNEL_2,
		.one_period = 4,
		.period = 5,//2.5us
		.sampling_period = 2, //1us
		.sda_pin = GPIO_PIN_1,
		.tim_cc_id = TIM_DMA_ID_CC2,
		.dma_ir_handle = DMA1_Channel2_IRQn,
		.dma_conf = DMA1_Channel2,
		.tim_conf = TIM2,
		.sda_gpio_handle = GPIOA,
		.func_handle = rcc_init,
		.gap_timer = &gap_timer,
		
		.pwm_channel_handle = &pwm_ch_conf,
		.dma_handle = &dma_han,
		.pwm_handle = &pwm_conf,
		.pwm_slave_handle = &slav_conf,
		.sda_handle = &gpio_conf
};
uint16_t data_cache_t[PACKS_NUM + 1];
void Merge_Test_Recv()
{
	HAL_Delay(1000);
	//datas
	char title[] = "Wrong at %";
	char title2[] = "Check Fin";
	uint16_t tmp = 0x0000;
	uint16_t cache = 0x0000;
	int piv = 0;
	//init OLED
	IIC1_Init(0, 0);
	SetIIC_Comm_Mode(1);//Polling mode
	OLED_Init();
	OLED_TurnOn_Screen();
	OLED_Flash_Screen(0x00);
	
	//nscp init
	NSCP_Recv_Config_Init(&merge_recv_handle);
	NSCP_Recv_Init(&merge_recv_handle);
	NSCP_Recv_Start(&merge_recv_handle);
	while(1) {
		cache = NSCP_Recv_Get_Data(&merge_recv_handle);
		if (tmp != cache) {
			tmp = cache;
			data_cache_t[piv++] = tmp;
		}
		if (piv >= PACKS_NUM) break;
		for (int i = 0; i < 100; i++);
	}
	tmp = 0x0001;
	for (int i = 0; i < PACKS_NUM; i++) {
		if (tmp != data_cache_t[i]) {
			title[9] = i + '0';
			OLED_WriteIn_16x8String(0, 0, 10, (unsigned char*)title);
			break;
		} else tmp++;
	}
	OLED_WriteIn_16x8String(0, 1, 9, (unsigned char*)title2);
	while(1);
}

//---------------------merge test end------------------------------

void DMA1_Channel7_IRQHandler()
{
	HAL_DMA_IRQHandler(&dma_han);
}

void DMA1_Channel2_IRQHandler()
{
	HAL_DMA_IRQHandler(&dma_han);
	NSCP_Recv_Trans_Post_Handle(&merge_recv_handle);
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Instance == TIM2)
  {
    HAL_TIM_PWM_Stop_DMA(&pwm_conf, TIM_CHANNEL_2);
		post_func_ptr(&merge_send_handle);
  }
}

#endif

#ifdef DELAY_COUNTER_TEST

void rcc_init_func()
{
	__HAL_RCC_TIM4_CLK_ENABLE();
}

void count_up_func()
{
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);
}

TIM_HandleTypeDef httmi;

DelayCounter_ConfigTypeDef conf = {
	.tim_conf = TIM4,
	.rcc_init_func = rcc_init_func,
	.ir_handle = TIM4_IRQn,
	.tim_handle_func = count_up_func,
	
	.htim = &httmi
};

void DC_COUNT_Test()
{
	//gpio init
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef gpio_conf = {
		.Pin = GPIO_PIN_6,
		.Mode = GPIO_MODE_OUTPUT_PP,
		.Pull = GPIO_NOPULL,
		.Speed = GPIO_SPEED_FREQ_HIGH
	};
	HAL_GPIO_Init(GPIOA, &gpio_conf);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
	//delay counter init
	DC_Init(&conf);
	
	//test
	conf.period = 7;//7us
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
	DC_Delay(&conf);
	__DSB();
	while(1);
}

void DC_Repeat_Count_Test1()
{
	//gpio init
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef gpio_conf = {
		.Pin = GPIO_PIN_6,
		.Mode = GPIO_MODE_OUTPUT_PP,
		.Pull = GPIO_NOPULL,
		.Speed = GPIO_SPEED_FREQ_HIGH
	};
	HAL_GPIO_Init(GPIOA, &gpio_conf);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
	//delay counter init
	DC_Init(&conf);
	
	//test
	conf.period = 10;
	for (int i = 0; i < 10; ++i) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
		DC_Delay(&conf);
		DC_Wait_Sync(&conf);
	}
	while(1);
}

void DC_Repeat_Count_Test2()
{
	//gpio init
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef gpio_conf = {
		.Pin = GPIO_PIN_6,
		.Mode = GPIO_MODE_OUTPUT_PP,
		.Pull = GPIO_NOPULL,
		.Speed = GPIO_SPEED_FREQ_HIGH
	};
	HAL_GPIO_Init(GPIOA, &gpio_conf);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
	//delay counter init
	DC_Init(&conf);
	
	//test
	conf.period = 7;//start from 7us
	for (int i = 0; i < 10; ++i) {
		conf.period += 1;
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
		DC_Delay(&conf);
		DC_Wait_Sync(&conf);
	}
	while(1);
}

void DC_Repeat_Count_Test3()
{
	//gpio init
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef gpio_conf = {
		.Pin = GPIO_PIN_6,
		.Mode = GPIO_MODE_OUTPUT_PP,
		.Pull = GPIO_NOPULL,
		.Speed = GPIO_SPEED_FREQ_HIGH
	};
	HAL_GPIO_Init(GPIOA, &gpio_conf);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
	//delay counter init
	DC_Init(&conf);
	
	//test
	conf.period = 7;//start from 7us
	for (int i = 0; i < 10; ++i) {
		conf.period += 1;
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
		DC_Delay(&conf);
		while(DC_Wait(&conf) == DC_BUZY);
	}
	while(1);
}

void DC_Repeat_Count_Test4()
{
	//gpio init
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef gpio_conf = {
		.Pin = GPIO_PIN_6,
		.Mode = GPIO_MODE_OUTPUT_PP,
		.Pull = GPIO_NOPULL,
		.Speed = GPIO_SPEED_FREQ_HIGH
	};
	HAL_GPIO_Init(GPIOA, &gpio_conf);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
	
	//delay counter init
	DC_Init(&conf);
	
	//test
	conf.period = 8; // standard gap is 8us
	for (int i = 1; i <= 4; ++i) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
		DC_Delay(&conf);
		while(DC_Wait(&conf) == DC_BUZY);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
		for (int j = 0; j < 50; j++);
	}
}

void DC_Repeat_Count_Test5()
{
	//gpio init
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef gpio_conf = {
		.Pin = GPIO_PIN_6,
		.Mode = GPIO_MODE_OUTPUT_PP,
		.Pull = GPIO_NOPULL,
		.Speed = GPIO_SPEED_FREQ_HIGH
	};
	HAL_GPIO_Init(GPIOA, &gpio_conf);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
	
	//delay counter init
	DC_Init(&conf);
	
	//test
	conf.period = 8; // standard gap is 8us
	for (int i = 1; i <= 4; ++i) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
		DC_Delay(&conf);
		DC_Wait_Sync(&conf);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
		for (int j = 0; j < 50; j++);
	}
}

void TIM4_IRQHandler()
{
	DC_IRQ_Handle_Func(&conf);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	HAL_TIM_Base_Stop_IT(htim);
}
#endif
//----------------------------------------------------
