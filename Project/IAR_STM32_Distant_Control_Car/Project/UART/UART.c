#include "UART.h"



u8 newLineReceived = 0;
u8 inputString[80] = {0};  

/**
* Function       uart1_init
* @author        WHL
* @date          2020.03.12   
* @brief         ����1��ʼ��������
* @param[in]     bound ������
* @param[out]    void
* @retval        void
* @par History   ��
*/
void Uart1_init(u32 bound)
{
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
	 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
  //USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
  //USART1_RX	  GPIOA.10��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

  //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
  NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

  USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
  USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
  USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 

}

/**
* Function       UART1SendByte
* @author        WHL
* @date          2020.03.12  
* @brief         ����1����һ�ַ�
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   ��
*/
void UART1SendByte(unsigned char SendData)
{
    USART_SendData(USART1, SendData);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART2->SR&0X40)==0);//ѭ������,ֱ���������   
    USART2->DR = (u8) ch;      
	return ch;
}
/**********************************************************
** ������:USART1_Putc
** ��������: ����1����һ�ַ�
** �������: c
** �������: ��
***********************************************************/
void USART1_Putc(unsigned char c)
{
    USART1->DR = (u8)c; //Ҫ���͵��ַ������������ݼĴ���  
    while((USART1->SR&0X40)==0); //�ȴ��������  
}



/*!
 *  @brief      ����ָ��len���ֽڳ������� ������ NULL Ҳ�ᷢ�ͣ�
 *  @param      UARTn_e       ģ��ţ�UART0~UART5��
 *  @param      buff        �����ַ
 *  @param      len         ��������ĳ���
 *  @since      v5.0
 *  Sample usage:       uart_putbuff (UART3,"1234567", 3); //ʵ�ʷ�����3���ֽ�'1','2','3'
 */
void uart_putbuff (USART_TypeDef* USARTx, uint8_t *buff, u32 len)
{
    while(len--)
    {
//      USART_SendData(USARTx, *buff);			//�����޷�������ӡ��������������Ϊ�������Ͳ�һ�µ�ԭ��
      USART1_Putc(*buff);
      buff++;
    }
}

void Uart2_init(u32 bound)
{
  	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	 
	//USART2_TX   GPIOA.2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
	
	//USART2_RX	  GPIOA.3��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  
	
	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	//USART ��ʼ������
	
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	
	USART_Init(USART2, &USART_InitStructure); //��ʼ������2
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ���2

}

int g_num = 0;
u8 startBit = 0;
int g_packnum = 0;

void USART2_IRQHandler(void)
{
	u8 uartvalue = 0;

	if (USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET)//ע�⣡����ʹ��if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)���ж�  
    {  
		USART_ClearFlag(USART2, USART_FLAG_ORE); //��SR��ʵ���������־
       	USART_ReceiveData(USART2);  		
    }
		
	if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE)!=RESET)
	{
    	USART_ClearITPendingBit(USART2, USART_IT_RXNE);

		uartvalue = USART2->DR;
	    if(uartvalue == '$')
	    {
	      	startBit = 1;
		    g_num = 0;
	    }
	    if(startBit == 1)
	    {
	       	inputString[g_num] = uartvalue;     
	    }  
	    if (startBit == 1 && uartvalue == '#') 
	    {
	    	
			newLineReceived = 1; 
			startBit = 0;
			g_packnum = g_num;	
		
	    }
		g_num++;
		if(g_num >= 80)
		{
			g_num = 0;
			startBit = 0;
			newLineReceived	= 0;
		}	 
	
	}

} 
	


