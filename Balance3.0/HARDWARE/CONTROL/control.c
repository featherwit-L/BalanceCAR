#include "control.h"


/*
0x00：刹车
0x01：前进
0x05：后退
0x03：左转
0x07：右转
*/
float Med_Angle=-2;//机械中值		-6
float Target_Speed=0;	//期望速度（俯仰）。---二次开发接口，用于控制小车前进后退及其速度。
float Turn_Speed=0;		//期望速度（偏航）

float
	Vertical_Kp=-210,	//直立环KP、KD    
	Vertical_Kd=-0.9;

//	Vertical_Kp=0,	//直立环KP、KD
//	Vertical_Kd=0;
float
	Velocity_Kp=-0.35,	//速度环KP、KI
	Velocity_Ki=-0.00275;

//		Velocity_Kp=0,	
//	Velocity_Ki=0;
float 
	Turn_Kp=1,
	Turn_Kd=-20;//1

//	Turn_Kp=0,
//	Turn_Kd=0;

#define SPEED_Y 40 //俯仰(前后)最大设定速度
#define SPEED_Z 30//偏航(左右)最大设定速度 

int Vertical_out=0,Velocity_out=0,Turn_out=0;//直立环&速度环&转向环的输出变量

int Vertical(float Med,float Angle,float gyro_Y);
int Velocity(int Target,int encoder_left,int encoder_right);
int Turn(int gyro_Z,int RC);

void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line5)!=0)//初次判定
	{
		int PWM_out;
		if(PBin(5)==0)//再次判定
		{
			EXTI_ClearITPendingBit(EXTI_Line5);//清除中断标志位
			
			//采集编码器数据和MPU6050角度信息。
			Encoder_Left=-Read_Speed(2);
			//电机是相对安装，刚好相差180度，为了编码器输出极性一致，就需要对其中一个取反。
			Encoder_Right=Read_Speed(4);
			
			mpu_dmp_get_data(&Pitch,&Roll,&Yaw);		//角度
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//陀螺仪
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//加速度
			
			Pitch=Pitch;
			
			/*********************************************************************************************/
			/*前后*/
			if((Fore==0)&&(Back==0))
			{
				if(Target_Speed>0)
					Target_Speed--;
				if(Target_Speed<0)
					Target_Speed++;
				//Target_Speed=0;//未接受到前进后退指令-->速度逐步清零，防止骤停摔倒
			}
			if(Fore==1)
			{
				Target_Speed++;//前进1标志位拉高-->需要前进
				stop=1;
			}
			if(Back==1)
			{
				Target_Speed--;
				stop=1;
			}
			Target_Speed=Target_Speed>SPEED_Y?SPEED_Y:(Target_Speed<-SPEED_Y?(-SPEED_Y):Target_Speed);//限幅
			
			/*左右*/
			if((Left==0)&&(Right==0))
			{
				if(Turn_Speed>0)
					Turn_Speed--;
				if(Turn_Speed<0)
					Turn_Speed++;
				Turn_Kp=1;//若无左右转向指令，则开启转向约束
			}
			if(Left==1)
			{
				Turn_Speed+=20;	//左转
				stop=1;
				Turn_Kp=0;//若左右转向指令接收到，则去掉转向约束
			}
			if(Right==1)
			{
				Turn_Speed-=20;	//右转
				stop=1;
				Turn_Kp=0;//若左右转向指令接收到，则去掉转向约束
			}
			Turn_Speed=Turn_Speed>SPEED_Z?SPEED_Z:(Turn_Speed<-SPEED_Z?(-SPEED_Z):Turn_Speed);
			//限幅( (20*100) * 100   )
			
//			/*转向约束*/
//			if((Left==0)&&(Right==0))
//				Turn_Kp=1.4;//若无左右转向指令，则开启转向约束
//			else if((Left==1)||(Right==1))
//				Turn_Kp=0;//若左右转向指令接收到，则去掉转向约束
			/*********************************************************************************************/
			
			//printf("\n\r ax=%d      ay=%d      az=%d \n\r",aacx,aacy,aacz); 
			//printf("\n\r 俯仰角=%.2f      横滚角=%.2f      偏航角=%.2f \n\r",Pitch,Roll,Yaw); 
			
			//将数据压入闭环控制中，计算出控制输出量。
			Vertical_out=Vertical(Med_Angle,Pitch,gyroy);		//直立环
			Velocity_out=Velocity(Target_Speed,Encoder_Left,Encoder_Right);	//速度环
			
			//Vertical_out=Vertical(Velocity_out+Med_Angle,Pitch,gyroy);		//直立环
			
			Turn_out=Turn(gyroz,Turn_Speed);								//转向环
			
			PWM_out=Vertical_out-Vertical_Kp*Velocity_out;//最终输出
			
			//PWM_out=Vertical_out;//最终输出			
			//PWM_out=Velocity_out;
			
			//把控制输出量加载到电机上，完成最终的的控制。
			MOTO1=PWM_out-Turn_out;//左电机
			MOTO2=PWM_out+Turn_out;//右电机

			Limit(&MOTO1,&MOTO2);//PWM限幅			
			//Load(MOTO1,MOTO2);//加载到电机上。
			
			if(Pick_Up(aacz,Pitch,Encoder_Left,Encoder_Right))//===检查是否小车被那起
				Flag_Stop=1;	                                                      //===如果被拿起就关闭电机
			if(Put_Down(Pitch,Encoder_Left,Encoder_Right))              //===检查是否小车被放下
				Flag_Stop=0;
			
			if(Stop(&Med_Angle,&Pitch)==0)
				Load(MOTO1,MOTO2);//加载到电机上 ;//安全检测
		}
		else 
			EXTI_ClearITPendingBit(EXTI_Line5);//清除中断标志位
	}
}




/*
直立环PD控制器：Kp*Ek+Kd*Ek_D

入口：期望角度、真实角度、真实角速度
出口：直立环输出
*/
int Vertical(float Med,float Angle,float gyro_Y)
{
	int PWM_out;
	
	PWM_out=Vertical_Kp*(Angle-Med)+Vertical_Kd*(gyro_Y-0);
	return PWM_out;
}



/*
速度环PI：Kp*Ek+Ki*Ek_S
*/
int Velocity(int Target,int encoder_left,int encoder_right)
{
	static int PWM_out,Encoder_Err,Encoder_S,EnC_Err_Lowout,EnC_Err_Lowout_last=0;
	float a=0.7;
	
	//计算速度偏差
	Encoder_Err=((encoder_left+encoder_right)-Target);//舍去误差
	//对速度偏差进行低通滤波
	//low_out=(1-a)*Ek+a*low_out_last;
	EnC_Err_Lowout=(1-a)*Encoder_Err+a*EnC_Err_Lowout_last;//使得波形更加平滑，滤除高频干扰，防止速度突变。
	EnC_Err_Lowout_last=EnC_Err_Lowout;//防止速度过大的影响直立环的正常工作。
	//对速度偏差积分，积分出位移
	Encoder_S+=EnC_Err_Lowout;
	//积分限幅
	Encoder_S=Encoder_S>8000?8000:(Encoder_S<(-8000)?(-8000):Encoder_S);
	
	if(stop==1) 
	{
		Encoder_S=0;
		stop=0;//清零积分量
	}
	
	//速度环控制输出计算
	PWM_out=Velocity_Kp*EnC_Err_Lowout+Velocity_Ki*Encoder_S;
	return PWM_out;
}



/*
转向环：系数*Z轴角速度
*/
int Turn(int gyro_Z,int RC)
{
	int PWM_out;
	
	PWM_out=Turn_Kp*gyro_Z-Turn_Kd*RC;
	return PWM_out;
}


