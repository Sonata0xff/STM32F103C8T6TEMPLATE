#include "bsp_config.h"

#ifdef NRF2401_API_EN
#include "nrf2401.h"
#include "OLED.h"
#include "utils.h"
static int CSN_ORDER = 0;
unsigned char comm_addr[3] = {0xaa, 0xfe, 0xaa};//wait for check
static int data_len = 2;//Bytes
enum CommStatus comm_status = CommStatus_Idle; //init value
AtomVarType NRF_SEND;//RESET means send finish 

void NRF2401_Init(int CS_Line)
{
	//init IRQ source
	Input_Port_IT_Init(NRF2401_IRQ_Handler);
	//init CE source
	GPIO_InitTypeDef ce_config = {
		.Mode = GPIO_MODE_OUTPUT_PP,
		.Pin = GPIO_PIN_2,
		.Pull = GPIO_NOPULL,
		.Speed = GPIO_SPEED_FREQ_HIGH
	};
	HAL_GPIO_Init(GPIOA, &ce_config);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
	//init CSN
	CSN_ORDER = CS_Line;
	HAL_Delay(100);
	comm_status = CommStatus_Shutdown;
}

void NRF2401_Start()
{
	//data store variable
	unsigned char orders[] = {0x00, 0x00};
	unsigned char recData[] = {0x00, 0x00};
	
	//reset STATUS, first time STATUS may be wrong(FUCK YOU SI24R1!!!)
	ISPI1_SelectDevice(CSN_ORDER);
	ISPI1_SenRecBytes(orders, recData, 2);
	ISPI_Comm_Block_Wait();
	ISPI1_UnSelectDevice(CSN_ORDER);
	orders[0] = 0x27;
	orders[1] = recData[0];
	ISPI1_SelectDevice(CSN_ORDER);
	ISPI1_SendBytes(orders, 2);
	ISPI_Comm_Block_Wait();
	ISPI1_UnSelectDevice(CSN_ORDER);
	
	
	
	//read reg0
	orders[0] = 0x00;
	orders[1] = 0x00;
	ISPI1_SelectDevice(CSN_ORDER);
	ISPI1_SenRecBytes(orders, recData, 2);
	ISPI_Comm_Block_Wait();
	ISPI1_UnSelectDevice(CSN_ORDER);
	
	//Set PWR_UP to 1, transport into standby-I
	orders[0] = 0x20;
	orders[1] = (recData[1] | 0x02);
	ISPI1_SelectDevice(CSN_ORDER);
	ISPI1_SendBytes(orders, 2);
	ISPI_Comm_Block_Wait();
	ISPI1_UnSelectDevice(CSN_ORDER);
	
	HAL_Delay(2);
	
	comm_status = CommStatus_Standby;
	
	//test func
	/*orders[0] = 0x00;
	orders[1] = 0x00;
	ISPI1_SelectDevice(CSN_ORDER);
	ISPI1_SenRecBytes(orders, recData, 2);
	ISPI_Comm_Block_Wait();
	ISPI1_UnSelectDevice(CSN_ORDER);
	char tmp[4] = {'#', '#', '#', '#'};
	for (int i = 0; i < 2; i++) {
		TransNum2String(recData[i], tmp);
		OLED_WriteIn_16x8String(i*4, 0, 4, (unsigned char*)tmp);
	}*/
}

void NRF2401_Stop()
{
	//Pull down CE
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
	
	//order variable
	unsigned char orders[] = {0x00, 0x00};
	unsigned char recData[] = {0x00, 0x00};

	//read reg0
	ISPI1_SelectDevice(CSN_ORDER);
	ISPI1_SenRecBytes(orders, recData, 2);
	ISPI_Comm_Block_Wait();
	ISPI1_UnSelectDevice(CSN_ORDER);
	
	//Set PWR_UP to 0, transport into shutdown mode.
	orders[0] = 0x20;
	orders[1] = (recData[1] & (~0x02));
	ISPI1_SelectDevice(CSN_ORDER);
	ISPI1_SendBytes(orders, 2);
	ISPI_Comm_Block_Wait();
	ISPI1_UnSelectDevice(CSN_ORDER);
	
	//Change status
	comm_status = CommStatus_Shutdown;
	
	
	//test func
	/*orders[0] = 0x00;
	orders[1] = 0x00;
	ISPI1_SelectDevice(CSN_ORDER);
	ISPI1_SenRecBytes(orders, recData, 2);
	ISPI_Comm_Block_Wait();
	ISPI1_UnSelectDevice(CSN_ORDER);
	char tmp[4] = {'#', '#', '#', '#'};
	for (int i = 0; i < 2; i++) {
		TransNum2String(recData[i], tmp);
		OLED_WriteIn_16x8String(i*4, 0, 4, (unsigned char*)tmp);
	}*/
}

void NRF2401_Send_Mode()
{
	
	//order variable
	unsigned char orders[] = {0x00, 0x00, 0x00, 0x00};
	unsigned char recData[] = {0x00, 0x00};
	
	//set PRIM_RX to 0
	ISPI1_SelectDevice(CSN_ORDER);
	ISPI1_SenRecBytes(orders, recData, 2);
	ISPI_Comm_Block_Wait();
	ISPI1_UnSelectDevice(CSN_ORDER);
	orders[0] = 0x20;
	orders[1] = (recData[1] & (~0x01));
	ISPI1_SelectDevice(CSN_ORDER);
	ISPI1_SendBytes(orders, 2);
	ISPI_Comm_Block_Wait();
	ISPI1_UnSelectDevice(CSN_ORDER);
	
	//set addr len=3
	orders[0] = 0x23;
	orders[1] = 0x01;
	ISPI1_SelectDevice(CSN_ORDER);
	ISPI1_SendBytes(orders, 2);
	ISPI_Comm_Block_Wait();
	ISPI1_UnSelectDevice(CSN_ORDER);
	
	//set addr
	orders[0] = 0x2a;
	orders[1] = comm_addr[0];
	orders[2] = comm_addr[1];
	orders[3] = comm_addr[2];
	ISPI1_SelectDevice(CSN_ORDER);
	ISPI1_SendBytes(orders, 4);
	ISPI_Comm_Block_Wait();
	ISPI1_UnSelectDevice(CSN_ORDER);
	orders[0] = 0x30;
	ISPI1_SelectDevice(CSN_ORDER);
	ISPI1_SendBytes(orders, 4);
	ISPI_Comm_Block_Wait();
	ISPI1_UnSelectDevice(CSN_ORDER);
	
	//init atom var
	Atom_Write(&NRF_SEND, ATOM_VALUE_RESET);
	
	//transport into send mode
	comm_status = CommStatus_Send;
	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET); //CE = 1
	
}

void NRF2401_Send(unsigned char* datas)
{
	//orders
	unsigned char orders[data_len + 1];
	//flash tx cache
	orders[0] = 0xe1;
	ISPI1_SelectDevice(CSN_ORDER);
	ISPI1_SendBytes(orders, 1);
	ISPI_Comm_Block_Wait();
	ISPI1_UnSelectDevice(CSN_ORDER);
	//atom set
	Atom_Write(&NRF_SEND, ATOM_VALUE_SET);
	//write in data
	orders[0] = 0xa0;
	for (int i = 0; i < data_len; ++i) {
		orders[i + 1] = datas[i];
	}
	ISPI1_SelectDevice(CSN_ORDER);
	ISPI1_SendBytes(orders, data_len + 1);
	ISPI_Comm_Block_Wait();
	ISPI1_UnSelectDevice(CSN_ORDER);
	//CE = 1, launch!
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
}

void NRF2401_Send_FinHandle()
{
	//wait for coding ...
}

void NRF2401_Recv_Mode()
{
	//wait for coding ...
	
}

void NRF2401_Recv(unsigned char* datas)
{
	//wait for coding ...
}

void NRF2401_Recv_FinHandle()
{
	//wait for coding ...
}

void NRF2401_Revert_Standby()
{
	//wait for coding ...
}

void NRF2401_IRQ_Handler()
{
	//wait for coding ...
}

#endif