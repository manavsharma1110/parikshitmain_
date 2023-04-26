#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"


int main()
{
	int i;
  GPIO_InitTypeDef GPIO_InitStruct;
	//Enabling the AHB1 peripheral clock for GPIOD
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;

	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	
	GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	while(1)
	{
	GPIO_SetBits(GPIOD, GPIO_Pin_12);
	}
	
	//Christmas lights
	/*while(1)
	{
	
		GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
		for(i=0;i<=1000000;i++){
		}
		GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
		for(i=0;i<=1000000;i++){
		}
		GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
		for(i=0;i<=1000000;i++){
		}
		GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
		for(i=0;i<=1000000;i++){
		}
	}*/
	
	//blinking LED light
	/*while(1)
	{
	
		GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
		for(i=0;i<=1000000;i++){
		}
	*/
	
	//two at a time
		/*while(1)
	{
	
		GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
		GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
		for(i=0;i<=1000000;i++){
		}
		GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
		GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
		for(i=0;i<=1000000;i++){
		}
	}*/
	}
