 #include "adc.h"
 #include "mq2.h"
 #include "delay.h"				
 /**********************************************************************
 * �������ƣ�Adc_Init
 * ����������adc���ó�ʼ��
 * �����������
 * ����ֵ����
 * ����˵�������Թ���ͨ��Ϊ��
 **********************************************************************/
void  Adc_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
 

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   									//����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M

	//PA0 ��Ϊģ��ͨ����������                         
	GPIO_InitStructure.GPIO_Pin = MQ2_GPIO_IO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;							//ģ����������
	GPIO_Init(MQ2_GPIO, &GPIO_InitStructure);	

	ADC_DeInit(MQ2_ADC);  //��λADC1 

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;						//ADC����ģʽ:ADC1�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;							//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;						//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;		//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;					//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;									//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(MQ2_ADC, &ADC_InitStructure);									//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   

  
	ADC_Cmd(MQ2_ADC, ENABLE);												//ʹ��ָ����ADC1
	
	ADC_ResetCalibration(MQ2_ADC);											//ʹ�ܸ�λУ׼  
	 
	while(ADC_GetResetCalibrationStatus(MQ2_ADC));							//�ȴ���λУ׼����
	
	ADC_StartCalibration(MQ2_ADC);	 										//����ADУ׼
 
	while(ADC_GetCalibrationStatus(MQ2_ADC));	 							//�ȴ�У׼����
 
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);									//ʹ��ָ����ADC1�����ת����������

}				  

/**********************************************************************
* �������ƣ�Get_Adc
* �������������ADCֵ
* ���������ch(ͨ��ֵ0~3)
* ����ֵ����
* ����˵������
**********************************************************************/
u16 Get_Adc(u8 ch)   
{
  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(MQ2_ADC, ch, MQ2_ADC_IO, ADC_SampleTime_239Cycles5 );		//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
  
	ADC_SoftwareStartConvCmd(MQ2_ADC, ENABLE);									//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(MQ2_ADC, ADC_FLAG_EOC ));							//�ȴ�ת������

	return ADC_GetConversionValue(MQ2_ADC);									//�������һ��ADC1�������ת�����
}
/**********************************************************************
* �������ƣ�Get_Adc_Average
* ����������ȡ���ADƽ��ֵ
* ���������ch(ͨ��ֵ0~3)��times(����)
* ����ֵ����
* ����˵������
**********************************************************************/
u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 	 



























