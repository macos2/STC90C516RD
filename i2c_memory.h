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

#define I2C_MEMORY_8_BIT_ADDRESS 0
#define I2C_MEMORY_16_BIT_ADDRESS 1
#define I2C_MEMORY_24_BIT_ADDRESS 2
#define I2C_MEMORY_32_BIT_ADDRESS 3

typedef struct{
	I2cBus *bus;
	unsigned char page_size;
	unsigned char dev_addr;//example: 0x50
	unsigned char n_bit_addr;//example: I2C_MEMORY_8_BIT_ADDRESS
}I2cMemory;

void i2c_memory_set_rw_param(__xdata I2cMemory *mem,__xdata unsigned long addr,__xdata unsigned char buf_size,__xdata unsigned char *i2c_mem_buf);
unsigned char i2c_memory_read();
unsigned char i2c_memory_write();


#endif /* I2C_MEMORY_H_ */
