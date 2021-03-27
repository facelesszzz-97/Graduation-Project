#include "relay.h"

/**********************************************************************
* �������ƣ�RELAY_Init
* �����������̵��� IO��ʼ��
* �����������
* ����ֵ����
* ����˵����
**********************************************************************/
void RELAY_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//ʹ��PC�˿�ʱ��
	
 	GPIO_InitStructure.GPIO_Pin = DOOR_GPIO_IO|MOTOR_GPIO_IO;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 	GPIO_Init(MOTOR_GPIO, &GPIO_InitStructure);					 //�����趨������ʼ��
}
 
