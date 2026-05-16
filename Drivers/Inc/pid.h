#ifndef __PID_H
#define __PID_H

#include <stdint.h>
#include <stm32_gpio.h>

/*
 * PID Speed
 */
typedef struct{
	float Kp;
	float Ki;
	float Kd;
	float P;
	float I;
	float D;
	float u;
} PID_Speed_t;

void PID_Speed_Control(uint16_t rpm, uint8_t dir, GPIO_RegDef_t *pGPIOx, uint8_t IN1, uint8_t IN2);

/*
 * PID Position
 */
typedef struct{
	float Kp;
	float Ki;
	float Kd;
	float P;
	float I;
	float D;
	float u;
} PID_Pos_t;

void PID_Pos_Control(uint16_t rpm, int32_t encoder_value, GPIO_RegDef_t *pGPIOx, uint8_t IN1, uint8_t IN2);









#endif	/*__PID_H*/
