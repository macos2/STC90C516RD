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

//#define GET_PORT(x) case x: return P##x;break;
//#define GET_PIN(x)  case x: switch (port){ case 0:return P0##x;break; case 1:return P1##x;break; case 2:return P2##x;break; case 3:return P3##x;break; case 4:return P4##x;break;}break;

//this is a test
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

//void gpio_set(gpio io,unsigned char value) {
//	unsigned char port=io>>4;
//	unsigned char pin=io&0x0f;
//	switch(pin){
//		case GPIO_ALL_PIN:
//			switch(port){
//				SET_PORT(0);
//				SET_PORT(1);
//				SET_PORT(2);
//				SET_PORT(3);
//				SET_PORT(4);
//			}
//			break;
//		SET_PIN(0)
//		SET_PIN(1)
//		SET_PIN(2)
//		SET_PIN(3)
//		SET_PIN(4)
//		SET_PIN(5)
//		SET_PIN(6)
//		SET_PIN(7)
//	};
//}

//unsigned char gpio_get(gpio io) {
//	unsigned char port=io>>4;
//	unsigned char pin=io&0x0f;
//	switch (pin){
//		case  GPIO_ALL_PIN:
//			switch(port){
//				GET_PORT(0)
//				GET_PORT(1)
//				GET_PORT(2)
//				GET_PORT(3)
//				GET_PORT(4)
//			}break;
//		GET_PIN(0)
//		GET_PIN(1)
//		GET_PIN(2)
//		GET_PIN(3)
//		GET_PIN(4)
//		GET_PIN(5)
//		GET_PIN(6)
//		GET_PIN(7)
//	}
//	return 0x00;
//}

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
