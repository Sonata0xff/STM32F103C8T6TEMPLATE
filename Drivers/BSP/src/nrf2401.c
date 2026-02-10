#include "bsp_config.h"

#ifdef NRF2401_API_EN
#include "nrf2401.h"
static int CSN_ORDER = 0;
unsigned char comm_addr[3] = {0xaa, 0xfe, 0xac};//wait for check
static int data_len = 2;//Bytes
enum CommStatus comm_status = CommStatus_Dummy; //init value
AtomVarType NRF_SEND;//RESET means no send
AtomVarType NRF_RECV;//RESET means no receive

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
	unsigned char orders[] = {0x00, 0x00, 0x00, 0x00};
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
	
	//set addr len=3
	orders[0] = 0x23;
	orders[1] = 0x01;
	ISPI1_SelectDevice(CSN_ORDER);
	ISPI1_SendBytes(orders, 2);
	ISPI_Comm_Block_Wait();
	ISPI1_UnSelectDevice(CSN_ORDER);
	
	//set addr
	orders[1] = comm_addr[0];
	orders[2] = comm_addr[1];
	orders[3] = comm_addr[2];
	orders[0] = 0x2a;
	ISPI1_SelectDevice(CSN_ORDER);
	ISPI1_SendBytes(orders, 4);
	ISPI_Comm_Block_Wait();
	ISPI1_UnSelectDevice(CSN_ORDER);
	orders[0] = 0x30;
	ISPI1_SelectDevice(CSN_ORDER);
	ISPI1_SendBytes(orders, 4);
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
}

void NRF2401_Send_Mode()
{
	
	//order variable
	unsigned char orders[] = {0x00, 0x00};
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
	
	//flash tx cache
	orders[0] = 0xe1;
	ISPI1_SelectDevice(CSN_ORDER);
	ISPI1_SendBytes(orders, 1);
	ISPI_Comm_Block_Wait();
	ISPI1_UnSelectDevice(CSN_ORDER);
	
	//CE = 1
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
	
	//atom set
	Atom_Write(&NRF_SEND, ATOM_VALUE_RESET);
	
	//transport into send mode
	comm_status = CommStatus_Send;
}

void NRF2401_Send(unsigned char* datas)
{
	
	//orders
	unsigned char orders[data_len + 1];
	
	//write data to tx
	orders[0] = 0xa0;
	for (int i = 0; i < data_len; ++i) {
		orders[i + 1] = datas[i];
	}
	ISPI1_SelectDevice(CSN_ORDER);
	ISPI1_SendBytes(orders, data_len + 1);
	ISPI_Comm_Block_Wait();
	ISPI1_UnSelectDevice(CSN_ORDER);
	
	//set atom
	Atom_Write(&NRF_SEND, ATOM_VALUE_SET);
	
}

void NRF2401_Revert_Standby()
{
	//orders
	unsigned char orders[] = {0x00, 0x00};
	unsigned char recData[] = {0x00, 0x00};
	//CE = 0
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
	
	//reset status
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
	
	//change comm_status
	comm_status = CommStatus_Standby;
}

void NRF2401_Send_Block_Wait()
{
	//orders
	unsigned char orders[] = {0x00, 0x00};
	unsigned char recData[] = {0x00, 0x00};
	
	//block wait
	while(Atom_Read(&NRF_SEND) == ATOM_VALUE_SET);
	
	//read status
	ISPI1_SelectDevice(CSN_ORDER);
	ISPI1_SenRecBytes(orders, recData, 2);
	ISPI_Comm_Block_Wait();
	ISPI1_UnSelectDevice(CSN_ORDER);
	
	//handle
	if ((recData[0] & 0x20) != 0x00) Send_Fin_Handle();
	if ((recData[0] & 0x10) != 0x00) Send_Time_Out_Handle();
	
	//flash tx cache
	orders[0] = 0xe1;
	ISPI1_SelectDevice(CSN_ORDER);
	ISPI1_SendBytes(orders, 1);
	ISPI_Comm_Block_Wait();
	ISPI1_UnSelectDevice(CSN_ORDER);
	
	//reset status
	orders[0] = 0x27;
	orders[1] = 0x30;
	ISPI1_SelectDevice(CSN_ORDER);
	ISPI1_SendBytes(orders, 2);
	ISPI_Comm_Block_Wait();
	ISPI1_UnSelectDevice(CSN_ORDER);
}

uint8_t NRF2401_Send_Wait()
{
	//orders
	unsigned char orders[] = {0x00, 0x00};
	unsigned char recData[] = {0x00, 0x00};
	
	//read atom
	if (Atom_Read(&NRF_SEND) == ATOM_VALUE_SET) return 0;
	
	//read status
	ISPI1_SelectDevice(CSN_ORDER);
	ISPI1_SenRecBytes(orders, recData, 2);
	ISPI_Comm_Block_Wait();
	ISPI1_UnSelectDevice(CSN_ORDER);
	
	//handle
	if ((recData[0] & 0x20) != 0x00) Send_Fin_Handle();
	if ((recData[0] & 0x10) != 0x00) Send_Time_Out_Handle();
	
	//flash tx cache
	orders[0] = 0xe1;
	ISPI1_SelectDevice(CSN_ORDER);
	ISPI1_SendBytes(orders, 1);
	ISPI_Comm_Block_Wait();
	ISPI1_UnSelectDevice(CSN_ORDER);
	
	//reset status
	orders[0] = 0x27;
	orders[1] = 0x30;
	ISPI1_SelectDevice(CSN_ORDER);
	ISPI1_SendBytes(orders, 2);
	ISPI_Comm_Block_Wait();
	ISPI1_UnSelectDevice(CSN_ORDER);
	
	return 1;
}

//interface
void Send_Time_Out_Handle() {}
void Send_Fin_Handle() {}

void NRF2401_Recv_Mode()
{
	//orders
	unsigned char orders[] = {0x00, 0x00};
	unsigned char recData[] = {0x00, 0x00};
	
	//set PRIM_RX to 1
	ISPI1_SelectDevice(CSN_ORDER);
	ISPI1_SenRecBytes(orders, recData, 2);
	ISPI_Comm_Block_Wait();
	ISPI1_UnSelectDevice(CSN_ORDER);
	orders[0] = 0x20;
	orders[1] = (recData[1] | 0x01);
	ISPI1_SelectDevice(CSN_ORDER);
	ISPI1_SendBytes(orders, 2);
	ISPI_Comm_Block_Wait();
	ISPI1_UnSelectDevice(CSN_ORDER);
	
	//flash rx
	orders[0] = 0xe2;
	ISPI1_SelectDevice(CSN_ORDER);
	ISPI1_SendBytes(orders, 1);
	ISPI_Comm_Block_Wait();
	ISPI1_UnSelectDevice(CSN_ORDER);
	
	//CE = 1
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
	
	//set atom
	Atom_Write(&NRF_RECV, ATOM_VALUE_RESET);
	
	//set comm_status
	comm_status = CommStatus_Receive;
}

void NRF2401_IRQ_Handler()
{
	if (comm_status == CommStatus_Send) Atom_Write(&NRF_SEND, ATOM_VALUE_RESET);
	if (comm_status == CommStatus_Receive) Atom_Write(&NRF_RECV, ATOM_VALUE_SET);
}


void NRF2401_Recv_Block_Wait(unsigned char* datas)
{
	//orders
	unsigned char orders[data_len + 1];
	unsigned char recData[data_len + 1];
	
	//block wait
	while(Atom_Read(&NRF_RECV) == ATOM_VALUE_RESET);
	
	//read datas
	orders[0] = 0x61;
	for (int i = 0; i < data_len; ++i) {
		orders[i + 1] = 0x00;
	}
	ISPI1_SelectDevice(CSN_ORDER);
	ISPI1_SenRecBytes(orders, recData, data_len + 1);
	ISPI_Comm_Block_Wait();
	ISPI1_UnSelectDevice(CSN_ORDER);
	for (int i = 0; i < data_len; ++i) {
		datas[i] = recData[i + 1];
	}
	
	//flash rx
	orders[0] = 0xe2;
	ISPI1_SelectDevice(CSN_ORDER);
	ISPI1_SendBytes(orders, 1);
	ISPI_Comm_Block_Wait();
	ISPI1_UnSelectDevice(CSN_ORDER);
	
	//handle
	NRF2401_Recv_Handle();
	
	//reset status
	orders[0] = 0x27;
	orders[1] = 0x40;
	ISPI1_SelectDevice(CSN_ORDER);
	ISPI1_SendBytes(orders, 2);
	ISPI_Comm_Block_Wait();
	ISPI1_UnSelectDevice(CSN_ORDER);
	
	//set atom
	Atom_Write(&NRF_RECV, ATOM_VALUE_RESET);
}

uint8_t NRF2401_Recv_Wait(unsigned char* datas)
{
	//orders
	unsigned char orders[data_len + 1];
	unsigned char recData[data_len + 1];
	
	//check
	if (Atom_Read(&NRF_RECV) == ATOM_VALUE_RESET) return 0;
	
	//read datas
	orders[0] = 0x61;
	for (int i = 0; i < data_len; ++i) {
		orders[i + 1] = 0x00;
	}
	ISPI1_SelectDevice(CSN_ORDER);
	ISPI1_SenRecBytes(orders, recData, data_len + 1);
	ISPI_Comm_Block_Wait();
	ISPI1_UnSelectDevice(CSN_ORDER);
	for (int i = 0; i < data_len; ++i) {
		datas[i] = recData[i + 1];
	}
	
	//flash rx
	orders[0] = 0xe2;
	ISPI1_SelectDevice(CSN_ORDER);
	ISPI1_SendBytes(orders, 1);
	ISPI_Comm_Block_Wait();
	ISPI1_UnSelectDevice(CSN_ORDER);
	
	//handle
	NRF2401_Recv_Handle();
	
	//reset status
	orders[0] = 0x27;
	orders[1] = 0x40;
	ISPI1_SelectDevice(CSN_ORDER);
	ISPI1_SendBytes(orders, 2);
	ISPI_Comm_Block_Wait();
	ISPI1_UnSelectDevice(CSN_ORDER);
	
	//set atom
	Atom_Write(&NRF_RECV, ATOM_VALUE_RESET);
	
	return 1;
}

void NRF2401_Get_Reg(unsigned char addr, unsigned char *res, unsigned char size)
{
	unsigned char orders[size];
	//read data
	orders[0] = addr;
	for (unsigned char i = 1; i < size; ++i) orders[i] = 0x00;
	ISPI1_SelectDevice(CSN_ORDER);
	ISPI1_SenRecBytes(orders, res, size);
	ISPI_Comm_Block_Wait();
	ISPI1_UnSelectDevice(CSN_ORDER);
}

//interface
void NRF2401_Recv_Handle() {}
#endif