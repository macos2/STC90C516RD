/*
 * spi_sd.c
 *
 *  Created on: 2020年12月23日
 *      Author: tom
 */
#include "spi_sd.h"

__xdata unsigned char __sd_respon[17];
__xdata unsigned char __sd_cmd[6];

unsigned char crc7_calc(unsigned int d){
	unsigned char i=0;
	unsigned int temp=d,p=0x4000,crc=sd_crc7_lsh7;
	while(i<7){
		if(p&temp){
			temp=temp^crc;
		}
		crc=crc>>1;
		p=p>>1;
		i++;
	}
	return temp;
}

unsigned int crc16_calc(unsigned long d){
	unsigned char i=0;
	unsigned long temp=d,p=0x80000000,crc=sd_crc16_lsh15;
	crc=crc<<15;
	while(i<15){
		if(p&temp){
			temp=temp^crc;
		}
		crc=crc>>1;
		p=p>>1;
		i++;
	}
	return temp;
}
