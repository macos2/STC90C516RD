/*
 * ds1302.c
 *
 *  Created on: 2020年12月12日
 *      Author: tom
 */


#include "ds1302.h"

void ds1302_write(Ds1302 *dev,unsigned is_ram_data,unsigned char addr,unsigned char data){
unsigned char command=0x80,i;
if(is_ram_data)command+=0x40;
command|=(addr&0x1F)<<1;
gpio_set(dev->rst,0);
gpio_set(dev->io,0);
gpio_set(dev->sclk,0);

gpio_set(dev->rst,1);
for(i=0;i<8;i++){
	gpio_set(dev->io,0x01&(command>>i));
	gpio_set(dev->sclk,1);
	gpio_set(dev->sclk,0);
}
for(i=0;i<8;i++){
	gpio_set(dev->io,0x01&(data>>i));
	gpio_set(dev->sclk,1);
	gpio_set(dev->sclk,0);
}
gpio_set(dev->rst,0);
}

void ds1302_read(Ds1302 *dev,unsigned is_ram_data,unsigned char *buf,unsigned char addr,unsigned char len){
unsigned char command=0x81,i;
if(is_ram_data)command+=0x40;
command|=(addr&0x1F)<<1;

gpio_set(dev->rst,0);
gpio_set(dev->io,0);
gpio_set(dev->sclk,0);

gpio_set(dev->rst,1);
for(i=0;i<8;i++){
	gpio_set(dev->sclk,0);
	gpio_set(dev->io,0x01&(command>>i));
	gpio_set(dev->sclk,1);

}
while(len){
for(i=0;i<8;i++){
	gpio_set(dev->sclk,0);
	*buf|=(gpio_get(dev->io)<<i);
	gpio_set(dev->sclk,1);
}
len--;
buf++;
}
gpio_set(dev->rst,0);
}

void ds1302_set_date(Ds1302 *dev,unsigned char year,unsigned char month,unsigned char day){

}

void ds1302_set_time(Ds1302 *dev,unsigned char hour,unsigned char minute,unsigned char second){

}
