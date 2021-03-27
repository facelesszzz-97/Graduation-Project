#ifndef __GLOB_H__
#define __GLOB_H__
#include "sys.h"
#include "datatype.h"

extern stWIFI *pESP8266;

extern u8 gUsartCount,ReceiveFlag,gReceive[];
extern stReceived *pReceived;
extern u8 gRecDataFormat[];

//���ڽ��ջ����� 	
extern u8 USART3_RX_BUF[]; 			//���ջ���,���USART3_MAX_RECV_LEN���ֽ�.
extern u8 USART3_TX_BUF[]; 			//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
extern u16 USART3_RX_STA; 

extern stDHT22 *pDHT22;

extern u8 dht22_RH_str[];			//ʪ���ַ�������
extern u8 dht22_Tem_str[];			//�¶��ַ�������
extern u8 send_Tem[];
extern u8 send_RH[];
extern u16 dht22_RH;				//ʪ�����ͱ���
extern u16 dht22_Tem;				//�¶����ͱ���

extern stMQ2 *pMQ2;

extern Byte8 gState;
extern u8 gSend[];

extern u8 BODY_Flag;
extern u8 BODY_Num;



#endif
