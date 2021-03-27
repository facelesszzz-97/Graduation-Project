#include "body.h"
#include "glob.h"
#include "usart3.h"
#include "oled.h"
/**********************************************************************
* �������ƣ�BODY_Init
* �����������������ģ��
* �����������
* ����ֵ����
* ����˵����
**********************************************************************/
void BODY_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 	 	//ʹ��GPIOC�˿�ʱ��
 
 GPIO_InitStructure.GPIO_Pin = BODY_GPIO_IO;						//BODY-->PC.0 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 			//��������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 			//�ٶ�Ϊ50MHz
 GPIO_Init(BODY_GPIO, &GPIO_InitStructure);							//���ݲ�����ʼ��GPIOC.0
}
/**********************************************************************
* �������ƣ�BODY_Handle
* �����������ڷ���ģʽ�½��������⣬�������򱨾�
* �����������
* ����ֵ����
* ����˵�������ӷ��ʹ������ƣ����һ�ξͷ���һ�����ݣ������ظ�����
**********************************************************************/
void BODY_Handle()
{
	if(BODY_Flag)
	{
		if(BODY_READ == 1)
		{
			gState.ALL |= 0x01;
			if(BODY_Num)//���Ʒ��ʹ���
			{
				BODY_Num = 0;
				u3_printf("protect");
			}
		}else
		{
			BODY_Num = 1;
			gState.ALL &= ~0x01;
		}
//		OLED_SHOWBMP(gState);
	}
}
