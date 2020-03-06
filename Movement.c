#include "Movement.h"

/**
* Function       Robot_Forward 
* @brief         The robot move forward
* @param[in]     void
* @param[out]    void
* @retval        void
*/
void Robot_Forward(void){
	LeftMotor_Forward();
	RightMotor_Forward()
}

/**
* Function       Robot_Back 
* @brief         The robot move forward
* @param[in]     void
* @param[out]    void
* @retval        void
*/
void Robot_Back(void){
	LeftMotor_Back();
	RightMotor_Back();
}

/**
* Function       Robot_Stop 
* @brief         The robot stop
* @param[in]     void
* @param[out]    void
* @retval        void
*/
void Robot_Stop(void){
	LeftMotor_Stop();
	RightMotor_Stop();
}

/**
* Function       Robot_SpinLeft
* @brief         The robot spin left
* @param[in]     void
* @param[out]    void
* @retval        void
*/
void Robot_SpinLeft(void){
	LeftMotor_Back();
	RightMotor_Forward()
}

/**
* Function       Robot_SpinRight 
* @brief         The robot spin right
* @param[in]     void
* @param[out]    void
* @retval        void
*/
void Robot_SpinRight(void){
	LeftMotor_Forward();
	RightMotor_Back();
}
