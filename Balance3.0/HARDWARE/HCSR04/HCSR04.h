#ifndef 	UltrasonicWave_H
#define 	UltrasonicWave_H
#include 	"sys.h"
#include 	"stm32f10x.h"

#define 	HCSR04_PORT 	GPIOB  
#define 	HCSR04 			RCC_APB2Periph_GPIOB
#define 	HCSR04_TRIG 	GPIO_Pin_5
#define 	HCSR04_ECHO 	GPIO_Pin_6

#define 	HC_TRIG 		PBout(5)
#define 	HC_ECHO			PBin(6)



void HCSR04_Init(void);
float Hcsr04GetLength(void);
	

#endif
