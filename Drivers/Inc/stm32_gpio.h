#ifndef INC_STM32_GPIO_H_
#define INC_STM32_GPIO_H_
#include "stm32f1.h"
typedef struct
{
	uint8_t GPIO_PinNumber;
	uint8_t GPIO_CNF;
	uint8_t GPIO_MODE;
	uint8_t GPIO_Pull;
//	uint8_t GPIO_PinOPType;
//	uint8_t GPIO_PinAltFunMode;
}GPIO_PinConfig_t;

typedef struct
{
	GPIO_RegDef_t *pGPIOx;
	GPIO_PinConfig_t GPIO_PinConfig;
}GPIO_Handle_t;
/*
 * GPIO PIN Macros
 */
#define GPIO_PIN_0		0
#define GPIO_PIN_1		1
#define GPIO_PIN_2		2
#define GPIO_PIN_3		3
#define GPIO_PIN_4		4
#define GPIO_PIN_5		5
#define GPIO_PIN_6		6
#define GPIO_PIN_7		7
#define GPIO_PIN_8		8
#define GPIO_PIN_9		9
#define GPIO_PIN_10		10
#define GPIO_PIN_11		11
#define GPIO_PIN_12		12
#define GPIO_PIN_13		13
#define GPIO_PIN_14		14
#define GPIO_PIN_15		15

/*
 * GPIO PIN Macros for CNF Field in CRL/CRH Register (Input Mode)
 */
#define GPIO_CNF_ANALOG			0		// Analog mode
#define GPIO_CNF_IN_FLOATING	1		// Floating input (reset state)
#define GPIO_CNF_IN_PUPD		2		// Input with pull-up / pull-down

/*
 * GPIO PIN Macros for CNF Field in CRL/CRH Register (Output Mode)
 */
#define GPIO_CNF_OUT_PP			0		// General purpose output push-pull
#define GPIO_CNF_OUT_OD       	1		// General purpose output Open-drain
#define GPIO_CNF_ALT_PP       	2		// Alternate function output Push-pull
#define GPIO_CNF_ALT_OD        	3		// Alternate function output Open-drain

/*
 * GPIO PIN Macros for MODE Field in CRL/CRH Register
 */
#define GPIO_MODE_INPUT			0		// Input mode (reset state)
#define GPIO_MODE_OUTPUT_10MHZ	1		// Output mode, max speed 10 MHz
#define GPIO_MODE_OUTPUT_2MHZ	2		// Output mode, max speed 2 MHz
#define GPIO_MODE_OUTPUT_50MHZ	3		// Output mode, max speed 50 MHz

/*
 * GPIO PIN Macros for Interrupt Rising/Falling Edge (EXTI)
 */
#define GPIO_MODE_IT_FT			4		// Falling Trigger
#define GPIO_MODE_IT_RT			5		// Rising Trigger
#define GPIO_MODE_IT_RFT		6		// Rising and Falling Trigger

/*
 * GPIO PIN Macros for Input Pull-up/Pull-down (ODR Register)
 */
#define GPIO_INPUT_PULLDOWN		0		// Input Pull Down
#define GPIO_INPUT_PULLUP		1		// Input Pull Up


void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t ENorDI);
/*
 * GPIO_PeriClockControl(GPIOB, ENABLE); ->> GPIOB: con trỏ tới thanh ghi của GPIOB
 * GPIOB thực chất là đang viết #define GPIOB ((GPIO_RegDef_t *) GPIOB_BASEADDR), tức là truyền vào
 * địa chỉ base của peripheral. Sau đó so sánh địa chỉ để quyết định bật clock
 *
 * GPIO_RegDef_t *pGPIOx của GPIO_PeriClockControl và GPIO_Handle_t không dùng chung một biến pGPIOx.
 * Chúng chỉ: cùng dùng kiểu dữ liệu GPIO_RegDef_t* và vô tình đặt cùng tên biến là pGPIOx
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx); // chỉ cần truyền vào base address rồi so sánh để biết GPIO nào, sau đó dùng thanh ghi RCC_APB2RSTR để reset

uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx); // trả về giá trị có 16 bit tương ứng giá trị của 16 Pin trên Port
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value);
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value); // uint16_t Value: Value có 16 bit tương ứng với 16 Pin
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);

void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t ENorDI);
void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void GPIO_IRQHandling(uint8_t PinNumber);




#endif /*INC_STM32_GPIO_H_*/
