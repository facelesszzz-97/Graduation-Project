#ifndef __ESP8266_H
#define __ESP8266_H	 
#include "sys.h"  
/**********************************************************************
*						WIFIָ��˵��
* ���ù���ģʽ 1��stationģʽ   2��APģʽ  3������ AP+stationģʽ
* "AT+CWMODE=1";

* ��ģ���������Լ����ȵ�
* "AT+CWJAP=\"Faceless\",\"05720041018\"";

* =0����·����ģʽ     =1����·����ģʽ
* "AT+CIPMUX=0";

* ����TCP����  ������ֱ������ Ҫ���ӵ�ID��0~4   ��������  Զ�̷�����IP��ַ   Զ�̷������˿ں�
* "AT+CIPSTART=\"TCP\",\"192.168.43.1\",8900";

* �Ƿ���͸��ģʽ  0����ʾ�ر� 1����ʾ����͸��
* "AT+CIPMODE=1";

* ͸��ģʽ�� ��ʼ�������ݵ�ָ�� ���ָ��֮��Ϳ���ֱ�ӷ�������
*"AT+CIPSEND";
**********************************************************************/
#define CONNECT_10MS 200

void WIFI_Connect(void);
void ESP8266_Init(void);
u8 ESP8266_Send_Cmd(u8 *cmd,u8 *ack,u16 waittime);
u8* ESP8266_Check_Cmd(u8 *str);
void ESP8266_SendToApp(void);
void ESP8266_RecToAPP(void);



#endif













