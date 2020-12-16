/*
 * ds1302.h
 *
 *  Created on: 2020年12月12日
 *      Author: tom
 */

#ifndef DS1302_H_
#define DS1302_H_

#include "gpio.h"

typedef struct {
	gpio sclk;
	gpio io;
	gpio rst;
}Ds1302;

void ds1302_write(Ds1302 *dev,unsigned is_ram_data,unsigned char addr,unsigned char data);
unsigned char ds1302_read(Ds1302 *dev, unsigned is_ram_data, unsigned char addr);

void ds1302_set_date(Ds1302 *dev,unsigned char year,unsigned char month,unsigned char day);
void ds1302_set_time(Ds1302 *dev,unsigned char hour,unsigned char minute,unsigned char second);


#endif /* DS1302_H_ */
