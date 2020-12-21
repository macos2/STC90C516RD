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

#define SPI_MEMORY_8_BIT_ADDRESS 8
#define SPI_MEMORY_9_BIT_ADDRESS 9
#define SPI_MEMORY_16_BIT_ADDRESS 16
#define SPI_MEMORY_24_BIT_ADDRESS 24

typedef struct{
	SpiBus *bus;
	unsigned char page_size;
	unsigned char spi_n_bit_address;
}SpiMemory;

void spi_memory_read(SpiMemory *mem,,unsigned char buf,unsigned char buf_len);
void spi_memory_write(SpiMemory *mem,unsigned char buf,unsigned char buf_len);

#endif /* SPI_MEMORY_H_ */
