#include "test.h"
//----------------------------------------------------
#ifdef NRF2401_TEST
#ifdef NRF2401_SENDER_TEST
void NRF2401_Sender()
{

}
#endif

#ifdef NRF2401_RECEIVER_TEST

void PrintReg()
{
	//orders
	unsigned char orders[2];
	unsigned char recData[2];
	
	//show STATUS and CONFIG
	
	//show ENAA
	
	//show EN_RXADDR
	
	//show SETUP_AW
	
	//show SETUP_RETR
	
	//show FIFO_STATUS

	//show FEATURE
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
	OLED_WriteIn_16x8String(0, 0, 8, (unsigned char *)title);
	NRF2401_Start();
	NRF2401_Recv_Mode();
	
	//main loop
	while (1) {
		res = NRF2401_Recv_Wait((unsigned char*)recv_bytes);
		if (res == 1) OLED_WriteIn_16x8String(8, 0, 2, (unsigned char*)recv_bytes);
	}
	
	//test code stop----------------------------
}

#endif
#endif
//----------------------------------------------------