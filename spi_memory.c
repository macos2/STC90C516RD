/*
 * spi_memory.c
 *
 *  Created on: 2020年12月21日
 *      Author: tom
 */

#include "spi_memory.h"
#define WRITE_OVERTIME 10

void spi_memory_read(SpiMemory *mem, unsigned long addr, unsigned char *buf,
		unsigned int buf_len) {
	unsigned int i, *p = &addr;
	spi_set_cs(mem->bus, 0);
	spi_write(mem->bus, SPI_MEMORY_READ);

	for (i = 0; i <= mem->n_bit_address; i++) {
		if (mem->bus->MSB_FIRST)
			spi_write(mem->bus, p[mem->n_bit_address - i]);
		else
			spi_write(mem->bus, p[i]);
	}

	for (i = 0; i < buf_len; i++) {
		buf[i] = spi_read(mem->bus);
	}

	spi_set_cs(mem->bus, 1);
}

unsigned int spi_memory_write(SpiMemory *mem, unsigned long addr, unsigned char *buf,
		unsigned int buf_len) {
	unsigned char i,j=0, *p = &addr;
	spi_set_cs(mem->bus, 0);

	do{
		spi_write(mem->bus,SPI_MEMORY_READ_STATUS);
		i=spi_read(mem->bus);
		j++;
	}while((i&0x01)&&(j<=WRITE_OVERTIME));

	if(j==WRITE_OVERTIME)return 0;

	spi_set_cs(mem->bus, 1);
	spi_set_cs(mem->bus, 0);
	spi_write(mem->bus, SPI_MEMORY_WRITE_ENABLE);
	spi_set_cs(mem->bus, 1);
	spi_set_cs(mem->bus, 0);
	spi_write(mem->bus, SPI_MEMORY_WRITE);

	for (i = 0; i <= mem->n_bit_address; i++) {
		if (mem->bus->MSB_FIRST)
			spi_write(mem->bus, p[mem->n_bit_address - i]);
		else
			spi_write(mem->bus, p[i]);
	}

	j=buf_len>mem->page_size?mem->page_size:buf_len;
	for (i = 0; i < j; i++) {
		spi_write(mem->bus, buf[i]);
	}

	spi_set_cs(mem->bus, 1);

	if(buf_len>mem->page_size){
		spi_memory_write(mem,addr+mem->page_size,buf+mem->page_size,buf_len-mem->page_size);
	}
	return buf_len;
}
