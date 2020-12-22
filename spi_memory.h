/*
 * spi_memory.h
 *
 *  Created on: 2020年12月21日
 *      Author: tom
 */

#ifndef SPI_MEMORY_H_
#define SPI_MEMORY_H_

#include "spi_bus.h"

#define SPI_MEMORY_WRITE_ENABLE 0b00000110
#define SPI_MEMORY_WRITE_DISABLE 0b00000100
#define SPI_MEMORY_WRITE_STATUS 0b00000101
#define SPI_MEMORY_READ_STATUS 0b00000001
#define SPI_MEMORY_READ 0b00000011
#define SPI_MEMORY_WRITE 0b00000010

#define SPI_MEMORY_8_BIT_ADDRESS 0
//#define SPI_MEMORY_9_BIT_ADDRESS 10
#define SPI_MEMORY_16_BIT_ADDRESS 1
#define SPI_MEMORY_24_BIT_ADDRESS 2
#define SPI_MEMORY_32_BIT_ADDRESS 3

typedef struct{
	SpiBus *bus;
	unsigned int page_size;
	unsigned char n_bit_address;//example:SPI_MEMORY_8_BIT_ADDRESS
}SpiMemory;

void spi_memory_read(SpiMemory *mem,unsigned long addr,unsigned char buf,unsigned int buf_len);
void spi_memory_write(SpiMemory *mem,unsigned long addr,unsigned char buf,unsigned int buf_len);

#endif /* SPI_MEMORY_H_ */
