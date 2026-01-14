#include "stm32f1xx.h"                  // Device header
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_conf.h"
#include "IIC.h"
#include "OLED.h"
#include "ISPI.h"
#include "nrf2401.h"

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    // ??HSE????(8MHz)-> PLL???72MHz
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;  // ????????
    RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;  // 8MHz * 9 = 72MHz
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    // ???????
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;    // HCLK = 72MHz
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;     // PCLK1 = 36MHz
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;     // PCLK2 = 72MHz (SPI1????)
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);
}

int main()
{
	//data
	uint32_t pin_group[1] = {GPIO_PIN_4};
	unsigned char res[2];
	//stand process
	HAL_Init();
	SystemClock_Config();
	
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
	
	NRF2401_Start(res);
	
	//OLED_Write1Byte(0,0,res[0]);
	while(1);
	return 0;
}