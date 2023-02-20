/*
用TIM3定时器驱动HCSR04
*/
#include "HCSR04.h"
#include "delay.h"

u16 overcount=0;//溢出计数

//超声波初始化
void HCSR04_Init(void)
{
		GPIO_InitTypeDef 			GPIO_InitStruct;
		TIM_TimeBaseInitTypeDef 	TIM_TimeBaseInitStruct;
		//时钟使能
		RCC_APB2PeriphClockCmd(HCSR04,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
		//trig脚-推挽输出
		GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
		GPIO_InitStruct.GPIO_Pin=HCSR04_TRIG;
		GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
		GPIO_Init(HCSR04_PORT,&GPIO_InitStruct);
		GPIO_ResetBits(HCSR04_PORT,HCSR04_TRIG);
	
		//echo脚-浮空输入
		GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;
		GPIO_InitStruct.GPIO_Pin=HCSR04_ECHO;
		GPIO_Init(HCSR04_PORT,&GPIO_InitStruct);
		GPIO_ResetBits(HCSR04_PORT,HCSR04_ECHO);
	
		//定时器TIM3初始化
		TIM_DeInit(TIM3);
		TIM_TimeBaseInitStruct.TIM_Period=1000-1;
		TIM_TimeBaseInitStruct.TIM_Prescaler=72-1;
		TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
		TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
		//定时器中断配置_更新中断
		TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
		//中断配置
		NVIC_Config();


		//关闭定时器使能
		TIM_Cmd(TIM3,DISABLE);
}

//打开定时器
void OpenTimeForHc(void)
{
		//计数器清空
		TIM_SetCounter(TIM3,0);
		overcount=0;
		TIM_Cmd(TIM3,ENABLE);
}

//关闭定时器
void CloseTimeForHc(void)
{
		TIM_Cmd(TIM3,DISABLE);
}

//获取定时器时间
u32 GetEchoTimer(void)
{
		u32 t=0;
		t=overcount*1000;
		t+=TIM_GetCounter(TIM3);
		//计数器清0
		TIM3->CNT=0;
		delay_ms(50);
	
		return t;
}

//超声波测距
float Hcsr04GetLength(void)
{
		u32 t=0;
		int i=0;
		float lengthTemp=0;
		float sum=0;
		while(i!=5)
		{
			HC_TRIG=1;
			delay_ms(20);
			HC_TRIG=0;
			while(HC_ECHO==0);
			OpenTimeForHc();
			i=i+1;
			while(HC_ECHO==1);
			CloseTimeForHc();
			t=GetEchoTimer();
			lengthTemp=((float)t/58.3);
			sum+=lengthTemp;
		}
		lengthTemp=sum/5;
		return lengthTemp;
}

//定时器3中断
void TIM3_IRQHandler(void)
{
		if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)
		{
				//清除中断标志位
				TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
				overcount++;
		}
}
