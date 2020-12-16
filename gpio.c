/*
 * gpio.c
 *
 *  Created on: 2020年11月27日
 *      Author: tom
 */

#include "gpio.h"
#define SET_IO(i)		case 0x##i##0:P##i##0=value;break;case 0x##i##1:P##i##1=value;break;case 0x##i##2:P##i##2=value;break;case 0x##i##3:P##i##3=value;break;\
					 	case 0x##i##4:P##i##4=value;break;case 0x##i##5:P##i##5=value;break;case 0x##i##6:P##i##6=value;break;case 0x##i##7:P##i##7=value;break;\
					 	case 0x##i##F:P##i=value;break;

#define GET_IO(i)		case 0x##i##0:result=P##i##0;break;case 0x##i##1:result=P##i##1;break;case 0x##i##2:result=P##i##2;break;case 0x##i##3:result=P##i##3;break;\
					 	case 0x##i##4:result=P##i##4;break;case 0x##i##5:result=P##i##5;break;case 0x##i##6:result=P##i##6;break;case 0x##i##7:result=P##i##7;break;\
					 	case 0x##i##F:result=P##i;break;

void gpio_set(gpio io,unsigned char value) {
	switch (io){
	SET_IO(0)
	SET_IO(1)
	SET_IO(2)
	SET_IO(3)
	SET_IO(4)
	default:break;
	}
	__asm__("RET");
}

unsigned char gpio_get(gpio io){
	unsigned char result=0x00;
	switch(io){
	GET_IO(0)
		GET_IO(1)
		GET_IO(2)
		GET_IO(3)
		GET_IO(4)
	default:break;
	}
	return result;
}


gpio gpio_format(unsigned char port,unsigned char pin){
	return (port<<4)+(pin&0x0F);
}
