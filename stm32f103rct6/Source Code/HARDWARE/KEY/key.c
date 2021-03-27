#include "stm32f10x.h"
#include "key.h"
#include "delay.h"
#include "oled.h"
#include "glob.h"
							    
//������ʼ������
void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);	//ʹ��PORTCʱ��
 	
	GPIO_InitStructure.GPIO_Pin  = KEY_GPIO_IO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 			//���ó���������
 	GPIO_Init(KEY_GPIO, &GPIO_InitStructure);				//��ʼ��GPIOC.6
}

//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY0����
//2��KEY1����
//3��KEY2���� 
//4��KEY3���� WK_UP
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>KEY3!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEY0==0))//||KEY1==0||KEY2==0||WK_UP==1))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY0==0)return KEY0_PRES;
//		else if(KEY1==0)return KEY1_PRES;
//		else if(KEY2==0)return KEY2_PRES;
//		else if(WK_UP==1)return WKUP_PRES;
	}else if(KEY0==1)key_up=1;//&&KEY1==1&&KEY2==1&&WK_UP==0)key_up=1; 	    
 	return 0;// �ް�������
}

u8 key = 0;
/**********************************************************************
* �������ƣ�KEY_Handle
* ����������ͨ������ʵ�ֱ�������
* �����������
* ����ֵ����
* ����˵������
**********************************************************************/
void KEY_Handle()
{
	key = KEY_Scan(0);
	if(key)
	{
		switch (key)
		{
		case 1:
			gState.bits.b4 = !gState.bits.b4;	
//			OLED_SHOWBMP(gState); 
			break;
		}
	}
}


