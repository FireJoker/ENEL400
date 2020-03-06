#ifndef _CONTROL_H_
#define _CONTROL_H_

#include "stm32f10x.h"


// 
#define DISTANCE_STRAIGHT 500 // mm
#define WHEEL_DIAMETER 65 // mm
#define WHEEL_DISTANCE 223 // mm
#define PI 3.14
#define WHEEL_PERIMTER WHEEL_DIAMETER*PI
#define ENCODER_PULSE 780 // pulse pre circle

#define DISTANCE_SPIN 2*PI*WHEEL_DISTANCE/4 // 90 degree

#define PULSE_STRAIGHT (DISTANCE_STRAIGHT/WHEEL_PERIMTER)*ENCODER_PULSE
#define PULSE_SPIN (DISTANCE_SPIN/WHEEL_PERIMTER)*ENCODER_PULSE


#define MOTOR_OUT_MAX 100
#define MOTOR_OUT_MIN (-100)


//void GetMotorPulse(void);


#endif
