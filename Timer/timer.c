#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

void TIM4_IRQHandler()
{
   // GPIO_ResetBits(GPIOD, GPIO_Pin_12);//to eable the interrupt
	if(TIM_GetITStatus(TIM4,TIM_IT_Update) !=RESET)
	{
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
		GPIO_ToggleBits(GPIOD,GPIO_Pin_12);
	}
}
int main()
{
	
	int i;
	
	GPIO_InitTypeDef GPIO_InitStruct; // gpio structure
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;//timer structure
	NVIC_InitTypeDef NVIC_InitStructure; // Interrupt structure
	
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); //Enabling the AHB1 peripheral clock for GPIOD

	
	TIM_TimeBaseInitStruct.TIM_Period = 9999;
  TIM_TimeBaseInitStruct.TIM_Prescaler = 2799;
	
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;

	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;


  NVIC_Init(&NVIC_InitStructure);
	GPIO_Init(GPIOD, &GPIO_InitStruct);
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);
	
	GPIO_SetBits(GPIOD, GPIO_Pin_12);
	TIM_Cmd(TIM4, ENABLE);
	TIM_ITConfig(TIM4, TIM_IT_Update ,ENABLE);
	
	while(1)
	  {
		}
	}
