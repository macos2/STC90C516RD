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
#include "i2c_bus.h"
#include "spi_bus.h"
#include "i2c_memory.h"
//#include "spi_memory.h"
#include "spi_sd.h"
#define fosc 11059200
#define baud 19200
 void usart_send(char *fmt, ...);

#endif /* MAIN_H_ */
