#include "stm32_spi.h"

void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t ENorDI)
{
	if(ENorDI == ENABLE)
	{
		 if(pSPIx == SPI1)
		 {
			 SPI1_PCLK_EN();
		 }
		 else if(pSPIx == SPI2)
		 {
			 SPI2_PCLK_EN();
		 }
		 else if(pSPIx == SPI3)
		 {
		 	 SPI3_PCLK_EN();
		 }
	}
	else
	{
		 if(pSPIx == SPI1)
		 {
			 SPI1_PCLK_DI();
		 }
		 else if(pSPIx == SPI2)
		 {
			 SPI2_PCLK_DI();
		 }
		 else if(pSPIx == SPI3)
		 {
		 	 SPI3_PCLK_DI();
		 }
	}
}


void SPI_Init(SPI_Handle_t *pSPIHandle)
{
	SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE);

	uint32_t tempreg = 0;
/*
 * Configure Device Mode
 */
	tempreg |= (pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR);

/*
 * Configure Bus Configuration
 */
	if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD)
	{
		tempreg &= ~(1 << SPI_CR1_BIDIMODE);
	}
	else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD)
	{
		tempreg |= (1 << SPI_CR1_BIDIMODE);
	}
	else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY)
	{
		tempreg &= ~(1 << SPI_CR1_BIDIMODE);
		tempreg |= (1 << SPI_CR1_RXONLY);
	}
/*
 * Configure SPI Clock Speed (Baud Rate)
 */
	tempreg |= (pSPIHandle->SPIConfig.SPI_SclkSpeed << SPI_CR1_BR);

/*
 * Configure SPI Data Frame
 */
	tempreg |= (pSPIHandle->SPIConfig.SPI_DFF << SPI_CR1_DFF);

/*
 * Configure SPI Clock Polarity
 */
	tempreg |= (pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL);

/*
 * Configure SPI Clock Phase
 */
	tempreg |= (pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA);

/*
 * Configure SPI Software Slave Management
 */
	tempreg |= (pSPIHandle->SPIConfig.SPI_SSM << SPI_CR1_SSM);

/*
 * Write to CR1 Register
 */
	pSPIHandle->pSPIx->CR1 = tempreg;
}
void SPI_DeInit(SPI_RegDef_t *pSPIx)
{
	if(pSPIx == SPI1)
	{
		SPI1_REG_RESET();
	}
	else if(pSPIx == SPI2)
	{
		SPI2_REG_RESET();
	}
	else if(pSPIx == SPI3)
	{
		SPI3_REG_RESET();
	}
}

uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName)
{
	if(pSPIx->SR & FlagName)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}

/*
 * Data Send and Receive
 */
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len)
{
	while(Len > 0)
	{
		//1. Wait until TXE is set
		while(SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG) == FLAG_RESET);//Khi TXE_FLAG = 0 thì không làm gì

		//2. Check the DFF bit in CR1
		if((pSPIx->CR1 &(1 << SPI_CR1_DFF)))
		{
			//16 bit DFF
			//1. Load the data in to the DR
			pSPIx->DR = *((uint16_t *) pTxBuffer);
			Len--;
			Len--;
			pTxBuffer += 2;
		}
		else
		{
			//8 bit DFF
			pSPIx->DR = *pTxBuffer;
			Len --;
			pTxBuffer++;
		}
	}
}
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len)
{
	while(Len > 0)
	{
		//1. Wait until RXNE is set
		while(SPI_GetFlagStatus(pSPIx, SPI_RXNE_FLAG) == FLAG_RESET); //Khi RXNE_FLAG = 0 thì không làm gì

		//2. Check the DFF bit in CR1
		if((pSPIx->CR1 &(1 << SPI_CR1_DFF)))
		{
		/*
		 * 16 bit DFF
		 */
			//1. Read the data from DR
			*((uint16_t*)pRxBuffer) = pSPIx->DR;
			Len--;
			Len--;
			pRxBuffer += 2;
		}
		else
		{
		/*
		 * 8 bit DFF
		 */
			//8 bit DFF
			*pRxBuffer = pSPIx->DR;
			Len --;
			pRxBuffer++;
		}
	}
}

void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t ENorDI)
{
	if(ENorDI == ENABLE)
	{
		pSPIx->CR1 |= (1 << SPI_CR1_SPE);
	}
	else
	{
		pSPIx->CR1 &= ~(1 << SPI_CR1_SPE);
	}
}
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t ENorDI)
{
	if(ENorDI == ENABLE)
	{
		pSPIx->CR1 |= (1 << SPI_CR1_SSI);
	}
	else
	{
		pSPIx->CR1 &= ~(1 << SPI_CR1_SSI);
	}
}

uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t Len);
uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t Len);

void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t ENorDI);
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority);
void SPI_IRQHandling(SPI_Handle_t *pHandle);


void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t ENorDI);
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName);
void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx);
void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle);
void SPI_CloseReception(SPI_Handle_t *pSPIHandle);
