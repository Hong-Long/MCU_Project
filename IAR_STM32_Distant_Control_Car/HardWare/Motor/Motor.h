#ifndef _Motor_H_
#define _Motor_H_

#include "sys.h"
extern int speed,speed1,run;

#define M1A PBout(3)// 
#define M2A PCout(12)// 
#define M1B PDout(2)// 	
#define M2B PCout(11)//


void MOTOR_GPIO_Init(void);
void MiniCar_Run(int Speed0,int Speed1);
void MiniCar_Back(int Speed0,int Speed1);
void MiniCar_Stop();
void MiniCar_TurnLeft(int Speed0,int Speed1);
void MiniCar_TurnRight(int Speed0,int Speed1);
void MiniCar_TTurnLeft(int Speed0,int Speed1);
void MiniCar_TTurnRight(int Speed0,int Speed1);



#endif

