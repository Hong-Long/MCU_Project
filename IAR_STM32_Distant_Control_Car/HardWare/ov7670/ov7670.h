	#ifndef _OV7670_H
#define _OV7670_H
#include "sys.h"
#include "sccb.h"

#define OV7670_VSYNC  	PAin(8)			//ͬ���źż��IO
#define OV7670_WRST		PBout(0)		//дָ�븴λ
#define OV7670_WREN		PCout(7)		//д��FIFOʹ��
//#define OV7670_RCK		PBout(4)		//������ʱ��
#define OV7670_RCK_L  GPIOC->BRR  = GPIO_Pin_6
#define OV7670_RCK_H  GPIOC->BSRR = GPIO_Pin_6

#define OV7670_RRST		PCout(5)  		//��ָ�븴λ
#define OV7670_CS		PBout(1)  		//Ƭѡ�ź�(OE)
															  					 
#define OV7670_DATA   GPIO_ReadInputData(GPIOC,0x00FF) 					//��������˿�
#define GRAY_2_RGB565(gray)             ((u16)((((u8)(gray)>>3)<<11)|(((u8)(gray)>>2)<<5)|((u8)(gray)>>3)))
//GPIOC->IDR&0x00FF 
/////////////////////////////////////////

typedef struct{
	u16 xsta;
	u16 ysta;
	u16 width;		//����ͷ��ʾͼ��Ŀ���
	u16 height;		//����ͷ��ʾͼ��ĸ߶�	
	u8 mode;
}OV7670_CONFIG;



	  
extern const u8 ov7670_init_reg_tbl_RGB565[171][2];		//�Ĵ����������ñ�
extern const u8 ov7670_init_reg_tbl_YUV[174][2];		//�Ĵ����������ñ�

extern OV7670_CONFIG ov7670_config;

u8   OV7670_Init(void);		  	   		 
void OV7670_Light_Mode(u8 mode);
void OV7670_Color_Saturation(u8 sat);
void OV7670_Brightness(u8 bright);
void OV7670_Contrast(u8 contrast);
void OV7670_Special_Effects(u8 eft);
void OV7670_Window_Set(u16 sx,u16 sy,u16 width,u16 height);
void config_ov7670_OutPut(u16 xsta,u16 ysta,u16 width,u16 height,u8 ouput_mode);

#endif




















