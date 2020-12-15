/*
 * main.h
 *
 *  Created on: 2020年11月27日
 *      Author: tom
 */

#ifndef MAIN_H_
#define MAIN_H_
#include <stdio.h>
#include "gpio.h"
#include "LM032L.h"
#include "one_wire_bus.h"
#include "ds18b20.h"
#include "ds1302.h"

#define fosc 11059200
#define baud 19200

//__xdata unsigned char usart_txbuf[256],__tx,__ptx;
//__xdata unsigned char usart_rxbuf[256],__rx,__prx;
//
//int putchar (int c){
//	usart_txbuf[__prx+__tx]=(char)c;
//	__tx++;
//	return c;
//}
//
//int getchar(void){
//	if(__rx>0)
//	__rx--;
//	__prx++;
//	return usart_rxbuf[__prx-1];
//}
//
//char *gets(char *buf){
//	char *p=usart_rxbuf+__prx;
//	if(buf!=NULL){
//		while(__rx>0){
//			*buf=usart_rxbuf[__prx];
//			buf++;
//			__rx--;
//			__prx++;
//		}
//	}else{
//	__prx=__prx+__rx;
//	__rx=0;
//	}
//	return p;
//}

#endif /* MAIN_H_ */
