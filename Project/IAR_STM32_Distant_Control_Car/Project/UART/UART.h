#ifndef _UART_H_
#define _UART_H_
	
#include "sys.h" 
#include "stm32f10x.h"
#include "stdio.h"

extern u8 inputString[80];
extern u8 newLineReceived;
extern int g_packnum;
void Uart1_init(u32 bound);
void UART1SendByte(unsigned char SendData);
void USART1_Putc(unsigned char c);
void uart_putbuff (USART_TypeDef* USARTx, uint8_t *buff, u32 len);
void Uart2_init(u32 bound);

#endif

