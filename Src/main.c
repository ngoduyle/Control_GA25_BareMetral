/*
 * TIM 1: Time-base 1MHz để dùng hàm delay
 * TIM 2: Encoder CH1(PA0) CH2(PA1)
 * TIM 3: Time-base đọc rpm
 * TIM 4: PWM Motor: CH1 (PB6)
 *
 */



#include <stm32f1.h>
#include <stm32_gpio.h>
#include <stm32_tim.h>
#include <math.h>
#include <stdlib.h>
#include <pid.h>
#include <lcd.h>
#include <stdio.h>

#define ENCODER_PPR 1980

extern int16_t set_speed;
extern int16_t set_speed_max;
extern int32_t set_pos_deg;
extern int32_t set_pos;
extern int8_t dir_status;
extern int32_t current_pos;
extern uint16_t pwm_PID_speed_controlled;
extern uint16_t pwm_PID_pos_controlled;
/*
 * TIM
 */
TIM_Handle_t htim1;
TIM_Handle_t htim2;
TIM_Handle_t htim3;
TIM_Handle_t htim4;

/*
 * LCD
 */
LCD_Handle_t lcd1 = {
		.RS = {GPIOB, GPIO_PIN_12},
		.E 	= {GPIOB, GPIO_PIN_13},
		.D4 = {GPIOB, GPIO_PIN_14},
		.D5 = {GPIOB, GPIO_PIN_15},
		.D6 = {GPIOA, GPIO_PIN_10},
		.D7 = {GPIOA, GPIO_PIN_11},
		.pTIMx = TIM1,
};

void TIM_Base_User_Init(void);
void TIM_Encoder_User_Init(void);
void TIM_PWM_User_Init(void);
void GPIO_User_Init(void);

volatile uint16_t rpm = 0;
volatile uint8_t dir = 0;
volatile int32_t encoder_value = 0;
volatile int32_t encoder_value_pre = 0;
volatile int16_t now = 0;
volatile int16_t prev = 0;

volatile uint32_t tick100ms = 0;
/*
 * Ngắt TIM3 tính rpm và PID
 */
void TIM3_IRQHandler()
{
//	for(int i = 0; i < 50000; i++);
	TIM_IRQHandling(htim3.pTIMx);
	//code xử lý interrupt
		now = TIM_Encoder_GetCount(TIM2);
		int16_t delta;
		delta = now - prev;
		encoder_value += delta;
		if(delta > 0) dir = 0;		//quay thuan
		else dir = 1;				//quay nghich
		rpm = (abs(delta) *600) / ENCODER_PPR;	//chu ky sample: 100ms
		prev = now;

		//Ticks for Delay
		tick100ms += 100;
	//PID Speed
		PID_Speed_Control(rpm, dir, GPIOA, GPIO_PIN_6, GPIO_PIN_7);
		TIM_PWM_SetDuty(TIM4, TIM_CHANNEL_1, pwm_PID_speed_controlled);

	//PID Position
//		PID_Pos_Control(rpm, encoder_value, GPIOA, GPIO_PIN_6, GPIO_PIN_7);
//		TIM_PWM_SetDuty(TIM4, TIM_CHANNEL_1, pwm_PID_pos_controlled);
//		PID_pos_control();
}

/*
 * ngắt BTN tăng giảm
 */
void EXTI3_IRQHandler()
{
	GPIO_IRQHandling(GPIO_PIN_3);
		set_pos_deg += 10;
		if(set_speed < set_speed_max) set_speed += 10;

}

void EXTI4_IRQHandler()
{
	GPIO_IRQHandling(GPIO_PIN_4);
		set_pos_deg -= 10;
		if(set_speed > -set_speed_max) set_speed -= 10;
}

/*
 * Print to LCD
 */
void print_to_LCD_speed(uint16_t rpm, int16_t set_speed, uint8_t dir, uint16_t pwm)
{
	char rpmc[30];
	char pwmc[30];
	char set_speedc[30];
	if(dir == 0 || rpm == 0)
	{
		LCD_SetCursor(0, 0, &lcd1);
		sprintf(rpmc, "rpm:%-5d ", rpm);
		LCD_Print(rpmc, &lcd1);
	}
	else
	{
		LCD_SetCursor(0, 0, &lcd1);
		sprintf(rpmc, "rpm:-%-4d", rpm);
		LCD_Print(rpmc, &lcd1);
	}
		LCD_SetCursor(1, 0, &lcd1);
		sprintf(set_speedc, "set:%-4d", set_speed);
		LCD_Print(set_speedc, &lcd1);

		LCD_SetCursor(0, 9, &lcd1);
		sprintf(pwmc, "pwm:%2d%%", pwm/10);
		LCD_Print(pwmc, &lcd1);
}

void print_to_LCD_pos(int32_t pos, int32_t set_pos, uint8_t dir, uint16_t pwm)
{
	char posc[30];
	char pwmc[30];
	char set_posc[30];

		LCD_SetCursor(0, 0, &lcd1);
		sprintf(posc, "p:%-7d ",(int)round(pos/5.5));
		LCD_Print(posc, &lcd1);

		LCD_SetCursor(1, 0, &lcd1);
		sprintf(set_posc, "s:%-8d",(int)round(set_pos/5.5));
		LCD_Print(set_posc, &lcd1);

		LCD_SetCursor(0, 10, &lcd1);
		sprintf(pwmc, "pwm:%2d", pwm/10);
		LCD_Print(pwmc, &lcd1);

		LCD_SetCursor(1, 10, &lcd1);
	if(dir == 0)
	{
		LCD_Print("Th    ", &lcd1);
	}
	else if(dir == 1)
	{
		LCD_Print("Ng    ", &lcd1);
	}
	else if(dir == 2)	LCD_Print("SET!!!", &lcd1);
}

int main()
{
	//Init
	TIM_Base_User_Init();
	GPIO_User_Init();
	GPIO_LCD_Init(&lcd1);
	TIM_Encoder_User_Init();
	TIM_PWM_User_Init();

	//Start
	TIM_Base_Start(TIM1);
	TIM_Base_Start_IT(htim3.pTIMx);
	TIM_Encoder_Start(TIM2);
	TIM_PWM_Start(TIM4, TIM_CHANNEL_1);
	TIM_PWM_Start(TIM4, TIM_CHANNEL_2);
	LCD_Init(&lcd1);

	//ngắt cho TIM3 Time-base tính rpm
	TIM_IRQInterruptConfig(IRQ_NO_TIM3, ENABLE);
	TIM_IRQPriorityConfig(IRQ_NO_TIM3, 3);

	//ngắt cho BTN tăng
	GPIO_IRQInterruptConfig(IRQ_NO_EXTI3, ENABLE);
	GPIO_IRQPriorityConfig(IRQ_NO_EXTI3, 1);

	//ngắt cho BTN giảm
	GPIO_IRQInterruptConfig(IRQ_NO_EXTI4, ENABLE);
	GPIO_IRQPriorityConfig(IRQ_NO_EXTI4, 2);

	uint32_t last_lcd_tick = 0;
	while(1)
	{
		if((tick100ms - last_lcd_tick) >= 200)
		{
			last_lcd_tick = tick100ms;
//			print_to_LCD_pos(current_pos, set_pos, dir_status, pwm_PID_pos_controlled);
			print_to_LCD_speed(rpm, set_speed, dir, pwm_PID_speed_controlled);
		}

	}
}

void GPIO_User_Init()
{
	GPIO_Handle_t GPIOHandle;
		GPIO_Handle_t *pGPIOHandle = &GPIOHandle;

		pGPIOHandle->pGPIOx = GPIOA;
		pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_0;	//PA0: Ch1 TIM2
		pGPIOHandle->GPIO_PinConfig.GPIO_CNF = GPIO_CNF_IN_FLOATING;
		pGPIOHandle->GPIO_PinConfig.GPIO_MODE = GPIO_MODE_INPUT;
		GPIO_Init(pGPIOHandle);

		pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_1;	//PA1: Ch2 TIM2
		GPIO_Init(pGPIOHandle);

		pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_3;	//PB10: BTN tăng
		pGPIOHandle->GPIO_PinConfig.GPIO_CNF = GPIO_CNF_IN_PUPD;
		pGPIOHandle->GPIO_PinConfig.GPIO_MODE = GPIO_MODE_IT_FT;
		pGPIOHandle->GPIO_PinConfig.GPIO_Pull = GPIO_INPUT_PULLUP;
		GPIO_Init(pGPIOHandle);

		pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_4;	//PB11: BTN giảm
		pGPIOHandle->GPIO_PinConfig.GPIO_CNF = GPIO_CNF_IN_PUPD;
		pGPIOHandle->GPIO_PinConfig.GPIO_MODE = GPIO_MODE_IT_FT;
		pGPIOHandle->GPIO_PinConfig.GPIO_Pull = GPIO_INPUT_PULLUP;
		GPIO_Init(pGPIOHandle);
		pGPIOHandle->pGPIOx = GPIOA;
		pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_6;	//PA6: IN1 L298
		pGPIOHandle->GPIO_PinConfig.GPIO_CNF = GPIO_CNF_OUT_PP;
		pGPIOHandle->GPIO_PinConfig.GPIO_MODE = GPIO_MODE_OUTPUT_10MHZ;
		GPIO_Init(pGPIOHandle);

		pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_7;	//PA7: IN2 L298
		pGPIOHandle->GPIO_PinConfig.GPIO_CNF = GPIO_CNF_OUT_PP;
		pGPIOHandle->GPIO_PinConfig.GPIO_MODE = GPIO_MODE_OUTPUT_10MHZ;
		GPIO_Init(pGPIOHandle);

		pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_9;	//PA9: LED
		pGPIOHandle->GPIO_PinConfig.GPIO_CNF = GPIO_CNF_OUT_PP;
		pGPIOHandle->GPIO_PinConfig.GPIO_MODE = GPIO_MODE_OUTPUT_10MHZ;
		GPIO_Init(pGPIOHandle);

		pGPIOHandle->pGPIOx = GPIOB;
		pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_6;	//PB6: Ch1 TIM4 -> PWM Motor
		pGPIOHandle->GPIO_PinConfig.GPIO_CNF = GPIO_CNF_ALT_PP;
		pGPIOHandle->GPIO_PinConfig.GPIO_MODE = GPIO_MODE_OUTPUT_10MHZ;
		GPIO_Init(pGPIOHandle);

		pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_7;	//PB7: Ch2 TIM4	-> PWM LED
		pGPIOHandle->GPIO_PinConfig.GPIO_CNF = GPIO_CNF_ALT_PP;
		pGPIOHandle->GPIO_PinConfig.GPIO_MODE = GPIO_MODE_OUTPUT_10MHZ;
		GPIO_Init(pGPIOHandle);
}

void TIM_Base_User_Init()
{
	htim3.pTIMx = TIM3;
	htim3.TIMConfig.TIM_CounterMode = TIM_COUNTERMODE_UP;
	htim3.TIMConfig.TIM_Prescaler = 7999;
	htim3.TIMConfig.TIM_Period = 99;
	TIM_Base_Init(&htim3);

	htim1.pTIMx = TIM1;
	htim1.TIMConfig.TIM_CounterMode = TIM_COUNTERMODE_UP;
	htim1.TIMConfig.TIM_Prescaler = 7999;
	htim1.TIMConfig.TIM_Period = 0xFFFF;
	TIM_Base_Init(&htim1);
}

void TIM_Encoder_User_Init()
{
	htim2.pTIMx = TIM2;
	htim2.TIMConfig.TIM_EncoderMode = TIM_ENCODER_MODE_3;
	TIM_Encoder_Init(&htim2);
}

void TIM_PWM_User_Init()
{
	htim4.pTIMx 						= TIM4;
	htim4.TIMConfig.TIM_Channel 		= TIM_CHANNEL_1;	//PWM Motor
	htim4.TIMConfig.TIM_CounterMode 	= TIM_COUNTERMODE_UP;
	htim4.TIMConfig.TIM_PWMMode 		= TIM_PWM_MODE_1;
	htim4.TIMConfig.TIM_Prescaler 		= 7;
	htim4.TIMConfig.TIM_Period 			= 999;
	htim4.TIMConfig.TIM_Duty 			= 230;
	TIM_PWM_Init(&htim4);

	htim4.TIMConfig.TIM_Channel 		= TIM_CHANNEL_2;	//PWM LED debug
	htim4.TIMConfig.TIM_CounterMode 	= TIM_COUNTERMODE_UP;
	htim4.TIMConfig.TIM_PWMMode 		= TIM_PWM_MODE_1;
	htim4.TIMConfig.TIM_Prescaler 		= 7;
	htim4.TIMConfig.TIM_Period 			= 999;
	htim4.TIMConfig.TIM_Duty 			= 500;
	TIM_PWM_Init(&htim4);
}
