/*
 * 80c51_gpio.h
 *
 *  Created on: 2020年11月27日
 *      Author: tom
 */

#ifndef GPIO_H_
#define GPIO_H_
#include "stc89xx_stc90xx.h"


/* Format the gpio in a 8bit value like 0bFFFF PPPP
 * FFFF:port value of the gpio
 * PPPP:pin  value of the gpio , if PPPP=1111, it will be indicated all the pin of the port.
 * example:
 * 	P32=0b0011 0010
 * 	P41=0b0100 0001
 * 	P2 =0b0010 1111
 *
 */
#define gpio unsigned char
#define GPIO_ALL_PIN 0x0f
#define GPIO_PIN_00 0x00
#define GPIO_PIN_01 0x01
#define GPIO_PIN_02 0x02
#define GPIO_PIN_03 0x03
#define GPIO_PIN_04 0x04
#define GPIO_PIN_05 0x05
#define GPIO_PIN_06 0x06
#define GPIO_PIN_07 0x07

void gpio_set(gpio io,unsigned char value) __naked;
unsigned char gpio_get(gpio io) ;
gpio gpio_format(unsigned char port,unsigned char pin);

#endif /* 80C51_GPIO_H_ */
