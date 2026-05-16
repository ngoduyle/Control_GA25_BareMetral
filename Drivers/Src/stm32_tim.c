#include <stm32_tim.h>

/*
 * PWM
 */

void TIM_PeriClockControl(TIM_RegDef_t *pTIMx, uint8_t ENorDI)
{
	if(ENorDI == ENABLE)
	{
		if(pTIMx == TIM1)	TIM1_PCLK_EN();
		else if(pTIMx == TIM2)	TIM2_PCLK_EN();
		else if(pTIMx == TIM3)	TIM3_PCLK_EN();
		else if(pTIMx == TIM4)	TIM4_PCLK_EN();
	}
	else
	{
		if(pTIMx == TIM1)	TIM1_PCLK_DI();
		else if(pTIMx == TIM2)	TIM2_PCLK_DI();
		else if(pTIMx == TIM3)	TIM3_PCLK_DI();
		else if(pTIMx == TIM4)	TIM4_PCLK_DI();
	}
}
	/*
	TIM_Handle_t TIM3_PWM;

	TIM3_PWM.pTIMx = TIM3;
	TIM3_PWM.TIMConfig.TIM_Prescaler = 8 - 1;
	TIM3_PWM.TIMConfig.TIM_Period = 1000 - 1;
	TIM3_PWM.TIMConfig.TIM_Channel = TIM_CHANNEL_1;
	TIM3_PWM.TIMConfig.TIM_PWMMode = TIM_PWM_MODE_1;
	TIM3_PWM.TIMConfig.TIM_Duty = 500;

	TIM_PWM_Init(&TIM3_PWM);
	 */


void TIM_PWM_Init(TIM_Handle_t *pTIMHandle)
{


	/*
	 * 0. Enable clock TIMER
	 */
	TIM_PeriClockControl(pTIMHandle->pTIMx, ENABLE);

	//1. Disable counter before configuration
	pTIMHandle->pTIMx->CR1 &= ~(1U << TIM_CR1_CEN);	//CEN = 0
	/*
	 * 2. Set counter mode
	 */
	if(pTIMHandle->TIMConfig.TIM_CounterMode == TIM_COUNTERMODE_UP)
	{
		pTIMHandle->pTIMx->CR1 &= ~(1U << TIM_CR1_DIR);		//DIR = 0
	}
	else
	{
		pTIMHandle->pTIMx->CR1 |= (1U << TIM_CR1_DIR);		//DIR = 1
	}
	/*
	 * 3. Set prescaler PSC
	 */
	pTIMHandle->pTIMx->PSC = pTIMHandle->TIMConfig.TIM_Prescaler;
	/*
	 * 4. Set period ARR
	 */
	pTIMHandle->pTIMx->ARR = pTIMHandle->TIMConfig.TIM_Period;
	/*
	 * 5. Set duty ban đầu khi init PWM (thanh ghi CCRx)
	 */
	TIM_PWM_SetDuty(pTIMHandle->pTIMx, \
					pTIMHandle->TIMConfig.TIM_Channel, \
					pTIMHandle->TIMConfig.TIM_Duty);
	/*
	 * 6. PWM mode + preload
	 */
	if(pTIMHandle->TIMConfig.TIM_Channel == TIM_CHANNEL_1)
	{
		//Configure CC1S bits 1:0
		pTIMHandle->pTIMx->CCMR1 &= ~(3U << TIM_CCMR1_CC1S);	//Cấu hình channel 1 là output
		//Configure OC1PE bit 3
		pTIMHandle->pTIMx->CCMR1 |= (1U << TIM_CCMR1_OC1PE);	// bật preload (khi CCR thay đổi thì không áp dụng ngay mà chờ hết chu kỳ mới đổi)
		//Configure OC1M bits 6:4
		pTIMHandle->pTIMx->CCMR1 &= ~(7U << TIM_CCMR1_OC1M);		//reset các bit của OC1M
		pTIMHandle->pTIMx->CCMR1 |= (pTIMHandle->TIMConfig.TIM_PWMMode << TIM_CCMR1_OC1M);	//set cấu hình PWM_Mode
	}
	else if(pTIMHandle->TIMConfig.TIM_Channel == TIM_CHANNEL_2)
	{
		//Configure CC1S bits 1:0
		pTIMHandle->pTIMx->CCMR1 &= ~(3U << TIM_CCMR1_CC2S);	//Cấu hình channel 2 là output
		//Configure OC1PE bit 3
		pTIMHandle->pTIMx->CCMR1 |= (1U << TIM_CCMR1_OC2PE);	// bật preload (khi CCR thay đổi thì không áp dụng ngay mà chờ hết chu kỳ mới đổi)
		//Configure OC1M bits 6:4
		pTIMHandle->pTIMx->CCMR1 &= ~(7U << TIM_CCMR1_OC2M);		//reset các bit của OC2M
		pTIMHandle->pTIMx->CCMR1 |= (pTIMHandle->TIMConfig.TIM_PWMMode << TIM_CCMR1_OC2M);	//set cấu hình PWM_Mode
	}
	else if(pTIMHandle->TIMConfig.TIM_Channel == TIM_CHANNEL_3)
	{
		//Configure CC1S bits 1:0
		pTIMHandle->pTIMx->CCMR2 &= ~(3U << TIM_CCMR2_CC3S);	//Cấu hình channel 3 là output
		//Configure OC1PE bit 3
		pTIMHandle->pTIMx->CCMR2 |= (1U << TIM_CCMR2_OC3PE);	// bật preload (khi CCR thay đổi thì không áp dụng ngay mà chờ hết chu kỳ mới đổi)
		//Configure OC1M bits 6:4
		pTIMHandle->pTIMx->CCMR2 &= ~(7U << TIM_CCMR2_OC3M);		//reset các bit của OC3M
		pTIMHandle->pTIMx->CCMR2 |= (pTIMHandle->TIMConfig.TIM_PWMMode << TIM_CCMR2_OC3M);	//set cấu hình PWM_Mode
	}
	else if(pTIMHandle->TIMConfig.TIM_Channel == TIM_CHANNEL_4)
	{
		//Configure CC1S bits 1:0
		pTIMHandle->pTIMx->CCMR2 &= ~(3U << TIM_CCMR2_CC4S);	//Cấu hình channel 2 là output
		//Configure OC1PE bit 3
		pTIMHandle->pTIMx->CCMR2 |= (1U << TIM_CCMR2_OC4PE);	// bật preload (khi CCR thay đổi thì không áp dụng ngay mà chờ hết chu kỳ mới đổi)
		//Configure OC1M bits 6:4
		pTIMHandle->pTIMx->CCMR2 &= ~(7U << TIM_CCMR2_OC4M);		//reset các bit của OC2M
		pTIMHandle->pTIMx->CCMR2 |= (pTIMHandle->TIMConfig.TIM_PWMMode << TIM_CCMR2_OC4M);	//set cấu hình PWM_Mode
	}
	/*
	 * 7. Enable preload cho ARR
	 */
	pTIMHandle->pTIMx->CR1 |= (1U << TIM_CR1_ARPE);	//ARPE = 1
	/*
	 * 8. Generate update event
	 */
	pTIMHandle->pTIMx->EGR |= (1U << TIM_EGR_UG);	//UG = 1
	/*
	 * 9. Nếu TIM1 thì Enable BDTR.MOE
	 */
	if(pTIMHandle->pTIMx == TIM1)
	{
		pTIMHandle->pTIMx->BDTR |= (1U << 15);  //MOE = 1		(TIM_BDTR_MOE = 15)
	}
}
	/*
	 * TIM_PWM_Start: cho phép PWM xuất ra ngoài PIN (thanh ghi CCER)
	 */
void TIM_PWM_Start(TIM_RegDef_t *pTIMx, uint8_t Channel)
{
	if(Channel == TIM_CHANNEL_1)
	{
		pTIMx->CCER |= (1U << TIM_CCER_CC1E);
	}
	else if(Channel == TIM_CHANNEL_2)
	{
		pTIMx->CCER |= (1U << TIM_CCER_CC2E);
	}
	else if(Channel == TIM_CHANNEL_3)
	{
		pTIMx->CCER |= (1U << TIM_CCER_CC3E);
	}
	else if(Channel == TIM_CHANNEL_4)
	{
		pTIMx->CCER |= (1U << TIM_CCER_CC4E);
	}

	if(pTIMx == TIM1)
	{
		pTIMx->BDTR |= (1U << 15);
	}

	/*
	 * Enable counter
	 */
	pTIMx->CR1 |= (1U << TIM_CR1_CEN);
}

void TIM_PWM_Stop(TIM_RegDef_t *pTIMx, uint8_t Channel)
{
	if(Channel == TIM_CHANNEL_1)
	{
		pTIMx->CCER &= ~(1U << TIM_CCER_CC1E);
	}
	else if(Channel == TIM_CHANNEL_2)
	{
		pTIMx->CCER &= ~(1U << TIM_CCER_CC2E);
	}
	else if(Channel == TIM_CHANNEL_3)
	{
		pTIMx->CCER &= ~(1U << TIM_CCER_CC3E);
	}
	else if(Channel == TIM_CHANNEL_4)
	{
		pTIMx->CCER &= ~(1U << TIM_CCER_CC4E);
	}

	if(pTIMx == TIM1)
	{
		pTIMx->BDTR &= ~(1U << 15);
	}
}
void TIM_PWM_SetDuty(	TIM_RegDef_t *pTIMx, uint8_t Channel, uint16_t Duty)
{
	if(Duty > pTIMx->ARR)	Duty = pTIMx->ARR;

	if(Channel == TIM_CHANNEL_1)
	{
		pTIMx->CCR1 = Duty;
	}
	else if(Channel == TIM_CHANNEL_2)
	{
		pTIMx->CCR2 = Duty;
	}
	else if(Channel == TIM_CHANNEL_3)
	{
		pTIMx->CCR3 = Duty;
	}
	else if(Channel == TIM_CHANNEL_4)
	{
		pTIMx->CCR4 = Duty;
	}
}

/*
 * Encoder Mode
 */

void TIM_Encoder_Init(TIM_Handle_t *pTIMHandle)
{
	//0. Enable Timer clock
	TIM_PeriClockControl(pTIMHandle->pTIMx, ENABLE);

	//1. Disable counter before configuration
	pTIMHandle->pTIMx->CR1 &= ~(1U << TIM_CR1_CEN);	//CEN = 0

	//2. Configure CH1 and CH2 as input (thanh ghi CCMR1, bit CCxS)
	pTIMHandle->pTIMx->CCMR1 &= ~(3U << TIM_CCMR1_CC1S);//Clear field CC1S
	pTIMHandle->pTIMx->CCMR1 |= (1U << TIM_CCMR1_CC1S); //CC1S = 01:  CC1 channel is configured as input, IC1 (Input Capture 1) is mapped on TI1 (Timer Input 1).
	pTIMHandle->pTIMx->CCMR1 &= ~(3U << TIM_CCMR1_CC2S);//Clear field CC2S
	pTIMHandle->pTIMx->CCMR1 |= (1U << TIM_CCMR1_CC2S); //CC2S = 01:  CC2 channel is configured as input, IC2 (Input Capture 2) is mapped on TI2 (Timer Input 2).

	//3. Configure Polarity
	pTIMHandle->pTIMx->CCER &= ~(1U << TIM_CCER_CC1P);	//CC1P = 0: non-inverted, capture ở rising edge
	pTIMHandle->pTIMx->CCER &= ~(1U << TIM_CCER_CC2P);	//CC2P = 0: non-inverted, capture ở rising edge
	//4. Select Encoder Mode 3 (encoder 2 kênh A và B)
	pTIMHandle->pTIMx->SMCR &= ~(7U << TIM_SMCR_SMS);	//clear SMS field: SMS = 000
	pTIMHandle->pTIMx->SMCR |= (pTIMHandle->TIMConfig.TIM_EncoderMode << TIM_SMCR_SMS); 	// set SMS = 011 : Encoder mode 3, counter count ở cả TI1 và TI2
	//5. Set ARR value
	pTIMHandle->pTIMx->ARR = 0xFFFF;
	//6. Reset counter
	pTIMHandle->pTIMx->CNT = 0;
}

void TIM_Encoder_Start(TIM_RegDef_t *pTIMx)
{
	pTIMx->CR1 |= (1U << TIM_CR1_CEN);
}

uint16_t TIM_Encoder_GetCount(TIM_RegDef_t *pTIMx)
{
	return (uint16_t)(pTIMx->CNT);
}

void TIM_Encoder_Reset (TIM_RegDef_t *pTIMx)
{
	pTIMx->CNT = 0;
}


/*
 * TIME-BASE
 */

void TIM_Base_Init(TIM_Handle_t *pTIMHandle)
{

	//0. Enable clock
	TIM_PeriClockControl(pTIMHandle->pTIMx, ENABLE);

	//1. Disable timer before configuration
	pTIMHandle->pTIMx->CR1 &= ~(1U << TIM_CR1_CEN);

	//2. Configure counter mode
	if(pTIMHandle->TIMConfig.TIM_CounterMode == TIM_COUNTERMODE_UP)
	{
		pTIMHandle->pTIMx->CR1 &= ~(1U << TIM_CR1_DIR);
	}
	else
	{
		pTIMHandle->pTIMx->CR1 |= (1U << TIM_CR1_DIR);
	}

	//3. Set prescaler và ARR
	pTIMHandle->pTIMx->PSC = pTIMHandle->TIMConfig.TIM_Prescaler;
	pTIMHandle->pTIMx->ARR = pTIMHandle->TIMConfig.TIM_Period;

	//4. (Optional) enable ARR Preload
//	pTIMHandle->pTIMx->CR1 |= (1U << TIM_CR1_ARPE);

	//5. Reset counter
	pTIMHandle->pTIMx->CNT = 0;
}


void TIM_Base_Start(TIM_RegDef_t *pTIMx)
{
	//Enable counter
	pTIMx->CR1 |= (1U << TIM_CR1_CEN);
}
void TIM_Base_Stop(TIM_RegDef_t *pTIMx)
{
	//Disable counter
	pTIMx->CR1 &= ~(1U << TIM_CR1_CEN);
}
void TIM_Base_Start_IT(TIM_RegDef_t *pTIMx)
{
	//Enable counter
	pTIMx->CR1 |= (1U << TIM_CR1_CEN);

	//Enable update interrupt (thanh ghi DIER, field UIE)
	pTIMx->DIER |= (1U << TIM_DIER_UIE);
}
void TIM_Base_Stop_IT(TIM_RegDef_t *pTIMx)
{
	//Enable counter
	pTIMx->CR1 &= ~(1U << TIM_CR1_CEN);

	//Enable update interrupt (thanh ghi DIER, field UIE)
	pTIMx->DIER &= ~(1U << TIM_DIER_UIE);
}

void TIM_IRQInterruptConfig(uint8_t IRQNumber, uint8_t ENorDI)
{
	//Thanh ghi ISER để enable interrupt và thanh ghi ICER để disable interrupt
	if(ENorDI == ENABLE)
	{
		if(IRQNumber < 32)
		{
			*NVIC_ISER0 |= (1U << IRQNumber);	//* là toán tử giải tham chiếu
		}
		else if(IRQNumber < 64)
		{
			*NVIC_ISER1 |= (1U << (IRQNumber % 32));	//* là toán tử giải tham chiếu
		}
	}
	else
	{
		if(IRQNumber < 32)
		{
			*NVIC_ICER0 |= (1U << IRQNumber);	//* là toán tử giải tham chiếu
		}
		else if(IRQNumber < 64)
		{
			*NVIC_ICER1 |= (1U << (IRQNumber % 32));	//* là toán tử giải tham chiếu
		}
	}
}
void TIM_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority)
{
	//Figure 19. NVIC_IPRx register mapping (Arm Cortex M3 User guide)
	//Thanh ghi IPRx (và các field IPx) để config mức độ ưu tiên. Số IRQPriority càng nhỏ thì mức độ ưu tiên càng cao
	uint32_t IPR_No = IRQNumber / 4;
	uint8_t IP_No = IRQNumber % 4;
	/*
	 * Chỉ dùng 4 bit cao, vì: The processor implements only bits[7:4] of each field, bits[3:0] read as zero and ignore writes.
	 */
	*(NVIC_IPR_BASEADDR + IPR_No) &= ~(15U << (IP_No * 8 + 4)); // clear 4 bit cao, vì field có nhiều bit //* là toán tử giải tham chiếu
	*(NVIC_IPR_BASEADDR + IPR_No) |= (IRQPriority << (IP_No * 8 + 4)); //set 4 bit cao //* là toán tử giải tham chiếu
}
void TIM_IRQHandling(TIM_RegDef_t *pTIMx)
{
	//Check UIF flag (update interrupt flag)
	if(pTIMx->SR & (1U << TIM_SR_UIF))	//nếu bit UIF đang được set
	{
		//clear bit UIF
		pTIMx->SR &= ~(1U << TIM_SR_UIF);
	}
}
