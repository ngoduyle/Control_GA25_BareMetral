#include "stm32_gpio.h"

void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t ENorDI)
{
	if(ENorDI == ENABLE)
	{
		if(pGPIOx == GPIOA)
		{
			GPIOA_PCLK_EN();
		}
		if(pGPIOx == GPIOB)
		{
			GPIOB_PCLK_EN();
		}
		if(pGPIOx == GPIOC)
		{
			GPIOC_PCLK_EN();
		}
		if(pGPIOx == GPIOD)
		{
			GPIOD_PCLK_EN();
		}
		if(pGPIOx == GPIOE)
		{
			GPIOE_PCLK_EN();
		}
	}
	else
	{
		//
	}
}
/*
 * GPIO_RegDef_t *pGPIOx
 * Pin		GPIO_PIN_5
 * Mode		GPIO_MODE_INPUT
 * Pull		GPIO_CNF_OUT_PP
 * Speed	GPIO_MODE_OUTPUT_2MHZ
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
//	uint8_t pin = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber;
//	uint8_t pos = (pin <= 7) ? pin : (pin - 8);
//	uint32_t temp = 0; // temporary register
//	// Enable the Peripheral Clock
//	GPIO_PeriClockControl(pGPIOHandle->pGPIOx, ENABLE);
///*
// * Phải sửa lại từ GPIO_PinConfig_t. Không để chia theo
// * Nên chia các phần Init theo kiểu chọn CRL hay CRH dựa vào việc so sánh PinNumber
// * Config MODE Field trước, vì CNF chia ra theo MODE == 00 và MODE > 00
// */
//
//
//	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber <= 7) // if PinNumber <= 7, then CRL
//	{
//			//đưa bit vào đúng vị trí của PinNumber:
//		temp = pGPIOHandle->GPIO_PinConfig.GPIO_MODE | (pGPIOHandle->GPIO_PinConfig.GPIO_CNF << 2);
//		temp <<= (4 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
//			//clear bit đang cần cấu hình:
//		pGPIOHandle->pGPIOx->CRL &= ~(0xF << (4 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
//			//set bit đang cần cấu hình:
//		pGPIOHandle->pGPIOx->CRL |= temp;
//	}
//	else
//	{
//		//đưa bit vào đúng vị trí của PinNumber:
//	temp = pGPIOHandle->GPIO_PinConfig.GPIO_MODE | (pGPIOHandle->GPIO_PinConfig.GPIO_CNF << 2);
//
//	temp <<= (4 * (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber - 8));
//		//clear bit đang cần cấu hình:
//	pGPIOHandle->pGPIOx->CRH &= ~(0xF << (4 * (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber - 8)));
//		//set bit đang cần cấu hình:
//	pGPIOHandle->pGPIOx->CRH |= temp;
//	}
//

/*
 * If Not Interrupt
 */
//	if(pGPIOHandle->GPIO_PinConfig.GPIO_MODE < GPIO_MODE_IT_FT)
//	{
		uint8_t pin = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber;
		uint8_t pos = 4 * ((pin <= 7) ? pin : (pin - 8));
		uint32_t gpio_mode = 0;
		uint32_t config = 0; // temporary register
	/*
	 * Enable the Peripheral Clock
	 */
		GPIO_PeriClockControl(pGPIOHandle->pGPIOx, ENABLE);

	/*
	 * Configure CRL/CRH Register. Cần chia ra 2 trường hợp: Mode INPUT và Mode Interrupt.
	 * Vì mode interrupt không phải cấu hình thực sự của GPIO_MODE, nên phải tách ra thì mới config GPIO_MODE đúng được.
	 */
		if(pGPIOHandle->GPIO_PinConfig.GPIO_MODE >= GPIO_MODE_IT_FT)
			gpio_mode = GPIO_MODE_INPUT;
		else
			gpio_mode = pGPIOHandle->GPIO_PinConfig.GPIO_MODE;
		config = gpio_mode | (pGPIOHandle->GPIO_PinConfig.GPIO_CNF << 2);
		if(pin <= 7) // if PinNumber <= 7, then CRL
		{
				//đưa bit vào đúng vị trí của PinNumber:
	//		config <<= pos; // gộp luôn vào phần set bit, để tránh làm mất dữ liệu gốc
				//clear bit đang cần cấu hình:
			pGPIOHandle->pGPIOx->CRL &= ~(0xF << pos);
				//set bit đang cần cấu hình:
			pGPIOHandle->pGPIOx->CRL |= (config << pos);
		}
		else
		{
			//đưa bit vào đúng vị trí của PinNumber:
	//		config <<= pos; // gộp luôn vào phần set bit, để tránh làm mất dữ liệu gốc
			//clear bit đang cần cấu hình:
		pGPIOHandle->pGPIOx->CRH &= ~(0xF << pos);
			//set bit đang cần cấu hình:
		pGPIOHandle->pGPIOx->CRH |= (config << pos);
		}
	/*
	 * Configure Input Pull-up/Pull-down (ODR Register)
	 */
/*
		if(pGPIOHandle->GPIO_PinConfig.GPIO_CNF == GPIO_CNF_IN_PUPD \
				&& (pGPIOHandle->GPIO_PinConfig.GPIO_MODE == GPIO_MODE_INPUT	\
				|| pGPIOHandle->GPIO_PinConfig.GPIO_MODE >= GPIO_MODE_IT_FT))
*/
		if(pGPIOHandle->GPIO_PinConfig.GPIO_CNF == GPIO_CNF_IN_PUPD)

		{
			if(pGPIOHandle->GPIO_PinConfig.GPIO_Pull == GPIO_INPUT_PULLDOWN)
			{
				pGPIOHandle->pGPIOx->BSRR = (0x1 << (pin + 16));
			}
			else if(pGPIOHandle->GPIO_PinConfig.GPIO_Pull == GPIO_INPUT_PULLUP)
			{
				pGPIOHandle->pGPIOx->BSRR = (0x1 << pin);
			}
		}
//	}
/*
 * Interrupt Mode
 */
	if(pGPIOHandle->GPIO_PinConfig.GPIO_MODE == GPIO_MODE_IT_FT \
			|| pGPIOHandle->GPIO_PinConfig.GPIO_MODE == GPIO_MODE_IT_RT \
			|| pGPIOHandle->GPIO_PinConfig.GPIO_MODE == GPIO_MODE_IT_RFT)
	{
	/*
	 * Config Rising/Falling Edge
	 */
		if(pGPIOHandle->GPIO_PinConfig.GPIO_MODE == GPIO_MODE_IT_FT)
		{
			// Set the FTSR bit
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			// Clear the corresponding RTSR bit -> xóa bit Rising đi, lỡ nó đang còn thì lại bug
			EXTI->RTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}
		else if(pGPIOHandle->GPIO_PinConfig.GPIO_MODE == GPIO_MODE_IT_RT)
		{
			// Set the RTSR bit
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			// Clear the corresponding FTSR bit -> xóa bit Falling đi, lỡ nó đang còn thì lại bug
			EXTI->FTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}
		else if(pGPIOHandle->GPIO_PinConfig.GPIO_MODE == GPIO_MODE_IT_RFT)
		{
			// Set the RTSR bit
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			// Set the FTSR bit
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}
	/*
	 * Configure AFIO
	 */
		AFIO_PCLK_EN(); //Enable AFIO Clock
		uint8_t EXTICR_No = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 4;
		uint8_t EXTI_No = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 4;
		uint8_t EXTI_Config  = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);
		/*
		 * Ví Dụ: GPIOA Pin 6 -> 6/4 = 1, EXTICR_No = 1, vào thanh ghi EXTICR2
		 * 6 % 4 = 2, EXTI_No = 2, vào field EXTI6
		 */
		AFIO->EXTICR[EXTICR_No] = EXTI_Config << (4 * EXTI_No);

	/*
	 * Config IMR (cho phép ngắt)
	 */
		EXTI->IMR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	}

}

void GPIO_DeInit(GPIO_RegDef_t *pGPIOx)
{
	if(pGPIOx == GPIOA)
	{
		GPIOA_REG_RESET();
	}
	if(pGPIOx == GPIOB)
	{
		GPIOB_REG_RESET();
	}
	if(pGPIOx == GPIOC)
	{
		GPIOC_REG_RESET();
	}
	if(pGPIOx == GPIOD)
	{
		GPIOD_REG_RESET();
	}
	if(pGPIOx == GPIOE)
	{
		GPIOE_REG_RESET();
	}
	if(pGPIOx == GPIOF)
	{
		GPIOF_REG_RESET();
	}
	if(pGPIOx == GPIOG)
	{
		GPIOG_REG_RESET();
	}
}
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	uint8_t value; //đọc 1 bit thôi chứ không phải đọc cả thanh ghi IDR, nên không cần dùng uint32_t
	value = (uint8_t) (pGPIOx->IDR >> PinNumber) & 0x00000001; //shift bit cần đọc về cuối rồi & với 1
	return value;
}
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)
{
	uint16_t value;
	value = pGPIOx->IDR;
	return value;
}
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value)
{
	if(Value == GPIO_PIN_SET)
	{
		pGPIOx->BSRR = 1 << PinNumber;
	}
	else
	{
		pGPIOx->BSRR = 1 << (PinNumber + 16);
	}
}

void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value)
{
	pGPIOx->ODR = Value;
}

void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	if(pGPIOx->ODR & (1 << PinNumber)) //Không so sánh pGPIOx->ODR & (1 << PinNumber))== GPIO_PIN_SET, vì pGPIOx->ODR & (1 << PinNumber) có thể bằng 0x20, 0x40... , còn GPIO_PIN_SET chỉ bằng 1.
		// Check bit = dùng mask, không so sánh bằng 1
	{
		pGPIOx->BSRR = (1 << (PinNumber + 16));
	}
	else
	{
		pGPIOx->BSRR = (1 << PinNumber);
	}
}
void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t ENorDI)
{
	if(ENorDI == ENABLE)
	{
		if(IRQNumber <= 31)
		{
			//set ISER0
			*NVIC_ISER0 |= (1 << IRQNumber);
		}
		else if(IRQNumber >= 32 && IRQNumber <= 63)
		{
			//set ISER1
			*NVIC_ISER1 |= (1 << (IRQNumber % 32));
		}
	}
	else
	{
		if(IRQNumber <= 31)
		{
			//set ICER0
			*NVIC_ICER0 |= (1 << IRQNumber);
		}
		else if(IRQNumber >= 32 && IRQNumber <= 63)
		{
			//set ICER1
			*NVIC_ICER1 |= (1 << (IRQNumber % 32));
		}
	}
}
void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
{
/*
 * Figure 19. NVIC_IPRx register mapping (Arm Cortex M3 User guide)
 */
	uint8_t IPR_No 	= IRQNumber / 4;
	uint8_t IP_No 	= IRQNumber % 4;
/*
 * Ví dụ: IRQNumber = 23
 * 23 / 4 = 5 -> IPR5
 * 23 % 4 = 3 -> byte offset 3 -> IP3 của IPR5
 */
	*(NVIC_IPR_BASEADDR + IPR_No) &= ~(IRQPriority << (8 * IP_No + 4)); // clear 4 bit cao
	*(NVIC_IPR_BASEADDR + IPR_No) |= IRQPriority << (8 * IP_No + 4); // set 4 bit cao (chỉ dùng 4 bit cao)
/*
 * *(NVIC_IPR_BASEADDR + IPR_No): Cộng con trỏ. pointer + 1 = nhảy 4 byte (vì uint32_t)
 */
}
void GPIO_IRQHandling(uint8_t PinNumber)
{
	//clear pending
	if(EXTI->PR & (1 << PinNumber))
	{
		EXTI->PR = (1 << PinNumber);
	}
}
