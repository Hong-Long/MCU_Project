#ifndef _sccb_H_
#define _sccb_H_

#include "sys.h"



/************************** OV7725 连接引脚定义********************************/
#define      OV7725_SIO_C_SCK_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define      OV7725_SIO_C_GPIO_CLK                       RCC_APB2Periph_GPIOA
#define      OV7725_SIO_C_GPIO_PORT                      GPIOA
#define      OV7725_SIO_C_GPIO_PIN                       GPIO_Pin_12

#define      OV7725_SIO_D_SCK_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define      OV7725_SIO_D_GPIO_CLK                       RCC_APB2Periph_GPIOA
#define      OV7725_SIO_D_GPIO_PORT                      GPIOA
#define      OV7725_SIO_D_GPIO_PIN                       GPIO_Pin_11



#define SCL_H         GPIO_SetBits(OV7725_SIO_C_GPIO_PORT , OV7725_SIO_C_GPIO_PIN) 
#define SCL_L         GPIO_ResetBits(OV7725_SIO_C_GPIO_PORT , OV7725_SIO_C_GPIO_PIN) 
   
#define SDA_H         GPIO_SetBits(OV7725_SIO_D_GPIO_PORT , OV7725_SIO_D_GPIO_PIN) 
#define SDA_L         GPIO_ResetBits(OV7725_SIO_D_GPIO_PORT , OV7725_SIO_D_GPIO_PIN) 

#define SCL_read      GPIO_ReadInputDataBit(OV7725_SIO_C_GPIO_PORT , OV7725_SIO_C_GPIO_PIN) 
#define SDA_read      GPIO_ReadInputDataBit(OV7725_SIO_D_GPIO_PORT , OV7725_SIO_D_GPIO_PIN) 

#define SCCB_ID   0x42

#define SCCB_SDA_IN()  {GPIOA->CRL&=0XFF0FFFFF;GPIOC->CRL|=0X00800000;}
#define SCCB_SDA_OUT() {GPIOA->CRL&=0XFF0FFFFF;GPIOC->CRL|=0X00300000;}

//IO操作函数	 
#define SCCB_SCL    		PAout(12)	 	//SCL
#define SCCB_SDA    		PAout(11) 		//SDA	 

#define SCCB_READ_SDA    	PAin(11)  		//输入SDA    




void SCCB_Init(void);
void SCCB_Start(void);
void SCCB_Stop(void);
void SCCB_No_Ack(void);
u8 SCCB_WR_Byte(u8 dat);
u8 SCCB_RD_Byte(void);
u8 SCCB_WR_Reg(u8 reg,u8 data);
u8 SCCB_RD_Reg(u8 reg);



#endif
