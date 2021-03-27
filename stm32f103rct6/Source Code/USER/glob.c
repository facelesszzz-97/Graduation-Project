#include "glob.h"
#include "usart3.h"

//-----------------------------------------------
//----------------ͨ������-----------------------
stWIFI gESP8266;
stWIFI *pESP8266 = &gESP8266;

u8 gUsartCount = 0,ReceiveFlag = 0,gReceive[RECEIVE_DATA_THENGTH];
stReceived gReceived;
stReceived *pReceived = &gReceived;
u8 gRecDataFormat[4] = {'d','e','9','9'};

u8 USART3_RX_BUF[USART3_MAX_RECV_LEN];	//���ջ���,���USART3_MAX_RECV_LEN���ֽ�.
u8 USART3_TX_BUF[USART3_MAX_SEND_LEN];	//���ͻ���,���USART3_MAX_SEND_LEN���ֽ�
u16 USART3_RX_STA;

Byte8 gState;
u8 gSend[SEND_DATA_THENGTH];
//-----------------------------------------------
//--------------����������-----------------------
stDHT22 gDHT22;
stDHT22 *pDHT22 = &gDHT22;

u8 dht22_RH_str[] = "00.0";				//ʪ���ַ�������
u8 dht22_Tem_str[] = "00.0";			//�¶��ַ�������
u8 send_Tem[] = "Tem:00.0";
u8 send_RH[] = "RH:00.0";
u16 dht22_RH;							//ʪ�����ͱ���
u16 dht22_Tem;							//�¶����ͱ���

stMQ2 gMQ2;
stMQ2 *pMQ2 = &gMQ2;

u8 BODY_Flag = 0;
u8 BODY_Num = 1;






