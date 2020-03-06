#include "stm32f10x.h"
#include "stdint.h"

#include "Motor.h"
#include "Movement.h"
#include "Control.h"

int main(void)
{	
	RCC_Init();
	TIM3_PWM_Init();
	TIM2_Encoder_Init();
	TIM4_Encoder_Init();
	
	while(1){
		
		for (int i = 0; i < 6000000; i++){}		
		Robot_Forward();
		while(TIM_GetCounter(TIM2) < PULSE_STRAIGHT){}
		TIM_SetCounter(TIM2,0);
		Robot_Stop();		
				
			
		for (int i = 0; i < 6000000; i++){}
		Robot_Back();
		while(TIM_GetCounter(TIM2) < PULSE_STRAIGHT){}
		TIM_SetCounter(TIM2,0);		
		Robot_Stop();				
			
		for (int i = 0; i < 6000000; i++){}
		Robot_SpinLeft();		
		while(TIM_GetCounter(TIM2) < PULSE_SPIN){}
		TIM_SetCounter(TIM2,0);		
		Robot_Stop();		
			
		for (int i = 0; i < 6000000; i++){}
		Robot_SpinRight();
		while(TIM_GetCounter(TIM2) < PULSE_SPIN){}
		TIM_SetCounter(TIM2,0);		
		Robot_Stop();		
	}
		
}


