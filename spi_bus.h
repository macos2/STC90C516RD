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
	unsigned char CPOL;
	unsigned char CPHA;
	unsigned char MSB_FIRST;
} SpiBus;

void spi_init(SpiBus *bus);
void spi_write(SpiBus *bus,unsigned char data);
unsigned char spi_read(SpiBus *bus);

#endif /* SPI_BUS_H_ */
