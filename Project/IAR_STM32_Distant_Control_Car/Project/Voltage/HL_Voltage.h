#ifndef _HL_Voltage_H_
#define _HL_Voltage_H_

#include "stm32f10x.h"
#include "delay.h"
void Adc_Init(void);
void ADC_GPIO_Init();
float Get_Meassure_Volotage(void);
float Get_Battery_Volotage(void);
void Get_Battery_Volotage_show(void);



#endif

