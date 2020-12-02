/*
 * one_wire_bus.c
 *
 *  Created on: 2020年11月30日
 *      Author: tom
 */


#include "one_wire_bus.h"

void one_wire_delay(unsigned int value){
	while(value>0)value--;
}

unsigned char one_wire_bus_present(gpio bus){
	unsigned char result=0x00;
	gpio_set(bus,0);
	one_wire_delay(480);
	gpio_set(bus,1);
	one_wire_delay(15);
	result=gpio_get(bus);
	return result;
}

void one_wire_bus_write(gpio bus,unsigned char value){
	unsigned char i=8;
	while(i>0){
		gpio_set(bus,0);
		one_wire_delay(15);
		gpio_set(bus,value&0x01);
		one_wire_delay(45);
		value=value>>1;
		gpio_set(bus,1);
		i--;
	}
}

unsigned char one_wire_bus_read(gpio bus){
	unsigned char i=8;
	unsigned char result=0x00;
	while(i>0){
		result=result<<1;
		gpio_set(bus,0);
		one_wire_delay(1);
		gpio_set(bus,1);
		one_wire_delay(14);
		result+=gpio_get(bus)&0x01;
		//one_wire_delay(45);
		i--;
	}
	return result;
}

void one_wire_bus_search_rom(gpio bus,unsigned *result,unsigned char n_rom){
	unsigned char fork,bit;
	while(n_rom){
		one_wire_bus_present(bus);
		one_wire_bus_write(bus,ONE_WIRE_SEARCH_ROM);
		for(bit=0;bit<64;bit++){
			gpio_set(bus,0);
			one_wire_delay(1);
			gpio_set(bus,1);
			one_wire_delay(14);

		}

	}
}
