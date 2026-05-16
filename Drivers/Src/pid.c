#include <pid.h>
#include <stm32f1.h>
#include <stdlib.h>

/*
 * PID Speed
 */

PID_Speed_t PID_speed_para = {
	.Kp = 7,		//13		13		7
	.Ki = 6,		//6			6		6
	.Kd = 0.02,		//0.05	0.04		0.02
	.P = 0,
	.I = 0,
	.D = 0,
	.u = 0,
};


volatile int16_t set_speed = 0;
int16_t set_speed_max = 110;
volatile int16_t error_speed = 0;
volatile int16_t error_speed_pre = 0;
volatile uint16_t pwm_PID_speed_controlled;
float delta_t = 0.1;

void PID_Speed_Control(uint16_t rpm, uint8_t dir, GPIO_RegDef_t *pGPIOx, uint8_t IN1, uint8_t IN2)
{
	if(set_speed > set_speed_max) set_speed = set_speed_max;
	if(set_speed < -set_speed_max) set_speed = -set_speed_max;
	int16_t signed_rpm;
	if(dir == 0)	signed_rpm = rpm;
	else signed_rpm = -rpm;

	error_speed = set_speed - signed_rpm;
	PID_speed_para.P = PID_speed_para.Kp * error_speed;
	PID_speed_para.I += PID_speed_para.Ki * (error_speed * delta_t);
	PID_speed_para.D = PID_speed_para.Kd * (error_speed - error_speed_pre) / delta_t;
	PID_speed_para.u = PID_speed_para.P + PID_speed_para.I + PID_speed_para.D;
	int16_t max_u = 999;
	int16_t max_i = 999;
	if(PID_speed_para.u > max_u) PID_speed_para.u = max_u;
	if(PID_speed_para.u < -max_u) PID_speed_para.u = -max_u;

	if(PID_speed_para.I > max_i) PID_speed_para.I = max_i;
	if(PID_speed_para.I < -max_i) PID_speed_para.I = -max_i;
	error_speed_pre = error_speed;
	if(PID_speed_para.u >= 0)								//Quay thuan
	{
		pGPIOx->BSRR = (1U << (IN2 + 16));		//RESET IN2 L298
		pGPIOx->BSRR = (1U << IN1);				//SET IN1 L298
	}
	else													//Quay nghich
	{
		pGPIOx->BSRR = (1U << IN2);				//SET IN2 L298
		pGPIOx->BSRR = (1U << (IN1 + 16));		//RESET IN1 L298
	}
	if(set_speed == 0)
	{
		pGPIOx->BSRR = (1U << (IN2 + 16));		//RESET IN2 L298
		pGPIOx->BSRR = (1U << (IN1 + 16));		//RESET IN1 L298
		pwm_PID_speed_controlled = 0;
		return;
	}
	pwm_PID_speed_controlled = abs((int)PID_speed_para.u);
}





/*
 * PID Position
 */

PID_Pos_t PID_pos_para = {
	.Kp = 1.35,				//	0.355 (1980)			0.355				1.35
	.Ki = 0,				//	0.1349						0.1349				0		pos_ok = 3 => 8/10
	.Kd = 0.15,					//	0.06							0.1				0.15	pwm_min 189
	.P = 0,
	.I = 0,
	.D = 0,
	.u = 0,
};

volatile int32_t set_pos_deg = 0;
volatile int32_t set_pos;
volatile int32_t current_pos = 0;
volatile int32_t error_pos = 0;
volatile int32_t error_pos_pre = 0;
volatile int8_t dir_status;
volatile uint16_t pwm_PID_pos_controlled;

void PID_Pos_Control(uint16_t rpm, int32_t encoder_value, GPIO_RegDef_t *pGPIOx, uint8_t IN1, uint8_t IN2)
{
	set_pos = set_pos_deg * 5.5;
	current_pos = encoder_value;
	error_pos = set_pos - current_pos;

	PID_pos_para.P = PID_pos_para.Kp * error_pos;
	if(abs(error_pos < 10)) PID_pos_para.I += PID_pos_para.Ki * (error_pos * delta_t);
	PID_pos_para.D = PID_pos_para.Kd * (error_pos - error_pos_pre) / delta_t;
	PID_pos_para.u = PID_pos_para.P + PID_pos_para.I + PID_pos_para.D;

	//// khai bao u_out
	float u_out = PID_pos_para.u;
	uint16_t pwm_min = 190;

	if(PID_pos_para.u > 0)								//Quay thuan
	{
		u_out += pwm_min;
		pGPIOx->BSRR = (1U << (IN2 + 16));		//RESET IN2 L298
		pGPIOx->BSRR = (1U << IN1);				//SET IN1 L298
		dir_status = 0;
	}
	else if(PID_pos_para.u < 0)											//Quay nghich
	{
		u_out -= pwm_min;
		pGPIOx->BSRR = (1U << IN2);				//SET IN2 L298
		pGPIOx->BSRR = (1U << (IN1 + 16));		//RESET IN1 L298
		dir_status = 1;
	}

	int16_t max_u = 999;
	if(PID_pos_para.u > max_u) u_out = max_u;
	if(PID_pos_para.u < -max_u) u_out = -max_u;


	pwm_PID_pos_controlled = abs((int)u_out);
//// khai bao pos_ok
	uint8_t pos_ok = 3; // position chap nhan duoc
	if(abs(error_pos) <= pos_ok)
	{
		GPIOB->BSRR = (1U << 28);		//RESET PIN12 - PORT B
		GPIOB->BSRR = (1U << 29);		//RESET PIN13 - PORT B
		pwm_PID_pos_controlled = 0;
		dir_status = 2;
	}

//// give up and accept point
	if( abs(error_pos) <= 5 && pwm_PID_pos_controlled <= 200)
	{
		pwm_PID_pos_controlled = 0;
		dir_status = 2;
	}
	error_pos_pre = error_pos;
}


