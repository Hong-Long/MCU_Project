#include "computer.h"
#include "UART.h"	 
#include "delay.h"
//��ͼ�����ݷ��͵�ɽ��๦�ܵ�����������ʾ			  
   
#define VCAN_PORT  USART1

/*!
 *  @brief      ɽ��๦�ܵ���������λ��������ͷ��ʾ����
 *  @param      imgaddr    ͼ����ʼ��ַ
 *  @param      imgsize    ͼ��ռ�ÿռ�Ĵ�С
 *  @since      v5.0
*  Sample usage:
             �����÷��ο�������:
            ��ɽ������ͷ��ӥ����λ�����̺�΢��Ч�� - ���ܳ�������
             http://vcan123.com/forum.php?mod=viewthread&tid=6242&ctid=27
 */
void vcan_sendimg(void *imgaddr, uint32_t imgsize)
{
#define CMD_IMG     1
//    uint8_t cmdf[2] = {CMD_IMG, ~CMD_IMG};    //ɽ����λ�� ʹ�õ�����
//    uint8_t cmdr[2] = {~CMD_IMG, CMD_IMG};    //ɽ����λ�� ʹ�õ�����

//		uint8_t cmdf[2] = {0x01, 0xFE};    //ɽ����λ�� ʹ�õ�����
//    uint8_t cmdr[2] = {0xFE, 0x01};    //ɽ����λ�� ʹ�õ�����	
  uint8_t cmdf[2] = {0x01, 0xfe};    //ɽ����λ�� ʹ�õ�����
  uint8_t cmdr[2] = {0xfe, 0x01};    //ɽ����λ�� ʹ�õ�����	
		
  uint8_t dis[1] = {0x05};    //ռλ
		
		//printf("hhh");					//���е������ˣ�������û�����  ��������Ϊ�������Ͳ�һ��
		//delay_ms(5);		
	//	uart_putbuff (USART1,"123456", 3); //ʵ�ʷ�����3���ֽ�'1','2','3'
  uart_putbuff(VCAN_PORT, dis, sizeof(dis));    //ռλ
		
		
  uart_putbuff(VCAN_PORT, cmdf, sizeof(cmdf));    //�ȷ�������

  uart_putbuff(VCAN_PORT, (uint8_t *)imgaddr, imgsize); //�ٷ���ͼ��

  uart_putbuff(VCAN_PORT, cmdr, sizeof(cmdr));    //�ȷ�������
}


/*!
 *  @brief      ɽ��๦�ܵ���������λ��������CCD��ʾ����
 *  @param      ccdaddr    CCDͼ����ʼ��ַ
 *  @param      ccdsize    CCDͼ��ռ�ÿռ�Ĵ�С
 *  @since      v5.0
*  Sample usage:
             �����÷��ο�������:
            ��ɽ������CCD����λ����ʾ���̺ͳ���ǲɼ�Ч�� - ���ܳ�������
             http://vcan123.com/forum.php?mod=viewthread&tid=6340&ctid=27
 */
void vcan_sendccd(void *ccdaddr, uint32_t ccdsize)
{
#define CMD_CCD     2
    uint8_t cmdf[2] = {CMD_CCD, ~CMD_CCD};    //��ͷ����
    uint8_t cmdr[2] = {~CMD_CCD, CMD_CCD};    //��β����

    uart_putbuff(VCAN_PORT, cmdf, sizeof(cmdf));    //�ȷ�������

    uart_putbuff(VCAN_PORT, (uint8_t *)ccdaddr, ccdsize); //�ٷ���ͼ��

    uart_putbuff(VCAN_PORT, cmdr, sizeof(cmdr));    //�ٷ�������
}


/*!
 *  @brief      ɽ��๦�ܵ���������λ��������ʾ������ʾ����
 *  @param      wareaddr    ����������ʼ��ַ
 *  @param      waresize    ��������ռ�ÿռ�Ĵ�С
 *  @since      v5.0
*  Sample usage:
             �����÷��ο�������:
            ��ɽ�����ϡ������Ǻͼ��ٶ� ��λ����ʾ���� - ���ܳ�������
             http://vcan123.com/forum.php?mod=viewthread&tid=6253&ctid=27
 */
void vcan_sendware(void *wareaddr, uint32_t waresize)
{
#define CMD_WARE     3
    uint8_t cmdf[2] = {CMD_WARE, ~CMD_WARE};    //���ڵ��� ʹ�õ�ǰ����
    uint8_t cmdr[2] = {~CMD_WARE, CMD_WARE};    //���ڵ��� ʹ�õĺ�����

    uart_putbuff(VCAN_PORT, cmdf, sizeof(cmdf));    //�ȷ���ǰ����
    uart_putbuff(VCAN_PORT, (uint8_t *)wareaddr, waresize);    //��������
    uart_putbuff(VCAN_PORT, cmdr, sizeof(cmdr));    //���ͺ�����

}
 
