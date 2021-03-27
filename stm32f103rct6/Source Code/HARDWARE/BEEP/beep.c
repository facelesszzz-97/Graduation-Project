#include "beep.h"
#include "delay.h"

/**********************************************************************
* �������ƣ�BEEP_Init
* ������������������ʼ��
* �����������
* ����ֵ����
* ����˵������ʼ��PC2Ϊ�����.��ʹ������ڵ�ʱ��	
**********************************************************************/
void BEEP_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 	//ʹ��GPIOC�˿�ʱ��
 
 GPIO_InitStructure.GPIO_Pin = BEEP_GPIO_IO;				 	//BEEP-->PC2 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 	//�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 	//�ٶ�Ϊ50MHz
 GPIO_Init(BEEP_GPIO, &GPIO_InitStructure);						//���ݲ�����ʼ��GPIOC.2
 
 //GPIO_SetBits(GPIOC,GPIO_Pin_2);							//���1���������������
}

/**********************************************************************
* �������ƣ�DiDa
* �������������÷�������Ӧģʽ
* ���������ms(��ʱ)��num(����)
* ����ֵ����
* ����˵������
**********************************************************************/
void DiDa(u16 ms, u8 num)
{
	u8 i;
	for(i = 0; i < num; i++)
	{
		BEEP_ON;
		delay_ms(ms);
		BEEP_OFF;	
	}
}
