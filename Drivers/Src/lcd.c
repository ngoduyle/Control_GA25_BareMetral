#include <lcd.h>
#include <stm32_gpio.h>

void GPIO_LCD_Init(LCD_Handle_t *pLCDx)
{
	GPIO_Handle_t GPIOHandle;
	GPIO_Handle_t *pGPIOHandle = &GPIOHandle;
//RS
	pGPIOHandle->pGPIOx = pLCDx->RS.pGPIOx;
	pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber = pLCDx->RS.GPIO_PinNumber;
	pGPIOHandle->GPIO_PinConfig.GPIO_CNF = GPIO_CNF_OUT_PP;
	pGPIOHandle->GPIO_PinConfig.GPIO_MODE = GPIO_MODE_OUTPUT_10MHZ;
	GPIO_Init(pGPIOHandle);
//E
	pGPIOHandle->pGPIOx = pLCDx->E.pGPIOx;
	pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber = pLCDx->E.GPIO_PinNumber;
	pGPIOHandle->GPIO_PinConfig.GPIO_CNF = GPIO_CNF_OUT_PP;
	pGPIOHandle->GPIO_PinConfig.GPIO_MODE = GPIO_MODE_OUTPUT_10MHZ;
	GPIO_Init(pGPIOHandle);

//D4
	pGPIOHandle->pGPIOx = pLCDx->D4.pGPIOx;
	pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber = pLCDx->D4.GPIO_PinNumber;
	pGPIOHandle->GPIO_PinConfig.GPIO_CNF = GPIO_CNF_OUT_PP;
	pGPIOHandle->GPIO_PinConfig.GPIO_MODE = GPIO_MODE_OUTPUT_10MHZ;
	GPIO_Init(pGPIOHandle);
//D5
	pGPIOHandle->pGPIOx = pLCDx->D5.pGPIOx;
	pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber = pLCDx->D5.GPIO_PinNumber;
	pGPIOHandle->GPIO_PinConfig.GPIO_CNF = GPIO_CNF_OUT_PP;
	pGPIOHandle->GPIO_PinConfig.GPIO_MODE = GPIO_MODE_OUTPUT_10MHZ;
	GPIO_Init(pGPIOHandle);
//D6
	pGPIOHandle->pGPIOx = pLCDx->D6.pGPIOx;
	pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber = pLCDx->D6.GPIO_PinNumber;
	pGPIOHandle->GPIO_PinConfig.GPIO_CNF = GPIO_CNF_OUT_PP;
	pGPIOHandle->GPIO_PinConfig.GPIO_MODE = GPIO_MODE_OUTPUT_10MHZ;
	GPIO_Init(pGPIOHandle);

//D7
	pGPIOHandle->pGPIOx = pLCDx->D7.pGPIOx;
	pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber = pLCDx->D7.GPIO_PinNumber;
	pGPIOHandle->GPIO_PinConfig.GPIO_CNF = GPIO_CNF_OUT_PP;
	pGPIOHandle->GPIO_PinConfig.GPIO_MODE = GPIO_MODE_OUTPUT_10MHZ;
	GPIO_Init(pGPIOHandle);
}

void delay_LCD_us(LCD_Handle_t *pLCDx, uint16_t us)
{
	pLCDx->pTIMx->CR1 |= (1U << TIM_CR1_CEN);
	pLCDx->pTIMx->CNT = 0;
	while(pLCDx->pTIMx->CNT < us);
	pLCDx->pTIMx->CR1 &= ~(1U << TIM_CR1_CEN);
}


void LCD_Write_4bit(uint8_t data, uint8_t mode, LCD_Handle_t *pLCDx)
{
	if(mode == 0x00)
	{
		GPIO_WriteToOutputPin(pLCDx->RS.pGPIOx, pLCDx->RS.GPIO_PinNumber, GPIO_PIN_RESET); //RS = 0, cmd
	}
	else if(mode == 0x01)
	{
		GPIO_WriteToOutputPin(pLCDx->RS.pGPIOx, pLCDx->RS.GPIO_PinNumber, GPIO_PIN_SET); //RS = 1, data
	}
	GPIO_WriteToOutputPin(pLCDx->D4.pGPIOx, pLCDx->D4.GPIO_PinNumber, (data & 0x01) ? 1 : 0);
	GPIO_WriteToOutputPin(pLCDx->D5.pGPIOx, pLCDx->D5.GPIO_PinNumber, (data & 0x02) ? 1 : 0);
	GPIO_WriteToOutputPin(pLCDx->D6.pGPIOx, pLCDx->D6.GPIO_PinNumber, (data & 0x04) ? 1 : 0);
	GPIO_WriteToOutputPin(pLCDx->D7.pGPIOx, pLCDx->D7.GPIO_PinNumber, (data & 0x08) ? 1 : 0);

	GPIO_WriteToOutputPin(pLCDx->E.pGPIOx, pLCDx->E.GPIO_PinNumber, GPIO_PIN_SET); //E = 1
	delay_LCD_us(pLCDx, 1);
	GPIO_WriteToOutputPin(pLCDx->E.pGPIOx, pLCDx->E.GPIO_PinNumber, GPIO_PIN_RESET); //E = 0
	delay_LCD_us(pLCDx, 1);

}

void LCD_Write_data(uint8_t data, LCD_Handle_t *pLCDx)
{
	LCD_Write_4bit((data >> 4), 1, pLCDx);
	LCD_Write_4bit((data & 0x0F), 1, pLCDx);
}

void LCD_Print(char *str, LCD_Handle_t *pLCDx)
{
	while(*str)
	{
		LCD_Write_data(*str++, pLCDx);
		delay_LCD_us(pLCDx, 2000);
	}
}

void LCD_Write_cmd(uint8_t cmd, LCD_Handle_t *pLCDx)
{
	LCD_Write_4bit((cmd >> 4), 0, pLCDx);
	LCD_Write_4bit((cmd & 0x0F), 0, pLCDx);
}

void LCD_SetCursor(uint8_t row, uint8_t col, LCD_Handle_t *pLCDx)
{
    uint8_t addr;

    if(row == 0)
        addr = 0x80 + col;
    else
        addr = 0xC0 + col;

    LCD_Write_cmd(addr, pLCDx);
}

void LCD_Init(LCD_Handle_t *pLCDx)
{
	//Khai bao pin


	delay_LCD_us(pLCDx, 20000);
	LCD_Write_4bit(0x03, 0, pLCDx);
	delay_LCD_us(pLCDx, 5000);
	LCD_Write_4bit(0x03, 0, pLCDx);
	delay_LCD_us(pLCDx, 200);
	LCD_Write_4bit(0x03, 0, pLCDx);

	LCD_Write_cmd(0x02, pLCDx);
	delay_LCD_us(pLCDx, 10000);
	LCD_Write_cmd(0x28, pLCDx);
	delay_LCD_us(pLCDx, 10000);
	LCD_Write_cmd(0x0C, pLCDx);	// 00001DCB Set display (D), cursor (C ), and blinking of cursor (B) on/off control bit. (execution time: 39us)
	delay_LCD_us(pLCDx, 10000);
	LCD_Write_cmd(0x01, pLCDx);	// 0x01 clear display
	delay_LCD_us(pLCDx, 10000);
	LCD_Write_cmd(0x06, pLCDx);	// 0x06 move cursor right, don’t shift display (this is the most common)
	delay_LCD_us(pLCDx, 10000);
}
