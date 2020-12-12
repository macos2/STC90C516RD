/*
 * ds18b20.c
 *
 *  Created on: 2020年12月11日
 *      Author: tom
 */


#include "ds18b20.h"

void ds18b20_convert_t(){
	unsigned int c=0xffff;
	one_wire_bus_write(DS18B20_CONVERT_T);
	while(c){
	c--;
	}
}

void ds18b20_write_scratchpad(unsigned char TL,unsigned char TH,unsigned CFG){
	unsigned char c=0xff;
	one_wire_bus_write(DS18B20_WRITE_SCRATCHPAD);
	one_wire_bus_write(TH);
	one_wire_bus_write(TL);
	one_wire_bus_write(CFG);
	while(c){
	c--;
	}
	one_wire_bus_present();


}
void ds18b20_read_scratchpad(unsigned char *result,unsigned char len){
	one_wire_bus_write(DS18B20_READ_SCRATCHPAD);
	if(len>9)len=9;
	while(len){
		*result=one_wire_bus_read();
		result++;
		len--;
	}
}
void ds18b20_copy_scratchpad(){
	one_wire_bus_write(DS18B20_COPY_SCRATCHPAD);
}
void ds18b20_recall_e2(){
	unsigned char c=0x00;
	one_wire_bus_write(DS18B20_RECALL_E2);
	while(c==0x00){
	c=one_wire_bus_read();
	}
}

unsigned char ds18b20_read_power_supply(){
	one_wire_bus_write(DS18B20_READ_POWER_SUPPLY);
return  one_wire_bus_read();
}
