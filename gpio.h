/*
 * 80c51_gpio.h
 *
 *  Created on: 2020年11月27日
 *      Author: tom
 */

#ifndef GPIO_H_
#define GPIO_H_
//#include "stc89xx_stc90xx.h"
#include "stc8a_f.h"
//#include <mcs51/at89x52.h>

/* Format the gpio in a 8bit value like 0bFFFF FFFF PPPP PPPP
 * FFFF FFFF:port value of the gpio
 * PPPP PPPP:pin  value of the gpio , if PPPP PPPP=1111 1111, it will be indicated all the pin of the port.
 * example:
 * 	P32=0b0000 1000 0000 0100
 * 	P41=0b0001 0000 0000 0010
 * 	P2 =0b0000 0100 1111 1111
 */

#define gpio unsigned int
#define gpio_pin unsigned char
#define GPIO_PIN_ALL 0xff
#define GPIO_PIN_00 0x01
#define GPIO_PIN_01 0x02
#define GPIO_PIN_02 0x04
#define GPIO_PIN_03 0x08
#define GPIO_PIN_04 0x10
#define GPIO_PIN_05 0x20
#define GPIO_PIN_06 0x40
#define GPIO_PIN_07 0x80

void gpio_set(gpio io,unsigned char value);
unsigned char gpio_get(gpio io) ;
unsigned char gpio_get_pin(gpio io,gpio_pin pin);
gpio gpio_format(unsigned char port,unsigned char pin);

#endif /* 80C51_GPIO_H_ */
