#include "Clock.h"
#include "stm32f10x.h"

/**
* Function       RCC_INIT 
* @brief         Initialize sisclk, enable ports
* @param[in]     void
* @param[out]    void
* @retval        void
*/
void RCC_Init(void){

	uint32_t temp = 0x00;

	RCC->CR |= 0x01010081;			// Turn on PLL, HSE, HSI
	RCC->CFGR |= 0x00050002;		// 24MHz system clock

	RCC->APB2ENR |= 0x0000401D;	// Turn on USART1, GPIOC, GPIOB, GPIOA, AFIO
	RCC->APB1ENR |= 0x00020007;	// Turn on USART3, TIM4, TIM3, TIM2
	
	while (temp != 0x02000000)  // Wait for the PLL to stabilize
	{
		temp = RCC->CR & 0x02000000; // Check to see if the PLL lock bit is set
	}
 }

void SYSTICK_Init(void)
{
	// Turn the SYSTICK systerm OFF
	SysTick->CTRL = 0x00;
	// Clear the counter value
	SysTick->VAL = 0x00;
	// Choose a reload value (24-bit in HEX max)
	SysTick->LOAD = 2400000;		// 0.1s
	// Enable the system with interrupt a 24 MHZ clock
	SysTick->CTRL = 0x07;
}