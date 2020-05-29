#ifndef _HL_MCP3421_H_
#define _HL_MCP3421_H_
#include <MSP430x24x.h>        //视具体单片机型号引用不同头文件
#include <string.h>
#include <stdio.h>
#define CPU_F ((double)8000000)
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))


// define iic port
#define Set_SDA_OUT                     P3DIR |= BIT3
#define Set_SDA_IN                      P3DIR &= ~BIT3
  
#define SDA_H                           P3OUT |= BIT3     //配置SDA的输出状态为高电平
#define SDA_L                           P3OUT &= ~BIT3     //配置SDA的输出状态为低电平
#define SDA_Read                        (P3IN&BIT3)




#define Set_SCL_OUT                     P3DIR |= BIT2

#define Set_SCL_IN                      P3DIR&= ~BIT2


#define SCL_H                           P3OUT |=BIT2     //配置SCL的输出状态为高电平
#define SCL_L                           P3OUT &=~BIT2    //配置SCL的输出状态为低电平
#define MCP3421_ADDREDD                 0xd0
//RDY O/C C1 C0 S1 S0 G1 G0    
//单次转化模式下,
//RDY  1   //开始新的转换
//C1 C0 无效，给00即可
//O/C   0 单次转化模式
//S1    S0 : 11  18bit
//G1    G0: 00   x1
//10001100
#define MCP3421_OneShot_18BIT_X1        0x8c


extern unsigned long int  elech;
extern unsigned long int elecm;
extern unsigned long int elecl;
extern unsigned long int config;
extern unsigned long int AD_B_Result;
extern float AD_F_Result;


void IIC_Init(void);
void IIC_Start(void);
void IIC_Stop(void);
unsigned char IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);
void IIC_SendByte(unsigned char txd);
unsigned char IIC_ReadByte(void);
void Write_MCP3421(unsigned char WR_Data);
float  READ_MCP3421(void);

#endif 
