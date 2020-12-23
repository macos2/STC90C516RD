/*
 * spi_sd.h
 *
 *  Created on: 2020年12月23日
 *      Author: tom
 */

#ifndef SPI_SD_H_
#define SPI_SD_H_

#define sd_crc7 0b10001001 // G(x) = x7+ x3+ 1
#define sd_crc16 0b10001000000100001 // G(x) = x16+x12+x5+1

unsigned char crc7_calc(unsigned int d);
unsigned int crc16_calc(unsigned long d);
#endif /* SPI_SD_H_ */
