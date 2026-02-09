#include "test.h"
//----------------------------------------------------
#ifdef NRF2401_TEST
#ifdef NRF2401_SENDER_TEST
void NRF2401_Sender()
{

}
#endif

#ifdef NRF2401_RECEIVER_TEST

void PrintAndCheckReg()
{
	int pos = 0;
	int line = 0;
	//orders
	unsigned char recData[2];
	char valueStr[2];
	//show STATUS and CONFIG
	NRF2401_Get_Reg(0x00, recData);
	TransNum2StringWOS(recData[0], valueStr);
	OLED_WriteIn_16x8String(pos, line, 2, (unsigned char *)valueStr);
	pos += 3;

	TransNum2StringWOS(recData[1], valueStr);
	OLED_WriteIn_16x8String(pos, line, 2, (unsigned char *)valueStr);
	pos += 3;
	
	//show ENAA
	NRF2401_Get_Reg(0x01, recData);
	TransNum2StringWOS(recData[1], valueStr);
	OLED_WriteIn_16x8String(pos, line, 2, (unsigned char *)valueStr);
	pos += 3;
	
	//show EN_RXADDR
	NRF2401_Get_Reg(0x02, recData);
	TransNum2StringWOS(recData[1], valueStr);
	OLED_WriteIn_16x8String(pos, line, 2, (unsigned char *)valueStr);
	pos += 3;
	
	//show SETUP_AW
	NRF2401_Get_Reg(0x03, recData);
	TransNum2StringWOS(recData[1], valueStr);
	OLED_WriteIn_16x8String(pos, line, 2, (unsigned char *)valueStr);
	pos += 3;
	
	//show SETUP_RETR
	NRF2401_Get_Reg(0x04, recData);
	TransNum2StringWOS(recData[1], valueStr);
	OLED_WriteIn_16x8String(pos, line, 2, (unsigned char *)valueStr);
	pos += 3;
	
	pos = 2;
	
	//show FIFO_STATUS
	NRF2401_Get_Reg(0x17, recData);
	TransNum2StringWOS(recData[1], valueStr);
	OLED_WriteIn_16x8String(pos, line + 1, 2, (unsigned char *)valueStr);
	pos += 3;
	
	//show FEATURE
	NRF2401_Get_Reg(0x1d, recData);
	TransNum2StringWOS(recData[1], valueStr);
	OLED_WriteIn_16x8String(pos, line + 1, 2, (unsigned char *)valueStr);
	pos += 3;
	
}

void NRF2401_Receiver()
{
	//test data
	char title[] = "Receive:";
	char recv_bytes[2] = {'#', '#'};
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
	PrintAndCheckReg();
	//NRF2401_Recv_Mode();
	
	//main loop
	while (1) {
		//res = NRF2401_Recv_Wait((unsigned char*)recv_bytes);
		//if (res == 1) OLED_WriteIn_16x8String(8, 0, 2, (unsigned char*)recv_bytes);
	}
	
	//test code stop----------------------------
}

#endif
#endif
//----------------------------------------------------