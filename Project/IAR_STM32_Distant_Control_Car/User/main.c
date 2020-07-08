
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


int g_modeSelect = 0;  //0��Ĭ��APK��λ��״̬;  1:����ң�� 2:Ѳ��ģʽ 3:���������� 4: �߲�̽�� 5: Ѱ��ģʽ 6: �������


int speed=0,run;


u8 ProtocolString[80] = {0};
int CarSpeedControl = 200;


/*С������״̬ö��*/

enum {

  enSTOP = 0,
  enRUN,
  enBACK,
  enLEFT,
  enRIGHT,
  enTLEFT,
  enTRIGHT

}enCarState;

#define 	run_car     '1'//����ǰ
#define 	back_car    '2'//������
#define 	left_car    '3'//������
#define 	right_car   '4'//������
#define 	stop_car    '0'//����ͣ

int g_CarState = enSTOP; //  1ǰ2��3��4��0ֹͣ

/**
* Function       StringFind
* @author        liusen
* @date          2017.08.18    
* @brief         �ַ�������
* @param[in]     pSrc:Դ�ַ���; pDst:���ҵ��ַ���; v_iStartPos:Դ�ַ�����ʼλ��
* @param[out]    void
* @retval        void
* @par History   ��
*/
void Servo_GPIO_Init(void)
{		
   	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*��������ʱ��*/
	RCC_APB2PeriphClockCmd(Servo_J1_RCC, ENABLE); 
	/*ѡ��Ҫ���Ƶ�����*/															   
  	GPIO_InitStructure.GPIO_Pin = Servo_J1_PIN;	
	/*��������ģʽΪͨ���������*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*������������Ϊ50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*���ÿ⺯������ʼ��PORT*/
  	GPIO_Init(Servo_J1_PORT, &GPIO_InitStructure);	
        
        	/*��������ʱ��*/
	RCC_APB2PeriphClockCmd(Servo_J2_RCC, ENABLE); 
	/*ѡ��Ҫ���Ƶ�����*/															   
  	GPIO_InitStructure.GPIO_Pin = Servo_J2_PIN;	
	/*��������ģʽΪͨ���������*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*������������Ϊ50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*���ÿ⺯������ʼ��PORT*/
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
* @brief         �������ݿ������µ�buf�з�ֹ��������б������ݸ���
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   ��
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
* @brief         Э�����
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   ��
*/
void Protocol(void)
{
	ProtocolCpyData();

	/*����ģʽ�л�*/
  	//���ж��Ƿ���ģʽѡ��	
	if(StringFind((const char *)ProtocolString, (const char *)"MODE", 0) > 0 
		&& StringFind((const char *)ProtocolString, (const char *)"4WD", 0) > 0)
	{
		
	    if (ProtocolString[10] == '0') //ֹͣģʽ
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


	
//	//������λ��������ͨ��Э��ָ��,��ִ����Ӧ�Ķ���
//	//��:$1,0,0,0,0,0,0,0,0,0#    С��ǰ��
	if (StringFind((const char *)ProtocolString, (const char *)"4WD", 0) == -1)
	{
		//С��ԭ�����������ж�
		if (ProtocolString[3] == '1')      //С��ԭ������
		{
			g_CarState = enTLEFT;
		}
		else if (ProtocolString[3] == '2') //С��ԭ������
		{
			g_CarState = enTRIGHT;
		}
		else
		{
			g_CarState = enSTOP;
		}


		//С���Ӽ����ж�
		if (ProtocolString[7] == '1')    //���٣�ÿ�μ�100
		{
			CarSpeedControl += 100;
			if (CarSpeedControl > 1000)
			{
				CarSpeedControl = 1000;
			}
		}
		if (ProtocolString[7] == '2')	//���٣�ÿ�μ�100
		{
			CarSpeedControl -= 100;
			if (CarSpeedControl < 100)
			{
				CarSpeedControl = 100;
			}
		}
                if (ProtocolString[15] == '1')    //�Ƿ����
                {
                    Servo_J1=1;
                    Servo_J2=0;
                }
                else
                {
                    Servo_J1=1;
                    Servo_J2=1;
                }


		//С����ǰ��,����,��ת,��ת,ֹͣ����
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

		//����С��״̬����Ӧ�Ķ���
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
	//С���������������ɼ�����Ϣ������λ����ʾ
	//�����ʽ��:
	//    ������ ��ѹ  �Ҷ�  Ѳ��  ������� Ѱ��
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

	// �л���ͬ����ģʽ, ����ģʽ��ʾ
	switch (g_modeSelect)
	{
		case 1: break; 								//��ʱ����
		case 2:  break; 			//Ѳ��ģʽ
		case 3:   break;  	//����������ģʽ
		default:break;
	}
	
	//��С������ƽ��ÿ�뷢�Ͳɼ������ݸ��ֻ�����apk
	//���⴮�ڴ�ӡ�����ٶȹ���,���apk�޷���������
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