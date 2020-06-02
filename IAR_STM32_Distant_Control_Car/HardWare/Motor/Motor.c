
#include "Motor.h"
extern int speed,speed1,run;
int speed1;
void MOTOR_GPIO_Init(void)
{		
  GPIO_InitTypeDef  GPIO_InitStructure;	//定义一个结构体
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);	 //使能PB,PC PD端口时钟
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//定义需要使能的引脚				
	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	
  GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化
          
  GPIO_SetBits(GPIOC,GPIO_Pin_11);//将引脚置高				 
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//定义需要使能的引脚		
	
  GPIO_Init(GPIOC, &GPIO_InitStructure);//根据设定参数初始化
	
  GPIO_SetBits(GPIOC,GPIO_Pin_12);//将引脚置高		 						 
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//定义需要使能的引脚		    
		
  GPIO_Init(GPIOD, &GPIO_InitStructure);//根据设定参数初始化
                  
  GPIO_SetBits(GPIOD,GPIO_Pin_2); //将引脚置高								 
		
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE); 
		
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //PB3禁止JTAG	
		
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//定义需要使能的引脚		    		
		
  GPIO_Init(GPIOB, &GPIO_InitStructure);//根据设定参数初始化
		
  GPIO_SetBits(GPIOB,GPIO_Pin_3); //将引脚置高								  	
		 

}

/**
* Function       MiniCar_Run
* @author        john
* @date          2018.10.9    
* @brief         小车前进
* @param[in]     Speed  （0~1000） 速度范围
* @param[out]    void
* @retval        void
* @par History   无
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
* @brief         小车后退
* @param[in]     Speed  （0~1000） 速度范围
* @param[out]    void
* @retval        void
* @par History   无
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
* @brief         小车停止
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   无
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

