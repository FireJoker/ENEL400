#include "Clock.h"
#include "Motor.h"
#include "stm32f10x.h"
#include "stdint.h"

/* motor driver pins connection
PWMA TIM3_CH1 -> PA6, GPIO -> PA4, PA5
PWMB TIM3_CH2 -> PA7, GPIO -> PB0, PB1
Encoder_L TIM2_CH1 -> PA0, TIM2_CH2 -> PA1
Encoder_R TIM4_CH1 -> PB6, TIM4_CH2 -> PB7
*/


void RCC_Init(void){

	uint32_t temp = 0x00;

	RCC->CR |= 0x01010081;			// Turn on PLL, HSE, HSI
	RCC->CFGR |= 0x00050002;		// 24MHz system clock
	
	while (temp != 0x02000000)  // Wait for the PLL to stabilize
	{
		temp = RCC->CR & 0x02000000; // Check to see if the PLL lock bit is set
	}
 }

/**
* Function       TIM3_PWM_INIT 
* @brief         Set TIM3 as PWM output, and pins setup
* @param[in]     void
* @param[out]    void
* @retval        void
*/
void TIM3_PWM_Init(void){

	TIM3_IO_Init();
	TIM3_MODE_Init();
}

/**
* Function       TIM3_IO_Init 
* @brief         2-channel PWM, and GPIO setup
* @param[in]     void
* @param[out]    void
* @retval        void
*/
static void TIM3_IO_Init(void){

	GPIO_InitTypeDef GPIO_InitStructure;

  // GPIOA, GPIOB, TIM3 enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 
	
	// PA4, PA5 as general purpose output push-pull
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// PB0, PB1 as general purpose output push-pull
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);		  
	
  // PA6, PA7 as alternate function output push-pull
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}
	
/**
* Function       TIM3_MODE_Init 
* @brief         Set pin config
* @param[in]     void
* @param[out]    void
* @retval        void
*/
static void TIM3_MODE_Init(void){

  // Time base configuration	 	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;		
	
	//PSC = (SYSCLK / PSC_CLK) - 1 = (24M/1K)-1
  TIM_TimeBaseStructure.TIM_Period = 100;
  TIM_TimeBaseStructure.TIM_Prescaler = 23999;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	
	TIM_OCInitTypeDef  TIM_OCInitStructure;

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = 50;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;


  TIM_OC1Init(TIM3, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

  TIM_OCInitStructure.TIM_Pulse = 50;
  TIM_OC2Init(TIM3, &TIM_OCInitStructure);
  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);	
	
  TIM_ARRPreloadConfig(TIM3, ENABLE);

  TIM_Cmd(TIM3, ENABLE); 	
}

/**
* Function       TIM3_PWM_Updete
* @brief         PWM duty cycle update
* @param[in]     uint16_t CCR1_Val, uint16_t CCR2_Val
* @param[out]    void
* @retval        void
*/
void TIM3_PWM_Updete(uint16_t CCR1_Val, uint16_t CCR2_Val){
	
	TIM3->CCR1 = CCR1_Val;	// Defines Pulse Width, duty cycle
	TIM3->CCR2 = CCR2_Val;	// Defines Pulse Width, duty cycle
	TIM3->EGR |= 0x0001;
}

void TIM2_Encoder_Init(void){
  
	// TIM2 IO Init
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  // Enable GPIOA, TIM2 clock
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  
  GPIO_StructInit(&GPIO_InitStructure);
	
  // Set PA0, PA1 as encoder input
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  // Enable the TIM3 Update Interrupt
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);


	// Timer configuration in Encoder mode
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_ICInitTypeDef TIM_ICInitStructure;
	
  TIM_DeInit(TIM2);
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0;
  TIM_TimeBaseStructure.TIM_Period = 0xFFFF;  
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
  TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, 
                             TIM_ICPolarity_Rising, 
														 TIM_ICPolarity_Rising); 
  TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_ICFilter = 6;
  TIM_ICInit(TIM2, &TIM_ICInitStructure);
  
 // Clear all pending interrupts
  TIM_ClearFlag(TIM2, TIM_FLAG_Update);
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	TIM_SetCounter(TIM2, 0);
  TIM_Cmd(TIM2, ENABLE);
	
}

void TIM4_Encoder_Init(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_ICInitTypeDef TIM_ICInitStructure;
  
/* Encoder unit connected to TIM4, 4X mode */    
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  // Enable GPIOB, TIM4
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  
  GPIO_StructInit(&GPIO_InitStructure);
  // Set PB6, PB7 as encoder input
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /* Enable the TIM4 Update Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* Timer configuration in Encoder mode */
  TIM_DeInit(TIM4);
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0;
  TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
 
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, 
                             TIM_ICPolarity_Rising, 
														 TIM_ICPolarity_Rising); 
  TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_ICFilter = 6;
  TIM_ICInit(TIM4, &TIM_ICInitStructure);
  
 // Clear all pending interrupts
  TIM_ClearFlag(TIM4, TIM_FLAG_Update);
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

	TIM_SetCounter(TIM4, 0);
  TIM_Cmd(TIM4, ENABLE);
}

void TIM2_IRQHandler(void)
{ 		    		  			    
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);	    
}
void TIM4_IRQHandler(void)
{ 		    		  			    
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);	    
}
