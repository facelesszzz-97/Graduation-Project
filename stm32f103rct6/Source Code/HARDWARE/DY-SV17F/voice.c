#include "voice.h"
#include "usart2.h"
#include <string.h>


typedef struct
{
	u8 Start;
	u8 Data;
	u8 Length;
	u8 Check;
}stVOICE;

stVOICE gVOICE;
stVOICE *pVOICE = &gVOICE;


/**********************************************************************
* �������ƣ�VOICE_Send
* ��������������ģ�鷢��ָ��
* ���������data
* ����ֵ����
* ����˵����ָ���ʽ				AA xx 00 (AA+00+xx) 
* 			����(02)����ͣ(03)��ֹͣ(04)����һ��(05)����һ��(06)
*			����+(14)������-(15)	
**********************************************************************/
void VOICE_Send(u8 data)
{
	u8 i;
	pVOICE->Start = 0xAA;											//ָ����
	pVOICE->Data = data;											//ָ������
	pVOICE->Length = 0x00;											//ָ���
	pVOICE->Check = pVOICE->Start + pVOICE->Data + pVOICE->Length;	//У���
	memcpy(USART2_TX_BUF,pVOICE,VOICE_DATA_LENGHT);
	for(i = 0; i < VOICE_DATA_LENGHT; i++)							//ѭ����������
	{
	  while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); 		//ѭ������,ֱ���������   
		USART_SendData(USART2,USART2_TX_BUF[i]);
	} 
}


/**********************************************************************
* �������ƣ�VOICE_Mode
* ��������������ģ�鹤��ģʽ����
* �����������
* ����ֵ����
* ����˵������
**********************************************************************/
void VOICE_Mode(void)
{
	VOICE_CON1_0;
	VOICE_CON2_0;
	VOICE_CON3_1;
	usart2_init(9600);
}

/**********************************************************************
* �������ƣ�VOICE_Init
* ������������ʼ������ģ��Ϊ���ڿ���
* �����������
* ����ֵ����
* ����˵����
**********************************************************************/
void VOICE_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = VOICE_CON1 | VOICE_CON2 | VOICE_CON3;		//��������ģ��GPIO��
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;						//����Ϊ�������ģʽ 		 
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 				//�����������50MHz
 	GPIO_Init(VOICE_CON_GPIO, &GPIO_InitStructure);							//ʹ��GIPO��
	VOICE_Mode();															//��������ģ�鹤��ģʽ

}




