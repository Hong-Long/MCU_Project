#include <key.h>
#include "LCD1602.h"

unsigned char key_value=0;
int keyenternum=0;
unsigned char KeyEnterFlag=0;
unsigned char idata ModeType;
unsigned int KeyNum=20;
void KeyScan3_3(void)
{
	unsigned char temp;//定义临时变量
	unsigned char row_value=0,line_value=0;
	KeyPort = 0x0f;                                      //设置KeyPort高四位为0，第四位为1
    temp = KeyPort;                                      //用一个变量存储KeyPort口的状态，以免外界影响KeyPort口，从而造成扫描有误
	if(temp != 0x0f)
    {
        DelayMs(10);
        if(temp!= 0x0f)                             //确定有键按下
        {       
            switch(temp)                            //查看KeyPort（temp）的状态
            {
                case 0x07 :line_value = 1; break;    //按下的为第一列
                case 0x0b :line_value = 2; break;    //按下的为第二列
                case 0x0d :line_value = 3; break;    //按下的为第三列
				case 0x0e :line_value = 4; break;    //按下的为第四列
            }	
			KeyPort = 0xf0;                                  //再将 KeyPort 口重新赋值，高四位为1，第四位为0
			temp = KeyPort;
			switch(temp)
			{
				case 0x70 : row_value = 1 ; break;   //按下的是第一行
				case 0xb0 : row_value = 2 ; break;	 //按下的是第二行
				case 0xd0 : row_value = 3 ; break;	 //按下的是第三行
				case 0xe0 : row_value = 4 ; break;	 //按下的是第三行
			}
			key_value=4*(row_value-1)+line_value;		//松手检测，因为最近的一次赋值是KeyPort = 0xf0
			switch(key_value)
			{
				case 1:KeyNum=1;break;
				case 2:KeyNum=2;break;
				case 3:KeyNum=3;break;
				case 4:KeyNum=4;break;
				case 5:KeyNum=5;break;
				case 6:KeyNum=6;break;
				case 7:KeyNum=7;break;
				case 8:
				{
					KeyNum=8;
					
					break;
				}
				case 9:KeyNum=9;break;
				case 10:KeyNum=10;break;
				case 11:KeyNum=11;break;
				case 12:break;
				case 13:KeyNum=13;break;
				case 14:KeyNum=14;break;
				case 15:KeyNum=15;break;
				case 16:break;
				default:break;
			}
			while(KeyPort != 0xf0);  
		}   
   }
	
}
		
		
