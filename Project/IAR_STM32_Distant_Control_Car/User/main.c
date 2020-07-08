
#include "stm32f10x.h"
#include "DeviceConfig.h"
#include "UART.h"
#include "delay.h"
#include "sys.h" 
#include "stdio.h"
#include "led.h"
#include <HL_Voltage.h>
#include "Motor.h"
#include "Timer.h"
#include <stdlib.h>
#include <string.h>
#define Servo_J1 PBout(4)// PB4
#define Servo_J2 PBout(5)// PB5
#define Servo_J1_RCC	RCC_APB2Periph_GPIOB
#define Servo_J2_RCC	RCC_APB2Periph_GPIOB
#define Servo_J1_PORT	GPIOB
#define Servo_J2_PORT	GPIOB
#define Servo_J1_PIN	GPIO_Pin_4
#define Servo_J2_PIN	GPIO_Pin_5


int g_modeSelect = 0;  //0是默认APK上位机状态;  1:红外遥控 2:巡线模式 3:超声波避障 4: 七彩探照 5: 寻光模式 6: 红外跟踪


int speed=0,run;


u8 ProtocolString[80] = {0};
int CarSpeedControl = 200;


/*小车运行状态枚举*/

enum {

  enSTOP = 0,
  enRUN,
  enBACK,
  enLEFT,
  enRIGHT,
  enTLEFT,
  enTRIGHT

}enCarState;

#define 	run_car     '1'//按键前
#define 	back_car    '2'//按键后
#define 	left_car    '3'//按键左
#define 	right_car   '4'//按键右
#define 	stop_car    '0'//按键停

int g_CarState = enSTOP; //  1前2后3左4右0停止

/**
* Function       StringFind
* @author        liusen
* @date          2017.08.18    
* @brief         字符串查找
* @param[in]     pSrc:源字符串; pDst:查找的字符串; v_iStartPos:源字符串起始位置
* @param[out]    void
* @retval        void
* @par History   无
*/
void Servo_GPIO_Init(void)
{		
   	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*开启外设时钟*/
	RCC_APB2PeriphClockCmd(Servo_J1_RCC, ENABLE); 
	/*选择要控制的引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = Servo_J1_PIN;	
	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化PORT*/
  	GPIO_Init(Servo_J1_PORT, &GPIO_InitStructure);	
        
        	/*开启外设时钟*/
	RCC_APB2PeriphClockCmd(Servo_J2_RCC, ENABLE); 
	/*选择要控制的引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = Servo_J2_PIN;	
	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化PORT*/
  	GPIO_Init(Servo_J2_PORT, &GPIO_InitStructure);
        Servo_J1=1;
        Servo_J2=1;
}
int StringFind(const char *pSrc, const char *pDst, int v_iStartPos)  
{  
    int i, j;  
    for (i = v_iStartPos; pSrc[i]!='\0'; i++)  
    {  
        if(pSrc[i]!=pDst[0])  
            continue;         
        j = 0;  
        while(pDst[j] !='\0' && pSrc[i+j]!='\0')  
        {  
            j++;  
            if(pDst[j]!=pSrc[i+j])  
            break;  
        }  
        if(pDst[j]=='\0')  
            return i;  
    }  
    return -1;  
} 

/**
* Function       ProtocolCpyData
* @author        liusen
* @date          2017.08.18    
* @brief         串口数据拷贝到新的buf中防止处理过程中被新数据覆盖
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   无
*/
void ProtocolCpyData(void)
{
	memcpy(ProtocolString, inputString, g_packnum + 1);
	memset(inputString, 0x00, sizeof(inputString));
}

/**
* Function       Protocol
* @author        liusen
* @date          2017.08.18    
* @brief         协议入口
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   无
*/
void Protocol(void)
{
	ProtocolCpyData();

	/*解析模式切换*/
  	//先判断是否是模式选择	
	if(StringFind((const char *)ProtocolString, (const char *)"MODE", 0) > 0 
		&& StringFind((const char *)ProtocolString, (const char *)"4WD", 0) > 0)
	{
		
	    if (ProtocolString[10] == '0') //停止模式
	    {
	    	MiniCar_Stop();
		g_CarState = enSTOP;
		g_modeSelect = 0;

	    }
	    else
	    {
              switch (ProtocolString[9])
              {
                  case '0': g_modeSelect = 0; break;
                  case '1': g_modeSelect = 1; break;
                  case '2': g_modeSelect = 2; break;
                  case '3': g_modeSelect = 3; break;
                  case '4': g_modeSelect = 4; break;
                  case '5': g_modeSelect = 5; break;
                  case '6': g_modeSelect = 6; break;
                  default: g_modeSelect = 0;  break;
              }

	    }
	    newLineReceived = 0;  
		memset(ProtocolString, 0x00, sizeof(ProtocolString));
	    return;
  	}


	
//	//解析上位机发来的通用协议指令,并执行相应的动作
//	//如:$1,0,0,0,0,0,0,0,0,0#    小车前进
	if (StringFind((const char *)ProtocolString, (const char *)"4WD", 0) == -1)
	{
		//小车原地左旋右旋判断
		if (ProtocolString[3] == '1')      //小车原地左旋
		{
			g_CarState = enTLEFT;
		}
		else if (ProtocolString[3] == '2') //小车原地右旋
		{
			g_CarState = enTRIGHT;
		}
		else
		{
			g_CarState = enSTOP;
		}


		//小车加减速判断
		if (ProtocolString[7] == '1')    //加速，每次加100
		{
			CarSpeedControl += 100;
			if (CarSpeedControl > 1000)
			{
				CarSpeedControl = 1000;
			}
		}
		if (ProtocolString[7] == '2')	//减速，每次减100
		{
			CarSpeedControl -= 100;
			if (CarSpeedControl < 100)
			{
				CarSpeedControl = 100;
			}
		}
                if (ProtocolString[15] == '1')    //是否灭火
                {
                    Servo_J1=1;
                    Servo_J2=0;
                }
                else
                {
                    Servo_J1=1;
                    Servo_J2=1;
                }


		//小车的前进,后退,左转,右转,停止动作
		if (g_CarState != enTLEFT && g_CarState != enTRIGHT)
		{
			switch (ProtocolString[1])
				
			{
				case run_car:   g_CarState = enRUN;  break;
				case back_car:  g_CarState = enBACK;  break;
				case left_car:  g_CarState = enLEFT; break;
				case right_car: g_CarState = enRIGHT;  break;
				case stop_car:  g_CarState = enSTOP;  break;
				default: g_CarState = enSTOP; break;
			}
		}

		newLineReceived = 0;  
		memset(ProtocolString, 0x00, sizeof(ProtocolString));  

		//根据小车状态做相应的动作
		switch (g_CarState)
		{
			case enSTOP: MiniCar_Stop(); break;
			case enRUN: MiniCar_Run(CarSpeedControl,CarSpeedControl-20); break;
			case enLEFT: MiniCar_TurnLeft(CarSpeedControl-200,CarSpeedControl); break;
			case enRIGHT:  MiniCar_TurnRight(CarSpeedControl,CarSpeedControl-200); break;
			case enBACK: MiniCar_Back(CarSpeedControl,CarSpeedControl-20); break;
			case enTLEFT: MiniCar_TTurnLeft(CarSpeedControl,CarSpeedControl); break;
			case enTRIGHT: MiniCar_TTurnRight(CarSpeedControl,CarSpeedControl); break;
			default: MiniCar_Stop(); break;
		}
	}
	

}




void serial_data_postback()
{
	//小车超声波传感器采集的信息发给上位机显示
	//打包格式如:
	//    超声波 电压  灰度  巡线  红外避障 寻光
	//$4WD,CSB120,PV8.3,GS214,LF1011,HW11,GM11#
	 float Battery_Volotage;
         float csbLen=0.0f; 
         u16 gs=0, lf=0; 
         u8 hw=0, gm=0;
	 Battery_Volotage = Get_Battery_Volotage();

	printf("$4WD,CSB%.2f,PV%1.2f,GS%d,LF%04d,HW%02d,GM%02d#",csbLen, Battery_Volotage, gs, lf, hw, gm);																				
	return;
}
void app_bluetooth_deal(void)
{
	if (newLineReceived)
   	{
		Protocol();
	}

	// 切换不同功能模式, 功能模式显示
	switch (g_modeSelect)
	{
		case 1: break; 								//暂时保留
		case 2:  break; 			//巡线模式
		case 3:   break;  	//超声波避障模式
		default:break;
	}
	
	//让小车串口平均每秒发送采集的数据给手机蓝牙apk
	//避免串口打印数据速度过快,造成apk无法正常运行
	if (g_modeSelect == 0 )
	{
		if (g_count >= 100000)
		{ 
			g_count = 0;
			serial_data_postback();
		}		
	}

}
void LED_Tri()
{
      LED=1;
    delay_ms(100);
    LED=0;
    delay_ms(100);
}

void main()
{
 
  Hard_Configure();
  Servo_GPIO_Init();
  
  while(1)
  {

    app_bluetooth_deal();
    
//    MiniCar_TTurnLeft(200);
//    delay_ms(1000);
//    MiniCar_Stop();
//    delay_ms(1000);
////    MiniCar_TurnRight(200);
////    delay_ms(1000);
////    MiniCar_Stop();
////    delay_ms(1000);
//             
  }
}