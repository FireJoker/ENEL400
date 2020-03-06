#ifndef _MOVEMENT_H_
#define _MOVEMENT_H_

#include "Motor.h"
#include "stm32f10x.h"

// GPIO Part and Pin define for motor driver input
#define Left_Motor_Port	GPIOA
#define Left_MotoA_Pin	GPIO_Pin_4
#define Left_MotoB_Pin	GPIO_Pin_5

#define Right_Motor_Port	GPIOB
#define Right_MotoA_Pin	GPIO_Pin_0
#define Right_MotoB_Pin	GPIO_Pin_1

// Control function for motor by (re)set Pins
#define LeftMotor_Forward(){\
	GPIO_SetBits(Left_Motor_Port, Left_MotoA_Pin);\
	GPIO_ResetBits(Left_Motor_Port, Left_MotoB_Pin);\
	}
#define LeftMotor_Back(){\
	GPIO_ResetBits(Left_Motor_Port, Left_MotoA_Pin);\
	GPIO_SetBits(Left_Motor_Port, Left_MotoB_Pin);\
	}
#define LeftMotor_Stop(){\
	GPIO_ResetBits(Left_Motor_Port, Left_MotoA_Pin);\
	GPIO_ResetBits(Left_Motor_Port, Left_MotoB_Pin);\
	}

#define RightMotor_Forward(){\
	GPIO_SetBits(Right_Motor_Port, Right_MotoA_Pin);\
	GPIO_ResetBits(Right_Motor_Port, Right_MotoB_Pin);\
	}
#define RightMotor_Back(){\
	GPIO_ResetBits(Right_Motor_Port, Right_MotoA_Pin);\
	GPIO_SetBits(Right_Motor_Port, Right_MotoB_Pin);\
	}
#define RightMotor_Stop(){\
	GPIO_ResetBits(Right_Motor_Port, Right_MotoA_Pin);\
	GPIO_ResetBits(Right_Motor_Port, Right_MotoB_Pin);\
	}

void Robot_Forward(void);
void Robot_Back(void);
void Robot_Stop(void);
void Robot_SpinLeft(void);
void Robot_SpinRight(void);

#endif
