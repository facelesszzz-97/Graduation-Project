#ifndef __DHT22_H
#define __DHT22_H 
#include "sys.h"   
#include "datatype.h"

#define DHT22_GPIO				GPIOC
#define DHT22_GPIO_IO			GPIO_Pin_3   
#define DHT22_DQ_OUT 			PCout(3) 			
#define DHT22_DQ_IN  			PCin(3)  

#define DHT22_TEM_LIMIT			30
#define DHT22_HUMI_LIMIT		70


u8 DHT22_Init(void); 					//��ʼ��DHT22
u8 DHT22_Read_Data(void); 					//��ȡ��ʪ��
u8 DHT22_Read_Byte(void); 				//����һ���ֽ�
u8 DHT22_Read_Bit(void); 				//����һ��λ
u8 DHT22_Check(void); 					//����Ƿ����DHT22
void DHT22_Rst(void); 					//��λDHT22    
u32 DHT22_RH2Ten(u8 RH_data_H,u8 RH_data_L);
u32 DHT22_Tem2Ten(u8 Tem_data_H,u8 Tem_data_L);
void RH_Tem_U32toString(u32 RH,u32 Tem);
void DHT22_LIMIT(Byte8 data);



#endif
