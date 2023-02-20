#include "stm32f10x.h"
#include "sys.h" 

float Pitch,Roll,Yaw;			//角度
short gyrox,gyroy,gyroz;		//陀螺仪--角速度
short aacx,aacy,aacz;			//加速度
int Encoder_Left,Encoder_Right;	//编码器数据（速度）

int  PWM_MAX=7200,PWM_MIN=-7200;	//PWM限幅变量
int MOTO1,MOTO2;				//电机装载变量

u8 status = 0;		 //用于判断接收/发送状态
u8 txbuf[]={0x09};	 //发送缓冲
u8 rxbuf[1];	 //接收缓冲

char 	string[20] = {0};//OLED显示缓存数组
extern	u16 overcount;
float	length;//HCSR04的距离值
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
		//length=Hcsr04GetLength();//获取距离
	} 	
}



