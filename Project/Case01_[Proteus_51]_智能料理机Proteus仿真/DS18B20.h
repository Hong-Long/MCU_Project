#ifndef _DS18B20_H_
#define _DS18B20_H_

#include "reg52.h"
#include "delay.h"

void delay_us(unsigned char n);
void Init_DS18B20(void);


unsigned char ReadOneChar(void);



/*****写一个字节*****/
void WriteOneChar(unsigned char dat);



/*****读取温度*****/
unsigned int ReadTemperature(void);

#endif


