#include <LCD1602.h>
/*定义LCD控制信号线*/
sbit SCL = P2^0;
sbit SDA = P2^1;
bit ack;
unsigned char LCD_data=0x08;



//*****************延时************************
void delay_nus(unsigned int n) //N us延时函数
{
	unsigned int i=0;
		for (i=0;i<n;i++)
			_nop_();
}
void delay_nms(unsigned int n) //N ms延时函数
{
	unsigned int i,j;
		for (i=0;i<n;i++)
			for (j=0;j<1140;j++);
}
void delay() 
{ 
	int i,j; 
	for(i=0;i<255;i++) 
	for(j=0;j<120;j++); 
}

//****************************************************
//MS延时函数(12M晶振下测试)
//****************************************************
void Delay_ms(unsigned int n)
{
	unsigned int  i,j;
	for(i=0;i<n;i++)
		for(j=0;j<123;j++);
}

void nop4()
{
	 _nop_();     //等待一个机器周期
	 _nop_();     //等待一个机器周期
	 _nop_();     //等待一个机器周期
	 _nop_();     //等待一个机器周期
}
//***************************************
void Start()
{
 	SDA=1;
    _nop_();
    SCL=1;
	nop4();
    SDA=0;
	nop4();
    SCL=0;
    _nop_();
	_nop_();
}
void Stop()
{
 	SDA=0;
    _nop_();
	SCL=0;
	nop4();//>4us后SCL跳变
	SCL=1;
	nop4();
	SDA=1;
    _nop_();
    _nop_();
}
//******************************************
void  Write_A_Byte(unsigned char c)
{
 unsigned char BitCnt;
  for(BitCnt=0;BitCnt<8;BitCnt++)  //要传送的数据长度为8位
    {
     if((c<<BitCnt)&0x80)  SDA=1;   //判断发送位
     else  SDA=0;                
     _nop_();
     SCL=1;               //置时钟线为高，通知被控器开始接收数据位
     nop4(); 
     _nop_();       
     SCL=0; 
    }  
    _nop_();
    _nop_();
    SDA=1;               //8位发送完后释放数据线，准备接收应答位
    _nop_();
    _nop_();  
    SCL=1;
    _nop_();
    _nop_();
    _nop_();
    if(SDA==1)ack=0;     
       else 
	   {
	   ack=1;        //判断是否接收到应答信号
	  // P1=0x00;
	   }	 	   	   
    SCL=0;
    _nop_();
    _nop_();
}

bit Write_Random_Address_Byte(unsigned char add,unsigned char dat)
{
 	Start();    //启动总线
	Write_A_Byte(add<<1|0); //发送器件地址
    if(ack==0)return(0);
	Write_A_Byte(dat);   //发送数据
    if(ack==0)return(0);
	Stop(); //结束总线
    return(1);
}
//********************液晶屏使能*********************
void Enable_LCD_write()
{
    LCD_data|=(1<<(3-1));//E=1;
	Write_Random_Address_Byte(0x27,LCD_data);  //
	delay_nus(2);
    LCD_data&=~(1<<(3-1));//E=0;
    Write_Random_Address_Byte(0x27,LCD_data);
}

//*************写命令****************************
void LCD_write_command(unsigned char command)
{
	delay_nus(16);
	LCD_data&=~(1<<(1-1));//RS=0;
	LCD_data&=~(1<<(2-1));//RW=0;
	//LCD_data&=~(1<<(4-1));
    Write_Random_Address_Byte(0x27,LCD_data);

	LCD_data&=0X0f; //清高四位
	LCD_data|=command & 0xf0; //写高四位
    Write_Random_Address_Byte(0x27,LCD_data);
    Enable_LCD_write();

	command=command<<4; //低四位移到高四位
	LCD_data&=0x0f; //清高四位
	LCD_data|=command&0xf0; //写低四位
    Write_Random_Address_Byte(0x27,LCD_data);
    Enable_LCD_write();
}
//*************写数据****************************
void LCD_write_data(unsigned char value) 
{
	delay_nus(16);
	LCD_data|=(1<<(1-1));//RS=1;
	LCD_data&=~(1<<(2-1));//RW=0;
    Write_Random_Address_Byte(0x27,LCD_data);

	LCD_data&=0X0f; //清高四位
	LCD_data|=value&0xf0; //写高四位
    Write_Random_Address_Byte(0x27,LCD_data);
    Enable_LCD_write();			

	value=value<<4; //低四位移到高四位
	LCD_data&=0x0f; //清高四位
	LCD_data|=value&0xf0; //写低四位
    Write_Random_Address_Byte(0x27,LCD_data);
    Enable_LCD_write();
}

//**********************设置显示位置*********************************
void set_position(unsigned char x,unsigned char y)
{
	unsigned char position;
	if (y == 0)
        position = 0x80 + x;
	else 
		position = 0xc0 + x;
    LCD_write_command(position);
}
/***************************************************
*函数名：  void LCD_Clear(void) 
*参数：无
*功能说明：清屏函数
****************************************************/
void LCD_Clear(void) 
{ 
	 LCD_write_command(0x01); 
	 DelayMs(5);
}

/***************************************************
*函数名：void LCD_Write_String(unsigned char x,unsigned char y,unsigned char *s) 
*参数：char x：横坐标设置；char y：纵坐标设置；char *s字符型数组指针
*功能说明：向LCD中写入字符串
****************************************************/
void LCD_Write_String(unsigned char x,unsigned char y,unsigned char *s) 
{     
 if (y == 0) 
 	{     
	 LCD_write_command(0x80 + x);     //表示第一行
 	}
 else 
 	{      
 	LCD_write_command(0xC0 + x);      //表示第二行
 	}        
 while (*s) 
 	{     
 LCD_write_data( *s);     
 s ++;     
 	}
}

/***************************************************
*函数名：void LCD_Write_Char(unsigned char x,unsigned char y,unsigned char Data)
*参数：char x：横坐标设置；char y：纵坐标设置；char Data字符变量
*功能说明：向LCD中写入字符
****************************************************/
void LCD_Write_Char(unsigned char x,unsigned char y,unsigned char Data) 
{     
 if (y == 0) 
 	{     
 	LCD_write_command(0x80 + x);     
 	}    
 else 
 	{     
 	LCD_write_command(0xC0 + x);     
 	}   
    if(Data>=0&&Data<=9)//当输入的为数字时
      LCD_write_data( Data+48);
    else//为ANSALL
      LCD_write_data( Data);
}
/***************************************************
*函数名：void LCD_Init(void)
*参数：无
*功能说明：初始化LCD液晶屏，用户可以修改该函数中的参数来实现自己所需要的功能
****************************************************/
//*************液晶初始化****************************
void LCD_Init(void) 
{ 
	LCD_write_command(0x28);
	delay_nus(40); 
	LCD_write_command(0x28);
	delay_nus(40); 
    Enable_LCD_write();
	delay_nus(40);
	LCD_write_command(0x28); //4位显示！！！！！！！！！！！！！！！！！！
	LCD_write_command(0x0c); //显示开
	LCD_write_command(0x01); //清屏
	delay_nms(2);
}


/***************************************************
*函数名：void LED_PrintValueFP(unsigned char x, unsigned char y, short int Data, short int num)
*参数：char x：横坐标设置；char y：纵坐标设置；int Data字符变量;int num显示小数点位数；
*功能说明：向LCD中写入一个小数
****************************************************/
void LED_PrintValueFP(unsigned char x, unsigned char y, short int Data, short int num)
 {
 	short int m,i,j,k;  	
 	LCD_Write_Char(x, y, '.');
	m= Data/1000;
	i = (Data%1000)/100;
	j = (Data%100)/10;
	k = Data%10;
	switch(num)
	{
		case 1:  	LCD_Write_Char(x+1,y,k+48);
				break;
		case 2:  	LCD_Write_Char(x+1,y,j+48);
				LCD_Write_Char(x+2,y,k+48);
				break;
		case 3:	LCD_Write_Char(x+1,y,i+48);
				LCD_Write_Char(x+2,y,j+48);
				LCD_Write_Char(x+3,y,k+48);
				break;
		case 4: 	LCD_Write_Char(x+1,y,m+48);
				LCD_Write_Char(x+2,y,i+48);
				LCD_Write_Char(x+3,y,j+48);
				LCD_Write_Char(x+4,y,k+48);
				break;	
	}
 }
 
/***************************************************
 *函数名:void LED_PrintValueF(uchar x, uchar y, float data, uchar num)
 *功能  :将一个float型数转换成整数部分5位带小数和符号的数据并进行显示
 *参数  :x的范围为0～62，y为页的范围0～7，data为需要转化显示的数值整数部分最多位5位  num表示保留的小数位0~4
****************************************************/
void LED_PrintValueF(short int x, unsigned short int y, float ddata, unsigned short int num)
{
 	unsigned short int l,m,i,j,k;  //万千百十个
 	unsigned short int databiti = 6; //整数位数
 	unsigned int  tempdataui = 0;
  	int tempdataii = (int)ddata; //整数部分
 	long int tempdatalp = (long int)((ddata - (int)ddata)*10000); //取小数位后4位
 	
 	//整数部分显示
 	if(ddata < 0.0000001)  LCD_Write_Char(x, y,'-'); 
 	else LCD_Write_Char(x, y,'+');
	if(tempdataii < 0)tempdataii = - tempdataii;  //去掉整数部分负号
	tempdataui = tempdataii;
 	l  = tempdataui/10000;
	m= (tempdataui%10000)/1000;
	i = (tempdataui%1000)/100;
	j = (tempdataui%100)/10;
	k = tempdataui%10;
 	if (l != 0)  //五位
 	{
 		LCD_Write_Char(x+1,y,l+48);
 		LCD_Write_Char(x+2,y,m+48);
		LCD_Write_Char(x+3,y,i+48);
		LCD_Write_Char(x+4,y,j+48);
		LCD_Write_Char(x+5,y,k+48);
 	}
 	else if(m != 0) //四位
 	{
 		databiti = 5;
 		LCD_Write_Char(x+1,y,m+48);
 		LCD_Write_Char(x+2,y,i+48);
		LCD_Write_Char(x+3,y,j+48);
		LCD_Write_Char(x+4,y,k+48);
 	}
  	else if(i != 0) //三位
  	{
  		databiti = 4;
  	 	LCD_Write_Char(x+1,y,i+48);
 		LCD_Write_Char(x+2,y,j+48);
		LCD_Write_Char(x+3,y,k+48);
  	}
  	else if(j != 0) //两位
  	{
    		databiti = 3;	
  		LCD_Write_Char(x+1,y,j+48);
 		LCD_Write_Char(x+2,y,k+48);
  	}
	else 	
	{
		databiti = 2;
		LCD_Write_Char(x+1,y,k+48);
	}	
 	if(tempdatalp < 0)tempdatalp = - tempdatalp;	//去掉小数部分负号
	switch(num)
	{
		case 0: break;
		case 1:  LED_PrintValueFP(x + databiti * 1, y, (unsigned int)(tempdatalp / 1000),num);break;
		case 2:  LED_PrintValueFP(x + databiti * 1, y, (unsigned int)(tempdatalp / 100),num);break;
		case 3:  LED_PrintValueFP(x + databiti * 1, y, (unsigned int)(tempdatalp / 10),num);break;
		case 4:  LED_PrintValueFP(x + databiti * 1, y, (unsigned int)(tempdatalp),num);break;					
	}
}
