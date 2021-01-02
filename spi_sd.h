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
	bool is_Version2;
	bool is_SDSD;
}SpiSd;

void spi_sd_gen_command(__xdata unsigned char cmd,__xdata unsigned char *args,__xdata unsigned char *result);
__xdata unsigned char spi_sd_send_command(__xdata SpiSd *sd,__xdata unsigned char cmd,__xdata unsigned char *args);
__xdata unsigned char spi_sd_send_app_command(__xdata SpiSd *sd,__xdata unsigned char cmd,__xdata unsigned char *args);
__xdata unsigned char spi_sd_init(__xdata SpiSd *sd,__xdata unsigned int block_size_for_sdsd_mmc,__xdata bool crc_off);

void spi_sd_set_rw_param(__xdata SpiSd *sd,__xdata unsigned long block_addr,__xdata unsigned char *sd_buf,__xdata unsigned int num_block);
//__xdata unsigned int spi_sd_read(__xdata SpiSd *sd,__xdata unsigned long block_addr,__xdata unsigned char *buf,__xdata unsigned int num_block);
//__xdata unsigned int spi_sd_write(__xdata SpiSd *sd,__xdata unsigned long block_addr,__xdata unsigned char *buf,__xdata unsigned int num_block);
unsigned int spi_sd_read();
unsigned int spi_sd_write();
extern void usart_send(char *fmt, ...) ;

#endif /* SPI_SD_H_ */
