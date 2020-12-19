/*
 * spi_bus.c
 *
 *  Created on: 2020年12月19日
 *      Author: tom
 */


#include "spi_bus.h"

void spi_init(SpiBus *bus){
	gpio_set(bus->CS,1);
	gpio_set(bus->SCK,bus->CPOL);
}


void spi_write(SpiBus *bus,unsigned char data){
	unsigned char i,tmp;
gpio_set(bus->CS,0);
for(i=0;i<8;i++){
	if(bus->MSB_FIRST){
		tmp=(data&0x80)?0x01:0x00;
		data=data<<1;
	}else{
		tmp=data&0x01;
		data=data>>1;
	}

	if(bus->CPHA){
		gpio_set(bus->SCK,(~bus->CPOL)&0x01);
		gpio_set(bus->MOSI,tmp);
		gpio_set(bus->SCK,bus->CPOL);
	}else{
		gpio_set(bus->MOSI,tmp);
		gpio_set(bus->SCK,(~bus->CPOL)&0x01);
		gpio_set(bus->SCK,bus->CPOL);
	}
}
gpio_set(bus->CS,1);
}

unsigned char spi_read(SpiBus *bus){
	unsigned char i,tmp,result=0x00;
gpio_set(bus->CS,0);
for(i=0;i<8;i++){
	if(bus->CPHA){
		gpio_set(bus->SCK,(~bus->CPOL)&0x01);
		gpio_set(bus->SCK,bus->CPOL);
		tmp=gpio_get(bus->MISO);
	}else{
		gpio_set(bus->SCK,(~bus->CPOL)&0x01);
		tmp=gpio_get(bus->MISO);
		gpio_set(bus->SCK,bus->CPOL);
	}

	if(bus->MSB_FIRST){
		result|=tmp<<(7-i);
	}else{
		result|=tmp<<i;
	}
}
gpio_set(bus->CS,1);
return result;
}
