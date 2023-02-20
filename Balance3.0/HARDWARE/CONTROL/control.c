#include "control.h"


/*
0x00��ɲ��
0x01��ǰ��
0x05������
0x03����ת
0x07����ת
*/
float Med_Angle=-2;//��е��ֵ		-6
float Target_Speed=0;	//�����ٶȣ���������---���ο����ӿڣ����ڿ���С��ǰ�����˼����ٶȡ�
float Turn_Speed=0;		//�����ٶȣ�ƫ����

float
	Vertical_Kp=-210,	//ֱ����KP��KD    
	Vertical_Kd=-0.9;

//	Vertical_Kp=0,	//ֱ����KP��KD
//	Vertical_Kd=0;
float
	Velocity_Kp=-0.35,	//�ٶȻ�KP��KI
	Velocity_Ki=-0.00275;

//		Velocity_Kp=0,	
//	Velocity_Ki=0;
float 
	Turn_Kp=1,
	Turn_Kd=-20;//1

//	Turn_Kp=0,
//	Turn_Kd=0;

#define SPEED_Y 40 //����(ǰ��)����趨�ٶ�
#define SPEED_Z 30//ƫ��(����)����趨�ٶ� 

int Vertical_out=0,Velocity_out=0,Turn_out=0;//ֱ����&�ٶȻ�&ת�򻷵��������

int Vertical(float Med,float Angle,float gyro_Y);
int Velocity(int Target,int encoder_left,int encoder_right);
int Turn(int gyro_Z,int RC);

void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line5)!=0)//�����ж�
	{
		int PWM_out;
		if(PBin(5)==0)//�ٴ��ж�
		{
			EXTI_ClearITPendingBit(EXTI_Line5);//����жϱ�־λ
			
			//�ɼ����������ݺ�MPU6050�Ƕ���Ϣ��
			Encoder_Left=-Read_Speed(2);
			//�������԰�װ���պ����180�ȣ�Ϊ�˱������������һ�£�����Ҫ������һ��ȡ����
			Encoder_Right=Read_Speed(4);
			
			mpu_dmp_get_data(&Pitch,&Roll,&Yaw);		//�Ƕ�
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//������
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//���ٶ�
			
			Pitch=Pitch;
			
			/*********************************************************************************************/
			/*ǰ��*/
			if((Fore==0)&&(Back==0))
			{
				if(Target_Speed>0)
					Target_Speed--;
				if(Target_Speed<0)
					Target_Speed++;
				//Target_Speed=0;//δ���ܵ�ǰ������ָ��-->�ٶ������㣬��ֹ��ͣˤ��
			}
			if(Fore==1)
			{
				Target_Speed++;//ǰ��1��־λ����-->��Ҫǰ��
				stop=1;
			}
			if(Back==1)
			{
				Target_Speed--;
				stop=1;
			}
			Target_Speed=Target_Speed>SPEED_Y?SPEED_Y:(Target_Speed<-SPEED_Y?(-SPEED_Y):Target_Speed);//�޷�
			
			/*����*/
			if((Left==0)&&(Right==0))
			{
				if(Turn_Speed>0)
					Turn_Speed--;
				if(Turn_Speed<0)
					Turn_Speed++;
				Turn_Kp=1;//��������ת��ָ�����ת��Լ��
			}
			if(Left==1)
			{
				Turn_Speed+=20;	//��ת
				stop=1;
				Turn_Kp=0;//������ת��ָ����յ�����ȥ��ת��Լ��
			}
			if(Right==1)
			{
				Turn_Speed-=20;	//��ת
				stop=1;
				Turn_Kp=0;//������ת��ָ����յ�����ȥ��ת��Լ��
			}
			Turn_Speed=Turn_Speed>SPEED_Z?SPEED_Z:(Turn_Speed<-SPEED_Z?(-SPEED_Z):Turn_Speed);
			//�޷�( (20*100) * 100   )
			
//			/*ת��Լ��*/
//			if((Left==0)&&(Right==0))
//				Turn_Kp=1.4;//��������ת��ָ�����ת��Լ��
//			else if((Left==1)||(Right==1))
//				Turn_Kp=0;//������ת��ָ����յ�����ȥ��ת��Լ��
			/*********************************************************************************************/
			
			//printf("\n\r ax=%d      ay=%d      az=%d \n\r",aacx,aacy,aacz); 
			//printf("\n\r ������=%.2f      �����=%.2f      ƫ����=%.2f \n\r",Pitch,Roll,Yaw); 
			
			//������ѹ��ջ������У�����������������
			Vertical_out=Vertical(Med_Angle,Pitch,gyroy);		//ֱ����
			Velocity_out=Velocity(Target_Speed,Encoder_Left,Encoder_Right);	//�ٶȻ�
			
			//Vertical_out=Vertical(Velocity_out+Med_Angle,Pitch,gyroy);		//ֱ����
			
			Turn_out=Turn(gyroz,Turn_Speed);								//ת��
			
			PWM_out=Vertical_out-Vertical_Kp*Velocity_out;//�������
			
			//PWM_out=Vertical_out;//�������			
			//PWM_out=Velocity_out;
			
			//�ѿ�����������ص�����ϣ�������յĵĿ��ơ�
			MOTO1=PWM_out-Turn_out;//����
			MOTO2=PWM_out+Turn_out;//�ҵ��

			Limit(&MOTO1,&MOTO2);//PWM�޷�			
			//Load(MOTO1,MOTO2);//���ص�����ϡ�
			
			if(Pick_Up(aacz,Pitch,Encoder_Left,Encoder_Right))//===����Ƿ�С��������
				Flag_Stop=1;	                                                      //===���������͹رյ��
			if(Put_Down(Pitch,Encoder_Left,Encoder_Right))              //===����Ƿ�С��������
				Flag_Stop=0;
			
			if(Stop(&Med_Angle,&Pitch)==0)
				Load(MOTO1,MOTO2);//���ص������ ;//��ȫ���
		}
		else 
			EXTI_ClearITPendingBit(EXTI_Line5);//����жϱ�־λ
	}
}




/*
ֱ����PD��������Kp*Ek+Kd*Ek_D

��ڣ������Ƕȡ���ʵ�Ƕȡ���ʵ���ٶ�
���ڣ�ֱ�������
*/
int Vertical(float Med,float Angle,float gyro_Y)
{
	int PWM_out;
	
	PWM_out=Vertical_Kp*(Angle-Med)+Vertical_Kd*(gyro_Y-0);
	return PWM_out;
}



/*
�ٶȻ�PI��Kp*Ek+Ki*Ek_S
*/
int Velocity(int Target,int encoder_left,int encoder_right)
{
	static int PWM_out,Encoder_Err,Encoder_S,EnC_Err_Lowout,EnC_Err_Lowout_last=0;
	float a=0.7;
	
	//�����ٶ�ƫ��
	Encoder_Err=((encoder_left+encoder_right)-Target);//��ȥ���
	//���ٶ�ƫ����е�ͨ�˲�
	//low_out=(1-a)*Ek+a*low_out_last;
	EnC_Err_Lowout=(1-a)*Encoder_Err+a*EnC_Err_Lowout_last;//ʹ�ò��θ���ƽ�����˳���Ƶ���ţ���ֹ�ٶ�ͻ�䡣
	EnC_Err_Lowout_last=EnC_Err_Lowout;//��ֹ�ٶȹ����Ӱ��ֱ����������������
	//���ٶ�ƫ����֣����ֳ�λ��
	Encoder_S+=EnC_Err_Lowout;
	//�����޷�
	Encoder_S=Encoder_S>8000?8000:(Encoder_S<(-8000)?(-8000):Encoder_S);
	
	if(stop==1) 
	{
		Encoder_S=0;
		stop=0;//���������
	}
	
	//�ٶȻ������������
	PWM_out=Velocity_Kp*EnC_Err_Lowout+Velocity_Ki*Encoder_S;
	return PWM_out;
}



/*
ת�򻷣�ϵ��*Z����ٶ�
*/
int Turn(int gyro_Z,int RC)
{
	int PWM_out;
	
	PWM_out=Turn_Kp*gyro_Z-Turn_Kd*RC;
	return PWM_out;
}


