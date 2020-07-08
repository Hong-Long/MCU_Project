
#include "Timer.h"
#include "Motor.h"
#include "UART.h"
/**
* Function       TIM1_Int_Init
* @author        liusen
* @date          2015.01.03    
* @brief         定时器1初始化接口
* @param[in]     arr：自动重装值。psc：时钟预分频数
* @param[out]    void
* @retval        void
* @par History   这里时钟选择为APB1的2倍，而APB1为36M
*/
void TIM1_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //时钟使能
	
  //定时器TIM1初始化
  TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
  TIM_TimeBaseStructure.TIM_Prescaler = (psc-1); //设置用来作为TIMx时钟频率除数的预分频值
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim   //36Mhz
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;    //重复计数关闭
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
  TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE ); //使能指定的TIM1中断,允许更新中断

	//中断优先级NVIC设置
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  //TIM1中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
  NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


  TIM_Cmd(TIM1, ENABLE);  //使能TIMx					 
}

int num = 0;	 /*用来计算舵机时间*/
long g_count = 0; /*用来上报数据*/
int speed2=0,speed2_1=0;
extern int speed,speed1;
void TIM1_UP_IRQHandler(void)   //TIM3中断
{
	speed2=speed*2;//左轮速度
        speed2_1 = speed1*2;//右轮速度
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);  //清除TIMx更新中断标志 
		num++;
		g_count++;
	
		if(run==0)
		{
			if(num <= speed2)//电机正转
			{
				M1A=0;//左轮正转
				M1B=1;
			}
			else
			{
				M1A=0;//电机停止
				M1B=0;
			}
			if(num <= speed2_1)//电机正转
			{
				M2A=0;//右轮正转
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
			if(num <= speed2)//电机反转
			{
				M1A=1;
				M1B=0;
			}
			else
			{
				M1A=0;//电机停止转动
				M1B=0;
			}
			if(num <= speed2_1)//电机反转
			{
				M2A=1;//右轮反转
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
			if(num <= speed2)//电机左转
			{
				M1A=0;
				M1B=1;

			}
			else
			{
				M1A=0;//电机停止转动
				M1B=0;
			}
			if(num <= speed2_1)//电机左转
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
			if(num <= speed2)//电机左转
			{
				M1A=0;
				M1B=1;

			}
			else
			{
				M1A=0;//电机停止转动
				M1B=0;
			}
			if(num <= speed2_1)//电机左转
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
			if(num <= speed2)//电机原地左转
			{
				M1A=1;
				M1B=0;
				M2A=0;
				M2B=1;
			}
			else
			{
				M1A=0;//电机停止
				M1B=0;
				M2A=0;
				M2B=0;
			}
		}
		if(run==5)
		{
			if(num <= speed2)//电机原地右转
			{
				M1A=0;
				M1B=1;
				M2A=1;
				M2B=0;
			}
			else
			{
				M1A=0;//电机停止
				M1B=0;
				M2A=0;
				M2B=0;
			}
		}
		if(num == 2000) //2000*10us = 20ms 周期
		{
			num = 0;
		}		
		
	}
	
}


void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM3, //TIM2
		TIM_IT_Update ,
		ENABLE  //使能
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设
							 
}

void TIM3_IRQHandler(void)   //TIM3中断
{
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
  {
		
		
   }
  TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
}
