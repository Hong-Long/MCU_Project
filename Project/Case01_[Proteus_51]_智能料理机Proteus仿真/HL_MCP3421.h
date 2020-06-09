#ifndef _HL_MCP3421_H_
#define _HL_MCP3421_H_
#include <reg52.h>       //�Ӿ��嵥Ƭ���ͺ����ò�ͬͷ�ļ�
#include <string.h>
#include <stdio.h>
#include "delay.h"
#include <intrins.h>

// define iic port
sbit MCP3421_SDA=P2^5;
sbit MCP3421_SCL=P2^6;

  
#define SDA_H                           MCP3421_SDA=1     //����SDA�����״̬Ϊ�ߵ�ƽ
#define SDA_L                           MCP3421_SDA=0    //����SDA�����״̬Ϊ�͵�ƽ
#define SDA_Read                        (MCP3421_SDA)


#define SCL_H                           MCP3421_SCL=1     //����SCL�����״̬Ϊ�ߵ�ƽ
#define SCL_L                           MCP3421_SCL=0    //����SCL�����״̬Ϊ�͵�ƽ


//RDY O/C C1 C0 S1 S0 G1 G0    
//����ת��ģʽ��,
//RDY  1   //��ʼ�µ�ת��
//C1 C0 ��Ч����00����
//O/C   0 ����ת��ģʽ
//S1    S0 : 11  18bit
//G1    G0: 11   x8
//10001111
#define MCP3421_ADDRESS_01					0xd0	//1101 000 0
#define MCP3421_OneShot_18BIT_X8        	0x8f
#define MCP3421_Read_Configura				0xd1	//1101 A2 A1 A0 1
													//1101 000 1 


#define MCP3421_ADDRESS_02					0xd2	//1101 001 0
#define MCP3421_Read_Configura_02			0xd3	//1101 A2 A1 A0 1
													// 1101 0011

#define MCP3421_ADDRESS_03					0xd4	//1101 010 0
#define MCP3421_Read_Configura_03			0xd5	//1101 A2 A1 A0 1
													// 1101 0101

#define MCP3421_ADDRESS_04					0XD6	//1101 	011 0 0XD6
#define MCP3421_Read_Configura_04			0xd7	//1101  011 1 0xd7

extern  long int  elech;
extern  long int elecm;
extern  long int elecl;
extern  long int config;
extern  long int AD_B_Result;
extern float AD_F_Result;


void IIC_Init(void);
void IIC_Start(void);
void IIC_Stop(void);
unsigned char IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);
void IIC_SendByte(unsigned char txd);
unsigned char IIC_ReadByte(void);
void Write_MCP3421(unsigned char address,unsigned char WR_Data);
float  READ_MCP3421(unsigned char Configure);

#endif 
