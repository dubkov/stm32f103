#include "stm32f10x_lib.h"

GPIO_InitTypeDef GPIO_InitStructure;
ErrorStatus HSEStartUpStatus;

void RCC_Configuration(void);
void NVIC_Configuration(void);
void Delay(vu32 nCount);

int main(void) 
{
	RCC_Configuration();										   /* Configure the system clocks */
	NVIC_Configuration();													/* NVIC Configuration */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);					/* Enable GPIOC clock */
															/* Configure PCA0 as Output push-pull */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	while (1)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_0);	Delay(0xBFFF0);
		GPIO_ResetBits(GPIOA, GPIO_Pin_0);	Delay(0xBFFF0);
		GPIO_SetBits(GPIOA, GPIO_Pin_1);	Delay(0xBFFF0);
		GPIO_ResetBits(GPIOA, GPIO_Pin_1);	Delay(0xBFFF0);
	}
}


void RCC_Configuration(void)
{
	RCC_DeInit();										   /* RCC system reset(for debug purpose) */
	RCC_HSEConfig(RCC_HSE_ON);														/* Enable HSE */
	HSEStartUpStatus = RCC_WaitForHSEStartUp();							/* Wait till HSE is ready */

	if(HSEStartUpStatus == SUCCESS)
	{
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);			/* Enable Prefetch Buffer */
		FLASH_SetLatency(FLASH_Latency_2);									/* Flash 2 wait state */
		RCC_HCLKConfig(RCC_SYSCLK_Div1); 										 /* HCLK = SYSCLK */
		RCC_PCLK2Config(RCC_HCLK_Div1); 										  /* PCLK2 = HCLK */
		RCC_PCLK1Config(RCC_HCLK_Div2);											/* PCLK1 = HCLK/2 */
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);		/* PLLCLK = 8MHz * 9 = 72 MHz */

		RCC_PLLCmd(ENABLE);															/* Enable PLL */
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);				/* Wait till PLL is ready */

		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);			 /* Select PLL as system clock source */
		while(RCC_GetSYSCLKSource() != 0x08);	  /* Wait till PLL is used as system clock source */
	}
}

void NVIC_Configuration(void)
{
	#ifdef  VECT_TAB_RAM  
	  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
	#else
	  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
	#endif
}

void Delay(vu32 nCount)
{
	for(nCount; nCount != 0; nCount--);
}
