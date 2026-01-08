#include "ISPI.h"

#ifdef ISPI_API_EN
/*
SPI1 will be matser, SPI2 will be the slave
*/
SPI_HandleTypeDef spi1_config = {
		.Instance = SPI1,
		.Init.Mode = SPI_MODE_MASTER,
		.Init.Direction = SPI_DIRECTION_2LINES,
		.Init.DataSize = SPI_DATASIZE_8BIT, //This need to be explosed to the user.
		.Init.NSS = SPI_NSS_SOFT,
		.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32,// 2Mhz, This need tobe explosed to the user.
		.Init.FirstBit = SPI_FIRSTBIT_MSB, // This need to be explosed to the user.
		.Init.TIMode = SPI_TIMODE_DISABLE,
		.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE,
		.State = HAL_SPI_STATE_RESET,
		.Init.CLKPolarity = SPI_POLARITY_LOW,// This need to be explosed to the user.
		.Init.CLKPhase = SPI_PHASE_1EDGE// This need to be explosed to the user.
};
void ISPI1_Init()
{
	//open the device clock
	__HAL_RCC_SPI1_CLK_ENABLE();
	//spi init called
	HAL_SPI_Init(&spi1_config);
	
}

// surrounding device init coding ...
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
		//GPIO Init
	
		//NVIC Init
		
}



//IT Handler
void SPI1_IRQHandler()
{
	HAL_SPI_IRQHandler(&spi1_config);
}

#endif