/*
 * spi_bus.c
 *
 *  Created on: 2020年12月19日
 *      Author: tom
 */

#include "spi_bus.h"

__xdata SpiBus *__spi_bus;

void spi_init(__xdata SpiBus *bus) {
	__spi_bus=bus;
	gpio_set(__spi_bus->CS, 1);
	gpio_set(__spi_bus->SCK, __spi_bus->cpol);
}

void spi_set_cs(__xdata SpiBus *bus, unsigned char cs) {
	__spi_bus=bus;
	gpio_set(__spi_bus->CS, cs);
}

void spi_write(__xdata SpiBus *bus, unsigned char data) {
	__spi_bus=bus;
	unsigned char i, tmp;
	for (i = 0; i < 8; i++) {
		if (__spi_bus->MSB_FIRST) {
			tmp = (data & 0x80) ? 0x01 : 0x00;
			data = data << 1;
		} else {
			tmp = data & 0x01;
			data = data >> 1;
		}

		if (__spi_bus->cpha) {
			gpio_set(__spi_bus->SCK, (~__spi_bus->cpol) & 0x01);
			gpio_set(__spi_bus->MOSI, tmp);
			gpio_set(__spi_bus->SCK, __spi_bus->cpol);
		} else {
			gpio_set(__spi_bus->MOSI, tmp);
			gpio_set(__spi_bus->SCK, (~__spi_bus->cpol) & 0x01);
			gpio_set(__spi_bus->SCK, __spi_bus->cpol);
		}
	}
}

unsigned char spi_read(__xdata SpiBus *bus) {
	__spi_bus=bus;
	unsigned char i, tmp, result = 0x00;
	for (i = 0; i < 8; i++) {
		if (__spi_bus->cpha) {
			gpio_set(__spi_bus->SCK, (~__spi_bus->cpol) & 0x01);
			gpio_set(__spi_bus->SCK, __spi_bus->cpol);
			tmp = gpio_get(__spi_bus->MISO);
		} else {
			gpio_set(__spi_bus->SCK, (~__spi_bus->cpol) & 0x01);
			tmp = gpio_get(__spi_bus->MISO);
			gpio_set(__spi_bus->SCK, __spi_bus->cpol);
		}

		if (__spi_bus->MSB_FIRST) {
			result |= tmp << (7 - i);
		} else {
			result |= tmp << i;
		}
	}
	return result;
}
