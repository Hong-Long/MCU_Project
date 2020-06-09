#ifndef _LCD1602_H_
#define _LCD1602_H_

#include <reg52.h>
#include<intrins.h>
#include "delay.h"

/*****************************程序引脚定义***********************************************/


#define uchar unsigned char
#define uint unsigned int


/*LCD驱动函数名声明*/
void Start();
void  Write_A_Byte(unsigned char c);
void LCD_Init(void);		 //初始化函数，设置LCD的显示模式
void LCD_Write_Char(unsigned char x,unsigned char y,unsigned char Data);			  //写入字符函数
void LCD_Write_String(unsigned char x,unsigned char y,unsigned char *s);			  //写入字符串函数
void LCD_Clear(void);												 				  //清屏函数
void LCD_Write_Data(unsigned char Data);					   						  //写入数据函数
void LCD_Write_Com(unsigned char com);												  //	 写入命令函数
void LCD_PrintChar(unsigned char ucIdxX, unsigned char ucIdxY, unsigned char cData);  //向LCD中写入一个Char型数据
void LED_PrintValueI(unsigned char x, unsigned char y, int Data);					  //向LCD写入一个整形数据
void LED_PrintValueF(short int x, unsigned short int y, float ddata, unsigned short int num);//向LCD中写入一个浮点型数据


#endif