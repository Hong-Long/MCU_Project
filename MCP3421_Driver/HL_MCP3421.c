#include <HL_MCP3421.h>

void IIC_Init(void)
{
  Set_SDA_OUT;
  Set_SCL_OUT;
  
  SDA_H;
  SCL_H;
  
}

void IIC_Start(void)
{
  Set_SDA_OUT;                 //Set SDA as output mode
  SDA_H;
  SCL_H;
  delay_us(10);                 
  SDA_L;                        //START:when CLK is high,DATA change form high to low 
  delay_us(10); 
  SCL_L;                        //Delay a little time, wait for receive and send data
}
void IIC_Stop(void)
{
  Set_SDA_OUT;                  //Set SDA as output mode
  SCL_L;
  SDA_L;                        //STOP:when CLK is high DATA change form low to high
  delay_us(10);
  SCL_H;
  delay_us(10);
  SDA_H;
  delay_us(10);
}

unsigned char IIC_Wait_Ack(void)
{
  unsigned char i=2;
  Set_SDA_IN;                  //Set SDA as output mode
  SDA_H;
  delay_us(10);
  SCL_H;
  delay_us(10);
  while(i--)
  {
    if(SDA_Read)
    {
      SCL_L;
      delay_us(10);
      SCL_H;
      delay_us(10);
    }
    else
      goto loop;
  }
loop:
  SCL_L;
  delay_us(10);
  return 0;
}

void IIC_Ack(void)
{
  SCL_L;
  Set_SDA_OUT;
  SDA_L;
  delay_us(10);
  SCL_H;
  delay_us(10);
  SCL_L;
}
void IIC_NAck(void)
{
  SCL_L;
  Set_SDA_OUT;
  SDA_H;
  delay_us(10);
  SCL_H;
  delay_us(10);
  SCL_L;
}

void IIC_SendByte(unsigned char txd)
{
  unsigned char t;
  Set_SDA_OUT;
  for(t=0;t<8;t++)
  {
    SCL_L;
    delay_us(10);
    if(txd&0x80)
      SDA_H;
    else
      SDA_L;
    txd<<=1;
    delay_us(10);
    SCL_H;
    delay_us(10);
  }
  SCL_L;
}

unsigned char IIC_ReadByte(void)
{
  unsigned char i,receive=0;
  Set_SDA_IN;
  for(i=0;i<8;i++)
  {
    receive<<=1;
    SCL_L;
    delay_us(10);
    SCL_H;
    delay_us(10);
    if(SDA_Read)
      receive |= 0x01;
    else
      receive &= 0xfe;
    
  }
  SCL_L;
  return receive;
}


void Write_MCP3421(unsigned char WR_Data)
{
  IIC_Start();
  IIC_SendByte(MCP3421_ADDREDD);   // 1101 a2 a1 a0 0  发送给第一个字节数据 MCP3421地址字节+R/W命令
                                    // 1101 0000 0xd0
  IIC_Wait_Ack(); 
  IIC_SendByte(WR_Data); //RDY O/C C1 C0 S1 S0 G1 G0    
                         
  IIC_Wait_Ack();
  IIC_Stop();  
}
  unsigned long int  elech;
  unsigned long int elecm;
  unsigned long int elecl;
  unsigned long int config;
  unsigned long int AD_B_Result;
  float AD_F_Result=0.0;
float  READ_MCP3421(void)
{
    float LSB;
    int PGA;
    IIC_Start();
    IIC_SendByte(0xd1);                    //0xd1=0b11010001, 最后一位1表示单片机接收数据
    IIC_Wait_Ack();                      //MCP3421发出应答ACK信号
    //读取第二个字节数据 Upper Data Byte
    elech=(long int)IIC_ReadByte();       //NULL NULL NULL NULL NULL NULL D17 D16
    IIC_Ack();                             //主器件发出应答信号
    //读取第三个字节数据 Lower Data Byte
    elecm=(long int)IIC_ReadByte();       //D15 D14 D13 D12 D11 D10 D9 D8
    
    IIC_Ack();                              ////主器件发出应答信号
    
    elecl=(long int)IIC_ReadByte();       //D7 D6 D5 D4 D3 D2 D1 D0
    IIC_Ack();
    config = (long int)(IIC_ReadByte());  //RDY C1 C0 O/C S1 S0 G1 G0
    IIC_NAck();// 停止接收
    IIC_Stop();

  //开始进行数据转化
    AD_B_Result=(elech<<16)|(elecm<<8)|(elecl); //将三个数据进行整合，整合成一个24位的数据
  
    AD_B_Result=AD_B_Result&0x01ffffff;          //由于数据elech的前6个字节没有用，所以将其清零
    LSB = (float)(2.0 * 2.048) / 262144;
    PGA = 1;
    AD_F_Result = (float)(LSB * AD_B_Result)/PGA;
//    AD_F_Result=(float)(((float)(2.048/131072)*AD_B_Result)/2);
    return AD_F_Result;
 }




