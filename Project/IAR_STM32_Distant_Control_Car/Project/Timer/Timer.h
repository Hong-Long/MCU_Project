#ifndef _Timer_H_
#define _Timer_H_

#include "stm32f10x.h"
extern long g_count; /*用来上报数据*/
void TIM1_Int_Init(u16 arr,u16 psc);
void TIM3_Int_Init(u16 arr,u16 psc);
#endif

