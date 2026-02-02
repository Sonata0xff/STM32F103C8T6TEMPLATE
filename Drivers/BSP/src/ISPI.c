#include "ISPI.h"

#ifdef ISPI_API_EN

AtomVarType ISPI_MASTER_COMM_FIN; //ATOM_VALUE_RESET means comm finished.
static uint8_t NSS_Line[16]; // nss line
static uint8_t NSS_LineSize = 0;//nss line num
static GPIO_TypeDef* Nss_Group = GPIOA;
/*
SPI1 will be matser, SPI2 will be the slave
*/

DMA_HandleTypeDef tx_dma = {
	.Instance = DMA1_Channel3,
	.Init.Direction = DMA_MEMORY_TO_PERIPH,
	.Init.PeriphInc = DMA_PINC_DISABLE,
	.Init.MemInc = DMA_MINC_ENABLE,
	.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE,
	.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE,
	.Init.Mode = DMA_NORMAL,
	.Init.Priority = DMA_PRIORITY_HIGH
};

DMA_HandleTypeDef rx_dma = {
	.Instance = DMA1_Channel2,
	.Init.Direction = DMA_PERIPH_TO_MEMORY,
	.Init.PeriphInc = DMA_PINC_DISABLE,
	.Init.MemInc = DMA_MINC_ENABLE,
	.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE,
	.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE,
	.Init.Mode = DMA_NORMAL,
	.Init.Priority = DMA_PRIORITY_HIGH
};

SPI_HandleTypeDef spi1_config = {
		.Instance = SPI1,
		.Init.Mode = SPI_MODE_MASTER,
		.Init.Direction = SPI_DIRECTION_2LINES,
		.Init.DataSize = SPI_DATASIZE_8BIT,
		.Init.NSS = SPI_NSS_SOFT,
		.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8,// 9Mhz, This need tobe explosed to the user.
		.Init.FirstBit = SPI_FIRSTBIT_MSB, // This need to be explosed to the user.
		.Init.TIMode = SPI_TIMODE_DISABLE,
		.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE,
		.State = HAL_SPI_STATE_RESET,
		.Init.CLKPolarity = SPI_POLARITY_LOW,// This need to be explosed to the user.
		.Init.CLKPhase = SPI_PHASE_1EDGE,// This need to be explosed to the user.
		.hdmatx = &tx_dma,
		.hdmarx = &rx_dma
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
		/*
		PA4 = NSS
		PA5 = SCK
		PA6 = MISO
		PA7 = MOSI
		*/
		//lock init
		Atom_Write(&ISPI_MASTER_COMM_FIN, ATOM_VALUE_RESET);
		//DMA Init
		__HAL_RCC_DMA1_CLK_ENABLE();
		HAL_DMA_Init(&tx_dma);
		HAL_DMA_Init(&rx_dma);
		__HAL_LINKDMA(&spi1_config, hdmatx, tx_dma);
		__HAL_LINKDMA(&spi1_config, hdmarx, rx_dma);
		//GPIO Init
		__HAL_RCC_GPIOA_CLK_ENABLE();
	
		//SCK Init
		GPIO_InitTypeDef io_config = {
			.Pin = GPIO_PIN_5,
			.Mode = GPIO_MODE_AF_PP,
			.Speed = GPIO_SPEED_FREQ_HIGH,
			.Pull = GPIO_NOPULL
		};
		HAL_GPIO_Init(GPIOA, &io_config);
		
		//MOSI Init
		io_config.Pin = GPIO_PIN_7;
		HAL_GPIO_Init(GPIOA, &io_config);
		
		//MISO Init
		io_config.Pin = GPIO_PIN_6;
		io_config.Mode = GPIO_MODE_AF_INPUT;
		io_config.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &io_config);
		
		//NVIC Init
		HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);
		HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 0, 0);
		HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);
		HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
}

HAL_StatusTypeDef ISPI1_SendBytes(unsigned char* val, int size)
{
	Atom_Write(&ISPI_MASTER_COMM_FIN, ATOM_VALUE_SET);
	HAL_StatusTypeDef res = HAL_SPI_Transmit_DMA(&spi1_config, val, size);
	return res;
}

HAL_StatusTypeDef ISPI1_SenRecBytes(unsigned char* sendPacks, unsigned char* recvPacks, int size)
{
	Atom_Write(&ISPI_MASTER_COMM_FIN, ATOM_VALUE_SET);
	return HAL_SPI_TransmitReceive_DMA(&spi1_config,
																		sendPacks, recvPacks, size);
}

HAL_StatusTypeDef ISPI1_RecvBytes(unsigned char* recvPacks, int size)
{
	Atom_Write(&ISPI_MASTER_COMM_FIN, ATOM_VALUE_SET);
	return HAL_SPI_Receive_DMA(&spi1_config, recvPacks, size);
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	Atom_Write(&ISPI_MASTER_COMM_FIN, ATOM_VALUE_RESET);
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
	Atom_Write(&ISPI_MASTER_COMM_FIN, ATOM_VALUE_RESET);
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
	Atom_Write(&ISPI_MASTER_COMM_FIN, ATOM_VALUE_RESET);
}

void ISPI1_NSS_Init(GPIO_TypeDef* gpioG, uint32_t* Pin, unsigned char size)
{
	if (size > 16) return;
	else NSS_LineSize = size;
	if (gpioG == 0) return;
	else Nss_Group = gpioG;
	GPIO_InitTypeDef io_config = {
		.Mode = GPIO_MODE_OUTPUT_PP,
		.Pull = GPIO_NOPULL,
		.Speed = GPIO_SPEED_FREQ_HIGH
	};
	for (unsigned char i = 0; i < NSS_LineSize; i++) {
		NSS_Line[i] = Pin[i];
		io_config.Pin = NSS_Line[i];
		HAL_GPIO_Init(Nss_Group, &io_config);
		HAL_GPIO_WritePin(Nss_Group, NSS_Line[i], GPIO_PIN_SET);
	}
}

void ISPI1_SelectDevice(int pos)
{
	if (pos >= NSS_LineSize) return;
	HAL_GPIO_WritePin(Nss_Group, NSS_Line[pos], GPIO_PIN_RESET);
}

void ISPI1_UnSelectDevice(int pos)
{
	if (pos >= NSS_LineSize) return;
	HAL_GPIO_WritePin(Nss_Group, NSS_Line[pos], GPIO_PIN_SET);
}

char ISPI1_GetDeviceStatus(int pos)
{
	if (pos >= NSS_LineSize) return -1;
	if (HAL_GPIO_ReadPin(Nss_Group, NSS_Line[pos]) == GPIO_PIN_RESET) return 0;
	else return 1;
}

void ISPI_Comm_Block_Wait()
{
	while(Atom_Read(&ISPI_MASTER_COMM_FIN) != ATOM_VALUE_RESET);
}

//DMA IT Handler
void DMA1_Channel2_IRQHandler()
{
	HAL_DMA_IRQHandler(&rx_dma);
}

void DMA1_Channel3_IRQHandler()
{
	HAL_DMA_IRQHandler(&tx_dma);
}

#endif