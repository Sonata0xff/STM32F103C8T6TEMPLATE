#include "start_up.h"
#ifdef START_UP_API_EN

void SystemClock_Config()
{	
		RCC_OscInitTypeDef RCC_OscInitStruct = {
			.OscillatorType = RCC_OSCILLATORTYPE_HSE,
			.HSEState = RCC_HSE_ON,
			.HSIState = RCC_HSI_OFF,
			.PLL.PLLState = RCC_PLL_ON,
			.PLL.PLLSource = RCC_PLLSOURCE_HSE,
			.PLL.PLLMUL = RCC_PLL_MUL9// 8MHz * 9 = 72MHz
		};
		
		RCC_ClkInitTypeDef RCC_ClkInitStruct = {
			.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2,
			.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK,
			.AHBCLKDivider = RCC_SYSCLK_DIV1, // HCLK = 72MHz
			.APB1CLKDivider = RCC_HCLK_DIV2,	// PCLK1 = 36MHz
			.APB2CLKDivider = RCC_HCLK_DIV1		// PCLK2 = 72MHz
		};
		
    HAL_RCC_OscConfig(&RCC_OscInitStruct);
		HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);
}

#endif