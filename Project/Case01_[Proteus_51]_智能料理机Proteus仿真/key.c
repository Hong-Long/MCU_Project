#include <key.h>
#include "LCD1602.h"

unsigned char key_value=0;
int keyenternum=0;
unsigned char KeyEnterFlag=0;
unsigned char idata ModeType;
unsigned int KeyNum=20;
void KeyScan3_3(void)
{
	unsigned char temp;//������ʱ����
	unsigned char row_value=0,line_value=0;
	KeyPort = 0x0f;                                      //����KeyPort����λΪ0������λΪ1
    temp = KeyPort;                                      //��һ�������洢KeyPort�ڵ�״̬���������Ӱ��KeyPort�ڣ��Ӷ����ɨ������
	if(temp != 0x0f)
    {
        DelayMs(10);
        if(temp!= 0x0f)                             //ȷ���м�����
        {       
            switch(temp)                            //�鿴KeyPort��temp����״̬
            {
                case 0x07 :line_value = 1; break;    //���µ�Ϊ��һ��
                case 0x0b :line_value = 2; break;    //���µ�Ϊ�ڶ���
                case 0x0d :line_value = 3; break;    //���µ�Ϊ������
				case 0x0e :line_value = 4; break;    //���µ�Ϊ������
            }	
			KeyPort = 0xf0;                                  //�ٽ� KeyPort �����¸�ֵ������λΪ1������λΪ0
			temp = KeyPort;
			switch(temp)
			{
				case 0x70 : row_value = 1 ; break;   //���µ��ǵ�һ��
				case 0xb0 : row_value = 2 ; break;	 //���µ��ǵڶ���
				case 0xd0 : row_value = 3 ; break;	 //���µ��ǵ�����
				case 0xe0 : row_value = 4 ; break;	 //���µ��ǵ�����
			}
			key_value=4*(row_value-1)+line_value;		//���ּ�⣬��Ϊ�����һ�θ�ֵ��KeyPort = 0xf0
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
		
		
