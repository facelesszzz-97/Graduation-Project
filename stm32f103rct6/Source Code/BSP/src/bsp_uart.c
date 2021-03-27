#include <stdio.h>
#include <string.h>
#include <stdarg.h>	
#include "bsp_uart.h"
#include "bsp_gpio.h"

static Uart_Data_t gUartx_Data[3];
static Uart_Data_t *pUart1_Data = &gUartx_Data[0];
static Uart_Data_t *pUart2_Data = &gUartx_Data[1];
static Uart_Data_t *pUart3_Data = &gUartx_Data[2];

uint16_t USART_RX_STA;         		//����״̬���

Uart_Data_t *GetUart_Data_t(uint8_t Uartx)
{
	switch(Uartx)
	{
		case Uart1_NUM:
		#if Uart1_EN
			return pUart1_Data;
		#endif
		case Uart2_NUM:
		#if Uart2_EN
			return pUart2_Data;
		#endif
		case Uart3_NUM:
		#if Uart3_EN
			return pUart3_Data;
		#endif
		default:
			break;
	}
	return NULL;
}

static void Uart1_Init(uint32_t bound)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);						
 	GPIO_PinInit(GPIO_PA9, GPIO_Mode_AF_PP);
	GPIO_PinInit(GPIO_PA10, GPIO_Mode_IN_FLOATING);
	
	//Usart1 NVIC ����
  	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Uart1_PreemptionPriority;	//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = Uart1_SubPriority;					//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;										//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);														//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;										//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;							//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;								//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;									//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;						//�շ�ģʽ

  	USART_Init(USART1, &USART_InitStructure); 											//��ʼ������1
  	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);										//�������ڽ����ж�
  	USART_Cmd(USART1, ENABLE); 															//ʹ�ܴ���1 
}

static void Uart2_Init(uint32_t bound)
{  
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE); 
	GPIO_PinInit(GPIO_PA2, GPIO_Mode_AF_PP);
	GPIO_PinInit(GPIO_PA3, GPIO_Mode_IN_FLOATING);

	//�����ж����ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Uart2_PreemptionPriority;	//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = Uart2_SubPriority;					//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;										//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);														//����ָ���Ĳ�����ʼ��VIC�Ĵ���

	USART_InitStructure.USART_BaudRate = bound;											//������һ������Ϊ115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;							//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;								//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;									//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;						//�շ�ģʽ
  
	USART_Init(USART2, &USART_InitStructure); 											//��ʼ������2
	//ʹ�ܽ����ж�
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);										//�����ж�  	
	USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ���
}

static void Uart3_Init(uint32_t bound)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); 							
	GPIO_PinInit(GPIO_PB10, GPIO_Mode_AF_PP);
   	GPIO_PinInit(GPIO_PB11, GPIO_Mode_IN_FLOATING);

	//�����ж����ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Uart3_PreemptionPriority;	//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = Uart3_SubPriority;					//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;										//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);														//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	USART_InitStructure.USART_BaudRate = bound;											//������һ������Ϊ115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;							//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;								//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;									//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;						//�շ�ģʽ
  
	USART_Init(USART3, &USART_InitStructure); 											//��ʼ������3
	//ʹ�ܽ����ж�
  	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);										//�����ж�  	
	USART_Cmd(USART3, ENABLE);                    										//ʹ�ܴ���
}

void BSP_Uart_Init(void)
{
#if	Uart1_EN
	Uart1_Init(9600);
#endif

#if	Uart2_EN
	Uart2_Init(9600);
#endif

#if	Uart2_EN
	Uart3_Init(9600);
#endif
}

//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

void USART1_IRQHandler(void)
{   
	uint8_t Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART1);	//��ȡ���յ�������
		if((USART_RX_STA&0x8000)==0)//����δ���
		{
			if(USART_RX_STA&0x4000)//���յ���0x0d
			{
				if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else USART_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					pUart1_Data->RxBuf[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USARTx_RX_LENGTH-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}   		 
    } 	 
}   

//void USART1_IRQHandler(void)
//{      	
//	u8 res;	      
//	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//���յ�����
//	{	 
//		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
//		res = USART_ReceiveData(USART2);		
//		if((USART2_RX_STA&(1<<15))==0)//�������һ������,��û�б�����,���ٽ�����������
//		{ 
//			if(USART2_RX_STA<USART2_MAX_RECV_LEN)	//�����Խ�������
//			{
//				TIM_SetCounter(TIM7,0);//���������          				//���������
//				if(USART2_RX_STA==0) 				//ʹ�ܶ�ʱ��7���ж� 
//				{
//					TIM_Cmd(TIM7,ENABLE);//ʹ�ܶ�ʱ��7
//				}
//				USART2_RX_BUF[USART2_RX_STA++]=res;	//��¼���յ���ֵ
//			}else 
//			{
//				USART2_RX_STA|=1<<15;				//ǿ�Ʊ�ǽ������
//			} 
//		}
//	}  		 
//}   

//void USART3_IRQHandler(void)
//{      	
//	u8 res;	      
//	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//���յ�����
//	{	 
//		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
//		res = USART_ReceiveData(USART2);		
//		if((USART2_RX_STA&(1<<15))==0)//�������һ������,��û�б�����,���ٽ�����������
//		{ 
//			if(USART2_RX_STA<USART2_MAX_RECV_LEN)	//�����Խ�������
//			{
//				TIM_SetCounter(TIM7,0);//���������          				//���������
//				if(USART2_RX_STA==0) 				//ʹ�ܶ�ʱ��7���ж� 
//				{
//					TIM_Cmd(TIM7,ENABLE);//ʹ�ܶ�ʱ��7
//				}
//				USART2_RX_BUF[USART2_RX_STA++]=res;	//��¼���յ���ֵ
//			}else 
//			{
//				USART2_RX_STA|=1<<15;				//ǿ�Ʊ�ǽ������
//			} 
//		}
//	}  		 
//}   

#if Uart3_Printf_EN
//����3,printf ����
//ȷ��һ�η������ݲ�����USART3_MAX_SEND_LEN�ֽ�
void u3_printf(char* fmt,...)  
{  
	static uint16_t i,j;
	Uart_Data_t *spUart3_Data = GetUart_Data_t(Uart3_NUM);
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char*)spUart3_Data->TxBuf,fmt,ap);
	va_end(ap);
	i=strlen((const char*)spUart3_Data->TxBuf);		//�˴η������ݵĳ���
	for(j=0;j<i;j++)							//ѭ����������
	{
	  	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //ѭ������,ֱ���������   
		USART_SendData(USART3,spUart3_Data->TxBuf[j]);
	} 
}
#endif
