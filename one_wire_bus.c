/*
 * one_wire_bus.c
 *
 *  Created on: 2020年11月30日
 *      Author: tom
 */


#include "one_wire_bus.h"

unsigned char delay;
//could not use the default gpio api function since the latch is so big
#define one_wire_bus P34

//ONE_WIRE_DELAY(1) =13.5us
//#define ONE_WIRE_DELAY(x) __delay=x;while(__delay--);

//1 unit=2.2us
void ONE_WIRE_DELAY(unsigned char x) __naked
{
	delay=x;
	__asm
	00001$:
	DJNZ _delay , 00001$
	RET
	__endasm;
}

unsigned char one_wire_bus_present(){
	unsigned char result=0x01;
	one_wire_bus=0;
	ONE_WIRE_DELAY(218);
	one_wire_bus=1;
	ONE_WIRE_DELAY(19);
	result=one_wire_bus;
	ONE_WIRE_DELAY(50);
	return result;
}

void one_wire_bus_write(unsigned char value){
	unsigned char i=8;
	one_wire_bus=1;
	while(i>0){
		if(value&0x01){//write 1
			one_wire_bus=0;
			//ONE_WIRE_DELAY(1);
			one_wire_bus=1;
			ONE_WIRE_DELAY(5);
		}else{//write 0
			one_wire_bus=0;
			ONE_WIRE_DELAY(10);
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
		ONE_WIRE_DELAY(5);
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



void one_wire_bus_search_rom(unsigned char *result,unsigned char n_rom){
	unsigned char bit,tmp,act=0;
	unsigned char i=0,*p=result;//record the position of the result
	unsigned char map=0x00,log_map=0x00,mix=0;//record the decision of re-act in the rom mix position
	while(n_rom){
		one_wire_bus_present();
		one_wire_bus_write(ONE_WIRE_SEARCH_ROM);
		map=log_map<<(8-mix);
		log_map=0xff;
		mix=0;
		for(bit=0;bit<64;bit++){
			tmp=0x00;
			//get first bit
			one_wire_bus=0;
			one_wire_bus=1;
			ONE_WIRE_DELAY(5);
			tmp=one_wire_bus;
			tmp=tmp<<1;

			ONE_WIRE_DELAY(10);
			//get secone bit
			one_wire_bus=0;
			one_wire_bus=1;
			ONE_WIRE_DELAY(5);
			tmp+=one_wire_bus;

			//decide
			switch(tmp)
			{
			case 0b00:
				act=map&0x80?0x01:0x00;
				map=map<<1;
				mix++;
				log_map=(log_map<<1)+act;
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
				return;
				break;
			}

			//save the bit of the rom
			*p=*p|(act<<i);
			i++;
			if(i>=8){p++;i=0;}

			//reaction to the bus
			if(act){//write 1
				one_wire_bus=0;
				//ONE_WIRE_DELAY(1);
				one_wire_bus=1;
				ONE_WIRE_DELAY(5);
			}else{//write 0
				one_wire_bus=0;
				ONE_WIRE_DELAY(10);
			}
			one_wire_bus=1;
		}
		if(log_map==0xff)return;//there is no other device for search
		log_map++;//new map for next search rom
		n_rom--;
	}
}
