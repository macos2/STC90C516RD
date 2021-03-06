/*
 * i2c_bus.h
 *
 *  Created on: 2020年12月17日
 *      Author: tom
 */

#ifndef I2cBus_H_
#define I2cBus_H_

#include "gpio.h"

typedef struct {
	gpio sck;
	gpio sda;
} I2cBus;

/*
 * The Standard I2C Operation
 * @param bus:the I2C bus
 */
void i2c_start(I2cBus *bus);
void i2c_stop(I2cBus *bus);
void i2c_reset(I2cBus *bus);

/*
 * The Standard I2C Operation
 * @param bus:the I2C bus
 * @param no_ack:don't ack after receive the data
 * @return the data receive from the bus
 */
unsigned char i2c_read(I2cBus *bus, bool no_ack);

/*
 * The Standard I2C Operation
 * @param bus:the I2C bus
 * @param data:the data write to the bus
 * @return	0 indicate received the ack signal
 * 			1 indicate haven't received the ack signal
 */
unsigned char i2c_write(I2cBus *bus, unsigned char data);

/*
 * @param bus:the I2C bus
 * @param addr:the 7bit or 10bit device address
 * @return ack:0 for ack , 1 for no ack.
 */
unsigned char i2c_send_7bit_addr(I2cBus *bus, unsigned char addr,
		bool is_read);
unsigned char i2c_send_10bit_addr(I2cBus *bus, unsigned char addr_h,
		unsigned char addr_l, bool is_read);
//unsigned char i2c_send_rand_bit_addr(I2cBus *bus,unsigned char *addr,unsigned char len,unsigned char is_read);


/*
 * @param bus:the I2C bus
 * @param result:the alloced buffer for saving the 7bit address of the device which have been detected.
 * @param num:the buffer size
 * @return the number of devices have been detected.
 */
unsigned char i2c_scan_7bit_addr_dev(I2cBus *bus, unsigned char *result,
		unsigned char num);

/*
 * @param bus:the I2C bus
 * @param result:the alloced buffer for saving the 10bit address of the device which have been detected.
 * @param num:the buffer size
 * @return the number of devices have been detected.
 */
unsigned char i2c_scan_10bit_addr_dev(I2cBus *bus, unsigned int *result,
		unsigned int num);

#endif /* I2cBus_H_ */
