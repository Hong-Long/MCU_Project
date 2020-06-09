#ifndef _LCD1602_H_
#define _LCD1602_H_

#include <reg52.h>
#include<intrins.h>
#include "delay.h"

/*****************************�������Ŷ���***********************************************/


#define uchar unsigned char
#define uint unsigned int


/*LCD��������������*/
void Start();
void  Write_A_Byte(unsigned char c);
void LCD_Init(void);		 //��ʼ������������LCD����ʾģʽ
void LCD_Write_Char(unsigned char x,unsigned char y,unsigned char Data);			  //д���ַ�����
void LCD_Write_String(unsigned char x,unsigned char y,unsigned char *s);			  //д���ַ�������
void LCD_Clear(void);												 				  //��������
void LCD_Write_Data(unsigned char Data);					   						  //д�����ݺ���
void LCD_Write_Com(unsigned char com);												  //	 д�������
void LCD_PrintChar(unsigned char ucIdxX, unsigned char ucIdxY, unsigned char cData);  //��LCD��д��һ��Char������
void LED_PrintValueI(unsigned char x, unsigned char y, int Data);					  //��LCDд��һ����������
void LED_PrintValueF(short int x, unsigned short int y, float ddata, unsigned short int num);//��LCD��д��һ������������


#endif