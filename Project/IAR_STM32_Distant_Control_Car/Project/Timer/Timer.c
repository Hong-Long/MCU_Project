
#include "Timer.h"
#include "Motor.h"
#include "UART.h"
/**
* Function       TIM1_Int_Init
* @author        liusen
* @date          2015.01.03    
* @brief         ��ʱ��1��ʼ���ӿ�
* @param[in]     arr���Զ���װֵ��psc��ʱ��Ԥ��Ƶ��
* @param[out]    void
* @retval        void
* @par History   ����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
*/
void TIM1_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //ʱ��ʹ��
	
  //��ʱ��TIM1��ʼ��
  TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
  TIM_TimeBaseStructure.TIM_Prescaler = (psc-1); //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim   //36Mhz
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;    //�ظ������ر�
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
  TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE ); //ʹ��ָ����TIM1�ж�,��������ж�

	//�ж����ȼ�NVIC����
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  //TIM1�ж�
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
  NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


  TIM_Cmd(TIM1, ENABLE);  //ʹ��TIMx					 
}

int num = 0;	 /*����������ʱ��*/
long g_count = 0; /*�����ϱ�����*/
int speed2=0,speed2_1=0;
extern int speed,speed1;
void TIM1_UP_IRQHandler(void)   //TIM3�ж�
{
	speed2=speed*2;//�����ٶ�
        speed2_1 = speed1*2;//�����ٶ�
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);  //���TIMx�����жϱ�־ 
		num++;
		g_count++;
	
		if(run==0)
		{
			if(num <= speed2)//�����ת
			{
				M1A=0;//������ת
				M1B=1;
			}
			else
			{
				M1A=0;//���ֹͣ
				M1B=0;
			}
			if(num <= speed2_1)//�����ת
			{
				M2A=0;//������ת
				M2B=1;
			}
			else
			{
				M2A=0;
				M2B=0;
			}
		}
		if(run==1)
		{
			if(num <= speed2)//�����ת
			{
				M1A=1;
				M1B=0;
			}
			else
			{
				M1A=0;//���ֹͣת��
				M1B=0;
			}
			if(num <= speed2_1)//�����ת
			{
				M2A=1;//���ַ�ת
				M2B=0;
			}
			else
			{
				M2A=0;
				M2B=0;
			}
		}
		

		if(run==2)
		{
			if(num <= speed2)//�����ת
			{
				M1A=0;
				M1B=1;

			}
			else
			{
				M1A=0;//���ֹͣת��
				M1B=0;
			}
			if(num <= speed2_1)//�����ת
			{

				M2A=0;
				M2B=1;
			}
			else
			{
				M2A=0;
				M2B=0;
			}
                        
		}
		
		if(run==3)
		{
			if(num <= speed2)//�����ת
			{
				M1A=0;
				M1B=1;

			}
			else
			{
				M1A=0;//���ֹͣת��
				M1B=0;
			}
			if(num <= speed2_1)//�����ת
			{

				M2A=0;
				M2B=1;
			}
			else
			{
				M2A=0;
				M2B=0;
			}
		}
		if(run==4)
		{
			if(num <= speed2)//���ԭ����ת
			{
				M1A=1;
				M1B=0;
				M2A=0;
				M2B=1;
			}
			else
			{
				M1A=0;//���ֹͣ
				M1B=0;
				M2A=0;
				M2B=0;
			}
		}
		if(run==5)
		{
			if(num <= speed2)//���ԭ����ת
			{
				M1A=0;
				M1B=1;
				M2A=1;
				M2B=0;
			}
			else
			{
				M1A=0;//���ֹͣ
				M1B=0;
				M2A=0;
				M2B=0;
			}
		}
		if(num == 2000) //2000*10us = 20ms ����
		{
			num = 0;
		}		
		
	}
	
}


void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM3, //TIM2
		TIM_IT_Update ,
		ENABLE  //ʹ��
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
							 
}

void TIM3_IRQHandler(void)   //TIM3�ж�
{
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
  {
		
		
   }
  TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
}
