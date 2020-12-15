/*
 * ds1302.c
 *
 *  Created on: 2020年12月12日
 *      Author: tom
 */

#include "ds1302.h"

void ds1302_write(Ds1302 *dev, unsigned is_ram_data, unsigned char addr,
		unsigned char data) {
	unsigned char command = 0x80, i;
	if (is_ram_data)
		command += 0x40;
	command |= (addr & 0x1F) << 1;
	gpio_set(dev->rst, 0);
	gpio_set(dev->io, 0);
	gpio_set(dev->sclk, 0);

	gpio_set(dev->rst, 1);
	for (i = 0; i < 8; i++) {
		gpio_set(dev->io, 0x01 & (command >> i));
		gpio_set(dev->sclk, 1);
		gpio_set(dev->sclk, 0);
	}
	for (i = 0; i < 8; i++) {
		gpio_set(dev->io, 0x01 & (data >> i));
		gpio_set(dev->sclk, 1);
		gpio_set(dev->sclk, 0);
	}
	gpio_set(dev->rst, 0);
}

void ds1302_read(Ds1302 *dev, unsigned is_ram_data,	unsigned char addr, unsigned char *result) {
	unsigned char command = 0x81, i;
	if (is_ram_data)
		command += 0x40;
	command |= (addr & 0x1F) << 1;

		gpio_set(dev->rst, 0);
		gpio_set(dev->io, 0);
		gpio_set(dev->sclk, 0);
		gpio_set(dev->rst, 1);
		command |= (addr & 0x1F) << 1;
		for (i = 0; i < 8; i++) {
			gpio_set(dev->sclk, 0);
			gpio_set(dev->io, 0x01 & (command >> i));
			gpio_set(dev->sclk, 1);

		}
		for (i = 0; i < 8; i++) {
			gpio_set(dev->sclk, 0);
			*result |= (gpio_get(dev->io) << i);
			gpio_set(dev->sclk, 1);
		}
		gpio_set(dev->rst, 0);
	}


unsigned char __tmp;
void hex2bcd() __naked {
	__asm
	MOV A,___tmp
	DA A
	MOV ___tmp,A
	RET
	__endasm;
}

void ds1302_set_date(Ds1302 *dev, unsigned char year, unsigned char month,
		unsigned char day) {
	__tmp=day;
	hex2bcd();
	ds1302_write(dev,0,0x03,__tmp);//day

	__tmp=month;
	hex2bcd();
	ds1302_write(dev,0,0x04,__tmp);//month

	__tmp=year;
	hex2bcd();
	ds1302_write(dev,0,0x06,__tmp);//year


}

void ds1302_set_time(Ds1302 *dev, unsigned char hour, unsigned char minute,
		unsigned char second) {
	__tmp=second;
	hex2bcd();
	ds1302_write(dev,0,0x00,__tmp);//second
	__tmp=minute;
	hex2bcd();
	ds1302_write(dev,0,0x01,__tmp);//minute
	__tmp=hour;
	hex2bcd();
	ds1302_write(dev,0,0x02,__tmp);//hour

}
