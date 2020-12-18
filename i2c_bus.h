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
	gpio sck;
	gpio sda;
}I2C_BUS;

/*
 * The Standard I2C Operation
 * @param bus:the I2C bus
 */
void i2c_start(I2C_BUS *bus);
void i2c_stop(I2C_BUS *bus);

/*
 * The Standard I2C Operation
 * @param bus:the I2C bus
 * @param no_ack:don't ack after receive the data
 * @return the data receive from the bus
 */
unsigned char i2c_read(I2C_BUS *bus,unsigned char no_ack);

/*
 * The Standard I2C Operation
 * @param bus:the I2C bus
 * @param data:the data write to the bus
 * @return	0 indicate received the ack signal
 * 			1 indicate haven't received the ack signal
 */
unsigned char i2c_write(I2C_BUS *bus,unsigned char data);


/*
 * @param bus:the I2C bus
 * @param addr:the 7bit or 10bit device address
 * @return ack:0 for ack , 1 for no ack.
 */
unsigned char i2c_send_7bit_addr(I2C_BUS *bus,unsigned char addr,unsigned char is_read);
unsigned char i2c_send_10bit_addr(I2C_BUS *bus,unsigned char addr_h,unsigned char addr_l,unsigned char is_read);
unsigned char i2c_send_rand_bit_addr(I2C_BUS *bus,unsigned char *addr,unsigned char len,unsigned char is_read);



#endif /* I2C_BUS_H_ */
