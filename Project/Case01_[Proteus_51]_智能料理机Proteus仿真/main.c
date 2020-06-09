#include <reg52.h>
#include <LCD1602.h>
#include "delay.h"
#include <key.h>
#include <DS18B20.h>
#include <PCF8591.h>
#include <HL_MCP3421.h>
sbit Beef = P2^5;
#define Beef_H	Beef = 0
#define Beef_L	Beef = 1
sbit Motor_A = P2^6;
sbit Motor_B = P2^7;
sbit Motor_C = P3^0;
sbit Motor_D = P3^1;
sbit Heater = P3^7;
sbit Key01 = P1^0;
sbit Key02 = P1^1;
sbit Key03 = P1^2;
#define Temperature_High_Value	50

void shown_Temperature(unsigned int Temperature);
void shown_Weight(float Weight);
void shown_oven(float Temperature);
void KeyScan(void);
unsigned char idata count;
unsigned char KeyNumSetCount;
long int idata NumSet;
unsigned char idata Heat_Flag,Water_Flag1,Water_Flag2,Water_Flag3,Water_Flag4;

void KeyScan(void)
{
	if(!Key01)
	{
		while(!Key01);
		Heat_Flag=!Heat_Flag;
	}
	if(!Key02)
	{
		while(!Key02);
		Water_Flag1=!Water_Flag1;	
	}
	if(!Key03)
	{
		while(!Key03);
		Water_Flag2=!Water_Flag2;	
	}
}
void INTO_Init(void)
{
	EA = 1;
	EX0 = 1;//外部中断0打开
	IT0 = 1;//边沿触发
}
unsigned int  idata Temperature;
float idata Weight,Vol,Oven;
void main(void)
{

	LCD_Init();
	INTO_Init();
	Start_I2c();
	Motor_A=0;
	Motor_B=0;
	Motor_C=0;
	Motor_D=0;
	LCD_Write_String(1,0,"Graduation Pro");
	LCD_Write_String(3,1,"Name");//客户可以自己修改名字或者学号等注意最多可以显示16个
	Temperature = ReadTemperature();//预读取温度，避免出现首次显示出错的情况
	DelayMs(255);
	DelayMs(255);
	DelayMs(255);
	DelayMs(255);
	LCD_Clear();
	while(1)
	{
		KeyPort = 0x0f;
		if(Heat_Flag)
		{
			Heater = 0;//加热
			LCD_Write_String(0,0,"H");
		}
		else
		{
			Heater = 1;
			LCD_Write_String(0,0,"N");
		}
		if(Water_Flag1)
		{
			Motor_A=1;
			Motor_B=0;
			LCD_Write_String(2,0,"W1");
		}
		else
		{
			Motor_A=0;
			Motor_B=0;	
			LCD_Write_String(2,0,"N ");
		}
		if(Water_Flag2)
		{
			Motor_C=1;
			Motor_D=0;
			LCD_Write_String(5,0,"W2");
		}
		else
		{
			Motor_C=0;
			Motor_D=0;
			LCD_Write_String(5,0,"N ");			
		}

		
		Temperature = ReadTemperature();			//读取温度值
		shown_Temperature(Temperature);
		if(Temperature>=Temperature_High_Value*10)
			Beef_H;
		else
			Beef_L;
		
		Vol = (float)(ReadADC(0x01))/255*5;		//读取电压
		Weight = Vol * 200;						//转换成重量
		shown_Weight(Weight);
		
		Oven = (float)(ReadADC(0x00))/255*5 * 20;		//读取OVEN温度
		shown_oven(Oven);							//将温度显示出来
		
	}
}

void shown_Temperature(unsigned int Temperature)
{
	unsigned int idata a,b,c,d,e;
	unsigned long int idata temp;
	temp = Temperature*10;	//将采集得到的温度扩大10倍，相当于整体扩大100倍
	
	a = temp % 10;			//分离得到最后一位，小数点后两位
	b = temp / 10 % 10;		//分离得到倒数第二位，小数点后一位
	c = temp / 100 % 10;	//分离得到倒数第三位，个位
	d = temp / 1000 % 10;	//分离得到倒数第四位，十位
	e = temp / 10000 % 10;	//分离得到倒数第五位，百位
	
	LCD_Write_Char(0,1,e+'0');
	LCD_Write_Char(1,1,d+'0');
	LCD_Write_Char(2,1,c+'0');
	LCD_Write_String(3,1,".");
	LCD_Write_Char(4,1,b+'0');
	LCD_Write_Char(6,1,'C');
	
}

void shown_Weight(float Weight)
{
	unsigned int idata a,b,c,d,e;
	unsigned long int idata temp;
	temp = (unsigned long int)(Weight*100);	//整体扩大100倍
	
	a = temp % 10;			//分离得到最后一位，小数点后两位
	b = temp / 10 % 10;		//分离得到倒数第二位，小数点后一位
	c = temp / 100 % 10;	//分离得到倒数第三位，个位
	d = temp / 1000 % 10;	//分离得到倒数第四位，十位
	e = temp / 10000 % 10;	//分离得到倒数第五位，百位
	
	LCD_Write_Char(8,1,e+'0');
	LCD_Write_Char(9,1,d+'0');
	LCD_Write_Char(10,1,c+'0');
	LCD_Write_String(11,1,".");
	LCD_Write_Char(12,1,b+'0');
	LCD_Write_Char(14,1,'g');
	
}

void shown_oven(float Oven)
{
	unsigned int idata a,b,c,d,e;
	unsigned long int idata temp;
	temp = (unsigned long int)(Oven*100);	//整体扩大100倍
	
	a = temp % 10;			//分离得到最后一位，小数点后两位
	b = temp / 10 % 10;		//分离得到倒数第二位，小数点后一位
	c = temp / 100 % 10;	//分离得到倒数第三位，个位
	d = temp / 1000 % 10;	//分离得到倒数第四位，十位
	e = temp / 10000 % 10;	//分离得到倒数第五位，百位
	
	LCD_Write_Char(8,0,e+'0');
	LCD_Write_Char(9,0,d+'0');
	LCD_Write_Char(10,0,c+'0');
	LCD_Write_String(11,0,".");
	LCD_Write_Char(12,0,b+'0');
	LCD_Write_Char(14,0,'C');

	
}



void int0_isr(void)interrupt 0 //中断0函数    
{
	KeyScan();
}
