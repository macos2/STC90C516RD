/*
 * gpio.c
 *
 *  Created on: 2020年11月27日
 *      Author: tom
 */

#include "gpio.h"
#define SET_PORT(x) case x: P##x=value;return;break;
#define SET_PIN(x) 	case x:	switch (port){ 	case 0:P0##x=value;return;break; case 1:P1##x=value;return;break; case 2:P2##x=value;return;break;	case 3:P3##x=value;return;break;}break;

#define GET_PORT(x) case x: return P##x;break;
#define GET_PIN(x)  case x: switch (port){ case 0:return P0##x;break; case 1:return P1##x;break; case 2:return P2##x;break; case 3:return P3##x;break; case 4:return P4##x;break;}break;


void gpio_set(gpio io,unsigned char value){
	unsigned char port=io>>4;
	unsigned char pin=io&0x0f;
	switch(pin){
		case GPIO_ALL_PIN:
			switch(port){
				SET_PORT(0);
				SET_PORT(1);
				SET_PORT(2);
				SET_PORT(3);
				SET_PORT(4);
			}
			break;
		SET_PIN(0)
		SET_PIN(1)
		SET_PIN(2)
		SET_PIN(3)
		SET_PIN(4)
		SET_PIN(5)
		SET_PIN(6)
		SET_PIN(7)
	};
}

unsigned char gpio_get(gpio io){
	unsigned char port=io>>4;
	unsigned char pin=io&0x0f;
	switch (pin){
		case  GPIO_ALL_PIN:
			switch(port){
				GET_PORT(0)
				GET_PORT(1)
				GET_PORT(2)
				GET_PORT(3)
				GET_PORT(4)
			}break;
		GET_PIN(0)
		GET_PIN(1)
		GET_PIN(2)
		GET_PIN(3)
		GET_PIN(4)
		GET_PIN(5)
		GET_PIN(6)
		GET_PIN(7)
	}
	return 0x00;
}

gpio gpio_format(unsigned char port,unsigned char pin){
	return (port<<4)+(pin&0x0F);
}
