/*
 * spi_sd.h
 *
 *  Created on: 2020年12月23日
 *      Author: tom
 */

#ifndef SPI_SD_H_
#define SPI_SD_H_

#include "spi_bus.h"
//#define sd_crc7 0b10001001 // G(x) = x7+ x3+ 1
//#define sd_crc16 0b10001000000100001 // G(x) = x16+x12+x5+1

#define sd_crc7_lsh7 0b100010010000000 // G(x) = x7+ x3+ 1
#define sd_crc16_lsh15 0b10001000000100001000000000000000 // G(x) = x16+x12+x5+1


typedef struct {
	SpiBus *spi;
	unsigned int block_size;//16bit up to 64Kib
	unsigned long cap;//32bit up to 4096Mib
}SpiSd;

unsigned char crc7_calc(unsigned int d);
unsigned int crc16_calc(unsigned long d);
unsigned char spi_sd_init();
unsigned long spi_sd_write();
unsigned long spi_sd_read();
#endif /* SPI_SD_H_ */
