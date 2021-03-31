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
	MAX30102_Init();			//MAX30102��ʼ��			
	OLED_Init();				//��ʾ����ʼ��
	GA6_Init();					//GA6ģ���ʼ��
	OLED_MAIN();				//OLED��ʾ
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
		if(sStateTime >= 30){			//����30s�ͽ��ж��ű���
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
	if(sStateTimeInterval){				//���͹�һ��֮��Ҫ���1���Ӳ��ܽ����´η���
		sTimeInterval++;
		if(sTimeInterval >= 60){
			sStateTimeInterval = 0;		//������ŷ���
			sTimeInterval = 0;
		}
	}
}
/**********************************************************************
* �������ƣ�Task_25MsHandle()
* ����������25MS�¼�����
* �����������
* ����ֵ����
* ����˵������
**********************************************************************/
void Task_25MsHandle()
{
	if(Ev25Ms) return;
	Ev25Ms = SysTick_25MS;
	OLED_SHOWDATA();
	Search_GY615_Data();
}
/**********************************************************************
* �������ƣ�Task_50MsHandle()
* ����������50MS�¼�����
* �����������
* ����ֵ����
* ����˵������
**********************************************************************/
void Task_50MsHandle()
{
	if(Ev50Ms) return;
	Ev50Ms = SysTick_50MS;
	GY615_Handle();
}
/**********************************************************************
* �������ƣ�Task_100MsHandle()
* ����������100MS�¼�����
* �����������
* ����ֵ����
* ����˵������
**********************************************************************/
void Task_100MsHandle()
{
	if(Ev100Ms) return;
	Ev100Ms = SysTick_100MS;
}
/**********************************************************************
* �������ƣ�Task_500MsHandle()
* ����������500MS�¼�����
* �����������
* ����ֵ����
* ����˵������
**********************************************************************/
void Task_500MsHandle()
{
	if(Ev500Ms) return;
	Ev500Ms = SysTick_500MS;
	MAX30102_Handle();
}
/**********************************************************************
* �������ƣ�Task_1SHandle()
* ����������1S�¼�����
* �����������
* ����ֵ����
* ����˵������
**********************************************************************/
void Task_1SHandle()
{
	if(Ev1S) return;
	Ev1S = SysTick_1S;
	State1sTimeHandle();
}
/**********************************************************************
* �������ƣ�System_Running()
* ������������̨ϵͳ����
* �����������
* ����ֵ����
* ����˵��������20��50��100��500��1000Ms���¼�����
**********************************************************************/
void System_Running()
{
	Task_25MsHandle();
	Task_50MsHandle();
	Task_100MsHandle();
	Task_500MsHandle();
	Task_1SHandle();
}

