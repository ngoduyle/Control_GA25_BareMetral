#include <stm32f1.h>
#include <stm32_tim.h>

typedef struct
{
	GPIO_RegDef_t *pGPIOx;
	uint8_t GPIO_PinNumber;
}LCD_RS_t;

typedef struct
{
	GPIO_RegDef_t *pGPIOx;
	uint8_t GPIO_PinNumber;
}LCD_E_t;

typedef struct
{
	GPIO_RegDef_t *pGPIOx;
	uint8_t GPIO_PinNumber;
}LCD_D4_t;

typedef struct
{
	GPIO_RegDef_t *pGPIOx;
	uint8_t GPIO_PinNumber;
}LCD_D5_t;

typedef struct
{
	GPIO_RegDef_t *pGPIOx;
	uint8_t GPIO_PinNumber;
}LCD_D6_t;

typedef struct
{
	GPIO_RegDef_t *pGPIOx;
	uint8_t GPIO_PinNumber;
}LCD_D7_t;

typedef struct
{
	LCD_RS_t RS;
	LCD_E_t  E;
	LCD_D4_t D4;
	LCD_D5_t D5;
	LCD_D6_t D6;
	LCD_D7_t D7;
	TIM_RegDef_t *pTIMx;
}LCD_Handle_t;

void delay_LCD_us(LCD_Handle_t *pLCDx, uint16_t us);

void GPIO_LCD_Init(LCD_Handle_t *pLCDx);
void LCD_SetCursor(uint8_t row, uint8_t col, LCD_Handle_t *pLCDx);
void LCD_Write_4bit(uint8_t data, uint8_t mode, LCD_Handle_t *pLCDx);
void LCD_Write_data(uint8_t data, LCD_Handle_t *pLCDx);
void LCD_Print(char *str, LCD_Handle_t *pLCDx);
void LCD_Write_cmd(uint8_t cmd, LCD_Handle_t *pLCDx);
void LCD_Init(LCD_Handle_t *pLCDx);
