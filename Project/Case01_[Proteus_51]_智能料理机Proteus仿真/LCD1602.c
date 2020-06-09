#include <LCD1602.h>
/*����LCD�����ź���*/
sbit SCL = P2^0;
sbit SDA = P2^1;
bit ack;
unsigned char LCD_data=0x08;



//*****************��ʱ************************
void delay_nus(unsigned int n) //N us��ʱ����
{
	unsigned int i=0;
		for (i=0;i<n;i++)
			_nop_();
}
void delay_nms(unsigned int n) //N ms��ʱ����
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
//MS��ʱ����(12M�����²���)
//****************************************************
void Delay_ms(unsigned int n)
{
	unsigned int  i,j;
	for(i=0;i<n;i++)
		for(j=0;j<123;j++);
}

void nop4()
{
	 _nop_();     //�ȴ�һ����������
	 _nop_();     //�ȴ�һ����������
	 _nop_();     //�ȴ�һ����������
	 _nop_();     //�ȴ�һ����������
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
	nop4();//>4us��SCL����
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
  for(BitCnt=0;BitCnt<8;BitCnt++)  //Ҫ���͵����ݳ���Ϊ8λ
    {
     if((c<<BitCnt)&0x80)  SDA=1;   //�жϷ���λ
     else  SDA=0;                
     _nop_();
     SCL=1;               //��ʱ����Ϊ�ߣ�֪ͨ��������ʼ��������λ
     nop4(); 
     _nop_();       
     SCL=0; 
    }  
    _nop_();
    _nop_();
    SDA=1;               //8λ��������ͷ������ߣ�׼������Ӧ��λ
    _nop_();
    _nop_();  
    SCL=1;
    _nop_();
    _nop_();
    _nop_();
    if(SDA==1)ack=0;     
       else 
	   {
	   ack=1;        //�ж��Ƿ���յ�Ӧ���ź�
	  // P1=0x00;
	   }	 	   	   
    SCL=0;
    _nop_();
    _nop_();
}

bit Write_Random_Address_Byte(unsigned char add,unsigned char dat)
{
 	Start();    //��������
	Write_A_Byte(add<<1|0); //����������ַ
    if(ack==0)return(0);
	Write_A_Byte(dat);   //��������
    if(ack==0)return(0);
	Stop(); //��������
    return(1);
}
//********************Һ����ʹ��*********************
void Enable_LCD_write()
{
    LCD_data|=(1<<(3-1));//E=1;
	Write_Random_Address_Byte(0x27,LCD_data);  //
	delay_nus(2);
    LCD_data&=~(1<<(3-1));//E=0;
    Write_Random_Address_Byte(0x27,LCD_data);
}

//*************д����****************************
void LCD_write_command(unsigned char command)
{
	delay_nus(16);
	LCD_data&=~(1<<(1-1));//RS=0;
	LCD_data&=~(1<<(2-1));//RW=0;
	//LCD_data&=~(1<<(4-1));
    Write_Random_Address_Byte(0x27,LCD_data);

	LCD_data&=0X0f; //�����λ
	LCD_data|=command & 0xf0; //д����λ
    Write_Random_Address_Byte(0x27,LCD_data);
    Enable_LCD_write();

	command=command<<4; //����λ�Ƶ�����λ
	LCD_data&=0x0f; //�����λ
	LCD_data|=command&0xf0; //д����λ
    Write_Random_Address_Byte(0x27,LCD_data);
    Enable_LCD_write();
}
//*************д����****************************
void LCD_write_data(unsigned char value) 
{
	delay_nus(16);
	LCD_data|=(1<<(1-1));//RS=1;
	LCD_data&=~(1<<(2-1));//RW=0;
    Write_Random_Address_Byte(0x27,LCD_data);

	LCD_data&=0X0f; //�����λ
	LCD_data|=value&0xf0; //д����λ
    Write_Random_Address_Byte(0x27,LCD_data);
    Enable_LCD_write();			

	value=value<<4; //����λ�Ƶ�����λ
	LCD_data&=0x0f; //�����λ
	LCD_data|=value&0xf0; //д����λ
    Write_Random_Address_Byte(0x27,LCD_data);
    Enable_LCD_write();
}

//**********************������ʾλ��*********************************
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
*��������  void LCD_Clear(void) 
*��������
*����˵������������
****************************************************/
void LCD_Clear(void) 
{ 
	 LCD_write_command(0x01); 
	 DelayMs(5);
}

/***************************************************
*��������void LCD_Write_String(unsigned char x,unsigned char y,unsigned char *s) 
*������char x�����������ã�char y�����������ã�char *s�ַ�������ָ��
*����˵������LCD��д���ַ���
****************************************************/
void LCD_Write_String(unsigned char x,unsigned char y,unsigned char *s) 
{     
 if (y == 0) 
 	{     
	 LCD_write_command(0x80 + x);     //��ʾ��һ��
 	}
 else 
 	{      
 	LCD_write_command(0xC0 + x);      //��ʾ�ڶ���
 	}        
 while (*s) 
 	{     
 LCD_write_data( *s);     
 s ++;     
 	}
}

/***************************************************
*��������void LCD_Write_Char(unsigned char x,unsigned char y,unsigned char Data)
*������char x�����������ã�char y�����������ã�char Data�ַ�����
*����˵������LCD��д���ַ�
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
    if(Data>=0&&Data<=9)//�������Ϊ����ʱ
      LCD_write_data( Data+48);
    else//ΪANSALL
      LCD_write_data( Data);
}
/***************************************************
*��������void LCD_Init(void)
*��������
*����˵������ʼ��LCDҺ�������û������޸ĸú����еĲ�����ʵ���Լ�����Ҫ�Ĺ���
****************************************************/
//*************Һ����ʼ��****************************
void LCD_Init(void) 
{ 
	LCD_write_command(0x28);
	delay_nus(40); 
	LCD_write_command(0x28);
	delay_nus(40); 
    Enable_LCD_write();
	delay_nus(40);
	LCD_write_command(0x28); //4λ��ʾ������������������������������������
	LCD_write_command(0x0c); //��ʾ��
	LCD_write_command(0x01); //����
	delay_nms(2);
}


/***************************************************
*��������void LED_PrintValueFP(unsigned char x, unsigned char y, short int Data, short int num)
*������char x�����������ã�char y�����������ã�int Data�ַ�����;int num��ʾС����λ����
*����˵������LCD��д��һ��С��
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
 *������:void LED_PrintValueF(uchar x, uchar y, float data, uchar num)
 *����  :��һ��float����ת������������5λ��С���ͷ��ŵ����ݲ�������ʾ
 *����  :x�ķ�ΧΪ0��62��yΪҳ�ķ�Χ0��7��dataΪ��Ҫת����ʾ����ֵ�����������λ5λ  num��ʾ������С��λ0~4
****************************************************/
void LED_PrintValueF(short int x, unsigned short int y, float ddata, unsigned short int num)
{
 	unsigned short int l,m,i,j,k;  //��ǧ��ʮ��
 	unsigned short int databiti = 6; //����λ��
 	unsigned int  tempdataui = 0;
  	int tempdataii = (int)ddata; //��������
 	long int tempdatalp = (long int)((ddata - (int)ddata)*10000); //ȡС��λ��4λ
 	
 	//����������ʾ
 	if(ddata < 0.0000001)  LCD_Write_Char(x, y,'-'); 
 	else LCD_Write_Char(x, y,'+');
	if(tempdataii < 0)tempdataii = - tempdataii;  //ȥ���������ָ���
	tempdataui = tempdataii;
 	l  = tempdataui/10000;
	m= (tempdataui%10000)/1000;
	i = (tempdataui%1000)/100;
	j = (tempdataui%100)/10;
	k = tempdataui%10;
 	if (l != 0)  //��λ
 	{
 		LCD_Write_Char(x+1,y,l+48);
 		LCD_Write_Char(x+2,y,m+48);
		LCD_Write_Char(x+3,y,i+48);
		LCD_Write_Char(x+4,y,j+48);
		LCD_Write_Char(x+5,y,k+48);
 	}
 	else if(m != 0) //��λ
 	{
 		databiti = 5;
 		LCD_Write_Char(x+1,y,m+48);
 		LCD_Write_Char(x+2,y,i+48);
		LCD_Write_Char(x+3,y,j+48);
		LCD_Write_Char(x+4,y,k+48);
 	}
  	else if(i != 0) //��λ
  	{
  		databiti = 4;
  	 	LCD_Write_Char(x+1,y,i+48);
 		LCD_Write_Char(x+2,y,j+48);
		LCD_Write_Char(x+3,y,k+48);
  	}
  	else if(j != 0) //��λ
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
 	if(tempdatalp < 0)tempdatalp = - tempdatalp;	//ȥ��С�����ָ���
	switch(num)
	{
		case 0: break;
		case 1:  LED_PrintValueFP(x + databiti * 1, y, (unsigned int)(tempdatalp / 1000),num);break;
		case 2:  LED_PrintValueFP(x + databiti * 1, y, (unsigned int)(tempdatalp / 100),num);break;
		case 3:  LED_PrintValueFP(x + databiti * 1, y, (unsigned int)(tempdatalp / 10),num);break;
		case 4:  LED_PrintValueFP(x + databiti * 1, y, (unsigned int)(tempdatalp),num);break;					
	}
}
