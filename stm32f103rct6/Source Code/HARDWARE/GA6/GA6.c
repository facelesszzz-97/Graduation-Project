#include "GA6.h"
#include "delay.h"
#include "GA6.h"
#include "string.h"	
#include "oled.h"

extern uint16_t USART_RX_STA;
extern Oled_Data_Show_t *pData_Show;

const static uint8_t *gSearchSim = "AT+CPIN?";
const static uint8_t *gSetText = "AT+CMGF=1";
const static uint8_t *gSetAscii = "AT+CSCS=\"GSM\"";
const static uint8_t *gSetStorage = "AT+CPMS=\"SM\",\"ME\",\"MT\"";
const static uint8_t *gSetStorageOk = "+CPMS";
const static uint8_t *gREADY = "READY";
const static uint8_t *gOK = "OK";

void GA6_Init()
{
	OLED_ShowString(32, 0, "GA6 Init....", 16);
	OLED_ShowString(0, 2, (uint8_t *)gSetStorage, 16);
	while(GA6_Send_Cmd((uint8_t *)gSetStorage, (uint8_t *)gSetStorageOk, GY6_SENDTIME));		//���ô洢λ��
	OLED_ShowString(0, 6, (uint8_t *)gSearchSim, 16);
	while(GA6_Send_Cmd((uint8_t *)gSearchSim, (uint8_t *)gREADY, GY6_SENDTIME));	//��ѯSIM���Ƿ����
//	while(GA6_Send_Cmd((uint8_t *)gSetText, (uint8_t *)gOK, GY6_SENDTIME));			//����TEXTģʽ
//	while(GA6_Send_Cmd((uint8_t *)gSetAscii, (uint8_t *)gOK, GY6_SENDTIME));		//�����ַ���ʽ
	OLED_ShowString(80, 6, (uint8_t *)gOK, 16);
	delay_ms(GY6_SENDTIME);
}
/*****************************
 * ���͵Ķ��Ÿ�ʽ����
 * HeartRate:85
 * Spo2:95.22
 * BodyTemperature:36.4
 * Numerical abnormality!!!
 * Attention!
 * 		From XiaoKang
 *****************************/
void GA6_SendMessage(Oled_Data_Show_t data)
{
	uint8_t sHeartRate[19], sSpo2[16], sTemp[23];
	uint8_t sValue[60];
	uint8_t sSendMessageBuf[100];
	sprintf(sHeartRate, "HeartRate:%s", data.HeartRate);
	sprintf(sSpo2, "Spo2:%s", data.Spo2);
	sprintf(sTemp, "BodyTemperature:%s", data.BodyTemp);
	sprintf(sValue, "%s\r\n%s\r\n%s\r\n", sHeartRate, sSpo2, sTemp);
	sprintf(sSendMessageBuf, "%sNumerical abnormality!!!\r\nAttention!\r\n		From XiaoKang", sValue);
	printf(sSendMessageBuf);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
		USART_SendData(USART1, GY6_SENDEND);
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
	USART_RX_STA=0;
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
		spUart_Data->RxResBuf[USART_RX_STA&0X7FFF]=0;						//��ӽ�����
		strx=strstr((const char*)spUart_Data->RxResBuf,(const char*)str);
	} 
	return (u8*)strx;
}

