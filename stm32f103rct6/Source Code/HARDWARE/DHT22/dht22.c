#include "dht22.h"
#include "delay.h"
#include "oled.h"
#include "string.h"
#include "glob.h"


/**********************************************************************
* �������ƣ�DHT22_Rst
* ������������λ
* �����������
* ����ֵ����
* ����˵������
**********************************************************************/

void DHT22_Rst(void)    
{     

    DHT22_DQ_OUT=0; 				//����DQ
    delay_ms(20);    				//��������18ms
    DHT22_DQ_OUT=1; 				//DQ=1 
	delay_us(30);      				//��������20~40us
}

/**********************************************************************
* �������ƣ�DHT22_Check
* �����������ȴ�DHT22�Ļ�Ӧ
* �����������
* ����ֵ��1��δ����DHT22����0�����ڣ�
* ����˵������
**********************************************************************/

u8 DHT22_Check(void)    
{   
	u8 retry=100;
    while ((!DHT22_DQ_IN)&&retry)	//DHT22������40~80us
    {
		retry--;
		delay_us(1);
	}  
	if(retry<1) return 1;
	else retry=100;
	while (DHT22_DQ_IN&&retry)		//DHT22���ͺ���ٴ�����40~80us
	{
		retry--;
		delay_us(1);
	}
	if(retry<1) 
		return 1;     
	return 0;
}

/**********************************************************************
* �������ƣ�DHT22_Read_Bit
* ������������DHT22��ȡһ��λ
* �����������
* ����ֵ��1��0
* ����˵������
**********************************************************************/
u8 DHT22_Read_Bit(void)  
{

	u8 dat;
	u8 retry=100;
	while((!DHT22_DQ_IN)&&retry)	//�ȴ���Ϊ�ߵ�ƽ
	{
		retry--;
		delay_us(1);
	}
	retry=100;
	dat=0;
	delay_us(30);					//�ȴ�30us
	if(DHT22_DQ_IN) 
		dat=1;    
	while(DHT22_DQ_IN&&retry)		//�ȴ���͵�ƽ
	{
		retry--;
		delay_us(1);
	}
	return dat;
}

/**********************************************************************
* �������ƣ�DHT22_Read_Byte
* ������������DHT22��ȡһ���ֽ�
* �����������
* ����ֵ������������
* ����˵������
**********************************************************************/

u8 DHT22_Read_Byte(void)    
{        
    u8 i,dat;
    dat=0;
	for (i=0;i<8;i++) 
	{
	    dat<<=1; 
	    dat|=DHT22_Read_Bit();
    }     
    return dat;
}
/**********************************************************************
* �������ƣ�DHT22_RH2Ten
* ������������ʪ������λ32bit���ݵ�16����ת����10����
* ���������RH_data_H��RH_data_L
* ����ֵ���¶���������
* ����˵������
**********************************************************************/
u32 DHT22_RH2Ten(u8 RH_data_H,u8 RH_data_L)
{
	//ʪ����ȡ
	u8 dht22_RH_HH4;//��8λǰ4λ
	u8 dht22_RH_HL4;//��8λ��4λ
	u8 dht22_RH_LH4;//��8λǰ4λ
	u8 dht22_RH_LL4;//��8λ��4λ
	dht22_RH_HH4 = RH_data_H >> 4;
	dht22_RH_HL4 = RH_data_H & 0x0f;
	dht22_RH_LH4 = RH_data_L >> 4;
	dht22_RH_LL4 = RH_data_L & 0x0f;
	return dht22_RH_HH4 * 4096 + dht22_RH_HL4 * 256 + dht22_RH_LH4 * 16 + dht22_RH_LL4;
}
/**********************************************************************
* �������ƣ�DHT22_Tem2Ten
* �������������¶�����λ32bit���ݵ�16����ת����10����
* ���������Tem_data_H��Tem_data_L
* ����ֵ���¶���������
* ����˵������
**********************************************************************/
u32 DHT22_Tem2Ten(u8 Tem_data_H, u8 Tem_data_L)
{
	//�¶���ȡ
	u8 dht22_Tem_HH4;//��8λǰ4λ
	u8 dht22_Tem_HL4;//��8λ��4λ
	u8 dht22_Tem_LH4;//��8λǰ4λ
	u8 dht22_Tem_LL4;//��8λ��4λ
	dht22_Tem_HH4 = Tem_data_H >> 4;
	dht22_Tem_HL4 = Tem_data_H & 0x0f;
	dht22_Tem_LH4 = Tem_data_L >> 4;
	dht22_Tem_LL4 = Tem_data_L & 0x0f;
	return dht22_Tem_HH4 * 4096 + dht22_Tem_HL4 * 256 + dht22_Tem_LH4 * 16 + dht22_Tem_LL4;
}
/**********************************************************************
* �������ƣ�RH_Tem_U32toString
* ������������DHT22����������ת�����ַ�������
* ���������RH��Tem
* ����ֵ����
* ����˵������
**********************************************************************/
void RH_Tem_U32toString(u32 RH,u32 Tem)
{
	u8 i = 4;
	while(i)
	{	
		i--;
		if(i == 2)			//����С����λ������
		{
			continue;
		}
		//�����ݴ����λ��ǰ�����ַ���
		dht22_RH_str[i] = RH % 10 + 0x30;
		dht22_Tem_str[i] = Tem % 10 + 0x30;
		send_Tem[i+4] = dht22_Tem_str[i];
		send_RH[i+3] = dht22_RH_str[i];
		RH /= 10;
		Tem /= 10;
	}
}

/**********************************************************************
* �������ƣ�DHT22_Read_Data
* ����������DHT22��ȡһ������
* ���������*temp(����)
* ����ֵ��1��ʧ�ܣ���0��������
* ����˵�����¶�ֵ(��Χ:0~50��)��ʪ��ֵ(��Χ:20%~90%)
**********************************************************************/

u8 DHT22_Read_Data()    
{        
	u8 buf[5];
	u8 i;
	DHT22_Rst();
	if(DHT22_Check()==0)
	{
		for(i=0;i<5;i++)//��ȡ40λ����
		{
			buf[i]=DHT22_Read_Byte();
		}
		memcpy(pDHT22,buf,sizeof(stDHT22));
		//У����ۼ�ʱ�����൱���ʻᳬ��8λ(��ֵ����1��char,����ǿ������ת����ֻ���ǵ�8λ)
		if((unsigned char)(pDHT22->HumiHigh + pDHT22->HumiLow + pDHT22->TempHigh + pDHT22->TempLow) == pDHT22->CheckSum)  
		{
			dht22_RH = DHT22_RH2Ten(pDHT22->HumiHigh,pDHT22->HumiLow);		//ʪ������ת������������
			dht22_Tem = DHT22_Tem2Ten(pDHT22->TempHigh,pDHT22->TempLow);	//�¶�����ת������������
			DHT22_LIMIT(gState);
			RH_Tem_U32toString(dht22_RH,dht22_Tem);							//����ʪ����������ת�����ַ�������	
			OLED_ShowString(20, 4, dht22_Tem_str, 16);
			OLED_ShowString(84, 4, dht22_RH_str, 16);
			return 0;
		}
	}
	return 1;   
}
/**********************************************************************
* �������ƣ�DHT22_LIMIT
* ������������DHT22��ʪ�Ƚ��бȽϣ��������򱨾�
* ���������data(��ǰģʽ)
* ����ֵ����
* ����˵������
**********************************************************************/
void DHT22_LIMIT(Byte8 data)
{
	if(data.bits.b2)//�Զ�ģʽ
	{
		if(dht22_Tem > DHT22_TEM_LIMIT * 10 || dht22_RH > DHT22_HUMI_LIMIT * 10)
		{
			if(dht22_Tem > DHT22_TEM_LIMIT * 10)
			{
				gState.ALL |= 0x41;
			}
			if(dht22_RH > DHT22_HUMI_LIMIT * 10)
			{
				gState.ALL |= 0x21;
			}
		}
		else
		{
			gState.ALL &= ~0x01;
		}
//		OLED_SHOWBMP(gState);
	}
}
 
/**********************************************************************
* �������ƣ�DHT22_Init
* ����������DHT22�˿����ó�ʼ�������DHT22�Ĵ���
* �����������
* ����ֵ��1�������ڣ���0�����ڣ�
* ����˵������
**********************************************************************/
u8 DHT22_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  	GPIO_InitStructure.GPIO_Pin = DHT22_GPIO_IO;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(DHT22_GPIO, &GPIO_InitStructure);
	DHT22_Rst();
	return DHT22_Check();
}
