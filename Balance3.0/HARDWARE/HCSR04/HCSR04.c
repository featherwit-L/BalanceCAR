/*
��TIM3��ʱ������HCSR04
*/
#include "HCSR04.h"
#include "delay.h"

u16 overcount=0;//�������

//��������ʼ��
void HCSR04_Init(void)
{
		GPIO_InitTypeDef 			GPIO_InitStruct;
		TIM_TimeBaseInitTypeDef 	TIM_TimeBaseInitStruct;
		//ʱ��ʹ��
		RCC_APB2PeriphClockCmd(HCSR04,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
		//trig��-�������
		GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
		GPIO_InitStruct.GPIO_Pin=HCSR04_TRIG;
		GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
		GPIO_Init(HCSR04_PORT,&GPIO_InitStruct);
		GPIO_ResetBits(HCSR04_PORT,HCSR04_TRIG);
	
		//echo��-��������
		GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;
		GPIO_InitStruct.GPIO_Pin=HCSR04_ECHO;
		GPIO_Init(HCSR04_PORT,&GPIO_InitStruct);
		GPIO_ResetBits(HCSR04_PORT,HCSR04_ECHO);
	
		//��ʱ��TIM3��ʼ��
		TIM_DeInit(TIM3);
		TIM_TimeBaseInitStruct.TIM_Period=1000-1;
		TIM_TimeBaseInitStruct.TIM_Prescaler=72-1;
		TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
		TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
		//��ʱ���ж�����_�����ж�
		TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
		//�ж�����
		NVIC_Config();


		//�رն�ʱ��ʹ��
		TIM_Cmd(TIM3,DISABLE);
}

//�򿪶�ʱ��
void OpenTimeForHc(void)
{
		//���������
		TIM_SetCounter(TIM3,0);
		overcount=0;
		TIM_Cmd(TIM3,ENABLE);
}

//�رն�ʱ��
void CloseTimeForHc(void)
{
		TIM_Cmd(TIM3,DISABLE);
}

//��ȡ��ʱ��ʱ��
u32 GetEchoTimer(void)
{
		u32 t=0;
		t=overcount*1000;
		t+=TIM_GetCounter(TIM3);
		//��������0
		TIM3->CNT=0;
		delay_ms(50);
	
		return t;
}

//���������
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

//��ʱ��3�ж�
void TIM3_IRQHandler(void)
{
		if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)
		{
				//����жϱ�־λ
				TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
				overcount++;
		}
}
