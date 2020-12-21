/*
 * i2c_memory.h
 *
 *  Created on: 2020年12月20日
 *      Author: tom
 */

#ifndef I2C_MEMORY_H_
#define I2C_MEMORY_H_
#include <stdarg.h>
#include "i2c_bus.h"
typedef struct{
	I2cBus *bus;
	unsigned char page_size;
	unsigned char dev_addr;
}I2cMemory;

unsigned char i2c_memory_read(I2cMemory *mem,unsigned int addr,unsigned char buf_size,unsigned char *buf);
unsigned char i2c_memory_write(I2cMemory *mem,unsigned int addr,unsigned char buf_size,unsigned char *buf);


#endif /* I2C_MEMORY_H_ */
