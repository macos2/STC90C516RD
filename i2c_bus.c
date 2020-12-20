/*
 * i2c_bus.c
 *
 *  Created on: 2020年12月17日
 *      Author: tom
 */

#include "i2c_bus.h"

void i2c_start(I2cBus *bus) {
	gpio_set(bus->sck, 1);
	gpio_set(bus->sda, 1);
	gpio_set(bus->sda, 0);
	gpio_set(bus->sck, 0);
}

void i2c_stop(I2cBus *bus) {
	gpio_set(bus->sck, 0);
	gpio_set(bus->sda, 0);
	gpio_set(bus->sck, 1);
	gpio_set(bus->sda, 1);
}

void i2c_reset(I2cBus *bus){
	unsigned char i;
	for(i=0;i<8;i++){
		gpio_set(bus->sck, 0);
		gpio_set(bus->sck, 1);
	}
	i2c_stop(bus);
}

unsigned char i2c_read(I2cBus *bus, unsigned char no_ack) {
	unsigned char result = 0, i;
	gpio_set(bus->sda, 1);
	for (i = 0; i < 8; i++) {
		result = result << 1;
		gpio_set(bus->sck, 1);
		result |= gpio_get(bus->sda);
		gpio_set(bus->sck, 0);
	}
	no_ack = no_ack > 0 ? 1 : 0;
	gpio_set(bus->sda, no_ack);
	gpio_set(bus->sck, 1);
	gpio_set(bus->sck, 0);
	return result;

}
unsigned char i2c_write(I2cBus *bus, unsigned char data) {
	unsigned char i, ack = 0x01;
	for (i = 0; i < 8; i++) {
		gpio_set(bus->sck, 0);
		gpio_set(bus->sda, 0x80 & data);
		gpio_set(bus->sck, 1);
		data = data << 1;
	}
	gpio_set(bus->sck, 0);
	gpio_set(bus->sda, 1);
	gpio_set(bus->sck, 1);
	if(gpio_get(bus->sda)==0)ack=0x00;
	gpio_set(bus->sck, 0);
	return ack;
}

unsigned char i2c_send_7bit_addr(I2cBus *bus, unsigned char addr,
		unsigned char is_read) {
	unsigned char i, ack = 0x01;
	is_read = is_read > 0 ? 1 : 0;
	addr = (addr << 1) | is_read;
	for (i = 0; i < 8; i++) {
		gpio_set(bus->sck, 0);
		gpio_set(bus->sda, 0x80 & addr);
		gpio_set(bus->sck, 1);
		addr = addr << 1;
	}
	gpio_set(bus->sck, 0);
	gpio_set(bus->sda, 1);
	gpio_set(bus->sck, 1);
	if(gpio_get(bus->sda)==0)ack=0x00;
	gpio_set(bus->sck, 0);
	return ack;
}

unsigned char i2c_send_10bit_addr(I2cBus *bus, unsigned char addr_h,
		unsigned char addr_l, unsigned char is_read) {
	unsigned char i, ack = 0x01;
	is_read = is_read > 0 ? 1 : 0;
	addr_h = 0xf0 | (addr_h << 1) | is_read;
	for (i = 0; i < 8; i++) {
		gpio_set(bus->sck, 0);
		gpio_set(bus->sda, 0x80 & addr_h);
		gpio_set(bus->sck, 1);
		addr_h = addr_h << 1;
	}

	gpio_set(bus->sck, 0);
	gpio_set(bus->sda, 1);
	gpio_set(bus->sck, 1);
	ack = gpio_get(bus->sda);
	gpio_set(bus->sck, 0);
	if (ack != 0)
		return 0x01;

	for (i = 0; i < 8; i++) {
		gpio_set(bus->sck, 0);
		gpio_set(bus->sda, 0x80 & addr_l);
		gpio_set(bus->sck, 1);
		addr_h = addr_l << 1;
	}
	gpio_set(bus->sck, 0);
	gpio_set(bus->sda, 1);
	gpio_set(bus->sck, 1);
	if(gpio_get(bus->sda)==0)ack=0x00;
	gpio_set(bus->sck, 0);
	return ack;
}

