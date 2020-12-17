/*
 * i2c_bus.h
 *
 *  Created on: 2020年12月17日
 *      Author: tom
 */

#ifndef I2C_BUS_H_
#define I2C_BUS_H_

#include "gpio.h"

typedef struct {
	gpio scl;
	gpio sda;
}I2C_BUS;

void i2c_start(I2C_BUS *bus);
unsigned char i2c_read(I2C_BUS *bus,unsigned char no_ack);
unsigned char i2c_write(I2C_BUS *bus,unsigned char data);

unsigned char i2c_send_7bit_addr(I2C_BUS *bus,unsigned char addr,unsigned char is_read);
unsigned char i2c_send_10bit_addr(I2C_BUS *bus,unsigned char addr_h,unsigned char addr_l,unsigned char is_read);
unsigned char i2c_send_rand_bit_addr(I2C_BUS *bus,unsigned char *addr,unsigned char len,unsigned char is_read);



#endif /* I2C_BUS_H_ */
