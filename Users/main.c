#include "stm32f1xx.h"                  // Device header
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_conf.h"
#include "IIC.h"
#include "OLED.h"
#include "ISPI.h"
#include "nrf2401.h"
#include "utils.h"

int main()
{
	//stand process
	Standard_process();
	
	//data
	uint32_t pin_group[1] = {GPIO_PIN_4};
	unsigned char res[3] = {0xaa, 0xf0, 0xfa};
	
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
	/*GPIO_InitTypeDef cc_con = {
		.Pin = GPIO_PIN_9,
		.Mode = GPIO_MODE_OUTPUT_PP,
		.Pull = GPIO_NOPULL,
		.Speed = GPIO_SPEED_FREQ_HIGH
	};
	HAL_GPIO_Init(GPIOB, &cc_con);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);*/
	
	ISPI1_SendBytes(res, 3);
	
	//NRF2401_Init(0);
	ISPI_Comm_Block_Wait();
	//NRF2401_Start(res);
	
	
	
	OLED_WriteIn_16x8Char(0,0,'b');
	//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
	
	
	
	while(1);
	return 0;
}