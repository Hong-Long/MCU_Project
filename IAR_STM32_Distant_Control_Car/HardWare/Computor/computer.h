#ifndef __COMPUTER_H
#define __COMPUTER_H
#include "sys.h"
	 
//��ͼ�����ݷ��͵�ɽ��๦�ܵ�����������ʾ			  



void vcan_sendimg(void *imgaddr, uint32_t imgsize);
void vcan_sendccd(void *ccdaddr, uint32_t ccdsize);
void vcan_sendware(void *wareaddr, uint32_t waresize);
		 				    
#endif
