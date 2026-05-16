#ifndef INC_STM32_TIM_H_
#define INC_STM32_TIM_H_

#include <stm32f1.h>

typedef struct
{
//PWM
	uint16_t	TIM_Prescaler;
	uint16_t 	TIM_Period;			//ARR
	uint8_t 	TIM_Channel;
	uint8_t 	TIM_PWMMode;
	uint16_t	TIM_Duty;			//CCR
	uint8_t 	TIM_CounterMode;
//Encoder
	uint8_t		TIM_EncoderMode;

}TIM_Config_t;

typedef struct
{
	TIM_RegDef_t *pTIMx;
	TIM_Config_t TIMConfig;
}TIM_Handle_t;

/*
 * Macros for TIM Channel
 */
#define TIM_CHANNEL_1		1
#define TIM_CHANNEL_2		2
#define TIM_CHANNEL_3		3
#define TIM_CHANNEL_4		4

/*
 * Macros for PWM Mode
 */
#define TIM_PWM_MODE_1		6	//OCxM = 110
#define TIM_PWM_MODE_2		7	//OCxM = 111

/*
 * Macros for Counter Mode
 */
#define TIM_COUNTERMODE_UP		0
#define TIM_COUNTERMODE_DOWN	1

/*
 * Macros for Encoder Mode
 */
#define TIM_ENCODER_MODE_1		1
#define TIM_ENCODER_MODE_2		2
#define TIM_ENCODER_MODE_3		3
void TIM_PeriClockControl(TIM_RegDef_t *pTIMx, uint8_t ENorDI);

/*
 * APIs for PWM
 */
void TIM_PWM_Init(TIM_Handle_t *pTIMHandle);
void TIM_PWM_DeInit(TIM_RegDef_t *pTIMx);
void TIM_PWM_Start(TIM_RegDef_t *pTIMx, uint8_t Channel);
void TIM_PWM_Stop(TIM_RegDef_t *pTIMx, uint8_t Channel);
void TIM_PWM_SetDuty(	TIM_RegDef_t *pTIMx, uint8_t Channel, uint16_t Duty);

/*
 * APIs for Encoder Mode
 */
void TIM_Encoder_Init(TIM_Handle_t *pTIMHandle);
void TIM_Encoder_Start(TIM_RegDef_t *pTIMx);
uint16_t TIM_Encoder_GetCount(TIM_RegDef_t *pTIMx);
void TIM_Encoder_Reset (TIM_RegDef_t *pTIMx);

/*
 * APIs for Time-base Mode
 */
void TIM_Base_Init(TIM_Handle_t *pTIMHandle);
void TIM_Base_Start(TIM_RegDef_t *pTIMx);
void TIM_Base_Stop(TIM_RegDef_t *pTIMx);
void TIM_Base_Start_IT(TIM_RegDef_t *pTIMx);
void TIM_Base_Stop_IT(TIM_RegDef_t *pTIMx);

void TIM_IRQInterruptConfig(uint8_t IRQNumber, uint8_t ENorDI);
void TIM_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority);
void TIM_IRQHandling(TIM_RegDef_t *pTIMx);












#endif /*INC_STM32_TIM_H_*/
