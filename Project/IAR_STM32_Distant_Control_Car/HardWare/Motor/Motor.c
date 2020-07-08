
#include "Motor.h"
extern int speed,speed1,run;
int speed1;
void MOTOR_GPIO_Init(void)
{		
  GPIO_InitTypeDef  GPIO_InitStructure;	//����һ���ṹ��
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);	 //ʹ��PB,PC PD�˿�ʱ��
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//������Ҫʹ�ܵ�����				
	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	
  GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��
          
  GPIO_SetBits(GPIOC,GPIO_Pin_11);//�������ø�				 
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//������Ҫʹ�ܵ�����		
	
  GPIO_Init(GPIOC, &GPIO_InitStructure);//�����趨������ʼ��
	
  GPIO_SetBits(GPIOC,GPIO_Pin_12);//�������ø�		 						 
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//������Ҫʹ�ܵ�����		    
		
  GPIO_Init(GPIOD, &GPIO_InitStructure);//�����趨������ʼ��
                  
  GPIO_SetBits(GPIOD,GPIO_Pin_2); //�������ø�								 
		
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE); 
		
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //PB3��ֹJTAG	
		
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//������Ҫʹ�ܵ�����		    		
		
  GPIO_Init(GPIOB, &GPIO_InitStructure);//�����趨������ʼ��
		
  GPIO_SetBits(GPIOB,GPIO_Pin_3); //�������ø�								  	
		 

}

/**
* Function       MiniCar_Run
* @author        john
* @date          2018.10.9    
* @brief         С��ǰ��
* @param[in]     Speed  ��0~1000�� �ٶȷ�Χ
* @param[out]    void
* @retval        void
* @par History   ��
*/
void MiniCar_Run(int Speed0,int Speed1)
{
  speed=Speed0;
  speed1=Speed1;
  run=0;
}

/**
* Function       MiniCar_Back
* @author        john
* @date          2018.10.9    
* @brief         С������
* @param[in]     Speed  ��0~1000�� �ٶȷ�Χ
* @param[out]    void
* @retval        void
* @par History   ��
*/
void MiniCar_Back(int Speed0,int Speed1)
{
  speed=Speed0;
  speed1=Speed1;
  run=1;
}

/**
* Function       MiniCar_Back
* @author        john
* @date          2018.10.9    
* @brief         С��ֹͣ
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   ��
*/
void MiniCar_Stop()
{
  speed=0;
  speed1=0;
}

void MiniCar_TurnLeft(int Speed0,int Speed1)
{
  speed=Speed0;
  speed1=Speed1;
  run=2;
}
void MiniCar_TurnRight(int Speed0,int Speed1)
{
  speed=Speed0;
  speed1=Speed1;
  run=3;
}

void MiniCar_TTurnLeft(int Speed0,int Speed1)
{
  speed=Speed0;
  speed1=Speed1;
  run=4;
}
void MiniCar_TTurnRight(int Speed0,int Speed1)
{
  speed=Speed0;
  speed1=Speed1;
  run=5;
}

