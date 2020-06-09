#include <PCF8591.h>

#define  _Nop()  _nop_()  //定义空指令
                         
bit ack1;	              //应答标志位

sbit SDA1 = P2^4;
sbit SCL1 = P2^3;

#define AddWr 0x90   //写数据地址 
#define AddRd 0x91   //读数据地址

/*------------------------------------------------
                    启动总线
------------------------------------------------*/
void Start_I2c()
{
  SDA1=1;   //发送起始条件的数据信号
  _Nop();
  SCL1=1;
  _Nop();    //起始条件建立时间大于4.7us,延时
  _Nop();
  _Nop();
  _Nop();
  _Nop();    
  SDA1=0;     //发送起始信号
  _Nop();    //起始条件锁定时间大于4μ
  _Nop();
  _Nop();
  _Nop();
  _Nop();       
  SCL1=0;    //钳住I2C总线，准备发送或接收数据
  _Nop();
  _Nop();
}
/*------------------------------------------------
                    结束总线
------------------------------------------------*/
void Stop_I2c()
{
  SDA1=0;    //发送结束条件的数据信号
  _Nop();   //发送结束条件的时钟信号
  SCL1=1;    //结束条件建立时间大于4μ
  _Nop();
  _Nop();
  _Nop();
  _Nop();
  _Nop();
  SDA1=1;    //发送I2C总线结束信号
  _Nop();
  _Nop();
  _Nop();
  _Nop();
}




/*----------------------------------------------------------------
                 字节数据传送函数               
函数原型: void  SendByte(unsigned char c);
功能:  将数据c发送出去,可以是地址,也可以是数据,发完后等待应答,并对
     此状态位进行操作.(不应答或非应答都使ack1=0 假)     
     发送数据正常，ack1=1; ack1=0表示被控器无应答或损坏。
------------------------------------------------------------------*/
void  SendByte(unsigned char c)
{
 unsigned char BitCnt;
 
 for(BitCnt=0;BitCnt<8;BitCnt++)  //要传送的数据长度为8位
    {
     if((c<<BitCnt)&0x80)SDA1=1;   //判断发送位
       else  SDA1=0;                
     _Nop();
     SCL1=1;               //置时钟线为高，通知被控器开始接收数据位
      _Nop(); 
      _Nop();             //保证时钟高电平周期大于4μ
      _Nop();
      _Nop();
      _Nop();         
     SCL1=0; 
    }
    
    _Nop();
    _Nop();
    SDA1=1;               //8位发送完后释放数据线，准备接收应答位
    _Nop();
    _Nop();   
    SCL1=1;
    _Nop();
    _Nop();
    _Nop();
    if(SDA1==1)ack1=0;     
       else ack1=1;        //判断是否接收到应答信号
    SCL1=0;
    _Nop();
    _Nop();
}







/*----------------------------------------------------------------
                 字节数据传送函数               
函数原型: unsigned char  RcvByte();
功能:  用来接收从器件传来的数据,并判断总线错误(不发应答信号)，
     发完后请用应答函数。  
------------------------------------------------------------------*/	
unsigned char  RcvByte()
{
  unsigned char retc;
  unsigned char BitCnt;
  
  retc=0; 
  SDA1=1;             //置数据线为输入方式
  for(BitCnt=0;BitCnt<8;BitCnt++)
      {
        _Nop();           
        SCL1=0;       //置时钟线为低，准备接收数据位
        _Nop();
        _Nop();      //时钟低电平周期大于4.7us
        _Nop();
        _Nop();
        _Nop();
        SCL1=1;       //置时钟线为高使数据线上数据有效
        _Nop();
        _Nop();
        retc=retc<<1;
        if(SDA1==1)retc=retc+1; //读数据位,接收的数据位放入retc中
        _Nop();
        _Nop(); 
      }
  SCL1=0;    
  _Nop();
  _Nop();
  return(retc);
}
/*----------------------------------------------------------------
                     非应答子函数
原型:  void Noack1_I2c(void);
 
----------------------------------------------------------------*/
void Noack1_I2c(void)
{
  
  SDA1=1;
  _Nop();
  _Nop();
  _Nop();      
  SCL1=1;
  _Nop();
  _Nop();              //时钟低电平周期大于4μ
  _Nop();
  _Nop();
  _Nop();  
  SCL1=0;                //清时钟线，钳住I2C总线以便继续接收
  _Nop();
  _Nop();    
}

/*------------------------------------------------
             读AD转值程序
输入参数 Chl 表示需要转换的通道，范围从0-3
返回值范围0-255
------------------------------------------------*/
unsigned char ReadADC(unsigned char Chl)
 {
   unsigned char Val;
   Start_I2c();               //启动总线
   SendByte(AddWr);             //发送器件地址
     if(ack1==0)return(0);
   SendByte(0x40|Chl);            //发送器件子地址
     if(ack1==0)return(0);
   Start_I2c();
   SendByte(AddWr+1);
      if(ack1==0)return(0);
   Val=RcvByte();
   Noack1_I2c();                 //发送非应位
   Stop_I2c();                  //结束总线
  return(Val);
 }
 