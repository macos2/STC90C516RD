/*
 * spi_bus.h
 *
 *  Created on: 2020年12月19日
 *      Author: tom
 */

#ifndef SPI_BUS_H_
#define SPI_BUS_H_

#include "gpio.h"

typedef struct {
	gpio CS;
	gpio SCK;
	gpio MOSI;
	gpio MISO;
	bool cpol;
	bool cpha;
	bool MSB_FIRST;
} SpiBus;

void spi_init( __xdata SpiBus *bus);
void spi_set_cs(__xdata SpiBus *bus,unsigned char cs);

void spi_write(__xdata SpiBus *bus,unsigned char data);
unsigned char spi_read(__xdata SpiBus *bus);

#endif /* SPI_BUS_H_ */
