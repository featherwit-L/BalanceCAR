#include "motor.h"

/*�����ʼ������*/
void Motor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//����ʱ��
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;//��ʼ��GPIO--PB12��PB13��PB14��PB15Ϊ�������
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_12 |GPIO_Pin_13 |GPIO_Pin_14 |GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);	
}

/*�޷�����*/
void Limit(int *motoA,int *motoB)
{
	if(*motoA>PWM_MAX)
		*motoA=PWM_MAX;
	if(*motoA<PWM_MIN)
		*motoA=PWM_MIN;
	
	if(*motoB>PWM_MAX)
		*motoB=PWM_MAX;
	if(*motoB<PWM_MIN)
		*motoB=PWM_MIN;
}

/*����ֵ����*/
int moto_abs(int p)
{
	int q;
	q=p>0?p:(-p);
	return q;
}

/*��ֵ���ص��������
��ڲ�����PID������ɺ������PWMֵ
*/
void Load(int moto1,int moto2)//moto1=-200����ת200������
{
	//1.�о������ţ���Ӧ����ת
	if(moto1>0)	
	{
		Ain1=1;
		Ain2=0;//��ת		
	}
	else 				
	{
		Ain1=0;
		Ain2=1;//��ת

	}
	TIM_SetCompare1(TIM1,moto_abs(moto1));
	
	if(moto2>0)	
	{
		Bin1=1;
		Bin2=0;
	}
	else 				
	{
		Bin1=0;
		Bin2=1;
	}
	TIM_SetCompare4(TIM1,moto_abs(moto2));
}

char PWM_Zero=0,stop=0;	
/*
�ж�С���Ƿ�����
*/
u8 Stop(float *Med_Jiaodu,float *Jiaodu)
{
	u8 temp;
	if((moto_abs(*Jiaodu-*Med_Jiaodu)>60)||Flag_Stop==1)
	{
		Load(PWM_Zero,PWM_Zero);
		stop=1;
		temp=1;
	}
	else
		temp=0;
	return temp;
}

int	Flag_Stop=0;		//���С���Ƿ�ֹͣ�ı�־λ

/**
�������ܣ����С���Ƿ�����
��ڲ�����int
����  ֵ��unsigned int
**/
int Pick_Up(float Acceleration,float Angle,int encoder_left,int encoder_right)
{ 		   
	static u16 flag,count0,count1,count2;
	if(flag==0)                    //��һ��
	{
		if(moto_abs(encoder_left)+moto_abs(encoder_right)<50)         //����1��С���ӽ���ֹ
			count0++;
        else 
			count0=0;		
        if(count0>10)				
		{
			flag=1;
			count0=0; 
		}
	 } 
	 if(flag==1)                             //����ڶ���
	 {
		if(++count1>200)       
		{
			count1=0;
			flag=0;   
		}
			//��ʱ���ٵȴ�2000ms,��ΪMPU6050ÿ10ms����һ���ж�
	    if(Acceleration>26000&&(Angle>(-20+Med_Angle))&&(Angle<(20+Med_Angle)))  
			//����2��С������0�ȸ���������
		    flag=2; 
	 } 
	 if(flag==2)    //������
	 {
		if(++count2>100)  
		{
			count2=0;
			flag=0;                                   //��ʱ���ٵȴ�1000ms
		}
	    if(moto_abs(encoder_left+encoder_right)>135)        //����3��С������̥��Ϊ�������ﵽ����ת��   
		{
			flag=0;                                                                                     
			return 1;           //��⵽С��������
		}
	 }
	return 0;
}

/**
�������ܣ����С���Ƿ񱻷���
��ڲ�����int
����  ֵ��unsigned int
**/
int Put_Down(float Angle,int encoder_left,int encoder_right)
{ 		   
	 static u16 flag,count;	 
	 if(Flag_Stop==0)     //��ֹ���      
		return 0;	                 
	 if(flag==0)                                               
	 {
		 //����1��С������0�ȸ�����
	     if(Angle>(-10+Med_Angle)&&Angle<(10+Med_Angle)&&encoder_left==0&&encoder_right==0)        
		    flag=1; 
	 } 
	 if(flag==1)                                               
	 {
		 if(++count>50)               //��ʱ���ٵȴ� 500ms
		 {
			count=0;
			flag=0;
		 }
	    if(encoder_left>3&&encoder_right>3&&encoder_left<60&&encoder_right<60)          
		//����2��С������̥��δ�ϵ��ʱ����Ϊת��  
		{
			flag=0;
			flag=0;
			return 1;                          //��⵽С��������
		}
	 }
	return 0;
}






