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
		
	SPI_StructInit(&SPI_InitStructure);                                       //�ָ�Ĭ��ֵ
	SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;//SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;		            //����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;		            //SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL              = SPI_CPOL_Low;		              //����ͬ��ʱ�ӵĿ���״̬Ϊ�͵�ƽ
	SPI_InitStructure.SPI_CPHA              = SPI_CPHA_1Edge;	              //��1��������Ч��������Ϊ����ʱ�����ݱ�����
	SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;		                //���ģʽ
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;      //������Ԥ��ƵֵΪ4   APB2/x 
	SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;	              //=���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial     = 7;	                            //CRCֵ����Ķ���ʽ
	SPI_Init(SPIx, &SPI_InitStructure);                                       //��ʼ������SPIx�Ĵ���
	
	SPI_Cmd(SPIx, ENABLE);   
	
	//SPI1_ReadWriteByte(0xff);//��������	
	//SPI1_SetSpeed(SPI_BaudRatePrescaler_4);                           //���岨����Ԥ��Ƶ
}	

#if	0
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI1->CR1&=0XFFC7;
	SPI1->CR1|=SPI_BaudRatePrescaler;	//����SPI1�ٶ� 
	SPI_Cmd(SPIx,ENABLE); 
} 

#endif


