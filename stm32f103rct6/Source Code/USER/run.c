#include "run.h"
#include "bsp.h"
#include <stdio.h>
#include "max30102.h"
#include "oled.h"
#include "GY615.h"
#include "GA6.h"
#include <string.h>


void System_Init()
{
	MAX30102_Init();
//	delay_init();	    	 							//��ʱ������ʼ��	  
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
//	//uart_init(115200);	 							//���ڳ�ʼ��Ϊ115200
	OLED_Init();										//��ʾ����ʼ��
	GA6_Init();											//GA6ģ���ʼ��
//	RELAY_Init();										//�̵�����ʼ��
//	BEEP_Init();										//��������ʼ��
//	KEY_Init();											//������ʼ��
//	//VOICE_Init();								
//	while(DHT22_Init());								//��ʪ�ȴ�������ʼ��
//	//VOICE_Send(0x02);
//	RC522_Init();										//RFID��ʼ��
//	MQ2_Init();											//����������ʼ��
//	WIFI_Connect();										//wifi����
	OLED_MAIN();										//OLED��ʾ
//	TIM3_Int_Init(499,7199);							//72Mhz/(7199+1)=10Khz��.����Ƶ�ʣ�0.1ms,  ������(499+1)Ϊ50ms  
}
/***************
 * gHandleState
 * b0:����ƫ��
 * b1:����ƫ��
 * b2:Ѫ��ƫ��
 * b3:Ѫ��ƫ��
 * b4:����ƫ��
 **************/
Byte8 gHandleState;
extern Oled_Data_Show_t *pData_Show;
void State1sTimeHandle()
{
	static Oled_Data_Show_t sData;
	static uint8_t sStateTime;					
	static uint8_t sStartSend, sStartSendTime;
	static uint8_t sTimeInterval, sStateTimeInterval;//ʱ������״̬���
	if(gHandleState.ALL > 0 && !sStateTimeInterval){
		sStateTime++;
		if(sStateTime >= 30){	//����30s�ͽ��ж��ű���
			sStateTime = 0;
			GA6_SendMessageCmd;
			memcpy(&sData, pData_Show, sizeof(Oled_Data_Show_t));
			sStartSend = 1;
		}
	}
	else{
		sStateTime = 0;
	}
	if(sStartSend){
		sStartSendTime++;
		if(sStartSendTime >= 5){		//5s֮���Ͷ�������
			sStartSend = 0;
			sStateTimeInterval = 1;
			//�������ݴ���
			GA6_SendMessage(sData);
		}
	}
	if(sStateTimeInterval){				//���͹�һ��֮��Ҫ���1���Ӳ��ܽ��ж����´η���
		sTimeInterval++;
		if(sTimeInterval >= 60){
			sStateTimeInterval = 0;		//������ŷ���
			sTimeInterval = 0;
		}
	}
}

void Task_25MsHandle()
{
	if(Ev25Ms) return;
	Ev25Ms = SysTick_25MS;
	OLED_SHOWDATA();
	Search_GY615_Data();
}

void Task_50MsHandle()
{
	if(Ev50Ms) return;
	Ev50Ms = SysTick_50MS;
	GY615_Handle();
}

void Task_100MsHandle()
{
	if(Ev100Ms) return;
	Ev100Ms = SysTick_100MS;
}

void Task_500MsHandle()
{
	if(Ev500Ms) return;
	Ev500Ms = SysTick_500MS;
	MAX30102_Handle();
}
void Task_1SHandle()
{
	if(Ev1S) return;
	Ev1S = SysTick_1S;
	State1sTimeHandle();
}

void System_Running()
{
	Task_25MsHandle();
	Task_50MsHandle();
	Task_100MsHandle();
	Task_500MsHandle();
	Task_1SHandle();
}

//void Task_ReceiveHandle()
//{
//	ESP8266_RecToAPP();
//	OLED_SHOWBMP(gState);
//}

//void Task_BodyHandle()
//{
//	BODY_Handle();
//}

//void Task_UARTHandle()
//{
//	if(UART_TIMER) return;
//	UART_TIMER = SYS_50MS;
//	ESP8266_SendToApp();
//}
//void Task_KEYHandle()
//{
//  	if(KEY_TIMER) return;
//  	KEY_TIMER = SYS_100MS;
//	KEY_Handle();
//}
//void Task_RC522Handle()
//{
//	if(RC522_TIMER) return;
//	RC522_TIMER = SYS_200MS;
//	RC522_Handle();
//}
//void Task_DHT22Handle()
//{
//	if(DHT22_TIMER) return;
//	DHT22_TIMER = SYS_500MS;
//	while(DHT22_Read_Data());
//}

//void Task_MQ2Handle()
//{
//	if(MQ2_TIMER) return;
//	MQ2_TIMER = SYS_1S;
//	MQ2_Handle();
//}
//	uint8_t t;
//	Uart_Data_t *sUart1_Data = GetUart_Data_t(Uart1_NUM);
//	if(USART_RX_STA&0x8000)
//	{					   
//		sUart1_Data->RxLen = USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
//		printf("\r\n�����͵���ϢΪ:\r\n\r\n");
//		for(t=0;t<sUart1_Data->RxLen;t++)
//		{
//			USART_SendData(USART1, sUart1_Data->RxBuf[t]);//�򴮿�1��������
//			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
//		}
//		printf("\r\n\r\n");//���뻻��
//		USART_RX_STA=0;
//	}

