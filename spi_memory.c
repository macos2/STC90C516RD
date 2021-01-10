/*
 * spi_memory.c
 *
 *  Created on: 2020年12月21日
 *      Author: tom
 */

#include "spi_memory.h"
#define WRITE_OVERTIME 10

__xdata SpiMemory *__spi_mem;
__xdata unsigned long __spi_mem_addr;
__xdata unsigned char *__spi_mem_buf;
__xdata	unsigned int __spi_mem_buf_len;

void spi_memory_read(__xdata SpiMemory *mem,__xdata unsigned long addr,__xdata unsigned char *buf,__xdata unsigned int buf_len){
	__spi_mem=mem;
	__spi_mem_addr=addr;
	__spi_mem_buf=buf;
	__spi_mem_buf_len=buf_len;
	unsigned int *p = &__spi_mem_addr;
	unsigned int  i;
	spi_set_cs(__spi_mem->bus, 0);
	spi_write(__spi_mem->bus, SPI_MEMORY_READ);

	for (i = 0; i <= __spi_mem->n_bit_address; i++) {
		if (__spi_mem->bus->MSB_FIRST)
			spi_write(__spi_mem->bus, p[__spi_mem->n_bit_address - i]);
		else
			spi_write(__spi_mem->bus, p[i]);
	}

	for (i = 0; i < __spi_mem_buf_len; i++) {
		__spi_mem_buf[i] = spi_read(__spi_mem->bus);
	}

	spi_set_cs(__spi_mem->bus, 1);
}

unsigned int spi_memory_write(__xdata SpiMemory *mem,__xdata unsigned long addr,__xdata unsigned char *buf,__xdata unsigned int buf_len) {
	__spi_mem=mem;
	__spi_mem_addr=addr;
	__spi_mem_buf=buf;
	__spi_mem_buf_len=buf_len;
	unsigned char *p = &__spi_mem_addr;
	unsigned int  i,j=0;
	spi_set_cs(__spi_mem->bus, 0);

	do{
		spi_write(__spi_mem->bus,SPI_MEMORY_READ_STATUS);
		i=spi_read(__spi_mem->bus);
		j++;
	}while((i&0x01)&&(j<=WRITE_OVERTIME));

	if(j==WRITE_OVERTIME)return 0;

	spi_set_cs(__spi_mem->bus, 1);
	spi_set_cs(__spi_mem->bus, 0);
	spi_write(__spi_mem->bus, SPI_MEMORY_WRITE_ENABLE);
	spi_set_cs(__spi_mem->bus, 1);
	spi_set_cs(__spi_mem->bus, 0);
	spi_write(__spi_mem->bus, SPI_MEMORY_WRITE);

	for (i = 0; i <= __spi_mem->n_bit_address; i++) {
		if (__spi_mem->bus->MSB_FIRST)
			spi_write(__spi_mem->bus, p[__spi_mem->n_bit_address - i]);
		else
			spi_write(__spi_mem->bus, p[i]);
	}

	j=__spi_mem_buf_len>__spi_mem->page_size?__spi_mem->page_size:__spi_mem_buf_len;
	for (i = 0; i < j; i++) {
		spi_write(__spi_mem->bus, __spi_mem_buf[i]);
	}

	spi_set_cs(__spi_mem->bus, 1);

	if(__spi_mem_buf_len>__spi_mem->page_size){
		spi_memory_write(__spi_mem,__spi_mem_addr+__spi_mem->page_size,__spi_mem_buf+__spi_mem->page_size,__spi_mem_buf_len-__spi_mem->page_size);
	}
	return __spi_mem_buf_len;
}
