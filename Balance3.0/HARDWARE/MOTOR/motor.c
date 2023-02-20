#include "motor.h"

/*电机初始化函数*/
void Motor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//开启时钟
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;//初始化GPIO--PB12、PB13、PB14、PB15为推挽输出
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_12 |GPIO_Pin_13 |GPIO_Pin_14 |GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);	
}

/*限幅函数*/
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

/*绝对值函数*/
int moto_abs(int p)
{
	int q;
	q=p>0?p:(-p);
	return q;
}

/*赋值加载到电机函数
入口参数：PID运算完成后的最终PWM值
*/
void Load(int moto1,int moto2)//moto1=-200：反转200个脉冲
{
	//1.研究正负号，对应正反转
	if(moto1>0)	
	{
		Ain1=1;
		Ain2=0;//正转		
	}
	else 				
	{
		Ain1=0;
		Ain2=1;//反转

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
判断小车是否运行
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

int	Flag_Stop=0;		//检测小车是否停止的标志位

/**
函数功能：检测小车是否被拿起
入口参数：int
返回  值：unsigned int
**/
int Pick_Up(float Acceleration,float Angle,int encoder_left,int encoder_right)
{ 		   
	static u16 flag,count0,count1,count2;
	if(flag==0)                    //第一步
	{
		if(moto_abs(encoder_left)+moto_abs(encoder_right)<50)         //条件1，小车接近静止
			count0++;
        else 
			count0=0;		
        if(count0>10)				
		{
			flag=1;
			count0=0; 
		}
	 } 
	 if(flag==1)                             //进入第二步
	 {
		if(++count1>200)       
		{
			count1=0;
			flag=0;   
		}
			//超时不再等待2000ms,因为MPU6050每10ms进入一次中断
	    if(Acceleration>26000&&(Angle>(-20+Med_Angle))&&(Angle<(20+Med_Angle)))  
			//条件2，小车是在0度附近被拿起
		    flag=2; 
	 } 
	 if(flag==2)    //第三步
	 {
		if(++count2>100)  
		{
			count2=0;
			flag=0;                                   //超时不再等待1000ms
		}
	    if(moto_abs(encoder_left+encoder_right)>135)        //条件3，小车的轮胎因为正反馈达到最大的转速   
		{
			flag=0;                                                                                     
			return 1;           //检测到小车被拿起
		}
	 }
	return 0;
}

/**
函数功能：检测小车是否被放下
入口参数：int
返回  值：unsigned int
**/
int Put_Down(float Angle,int encoder_left,int encoder_right)
{ 		   
	 static u16 flag,count;	 
	 if(Flag_Stop==0)     //防止误检      
		return 0;	                 
	 if(flag==0)                                               
	 {
		 //条件1，小车是在0度附近的
	     if(Angle>(-10+Med_Angle)&&Angle<(10+Med_Angle)&&encoder_left==0&&encoder_right==0)        
		    flag=1; 
	 } 
	 if(flag==1)                                               
	 {
		 if(++count>50)               //超时不再等待 500ms
		 {
			count=0;
			flag=0;
		 }
	    if(encoder_left>3&&encoder_right>3&&encoder_left<60&&encoder_right<60)          
		//条件2，小车的轮胎在未上电的时候被人为转动  
		{
			flag=0;
			flag=0;
			return 1;                          //检测到小车被放下
		}
	 }
	return 0;
}






