
#include "ov7725.h"
#include "ov7725config.h"
#include "delay.h"
#include "UART.h"

const u8 ov7725_init_reg_YUV[][2]= 
{   
  {CLKRC,     0x00}, /*clock config*/
	{COM7,      0x40}, /*YUV*/
  {HSTART,    0x3f},
	{HSIZE,     0x50},
	{VSTRT,     0x03},
	{VSIZE,     0x78},
	{HREF,      0x00},
	{HOutSize,  0x50},
	{VOutSize,  0x78},
	{EXHCH,     0x00},

	/*DSP control*/
	{TGT_B,     0x7f},
	{FixGain,   0x09},
	{AWB_Ctrl0, 0xe0},
	{DSP_Ctrl1, 0xff},
	{DSP_Ctrl2, 0x20},
	{DSP_Ctrl3,	0x00},
	{DSP_Ctrl4, 0x00},

	/*AGC AEC AWB*/
	{COM8,		  0xf0},
//	{COM4,		  0x81}, /*Pll AEC CONFIG*/
	{COM4,		  0xC1},
	{COM6,		  0xc5},
	{COM9,		  0x21},
	{BDBase,	  0xFF},
	{BDMStep,	  0x01},
	{AEW,		    0x34},
	{AEB,		    0x3c},
	{VPT,		    0xa1},
	{EXHCL,		  0x00},
	{AWBCtrl3,  0xaa},
	{COM8,		  0xff},
	{AWBCtrl1,  0x5d},

	{EDGE1,		  0x0a},
	{DNSOff,	  0x01},
	{EDGE2,		  0x01},
	{EDGE3,		  0x01},

	{MTX1,		  0x5f},
	{MTX2,		  0x53},
	{MTX3,		  0x11},
	{MTX4,		  0x1a},
	{MTX5,		  0x3d},
	{MTX6,		  0x5a},
	{MTX_Ctrl,  0x1e},

	{BRIGHT,	  0x00},
	{CNST,		  0x25},
	{USAT,		  0x65},
	{VSAT,		  0x65},
	{UVADJ0,	  0x81},
	{SDE,		    0x06},
	
    /*GAMMA config*/
	{GAM1,		  0x0c},
	{GAM2,		  0x16},
	{GAM3,		  0x2a},
	{GAM4,		  0x4e},
	{GAM5,		  0x61},
	{GAM6,		  0x6f},
	{GAM7,		  0x7b},
	{GAM8,		  0x86},
	{GAM9,		  0x8e},
	{GAM10,		  0x97},
	{GAM11,		  0xa4},
	{GAM12,		  0xaf},
	{GAM13,		  0xc5},
	{GAM14,		  0xd7},
	{GAM15,		  0xe8},
	{SLOP,		  0x20},

	{HUECOS,	  0x80},
	{HUESIN,	  0x80},
	{DSPAuto,	  0xff},
	{DM_LNL,	  0x00},
	{BDBase,	  0x99},
	{BDMStep,	  0x03},
	{LC_RADI,	  0x00},
	{LC_COEF,	  0x13},
	{LC_XC,		  0x08},
	{LC_COEFB,  0x14},
	{LC_COEFR,  0x17},
	{LC_CTR,	  0x05},
	
//	{COM3,		  0x00},
	{COM3,		  0xd0},/*Horizontal mirror image*/

	/*night mode auto frame rate control*/
	{COM5,		0xf5},	 /*��ҹ�ӻ����£��Զ�����֡�ʣ���֤���նȻ�������*/
	//{COM5,		0x31},	/*ҹ�ӻ���֡�ʲ���*/	
};



/************************************************
 * ��������FIFO_GPIO_Config
 * ����  ��FIFO GPIO����
 * ����  ����
 * ���  ����
 * ע��  ����
 ************************************************/
void FIFO_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
  /*����ʱ��*/
  RCC_APB2PeriphClockCmd (OV7725_OE_GPIO_CLK|OV7725_WRST_GPIO_CLK|
                        OV7725_RRST_GPIO_CLK|OV7725_RCLK_GPIO_CLK|
                          OV7725_WE_GPIO_CLK|OV7725_DATA_GPIO_CLK, ENABLE );
	
  /*(FIFO_OE--FIFO���ʹ��)*/
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
  GPIO_InitStructure.GPIO_Pin = OV7725_OE_GPIO_PIN;
  GPIO_Init(OV7725_OE_GPIO_PORT, &GPIO_InitStructure);
	
  /*(FIFO_WRST--FIFOд��λ)*/
  GPIO_InitStructure.GPIO_Pin = OV7725_WRST_GPIO_PIN;
	GPIO_Init(OV7725_WRST_GPIO_PORT, &GPIO_InitStructure);
	
  /*(FIFO_RRST--FIFO����λ) */
  GPIO_InitStructure.GPIO_Pin = OV7725_RRST_GPIO_PIN;
  GPIO_Init(OV7725_RRST_GPIO_PORT, &GPIO_InitStructure);
		
  /*(FIFO_RCLK-FIFO��ʱ��)*/
  GPIO_InitStructure.GPIO_Pin = OV7725_RCLK_GPIO_PIN;
  GPIO_Init(OV7725_RCLK_GPIO_PORT, &GPIO_InitStructure);

  /*(FIFO_WE--FIFOдʹ��)*/
  GPIO_InitStructure.GPIO_Pin = OV7725_WE_GPIO_PIN;	
  GPIO_Init(OV7725_WE_GPIO_PORT, &GPIO_InitStructure);
	

  /*(FIFO_DATA--FIFO�������)*/
  GPIO_InitStructure.GPIO_Pin = OV7725_DATA_0_GPIO_PIN | OV7725_DATA_1_GPIO_PIN |
                                OV7725_DATA_2_GPIO_PIN | OV7725_DATA_3_GPIO_PIN |
                                OV7725_DATA_4_GPIO_PIN | OV7725_DATA_5_GPIO_PIN |
                                OV7725_DATA_6_GPIO_PIN | OV7725_DATA_7_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(OV7725_DATA_GPIO_PORT, &GPIO_InitStructure);
		
		
  FIFO_OE_L();	  					/*����ʹFIFO���ʹ��*/
  FIFO_WE_H();   						/*����ʹFIFOд����*/
		
		
}


/************************************************
 * ��������VSYNC_GPIO_Config
 * ����  ��OV7725 VSYNC�ж��������
 * ����  ����
 * ���  ����
 * ע��  ����
 ************************************************/
void VSYNC_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
	
  /*��ʼ��ʱ�ӣ�ע���ж�Ҫ��AFIO*/
  RCC_APB2PeriphClockCmd ( RCC_APB2Periph_AFIO|OV7725_VSYNC_GPIO_CLK, ENABLE );	 
      
  /*��ʼ������*/
  GPIO_InitStructure.GPIO_Pin =  OV7725_VSYNC_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(OV7725_VSYNC_GPIO_PORT, &GPIO_InitStructure);

  /*�����ж�*/
  GPIO_EXTILineConfig(OV7725_VSYNC_EXTI_SOURCE_PORT, OV7725_VSYNC_EXTI_SOURCE_PIN);
  EXTI_InitStructure.EXTI_Line = OV7725_VSYNC_EXTI_LINE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling ; 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  EXTI_GenerateSWInterrupt(OV7725_VSYNC_EXTI_LINE);		
	
		/*�������ȼ�*/
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  NVIC_InitStructure.NVIC_IRQChannel = OV7725_VSYNC_EXTI_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


//��ʼ��OV7725
//����0:�ɹ�
//��������ֵ:�������
u8 OV7725_Init(void)
{
  u16 reg;
  u16 i=0;	  
  FIFO_GPIO_Config();
  VSYNC_GPIO_Config();

  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);	//SWD
  SCCB_Init();        		//��ʼ��SCCB ��IO��	
 
  printf("ov7725 Register Config Start......\n"); 
  if(0 != SCCB_WR_Reg(0x12,0x80))
  {
    printf("sccb write data error\n");
    return 1;	//��λOV7725
  }
  else
  {
    printf("Sensor reset success\n");
  }
  delay_ms(50); 
  reg=SCCB_RD_Reg(0X1c);		//��ȡ����ID �߰�λ
  reg<<=8;
  reg|=SCCB_RD_Reg(0X1d);		//��ȡ����ID �Ͱ�λ
  if(reg!=OV7725_MID)
  {
    printf("MID:%d\r\n",reg);
    return 1;
  }
  reg=SCCB_RD_Reg(0X0a);		//��ȡ����ID �߰�λ
  reg<<=8;
  reg|=SCCB_RD_Reg(0X0b);		//��ȡ����ID �Ͱ�λ
  if(reg!=OV7725_PID)
  {
    printf("HID:%d\r\n",reg);
    return 2;
  }   
  else
  {
    printf("Sensor ID read success,Sensor ID is 0x%x\n", reg);	
  }
	
    
  //��ʼ�� OV7725,����QVGA�ֱ���(320*240)  
  for(i=0;i<sizeof(ov7725_init_reg_YUV)/sizeof(ov7725_init_reg_YUV[0]);i++)
  {								
    SCCB_WR_Reg(ov7725_init_reg_YUV[i][0],ov7725_init_reg_YUV[i][1]);
  } 
  printf("ov7725 Register Config Success");
  return 0x00; 	//ok
} 

////////////////////////////////////////////////////////////////////////////
//OV7725��������
//��ƽ������
//0:�Զ�ģʽ
//1:����
//2,����
//3,�칫��
//4,����
//5,ҹ��
void OV7725_Light_Mode(u8 mode)
{
	switch(mode)
	{
		case 0:	//Auto���Զ�ģʽ
			SCCB_WR_Reg(0x13, 0xff); //AWB on 
			SCCB_WR_Reg(0x0e, 0x65);
			SCCB_WR_Reg(0x2d, 0x00);
			SCCB_WR_Reg(0x2e, 0x00);
			break;
		case 1://sunny������
			SCCB_WR_Reg(0x13, 0xfd); //AWB off
			SCCB_WR_Reg(0x01, 0x5a);
			SCCB_WR_Reg(0x02, 0x5c);
			SCCB_WR_Reg(0x0e, 0x65);
			SCCB_WR_Reg(0x2d, 0x00);
			SCCB_WR_Reg(0x2e, 0x00);
			break;	
		case 2://cloudy������
			SCCB_WR_Reg(0x13, 0xfd); //AWB off
			SCCB_WR_Reg(0x01, 0x58);
			SCCB_WR_Reg(0x02, 0x60);
			SCCB_WR_Reg(0x0e, 0x65);
			SCCB_WR_Reg(0x2d, 0x00);
			SCCB_WR_Reg(0x2e, 0x00);
			break;	
		case 3://office���칫��
			SCCB_WR_Reg(0x13, 0xfd); //AWB off
			SCCB_WR_Reg(0x01, 0x84);
			SCCB_WR_Reg(0x02, 0x4c);
			SCCB_WR_Reg(0x0e, 0x65);
			SCCB_WR_Reg(0x2d, 0x00);
			SCCB_WR_Reg(0x2e, 0x00);
			break;	
		case 4://home������
			SCCB_WR_Reg(0x13, 0xfd); //AWB off
			SCCB_WR_Reg(0x01, 0x96);
			SCCB_WR_Reg(0x02, 0x40);
			SCCB_WR_Reg(0x0e, 0x65);
			SCCB_WR_Reg(0x2d, 0x00);
			SCCB_WR_Reg(0x2e, 0x00);
			break;	

		case 5://night��ҹ��
			SCCB_WR_Reg(0x13, 0xff); //AWB on
			SCCB_WR_Reg(0x0e, 0xe5);
			break;
	}
}		  
//ɫ������
//sat:-4~+4
void OV7725_Color_Saturation(s8 sat)
{
 	if(sat>=-4 && sat<=4)
	{	
		SCCB_WR_Reg(USAT,(sat+4)<<4); 
		SCCB_WR_Reg(VSAT,(sat+4)<<4);
	}
}
//��������
//bright��-4~+4
void OV7725_Brightness(s8 bright)
{
	u8 bright_value,sign;
  	switch(bright)
	{
		case 4:
			bright_value = 0x48;
			sign = 0x06;
			break;
		case 3:
			bright_value = 0x38;
			sign = 0x06;		
			break;	
		case 2:
			bright_value = 0x28;
			sign = 0x06;			
			break;	
		case 1:
			bright_value = 0x18;
			sign = 0x06;			
			break;
		case 0:
			bright_value = 0x08;
			sign = 0x06;			
			break;	
		case -1:
			bright_value = 0x08;
			sign = 0x0e;		
			break;		
		case -2:
			bright_value = 0x18;
			sign = 0x0e;		
			break;	
		case -3:
			bright_value = 0x28;
			sign = 0x0e;		
			break;	
		case -4:
			bright_value = 0x38;
			sign = 0x0e;		
			break;	
	}
	SCCB_WR_Reg(BRIGHT, bright_value);
	SCCB_WR_Reg(SIGN, sign);
}
//�Աȶ�����
//contrast��-4~+4
void OV7725_Contrast(s8 contrast)
{
	if(contrast >= -4 && contrast <=4)
	{
		SCCB_WR_Reg(CNST,(0x30-(4-contrast)*4));
	}
}
//��Ч����
//0:��ͨģʽ    
//1,��Ƭ
//2,�ڰ�   
//3,ƫ��ɫ
//4,ƫ��ɫ
//5,ƫ��ɫ
//6,����	    
void OV7725_Special_Effects(u8 eft)
{
	switch(eft)
	{
		case 0://����
			SCCB_WR_Reg(0xa6, 0x06);
			SCCB_WR_Reg(0x60, 0x80);
			SCCB_WR_Reg(0x61, 0x80);
			break;
		case 1://�ڰ�
			SCCB_WR_Reg(0xa6, 0x26);
			SCCB_WR_Reg(0x60, 0x80);
			SCCB_WR_Reg(0x61, 0x80);
			break;	
		case 2://ƫ��
			SCCB_WR_Reg(0xa6, 0x1e);
			SCCB_WR_Reg(0x60, 0xa0);
			SCCB_WR_Reg(0x61, 0x40);	
			break;
		case 3://����
			SCCB_WR_Reg(0xa6, 0x1e);
			SCCB_WR_Reg(0x60, 0x40);
			SCCB_WR_Reg(0x61, 0xa0);	
			break;	
		case 4://ƫ��
			SCCB_WR_Reg(0xa6, 0x1e);
			SCCB_WR_Reg(0x60, 0x80);
			SCCB_WR_Reg(0x61, 0xc0);		
			break;	
		case 5://ƫ��
			SCCB_WR_Reg(0xa6, 0x1e);
			SCCB_WR_Reg(0x60, 0x60);
			SCCB_WR_Reg(0x61, 0x60);		
			break;	
		case 6://����
			SCCB_WR_Reg(0xa6, 0x46);
			break;	
	}
}	
//����ͼ���������
//width:���ͼ����,<=320
//height:���ͼ��߶�,<=240
//mode:0��QVGA���ģʽ��1��VGA���ģʽ
//QVGAģʽ���ӷ�Χ�㵫���ﲻ�Ǻ�������VGAģʽ���ӷ�ΧС��������
void OV7725_Window_Set(u16 width,u16 height,u8 mode)
{
	u8 raw,temp;
	u16 sx,sy;
	
	if(mode)
	{
		sx=(640-width)/2;
		sy=(480-height)/2;
		SCCB_WR_Reg(COM7,0x06);		//����ΪVGAģʽ
		SCCB_WR_Reg(HSTART,0x23); 	//ˮƽ��ʼλ��
		SCCB_WR_Reg(HSIZE,0xA0); 	//ˮƽ�ߴ�
		SCCB_WR_Reg(VSTRT,0x07); 	//��ֱ��ʼλ��
		SCCB_WR_Reg(VSIZE,0xF0); 	//��ֱ�ߴ�
		SCCB_WR_Reg(HREF,0x00);
		SCCB_WR_Reg(HOutSize,0xA0); //����ߴ�
		SCCB_WR_Reg(VOutSize,0xF0); //����ߴ�
	}
	else
	{
		sx=(320-width)/2;
		sy=(240-height)/2;
		SCCB_WR_Reg(COM7,0x40);		//����ΪQVGAģʽ
		SCCB_WR_Reg(HSTART,0x3f); 	//ˮƽ��ʼλ��
		SCCB_WR_Reg(HSIZE, 0x50); 	//ˮƽ�ߴ�
		SCCB_WR_Reg(VSTRT, 0x03); 	//��ֱ��ʼλ��
		SCCB_WR_Reg(VSIZE, 0x78); 	//��ֱ�ߴ�
		SCCB_WR_Reg(HREF,  0x00);
		SCCB_WR_Reg(HOutSize,0x50);	//����ߴ�
		SCCB_WR_Reg(VOutSize,0x78); //����ߴ�
	}
	raw=SCCB_RD_Reg(HSTART);
	temp=raw+(sx>>2);//sx��8λ����HSTART,��2λ����HREF[5:4]
	SCCB_WR_Reg(HSTART,temp);
	SCCB_WR_Reg(HSIZE,width>>2);//width��8λ����HSIZE,��2λ����HREF[1:0]
	
	raw=SCCB_RD_Reg(VSTRT);
	temp=raw+(sy>>1);//sy��8λ����VSTRT,��1λ����HREF[6]
	SCCB_WR_Reg(VSTRT,temp);
	SCCB_WR_Reg(VSIZE,height>>1);//height��8λ����VSIZE,��1λ����HREF[2]
	
	raw=SCCB_RD_Reg(HREF);
	temp=((sy&0x01)<<6)|((sx&0x03)<<4)|((height&0x01)<<2)|(width&0x03)|raw;
	SCCB_WR_Reg(HREF,temp);
	
	SCCB_WR_Reg(HOutSize,width>>2);
	SCCB_WR_Reg(VOutSize,height>>1);
	
	SCCB_RD_Reg(EXHCH);	
	temp = (raw|(width&0x03)|((height&0x01)<<2));	
	SCCB_WR_Reg(EXHCH,temp);	
}

extern u8 ov_sta;	//��exit.c���涨��
extern u8 ov_frame;	//��timer.c���涨��

//����LCD��ʾ(OV7725)
void OV7725_camera_refresh(void)
{
  u32 i,j;
  //u8 color1,color2;
  u8 color;
  if(ov_sta==2)
  {

    OV7725_CS=0;		
    OV7725_RRST=0;				//��ʼ��λ��ָ�� 
    OV7725_RCK=0;
    OV7725_RCK=1;
    OV7725_RCK=0;
    OV7725_RRST=1;				//��λ��ָ����� 
    OV7725_RCK=1; 
    for(i=0;i<OV7725_WINDOW_HEIGHT;i++)
    {
      for(j=0;j<OV7725_WINDOW_WIDTH;j++)
      {
       // GPIOB->CRL=0X88888888;
        OV7725_RCK=0;
        color= OV7725_DATA;	//������
        OV7725_RCK=1; 
       // color<<=8;  
        OV7725_RCK=0;
       // color|=OV7725_DATA;	//������
        OV7725_RCK=1;
        //color = color1<<8 + color2;
       // GPIOB->CRL=0X33333333;
      // printf("%c",color); 
       //printf("%c",color2);
      USART1_Putc(color);
      }
    }
    OV7725_CS=1;
    OV7725_RCK=0;
    OV7725_RCK=1;
    EXTI_ClearITPendingBit(EXTI_Line8);  //���EXTI8��·����λ
    ov_sta=0;					//����֡�жϱ��
    ov_frame++; 
    //LCD_Scan_Dir(DFT_SCAN_DIR);	//�ָ�Ĭ��ɨ�跽�� 
	} 
}


