#ifndef __SPI_H
#define __SPI_H
#include "stm32f10x.h"
#include "sys.h" 


#define		SPIx						SPI1
#define		SPI_APBxClock_FUN			RCC_APB2PeriphClockCmd	
#define		SPI_CLK						RCC_APB2Periph_SPI1

//SCK����
#define     SPI_SCK_APBxClock_FUN   	RCC_APB2PeriphClockCmd
#define     SPI_SCK_CLK             	RCC_APB2Periph_GPIOA   
#define     SPI_SCK_PORT            	GPIOA   
#define     SPI_SCK_PIN             	GPIO_Pin_5

//MISO����
#define     SPI_MISO_APBxClock_FUN      RCC_APB2PeriphClockCmd
#define     SPI_MISO_CLK                RCC_APB2Periph_GPIOA    
#define     SPI_MISO_PORT               GPIOA 
#define     SPI_MISO_PIN                GPIO_Pin_6

//MOSI����
#define     SPI_MOSI_APBxClock_FUN    	R CC_APB2PeriphClockCmd
#define     SPI_MOSI_CLK                RCC_APB2Periph_GPIOA    
#define     SPI_MOSI_PORT               GPIOA 
#define     SPI_MOSI_PIN                GPIO_Pin_7

void SPI1_Init(void);              //��ʼ��SPI��
void SPI1_SetSpeed(u8 SpeedSet);   //����SPI�ٶ�   
u8   SPI1_ReadWriteByte(u8 TxData);//SPI���߶�дһ���ֽ�


#endif
