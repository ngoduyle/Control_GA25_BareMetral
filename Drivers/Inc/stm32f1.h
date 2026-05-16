#ifndef INC_STM32F103C8TX_H_
#define INC_STM32F103C8TX_H_

#include "stdint.h"

#define __vo volatile

/*
 * NVIC ISERx
 */
#define NVIC_ISER0		((__vo uint32_t *)0xE000E100)
#define NVIC_ISER1		((__vo uint32_t *)0xE000E104)
#define NVIC_ISER2		((__vo uint32_t *)0xE000E108)

/*
 * NVIC ICERx
 */
#define NVIC_ICER0		((__vo uint32_t *)0xE000E180)
#define NVIC_ICER1		((__vo uint32_t *)0xE000E184)
#define NVIC_ICER2		((__vo uint32_t *)0xE000E188)

/*
 * NVIC IPRx (Priority)
 */
#define NVIC_IPR_BASEADDR		((__vo uint32_t *)0xE000E400)


//Buoc 1: Cau hinh BaseADDR cua FLASH, SRAM, ROM
#define FLASH_BASEADDR 	0x08000000U
#define SRAM_BASEADDR 	0x20000000U
#define ROM_BASEADDR	0x1FFFF111U

//Buoc 2: Cau hinh cac BUS
#define PERIPH_BASEADDR		0x40000000U
#define APB1PERIPH_BASEADDR	PERIPH_BASEADDR
#define APB2PERIPH_BASEADDR	0x40010000U
#define AHBPERIPH_BASEADDR	0x40018000U

//Buoc 3: Cau hinh AHB
#define RCC_BASEADDR	(AHBPERIPH_BASEADDR + 0x9000)

//Buoc 4: Cau hinh APB1
#define I2C1_BASEADDR	(APB1PERIPH_BASEADDR + 0x5400)
#define I2C2_BASEADDR	(APB1PERIPH_BASEADDR + 0x5800)

#define SPI2_BASEADDR	(APB1PERIPH_BASEADDR + 0x3800)
#define SPI3_BASEADDR	(APB1PERIPH_BASEADDR + 0x3C00)

#define USART2_BASEADDR	(APB1PERIPH_BASEADDR + 0x4400)
#define USART3_BASEADDR	(APB1PERIPH_BASEADDR + 0x4800)
#define UART4_BASEADDR	(APB1PERIPH_BASEADDR + 0x4C00)
#define UART5_BASEADDR	(APB1PERIPH_BASEADDR + 0x5000)

#define TIM2_BASEADDR 	APB1PERIPH_BASEADDR
#define TIM3_BASEADDR 	(APB1PERIPH_BASEADDR + 0x0400)
#define TIM4_BASEADDR 	(APB1PERIPH_BASEADDR + 0x0800)


//Buoc 5: Cau hinh APB2
#define GPIOA_BASEADDR	(APB2PERIPH_BASEADDR + 0x0800)
#define GPIOB_BASEADDR	(APB2PERIPH_BASEADDR + 0x0C00)
#define GPIOC_BASEADDR	(APB2PERIPH_BASEADDR + 0x1000)
#define GPIOD_BASEADDR	(APB2PERIPH_BASEADDR + 0x1400)
#define GPIOE_BASEADDR	(APB2PERIPH_BASEADDR + 0x1800)
#define GPIOF_BASEADDR	(APB2PERIPH_BASEADDR + 0x1C00)
#define GPIOG_BASEADDR	(APB2PERIPH_BASEADDR + 0x2000)

#define USART1_BASEADDR	(APB2PERIPH_BASEADDR + 0x3800)

#define SPI1_BASEADDR	(APB2PERIPH_BASEADDR + 0x3000)

#define EXTI_BASEADDR	(APB2PERIPH_BASEADDR + 0x0400)

#define AFIO_BASEADDR	APB2PERIPH_BASEADDR

#define TIM1_BASEADDR	(APB2PERIPH_BASEADDR + 0x2C00)

//Buoc 6: GPIO
typedef struct
{
	__vo uint32_t CRL;
	__vo uint32_t CRH;
	__vo uint32_t IDR;
	__vo uint32_t ODR;
	__vo uint32_t BSRR;
	__vo uint32_t BRR;
	__vo uint32_t LCKR;
}GPIO_RegDef_t; 			//Register Definition

//Tao con tro GPIO
#define GPIOA ((GPIO_RegDef_t *) GPIOA_BASEADDR) //con tro GPIOA_BASEADDR kieu GPIO_RegDef_t
#define GPIOB ((GPIO_RegDef_t *) GPIOB_BASEADDR)
#define GPIOC ((GPIO_RegDef_t *) GPIOC_BASEADDR)
#define GPIOD ((GPIO_RegDef_t *) GPIOD_BASEADDR)
#define GPIOE ((GPIO_RegDef_t *) GPIOE_BASEADDR)
#define GPIOF ((GPIO_RegDef_t *) GPIOF_BASEADDR)
#define GPIOG ((GPIO_RegDef_t *) GPIOG_BASEADDR)
/*
 * CRL nằm tại base address vì con trỏ trỏ vào đó,
 * còn các thanh ghi sau cách nhau 4 byte vì chúng đều là uint32_t
 * và struct được sắp xếp tuần tự
 */
/*

Từ khóa volatile: dùng để báo cho compiler rằng biến có thể thay đổi ngoài luồng chương trình
-> không được tối ưu hóa luồng truy cập.
tối ưu hóa luồng truy cập: nếu compiler thấy biến không thay đổi, thì nó sẽ chỉ đọc 1 lần.
Khi thêm volatile thì compiler biết rằng biến có thể thay đổi nên sẽ luôn đọc lại giá trị biến.
Ví dụ while(GPIOA->IDR & (1<<0)); sẽ tối ưu thành if(GPIOA->IDR & (1<<0)) while(1);

_t: quy tắc đặt tên, _t là type, là 1 kiểu dữ liệu (chứ không phải tên biến hay hàm hay gì khác)

typedef struct
{
 __vo uint32_t CRL;
 __vo uint32_t CHR;
//.......
}GPIO_RegDef_t;
Cái struct này như một cái layout. Và (GPIO_RegDef_t *) 0x40010800U thì có nghĩa là
"Hãy xem vùng nhớ bắt đầu từ 0x40010800 như một object có layout giống GPIO_RegDef_t".
Như vậy, Khi viết (GPIO_RegDef_t *)0x40010800U, ta bảo compiler hãy xem vùng nhớ bắt đầu từ 0x40010800
như một object có layout giống GPIO_RegDef_t. Vì CRL là field/member đầu tiên của struct
nên nó ở offset 0x00, tức địa chỉ 0x40010800. Các field/member sau cách nhau 4 byte vì
chúng đều có kiểu uint32_t.
 */


/*
 * RCC
 */
typedef struct
{
	__vo uint32_t CR;			/* Address offset: 0x00 */
	__vo uint32_t CFGR;			/* Address offset: 0x04 */
	__vo uint32_t CIR;			/* Address offset: 0x08 */
	__vo uint32_t APB2RSTR;		/* Address offset: 0x0C */
	__vo uint32_t APB1RSTR;		/* Address offset: 0x10 */
	__vo uint32_t AHBENR;		/* Address offset: 0x14 */
	__vo uint32_t APB2ENR;		/* Address offset: 0x18 */
	__vo uint32_t APB1ENR;		/* Address offset: 0x1C */
	__vo uint32_t BDCR;			/* Address offset: 0x20 */
	__vo uint32_t CSR;			/* Address offset: 0x24 */
}RCC_RegDef_t;
#define RCC ((RCC_RegDef_t *) RCC_BASEADDR)

/*
 * EXTI
 */
typedef struct
{
	__vo uint32_t IMR;			/* Address offset: 0x00 */
	__vo uint32_t EMR;			/* Address offset: 0x04 */
	__vo uint32_t RTSR;			/* Address offset: 0x08 */
	__vo uint32_t FTSR;			/* Address offset: 0x0C */
	__vo uint32_t SWIER;		/* Address offset: 0x10 */
	__vo uint32_t PR;			/* Address offset: 0x14 */

}EXTI_RegDef_t;
#define EXTI ((EXTI_RegDef_t *) EXTI_BASEADDR)

/*
 * SPI
 */
typedef struct
{
	__vo uint32_t CR1;			/* Address offset: 0x00 */
	__vo uint32_t CR2;			/* Address offset: 0x04 */
	__vo uint32_t SR;			/* Address offset: 0x08 */
	__vo uint32_t DR;			/* Address offset: 0x0C */
	__vo uint32_t CRCPR;		/* Address offset: 0x10 */
	__vo uint32_t RXCRCR;		/* Address offset: 0x14 */
	__vo uint32_t TXCRCR;		/* Address offset: 0x18 */
	__vo uint32_t I2SCFGR;		/* Address offset: 0x1C */
	__vo uint32_t I2SPR;		/* Address offset: 0x20 */
}SPI_RegDef_t;
#define SPI1 ((SPI_RegDef_t *) SPI1_BASEADDR)
#define SPI2 ((SPI_RegDef_t *) SPI2_BASEADDR)
#define SPI3 ((SPI_RegDef_t *) SPI3_BASEADDR)

/*
 * UART
 */
typedef struct
{
	__vo uint32_t SR;			/* Address offset: 0x00 */
	__vo uint32_t DR;			/* Address offset: 0x04 */
	__vo uint32_t BRR;			/* Address offset: 0x08 */
	__vo uint32_t CR1;			/* Address offset: 0x0C */
	__vo uint32_t CR2;			/* Address offset: 0x10 */
	__vo uint32_t CR3;			/* Address offset: 0x14 */
	__vo uint32_t GTPR;			/* Address offset: 0x18 */
}UART_RegDef_t;
#define USART1 ((UART_RegDef_t *) USART1_BASEADDR)
#define USART2 ((UART_RegDef_t *) USART2_BASEADDR)
#define USART3 ((UART_RegDef_t *) USART3_BASEADDR)
#define UART4 ((UART_RegDef_t *) USART4_BASEADDR)
#define UART5 ((UART_RegDef_t *) USART5_BASEADDR)

/*
 * AFIO
 */
typedef struct
{
	__vo uint32_t EVCR;				/* Address offset: 0x00 */
	__vo uint32_t MAPR;				/* Address offset: 0x04 */
	__vo uint32_t EXTICR[4];			/* Address offset: 0x08 - 0x14*/
	__vo uint32_t MAPR2;			/* Address offset: 0x18 */
}AFIO_RegDef_t;
#define AFIO ((AFIO_RegDef_t *) AFIO_BASEADDR)

/*
 * I2C
 */
typedef struct
{
	__vo uint32_t CR1;				/* Address offset: 0x00 */
	__vo uint32_t CR2;				/* Address offset: 0x04 */
	__vo uint32_t OAR1;			/* Address offset: 0x08 */
	__vo uint32_t OAR2;			/* Address offset: 0x0C */
	__vo uint32_t DR;			/* Address offset: 0x10 */
	__vo uint32_t SR1;			/* Address offset: 0x14 */
	__vo uint32_t SR2;			/* Address offset: 0x18 */
	__vo uint32_t CCR;			/* Address offset: 0x1C */
	__vo uint32_t TRISE;			/* Address offset: 0x20 */

}I2C_RegDef_t;
#define I2C1 ((I2C_RegDef_t *) I2C1_BASEADDR)
#define I2C2 ((I2C_RegDef_t *) I2C2_BASEADDR)

/*
 * TIMER
 */
typedef struct
{
	__vo uint32_t CR1;				/* Address offset: 0x00 */
	__vo uint32_t CR2;				/* Address offset: 0x04 */
	__vo uint32_t SMCR;				/* Address offset: 0x08 */
	__vo uint32_t DIER;				/* Address offset: 0x0C */
	__vo uint32_t SR;				/* Address offset: 0x10 */
	__vo uint32_t EGR;				/* Address offset: 0x14 */
	__vo uint32_t CCMR1;			/* Address offset: 0x18 */
	__vo uint32_t CCMR2;			/* Address offset: 0x1C */
	__vo uint32_t CCER;				/* Address offset: 0x20 */
	__vo uint32_t CNT;				/* Address offset: 0x24 */
	__vo uint32_t PSC;				/* Address offset: 0x28 */
	__vo uint32_t ARR;				/* Address offset: 0x2C */

	__vo uint32_t RCR;				/* Address offset: 0x30 */	//TIM1 có; TIM2/3/4 coi như reserved

	__vo uint32_t CCR1;				/* Address offset: 0x34 */
	__vo uint32_t CCR2;				/* Address offset: 0x38 */
	__vo uint32_t CCR3;				/* Address offset: 0x3C */
	__vo uint32_t CCR4;				/* Address offset: 0x40 */

	__vo uint32_t BDTR;				/* Address offset: 0x44 */  //TIM1 có; TIM2/3/4 coi như reserved

	__vo uint32_t DCR;				/* Address offset: 0x48 */
	__vo uint32_t DMAR;				/* Address offset: 0x4C */

}TIM_RegDef_t;
#define TIM1 ((TIM_RegDef_t *) TIM1_BASEADDR)
#define TIM2 ((TIM_RegDef_t *) TIM2_BASEADDR)
#define TIM3 ((TIM_RegDef_t *) TIM3_BASEADDR)
#define TIM4 ((TIM_RegDef_t *) TIM4_BASEADDR)

/*
 * Clock Enable Macros for GPIOx peripherals (RRC)
 */
#define GPIOA_PCLK_EN() (RCC->APB2ENR |= (1 << 2))
#define GPIOB_PCLK_EN() (RCC->APB2ENR |= (1 << 3))
#define GPIOC_PCLK_EN() (RCC->APB2ENR |= (1 << 4))
#define GPIOD_PCLK_EN() (RCC->APB2ENR |= (1 << 5))
#define GPIOE_PCLK_EN() (RCC->APB2ENR |= (1 << 6))

/*
 * Clock Enable Macros for I2Cx peripherals (RRC)
 */
#define I2C1_PCLK_EN() (RCC->APB1ENR |= (1 << 21))
#define I2C2_PCLK_EN() (RCC->APB1ENR |= (1 << 22))

/*
 * Clock Enable Macros for SPIx peripherals (RRC)
 */
#define SPI1_PCLK_EN() (RCC->APB2ENR |= (1 << 12))
#define SPI2_PCLK_EN() (RCC->APB1ENR |= (1 << 14))
#define SPI3_PCLK_EN() (RCC->APB1ENR |= (1 << 15))

/*
 * Clock Enable Macros for USARTx peripherals (RRC)
 */
#define USART1_PCLK_EN() 	(RCC->APB2ENR |= (1 << 14))
#define USART2_PCLK_EN() 	(RCC->APB1ENR |= (1 << 17))
#define USART3_PCLK_EN() 	(RCC->APB1ENR |= (1 << 18))
#define UART4_PCLK_EN() 	(RCC->APB2ENR |= (1 << 19))
#define UART5_PCLK_EN() 	(RCC->APB2ENR |= (1 << 20))

/*
 * Clock Enable Macros for AFIO peripherals (RRC)
 */
#define AFIO_PCLK_EN() 	(RCC->APB2ENR |= (1 << 0))

/*
 * Clock Enable Macros for TIM1
 */
#define TIM1_PCLK_EN() 	(RCC->APB2ENR |= (1 << 11))

/*
 * Clock Enable Macros for TIM2, 3, 4
 */
#define TIM2_PCLK_EN() 	(RCC->APB1ENR |= (1 << 0))
#define TIM3_PCLK_EN() 	(RCC->APB1ENR |= (1 << 1))
#define TIM4_PCLK_EN() 	(RCC->APB1ENR |= (1 << 2))

/*
 * Clock Disable Macros for GPIOx peripherals (RRC)
 */
#define GPIOA_PCLK_DI() (RCC->APB2ENR &= ~(1 << 2))
#define GPIOB_PCLK_DI() (RCC->APB2ENR &= ~(1 << 3))
#define GPIOC_PCLK_DI() (RCC->APB2ENR &= ~(1 << 4))
#define GPIOD_PCLK_DI() (RCC->APB2ENR &= ~(1 << 5))
#define GPIOE_PCLK_DI() (RCC->APB2ENR &= ~(1 << 6))

/*
 * Clock Disable Macros for I2Cx peripherals (RRC)
 */
#define I2C1_PCLK_DI() (RCC->APB1ENR &= ~(1 << 21))
#define I2C2_PCLK_DI() (RCC->APB1ENR &= ~(1 << 22))

/*
 * Clock Disable Macros for SPIx peripherals (RRC)
 */
#define SPI1_PCLK_DI() (RCC->APB2ENR &= ~(1 << 12))
#define SPI2_PCLK_DI() (RCC->APB1ENR &= ~(1 << 14))
#define SPI3_PCLK_DI() (RCC->APB1ENR &= ~(1 << 15))

/*
 * Clock Disable Macros for USARTx peripherals (RRC)
 */
#define USART1_PCLK_DI() 	(RCC->APB2ENR &= ~(1 << 14))
#define USART2_PCLK_DI() 	(RCC->APB1ENR &= ~(1 << 17))
#define USART3_PCLK_DI() 	(RCC->APB1ENR &= ~(1 << 18))
#define UART4_PCLK_DI() 	(RCC->APB2ENR &= ~(1 << 19))
#define UART5_PCLK_DI() 	(RCC->APB2ENR &= ~(1 << 20))

/*
 * Clock Disable Macros for AFIO peripherals (RCC)
 */
#define AFIO_PCLK_DI() 	(RCC->APB2ENR &= ~(1 << 0))

/*
 * Clock Disable Macros for TIM1
 */
#define TIM1_PCLK_DI() 	(RCC->APB2ENR &= ~(1 << 11))

/*
 * Clock Disable Macros for TIM2, 3, 4
 */
#define TIM2_PCLK_DI() 	(RCC->APB1ENR &= ~(1 << 0))
#define TIM3_PCLK_DI() 	(RCC->APB1ENR &= ~(1 << 1))
#define TIM4_PCLK_DI() 	(RCC->APB1ENR &= ~(1 << 2))
/*
 * Macros for Status
 */
#define ENABLE			1
#define DISABLE			0
#define SET				ENABLE
#define RESET			DISABLE
#define GPIO_PIN_SET	SET
#define GPIO_PIN_RESET	RESET
#define FLAG_SET		SET
#define FLAG_RESET		RESET

/*
 * DeInit Macros for GPIO (RCC)
 */
#define GPIOA_REG_RESET()	do{RCC->APB2RSTR |= (1 << 2); RCC->APB2RSTR &= ~(1 << 2);} while(0)
#define GPIOB_REG_RESET()	do{RCC->APB2RSTR |= (1 << 3); RCC->APB2RSTR &= ~(1 << 3);} while(0)
#define GPIOC_REG_RESET()	do{RCC->APB2RSTR |= (1 << 4); RCC->APB2RSTR &= ~(1 << 4);} while(0)
#define GPIOD_REG_RESET()	do{RCC->APB2RSTR |= (1 << 5); RCC->APB2RSTR &= ~(1 << 5);} while(0)
#define GPIOE_REG_RESET()	do{RCC->APB2RSTR |= (1 << 6); RCC->APB2RSTR &= ~(1 << 6);} while(0)
#define GPIOF_REG_RESET()	do{RCC->APB2RSTR |= (1 << 7); RCC->APB2RSTR &= ~(1 << 7);} while(0)
#define GPIOG_REG_RESET()	do{RCC->APB2RSTR |= (1 << 8); RCC->APB2RSTR &= ~(1 << 8);} while(0)


/*
 * Macros EXTIR
 */
#define GPIO_BASEADDR_TO_CODE(x)	((x == GPIOA) ? 0 : \
									 (x == GPIOB) ? 1 : \
									 (x == GPIOC) ? 2 : \
									 (x == GPIOD) ? 3 : \
									 (x == GPIOE) ? 4 : \
									 (x == GPIOF) ? 5 : \
									 (x == GPIOG))

/*
 * Macros for IRQ External Interrupt
 */
#define IRQ_NO_EXTI0		6
#define IRQ_NO_EXTI1		7
#define IRQ_NO_EXTI2		8
#define IRQ_NO_EXTI3		9
#define IRQ_NO_EXTI4		10
#define IRQ_NO_EXTI9_5		23
#define IRQ_NO_EXTI15_10	40

/*
 * Macros for IRQ Timer Interrupt
 */
#define IRQ_NO_TIM1_BRK		24
#define IRQ_NO_TIM1_UP		25
#define IRQ_NO_TIM1_TRG_COM	26
#define IRQ_NO_TIM1_CC		27

#define IRQ_NO_TIM2			28
#define IRQ_NO_TIM3			29
#define IRQ_NO_TIM4			30

/*
 * Macros for SPI_CR1 Register
 */
#define SPI_CR1_CPHA		0
#define SPI_CR1_CPOL		1
#define SPI_CR1_MSTR		2
#define SPI_CR1_BR			3
#define SPI_CR1_SPE			6
#define SPI_CR1_LSBFIRST	7
#define SPI_CR1_SSI			8
#define SPI_CR1_SSM			9
#define SPI_CR1_RXONLY		10
#define SPI_CR1_DFF 		11
#define SPI_CR1_CRCNEXT		12
#define SPI_CR1_CRCEN		13
#define SPI_CR1_BIDIOE		14
#define SPI_CR1_BIDIMODE	15

/*
 * Macros for SPI_CR2 Register
 */
#define SPI_CR2_RXDMAEN		0
#define SPI_CR2_TXDMAEN		1
#define SPI_CR2_SSOE 		2
#define SPI_CR2_ERRIE		5
#define SPI_CR2_RXNEIE		6
#define SPI_CR2_TXEIE 		7

/*
 * Macros for SPI_SR Register
 */
#define SPI_SR_RXNE			0
#define SPI_SR_TXE			1
#define SPI_SR_CHSIDE		2
#define SPI_SR_UDR			3
#define SPI_SR_CRCERR		4
#define SPI_SR_MODF			5
#define SPI_SR_OVR			6
#define SPI_SR_BSY			7

/*
 * DeInit Macros for SPI (RCC)
 */
#define SPI1_REG_RESET()	do{RCC->APB2RSTR |= (1 << 12); RCC->APB2RSTR &= ~(1 << 12);} while(0)
#define SPI2_REG_RESET()	do{RCC->APB1RSTR |= (1 << 14); RCC->APB1RSTR &= ~(1 << 14);} while(0)
#define SPI3_REG_RESET()	do{RCC->APB1RSTR |= (1 << 15); RCC->APB1RSTR &= ~(1 << 15);} while(0)

/*
 * Macros for TIMx_CR1 Register
 */
#define TIM_CR1_CEN			0
#define TIM_CR1_UDIS		1
#define TIM_CR1_URS			2
#define TIM_CR1_OPM			3
#define TIM_CR1_DIR			4
#define TIM_CR1_CMS			5
#define TIM_CR1_ARPE		7
#define TIM_CR1_CKD			8

/*
 * Macros for TIMx_EGR Register
 */
#define TIM_EGR_UG			0
#define TIM_EGR_CC1G		1
#define TIM_EGR_CC2G		2
#define TIM_EGR_CC3G		3
#define TIM_EGR_CC4G		4
#define TIM_EGR_TG			6


/*
 * Macros for TIMx_CCMR1 Register (Output compare mode)
 */
#define TIM_CCMR1_CC1S			0
#define TIM_CCMR1_OC1FE			2
#define TIM_CCMR1_OC1PE			3
#define TIM_CCMR1_OC1M			4
#define TIM_CCMR1_OC1CE			7
#define TIM_CCMR1_CC2S			8
#define TIM_CCMR1_OC2FE			10
#define TIM_CCMR1_OC2PE			11
#define TIM_CCMR1_OC2M			12
#define TIM_CCMR1_OC2CE			15

/*
 * Macros for TIMx_CCMR2 Register (Output compare mode)
 */
#define TIM_CCMR2_CC3S			0
#define TIM_CCMR2_OC3FE			2
#define TIM_CCMR2_OC3PE			3
#define TIM_CCMR2_OC3M			4
#define TIM_CCMR2_OC3CE			7
#define TIM_CCMR2_CC4S			8
#define TIM_CCMR2_OC4FE			10
#define TIM_CCMR2_OC4PE			11
#define TIM_CCMR2_OC4M			12
#define TIM_CCMR2_OC4CE			15

/*
 * Macros for TIMx_CCER Register
 */
#define TIM_CCER_CC1E			0
#define TIM_CCER_CC1P			1
#define TIM_CCER_CC2E			4
#define TIM_CCER_CC2P			5
#define TIM_CCER_CC3E			8
#define TIM_CCER_CC3P			9
#define TIM_CCER_CC4E			12
#define TIM_CCER_CC4P			13

/*
 * Macros for TIMx_SMCR Register
 */
#define TIM_SMCR_SMS			0
#define TIM_SMCR_TS				4
#define TIM_SMCR_MSM			7
#define TIM_SMCR_ETF			8
#define TIM_SMCR_ETPS			12
#define TIM_SMCR_ECE			14
#define TIM_SMCR_ETP			15

/*
 * Macros for TIMx_DIER Register
 */
#define TIM_DIER_UIE			0
#define TIM_DIER_CC1IE			1
#define TIM_DIER_CC2IE			2
#define TIM_DIER_CC3IE			3
#define TIM_DIER_CC4IE			4
#define TIM_DIER_TIE			6
#define TIM_DIER_UDE			8
#define TIM_DIER_CC1DE			9
#define TIM_DIER_CC2DE			10
#define TIM_DIER_CC3DE			11
#define TIM_DIER_CC4DE			12
#define TIM_DIER_TDE			14

/*
 * Macros for TIMx_SR Register
 */
#define TIM_SR_UIF				0
#define TIM_SR_CC1IF			1
#define TIM_SR_CC2IF			2
#define TIM_SR_CC3IF			3
#define TIM_SR_CC4IF			4
#define TIM_SR_TIF				6
#define TIM_SR_CC1OF			9
#define TIM_SR_CC2OF			10
#define TIM_SR_CC3OF			11
#define TIM_SR_CC4OF			12













#endif /* INC_STM32F103C8TX_H_ */
