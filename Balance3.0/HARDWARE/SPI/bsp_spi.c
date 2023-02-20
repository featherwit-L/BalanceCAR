#include	"bsp_spi.h"

void	SPI1_Init(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	SPI_InitTypeDef		SPI_InitStructure;
	
	SPI_APBxClock_FUN(SPI_CLK|RCC_APB2Periph_GPIOA ,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=SPI_SCK_PIN | SPI_MISO_PIN |SPI_MOSI_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
		
	SPI_StructInit(&SPI_InitStructure);                                       //恢复默认值
	SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;//SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;		            //设置为主SPI
	SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;		            //SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL              = SPI_CPOL_Low;		              //串行同步时钟的空闲状态为低电平
	SPI_InitStructure.SPI_CPHA              = SPI_CPHA_1Edge;	              //第1个边沿有效，上升沿为采样时刻数据被采样
	SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;		                //软件模式
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;      //波特率预分频值为4   APB2/x 
	SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;	              //=数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial     = 7;	                            //CRC值计算的多项式
	SPI_Init(SPIx, &SPI_InitStructure);                                       //初始化外设SPIx寄存器
	
	SPI_Cmd(SPIx, ENABLE);   
	
	//SPI1_ReadWriteByte(0xff);//启动传输	
	//SPI1_SetSpeed(SPI_BaudRatePrescaler_4);                           //定义波特率预分频
}	

#if	0
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI1->CR1&=0XFFC7;
	SPI1->CR1|=SPI_BaudRatePrescaler;	//设置SPI1速度 
	SPI_Cmd(SPIx,ENABLE); 
} 

#endif


