/*
 * spi_memory.c
 *
 *  Created on: 2020年12月21日
 *      Author: tom
 */


#include "spi_memory.h"

void spi_memory_read(SpiMemory *mem,,unsigned char buf,unsigned char buf_len){
	unsigned int i;
spi_set_cs(mem->bus,0);
spi_write(mem->bus,SPI_MEMORY_READ);
for(i=0;i<buf_len;i++){

}
spi_set_cs(mem->bus,1);
}

void spi_memory_write(SpiMemory *mem,unsigned char buf,unsigned char buf_len){

}
