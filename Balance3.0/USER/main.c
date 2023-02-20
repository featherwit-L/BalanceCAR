#include "stm32f10x.h"
#include "sys.h" 

float Pitch,Roll,Yaw;			//�Ƕ�
short gyrox,gyroy,gyroz;		//������--���ٶ�
short aacx,aacy,aacz;			//���ٶ�
int Encoder_Left,Encoder_Right;	//���������ݣ��ٶȣ�

int  PWM_MAX=7200,PWM_MIN=-7200;	//PWM�޷�����
int MOTO1,MOTO2;				//���װ�ر���

u8 status = 0;		 //�����жϽ���/����״̬
u8 txbuf[]={0x09};	 //���ͻ���
u8 rxbuf[1];	 //���ջ���

char 	string[20] = {0};//OLED��ʾ��������
extern	u16 overcount;
float	length;//HCSR04�ľ���ֵ
u8 i;


int main(void)	
{
	delay_init();
	uart_init(115200);	
	uart3_init(115200);
	
	OLED_Init();
	OLED_Clear();
	//LED_Init();
	//NRF_Init();
	//HCSR04_Init();
	
	MPU_Init();
	mpu_dmp_init();
	MPU6050_EXTI_Init();
	
	Encoder_TIM2_Init();
	Encoder_TIM4_Init();
	
	NVIC_Config();	
	Motor_Init();
	PWM_Init_TIM1(0,7199);
	
	//OLED_ShowString(0,0,"Pitch:",12);
  while(1)	
	{
		//delay_ms(999);
		sprintf((char *)string,"Pitch:%.2f",Pitch);
		OLED_ShowString(12,0,string,12);
		
//		OLED_ShowFNum(50,0,Pitch,4,12);
		//printf("%f\n",Pitch);
		//length=Hcsr04GetLength();//��ȡ����
	} 	
}



