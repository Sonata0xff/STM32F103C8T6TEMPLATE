#include "stm32f1xx.h"                  // Device header
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_conf.h"
#include "IIC.h"
#include "OLED.h"
#include "ISPI.h"
#include "nrf2401.h"
#include "utils.h"
#include "GPIO_EXIT.h"
#include "test.h"

int main()
{
	//stand process
	Standard_process();
	
	/*//data
	uint32_t pin_group[1] = {GPIO_PIN_4};
	
	//SPI init
	ISPI1_Init();
	ISPI1_NSS_Init(GPIOA, pin_group, 1);
	
	//IIC Init
	IIC1_Init(0, 0);
	
	//OLED init
	OLED_Init();
	OLED_TurnOn_Screen();
	OLED_Flash_Screen(0x00);
	
	//test code
	NRF2401_Init(0);
	while(1) {
		NRF2401_Start();
		HAL_Delay(2000);
		NRF2401_Stop();
		HAL_Delay(2000);
	};
	while(1);*/
	NRF2401_Receiver();
	//NRF2401_Sender();
	return 0;
}