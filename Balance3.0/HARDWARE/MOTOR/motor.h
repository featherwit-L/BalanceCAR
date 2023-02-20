#ifndef  _MOTOR_H
#define  _MOTOR_H

#include "sys.h" 

#define Ain1  PBout(14)
#define Ain2  PBout(15)

#define Bin1  PBout(13)
#define Bin2  PBout(12)

void 	Motor_Init(void);
void 	Limit(int *motoA,int *motoB);
int 	moto_abs(int p);
void 	Load(int moto1,int moto2);
u8	 	Stop(float *Med_Jiaodu,float *Jiaodu);
int 	Pick_Up(float Acceleration,float Angle,int encoder_left,int encoder_right);
int 	Put_Down(float Angle,int encoder_left,int encoder_right);

#endif

