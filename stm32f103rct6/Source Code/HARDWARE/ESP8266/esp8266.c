#include "delay.h"
#include "esp8266.h" 
#include "string.h"	
#include "stdio.h"
#include "glob.h"  
#include "beep.h"
#include "oled.h"
#include "bsp_uart.h"

/**********************************************************************
* �������ƣ�ESP8266_Init
* ��������������ESP8266��ATָ��
* �����������
* ����ֵ����
* ����˵������
**********************************************************************/
void ESP8266_Init(void)
{
//	usart3_init(115200);
	pESP8266->OK = (u8*)"OK";
	pESP8266->WokeMode = (u8*)"AT+CWMODE=1";
	pESP8266->HopSpot = (u8*)"AT+CWJAP=\"Faceless\",\"lmc05720041018\"";
	pESP8266->ConnectMode = (u8*)"AT+CIPMUX=0";
	pESP8266->ConnectTCP = (u8*)"AT+CIPSTART=\"TCP\",\"192.168.31.160\",8900";
	pESP8266->CipMode = (u8*)"AT+CIPMODE=1";
	pESP8266->CipSend = (u8*)"AT+CIPSEND";
}
/**********************************************************************
* �������ƣ�WIFI_Connect
* ����������ESP8266������λ��������
* �����������
* ����ֵ����
* ����˵������
**********************************************************************/
void WIFI_Connect(void)
{
	ESP8266_Init();
	while(ESP8266_Send_Cmd(pESP8266->WokeMode,pESP8266->OK,CONNECT_10MS));
	while(ESP8266_Send_Cmd(pESP8266->HopSpot,pESP8266->OK,CONNECT_10MS));
	while(ESP8266_Send_Cmd(pESP8266->ConnectMode,pESP8266->OK,CONNECT_10MS));
	while(ESP8266_Send_Cmd(pESP8266->ConnectTCP,pESP8266->OK,CONNECT_10MS));
	while(ESP8266_Send_Cmd(pESP8266->CipMode,pESP8266->OK,CONNECT_10MS));
	while(ESP8266_Send_Cmd(pESP8266->CipSend,pESP8266->OK,CONNECT_10MS));
	USART3_RX_STA=0;
	//ESP8266_SendToApp();
//	USART3_RX_STA|=0x8000;
}
/**********************************************************************
* �������ƣ�ESP8266_Send_Cmd
* ������������ESP8266��������
* ���������cmd(���͵������ַ���)��ack(�ڴ�Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��)
			waittime(�ȴ�ʱ�� ��λ:10ms)
* ����ֵ��0(���ͳɹ����õ��ڴ���Ӧ����),1(����ʧ��)
* ����˵������
**********************************************************************/
u8 ESP8266_Send_Cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART3_RX_STA=0;
	//printf("%s\r\n",cmd);	//��������
	u3_printf("%s\r\n",cmd);	//��������
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(USART3_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				//printf("%s\r\n",USART3_RX_BUF);
				if(ESP8266_Check_Cmd(ack))
				{
//					DiDa(100,1);
					//printf("ack:%s\r\n",ack);
					break;//�õ���Ч���� 
				}
				USART3_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
} 
/**********************************************************************
* �������ƣ�ESP8266_Check_Cmd
* ����������ESP8266���������,�����յ���Ӧ��
* ���������str(�ڴ���Ӧ����)
* ����ֵ��0(û�еõ��ڴ���Ӧ����),����(�ڴ�Ӧ����str��λ��)
* ����˵������
**********************************************************************/
u8* ESP8266_Check_Cmd(u8 *str)
{
	char *strx=0;
	if(USART3_RX_STA&0X8000)										//���յ�һ��������
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;						//��ӽ�����
		strx=strstr((const char*)USART3_RX_BUF,(const char*)str);
		//printf("%sr\n",strx);
	} 
	return (u8*)strx;
}
/**********************************************************************
* �������ƣ�ESP8266_SendToApp
* ����������ͨ��ESP8266�����ݷ��͸�APP
* �����������
* ����ֵ����
* ����˵������
**********************************************************************/
void ESP8266_SendToApp()
{
	u8 i,res = 0;
	gSend[0] = 0xDE;
	gSend[1] = 0x97;
	memcpy(&gSend[2], pMQ2, 2);
	memcpy(&gSend[4], pDHT22, 4);
	gSend[8] = gState.ALL;
	for(i = 0; i < SEND_DATA_THENGTH - 1; i++)
	{
		res += gSend[i];
	}
	gSend[i] = res;
	for(i = 0; i < SEND_DATA_THENGTH; i++)							//ѭ����������
	{
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //ѭ������,ֱ���������	 
			USART_SendData(USART3,gSend[i]);
	}
} 
/**********************************************************************
* �������ƣ�ESP8266_RecToAPP
* ����������ͨ��ESP8266�յ���λ�����͵�����
* �����������
* ����ֵ����
* ����˵������
**********************************************************************/
void ESP8266_RecToAPP()
{
	u8 i;
	if(USART3_RX_STA&0X8000)
	{
		//���յ����������͹ر�ָ��
		if(ESP8266_Check_Cmd("protect"))
		{
			BODY_Flag = 1;	
		}
		else if(ESP8266_Check_Cmd("close"))
		{
			BODY_Flag = 0;
		}
		//���յ�����֡
		for(i = 0; i < RECEIVE_DATA_THENGTH - 1; i++)
		{
			if(USART3_RX_BUF[i] == gRecDataFormat[i])
			{
				continue;
			}
			else
			{
				break;
			}
		}
		//�����յ������ݽ���״̬����
		if(i == 4)
		{
			i = 0;
			memcpy(pReceived,USART3_RX_BUF,RECEIVE_DATA_THENGTH);
			gState.ALL = (pReceived->Data[0].ALL - '0') * 10 + pReceived->Data[1].ALL - '0';
		}
		USART3_RX_STA = 0;
	}
}


















