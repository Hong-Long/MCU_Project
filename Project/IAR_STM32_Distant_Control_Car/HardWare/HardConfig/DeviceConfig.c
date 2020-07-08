
#include "DeviceConfig.h"

#include "stm32f10x.h"
#include "UART.h"
#include "delay.h"
#include "led.h"
#include "ov7725.h"
#include "exti.h"
#include "Timer.h"
#include "Motor.h"
#include <HL_Voltage.h>




void Hard_Configure(void)
{
  delay_init();
 // Uart1_init(115200);
  LED_Init();
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
  Adc_Init( );
  ADC_GPIO_Init();
  MOTOR_GPIO_Init();  
  TIM1_Int_Init(9, 72);		
  Uart2_init(9600);
  TIM3_Int_Init(10000,7199);			//TIM3,10Khz计数频率,1秒钟中断
  
}