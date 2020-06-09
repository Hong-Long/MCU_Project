#ifndef _key_H_
#define _key_h_

#include "reg52.h"
#include "delay.h"
extern unsigned char key_value,KeyEnterFlag;
extern int keyenternum;
extern unsigned int KeyNum;
extern unsigned char idata ModeType;
#define KeyPort P1
#define GPIO_KEY P1
void KeyScan(void);
void KeyScan3_3(void);
void KEYDown();

#endif


