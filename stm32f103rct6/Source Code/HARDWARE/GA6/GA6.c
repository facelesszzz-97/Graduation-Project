#include "GA6.h"
#include "delay.h"
#include "GA6.h"
#include "string.h"	
#include "stdio.h"

extern uint16_t USART_RX_STA;

const static uint8_t gSearchSim[] = "AT+CPIN?";
const static uint8_t gReady[] = "Ready";

void GA6_Init()
{
	while(!GA6_Send_Cmd((uint8_t *)gSearchSim, (uint8_t *)gReady, GY6_SENDTIME));
}

/**********************************************************************
* �������ƣ�GA6_Send_Cmd
* ������������GA6��������
* ���������cmd(���͵������ַ���)��ack(�ڴ�Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��)
			waittime(�ȴ�ʱ�� ��λ:1ms)
* ����ֵ��0(���ͳɹ����õ��ڴ���Ӧ����),1(����ʧ��)
* ����˵������
**********************************************************************/
uint8_t GA6_Send_Cmd(uint8_t *cmd,uint8_t *ack,u16 waittime)
{
	uint8_t res=0; 
	printf("%s\r\n",cmd);	//��������
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(1);
			if(USART_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				if(GA6_Check_Cmd(ack))
				{
					break;			//�õ���Ч���� 
				}
				USART_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
} 
/**********************************************************************
* �������ƣ�GA6_Check_Cmd
* ����������GA6���������,�����յ���Ӧ��
* ���������str(�ڴ���Ӧ����)
* ����ֵ��0(û�еõ��ڴ���Ӧ����),����(�ڴ�Ӧ����str��λ��)
* ����˵������
**********************************************************************/

uint8_t* GA6_Check_Cmd(uint8_t *str)
{
	char *strx=0;
	Uart_Data_t *spUart_Data = GetUart_Data_t(Uart1_NUM);
	if(USART_RX_STA&0X8000)										//���յ�һ��������
	{ 
		spUart_Data->RxBuf[USART_RX_STA&0X7FFF]=0;						//��ӽ�����
		strx=strstr((const char*)spUart_Data->RxBuf,(const char*)str);
	} 
	return (u8*)strx;
}

