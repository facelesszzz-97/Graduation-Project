#ifndef __USART3_H
#define __USART3_H
#include "sys.h"

#define USART3_MAX_RECV_LEN		600					//�����ջ����ֽ���
#define USART3_MAX_SEND_LEN		600					//����ͻ����ֽ���
#define USART3_RX_EN 			1					//0,������;1,����.

void usart3_init(u32 bound);				//����3��ʼ�� 
void u3_printf(char* fmt,...);
#endif
