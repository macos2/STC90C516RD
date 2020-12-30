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

#define sd_crc7_lsh8 0b1000100100000000 // G(x) = x7+ x3+ 1 <<8  0x9876=4B 0x1234=20
#define sd_crc16_lsh15 0b10001000000100001000000000000000 // G(x) = x16+x12+x5+1 <<15

//r1 1byte read as unsigned char
#define spi_sd_r1_idle_state 0x01
#define spi_sd_r1_erase_reset 0x02
#define spi_sd_r1_illegal_cmd 0x04
#define spi_sd_r1_crc_error 0x08
#define spi_sd_r1_erase_seq_error 0x10
#define spi_sd_r1_address_error 0x20
#define spi_sd_r1_parameter_error 0x40

//r2 the second byte after r1
#define spi_sd_r2_card_locked 0x01
#define spi_sd_r2_wp_skip 0x02
#define spi_sd_r2_error 0x04
#define spi_sd_r2_cc_error 0x08
#define spi_sd_r2_card_ecc_failed 0x10
#define spi_sd_r2_wp_violation 0x20
#define spi_sd_r2_earse_param 0x40
#define spi_sd_r2_out_range_csd_overwrite 0x80


typedef struct {
	SpiBus *spi;
	unsigned int block_size;//16bit up to 64Kib
	unsigned long cap;//32bit up to 4096Mib
	unsigned version;
	unsigned char is_SDSD;
}SpiSd;

void spi_sd_gen_command(unsigned char cmd,unsigned char *args,unsigned char *result);
unsigned char spi_sd_send_command(SpiSd *sd,unsigned char cmd,unsigned char *args);
unsigned char spi_sd_send_app_command(SpiSd *sd,unsigned char cmd,unsigned char *args);
unsigned char spi_sd_init(SpiSd *sd,unsigned int block_size_for_sdsd_mmc,unsigned char crc_off);
unsigned long spi_sd_write(SpiSd *sd);
unsigned long spi_sd_read(SpiSd *sd,unsigned long block_addr,unsigned char *buf,unsigned long num_block);

extern void usart_send(char *fmt, ...) ;
#endif /* SPI_SD_H_ */
