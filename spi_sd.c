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
	unsigned int temp=d,p=0x8000,crc=sd_crc7_lsh8;
	while(i<8){
		if(p&temp){
			temp=temp^crc;
		}
		crc=crc>>1;
		p=p>>1;
		i++;
	}
	return temp;
}
unsigned char crc7_calc_end(unsigned int d){
	unsigned char i=0;
	unsigned int temp=d,p=0x4000,crc=sd_crc7_lsh8>>1;
	while(i<8){
		if(p&temp){
			temp=temp^crc;
		}
		crc=crc>>1;
		p=p>>1;
		i++;
	}
	return temp;
}

unsigned long crc16_calc(unsigned long d){
	unsigned char i=0;
	unsigned long temp=d,p=0x80000000,crc=sd_crc16_lsh15;
	while(i<15){
		if(p&temp){
			temp=temp^crc;
		}
		crc=crc>>1;
		p=p>>1;
		i++;
	}
	if(temp&0x10000)temp=temp^crc;
	return temp;
}

unsigned long crc16_calc_end(unsigned long d){
	unsigned char i=0;
	unsigned long temp=d,p=0x80000000,crc=sd_crc16_lsh15;
	while(i<15){
		if(p&temp){
			temp=temp^crc;
		}
		crc=crc>>1;
		p=p>>1;
		i++;
	}
	if(temp&0x10000)temp=temp^crc;
	return temp;
}

unsigned long crc16_calc_debug(unsigned long d,unsigned char num){
	unsigned char i=0;
	unsigned long temp=d,p=0x80000000,crc=sd_crc16_lsh15;
	while(i<num&&i<15){
		if(p&temp){
			temp=temp^crc;
		}
		crc=crc>>1;
		p=p>>1;
		i++;
	}
	while(i<num){
		if(i!=15)temp=temp<<1;
		if(temp&0x10000)temp=temp^crc;
		i++;
	}
	return temp;
}
