#ifndef MOTOR_H
#define MOTOR_H

#include "stm32f10x.h"
#include "stdint.h"

void RCC_Init(void);
void TIM3_PWM_Init(void);
void TIM3_IO_Init(void);
void TIM3_MODE_Init(void);
void TIM3_PWM_Updete(uint16_t CCR1_Val, uint16_t CCR2_Val);

void TIM2_Encoder_Init(void);
void TIM4_Encoder_Init(void);

#endif
