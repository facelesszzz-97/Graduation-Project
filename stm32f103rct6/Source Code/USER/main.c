#include "run.h"
#include "warning.h"
#include "bsp.h"

int main(void)
{		
	BSP_Init();					//Ӳ���ײ��ʼ��
	System_Init();			//ģ�����ó�ʼ��
  	while(1)
	{
		System_Running();	//��̨ϵͳ������
	}	 
}	 
 
