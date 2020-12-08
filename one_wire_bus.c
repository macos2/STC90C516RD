/*
 * one_wire_bus.c
 *
 *  Created on: 2020年11月30日
 *      Author: tom
 */


#include "one_wire_bus.h"

unsigned int __delay;
//could not use the default gpio api function since the latch is so big
#define one_wire_bus P37

//ONE_WIRE_DELAY(1) =13.5us
#define ONE_WIRE_DELAY(x) __delay=x;while(__delay--);

unsigned char one_wire_bus_present(){
	unsigned char result=0x01;
	one_wire_bus=0;
	ONE_WIRE_DELAY(38);
	one_wire_bus=1;
	ONE_WIRE_DELAY(1);
	result=one_wire_bus;
	while(!one_wire_bus);
	return result;
}

void one_wire_bus_write(unsigned char value){
	unsigned char i=8;
	one_wire_bus=1;
	while(i>0){
		if(value&0x01){//write 1
			one_wire_bus=0;
			ONE_WIRE_DELAY(0);
			one_wire_bus=1;
			ONE_WIRE_DELAY(1);
		}else{//write 0
			one_wire_bus=0;
			ONE_WIRE_DELAY(2);
		}
		one_wire_bus=1;
		value=value>>1;
		i--;
	}
}

unsigned char one_wire_bus_read(){
	unsigned char i=8;
	unsigned char result=0x00;
	while(i>0){
		result=result>>1;
		one_wire_bus=0;
		one_wire_bus=1;
		ONE_WIRE_DELAY(0);
		if(one_wire_bus)result|=0x80;
		//one_wire_delay(45);
		i--;
	}
	return result;
}

unsigned char get_bit(unsigned bit,unsigned char *d){
while(bit>=8){
	bit=bit-8;
	d=d+1;
}
return *d&(0x01<<bit);
}

void set_bit(unsigned bit,unsigned char *d){
while(bit>=8){
	bit=bit-8;
	d=d+1;
}
*d=*d|(0x01<<bit);
}

void reset_bit(unsigned bit,unsigned char *d){
while(bit>=8){
	bit=bit-8;
	d=d+1;
}
*d=*d&~(0x01<<bit);
}

void one_wire_bus_search_rom(gpio bus,unsigned char *result,unsigned char n_rom){
	unsigned char bit,tmp,act=0;
	unsigned char fork[8],*f;
	while(n_rom){
		one_wire_bus_present();
		one_wire_bus_write(ONE_WIRE_SEARCH_ROM);
		for(bit=0;bit<64;bit++){
			//get first bit
			gpio_set(bus,0);
			ONE_WIRE_DELAY(0);
			gpio_set(bus,1);
			ONE_WIRE_DELAY(0);
			tmp=gpio_get(bus);
			tmp=tmp<<1;

			//get secone bit
			gpio_set(bus,0);
			ONE_WIRE_DELAY(0);
			gpio_set(bus,1);
			ONE_WIRE_DELAY(0);
			tmp+=gpio_get(bus);

			//decide
			switch(tmp)
			{
			case 0b00:
				if(fork[0]>bit){
					//this fork have been detected before
					act=1;
				}else{
					//this fork have never been detected before
					act=0;
					fork[0]=bit;
				}
				//there is a fork of this bit.
				break;
			case 0b01:
				//the bit is sure is 0.
				act=0;
				break;
			case 0b10:
				//the bit is sure is 1.
				act=1;
				break;
			default:
				//0b11
				//there is no device on the bus
				break;
			}
			gpio_set(bus,0);
			ONE_WIRE_DELAY(15);
			gpio_set(bus,act);
			ONE_WIRE_DELAY(45);
			gpio_set(bus,1);
			//result
		}

	}
}
