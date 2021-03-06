/*
 * one_wire_bus.c
 *
 *  Created on: 2020年11月30日
 *      Author: tom
 */

#include "one_wire_bus.h"

unsigned char delay;
//could not use the default gpio api function since the latch is so big

//ONE_WIRE_DELAY(1) =13.5us
//#define ONE_WIRE_DELAY(x) __delay=x;while(__delay--);

//1 unit=2.2us
//#define ONE_WIRE_DELAY_5_US __asm__("nop\nnop\nnop\nnop\nnop\n")
//#define ONE_WIRE_DELAY_10_US __asm__("nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n")
//#define ONE_WIRE_DELAY_480_US ONE_WIRE_DELAY(218)
//#define ONE_WIRE_DELAY_37_US ONE_WIRE_DELAY(19);
//#define  ONE_WIRE_DELAY_100_US ONE_WIRE_DELAY(50)

//1 unit=1/6us
#define ONE_WIRE_DELAY_1_US __asm__("nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop")
#define ONE_WIRE_DELAY_5_US ONE_WIRE_DELAY_1_US;ONE_WIRE_DELAY_1_US;ONE_WIRE_DELAY_1_US;ONE_WIRE_DELAY_1_US;ONE_WIRE_DELAY_1_US
#define ONE_WIRE_DELAY_10_US ONE_WIRE_DELAY_5_US;ONE_WIRE_DELAY_5_US
#define ONE_WIRE_DELAY_37_US ONE_WIRE_DELAY_10_US;ONE_WIRE_DELAY_10_US;ONE_WIRE_DELAY_10_US;ONE_WIRE_DELAY_5_US;ONE_WIRE_DELAY_1_US
#define ONE_WIRE_DELAY_100_US ONE_WIRE_DELAY_10_US;ONE_WIRE_DELAY_10_US;ONE_WIRE_DELAY_10_US;ONE_WIRE_DELAY_10_US;ONE_WIRE_DELAY_10_US;ONE_WIRE_DELAY_10_US;ONE_WIRE_DELAY_10_US;ONE_WIRE_DELAY_10_US;ONE_WIRE_DELAY_10_US;ONE_WIRE_DELAY_10_US
#define ONE_WIRE_DELAY_480_US ONE_WIRE_DELAY_100_US;ONE_WIRE_DELAY_100_US;ONE_WIRE_DELAY_100_US;ONE_WIRE_DELAY_100_US;ONE_WIRE_DELAY_37_US;ONE_WIRE_DELAY_37_US

void ONE_WIRE_DELAY(unsigned char x)
__naked
{
	delay=x;
	__asm
	00001$:
	DJNZ _delay , 00001$
	RET
	__endasm;
}

unsigned char one_wire_bus_present() {
	unsigned char result = 0x01;
	one_wire_bus = 0;
	ONE_WIRE_DELAY_480_US;
	one_wire_bus = 1;
	ONE_WIRE_DELAY_37_US;
	result = one_wire_bus;
	ONE_WIRE_DELAY_100_US;
	ONE_WIRE_DELAY_37_US;
	ONE_WIRE_DELAY_10_US;
	return result;
}

void one_wire_bus_write(unsigned char value) {
	unsigned char i = 8;
	one_wire_bus = 1;
	while (i > 0) {
		if (value & 0x01) { //write 1
			one_wire_bus = 0;
			ONE_WIRE_DELAY_1_US;
			one_wire_bus = 1;
			ONE_WIRE_DELAY_10_US;
			ONE_WIRE_DELAY_10_US;
			ONE_WIRE_DELAY_10_US;
		} else { //write 0
			one_wire_bus = 0;
			ONE_WIRE_DELAY_10_US;
			ONE_WIRE_DELAY_10_US;
			ONE_WIRE_DELAY_10_US;
		}
		one_wire_bus = 1;
		value = value >> 1;
		i--;
	}
}

unsigned char one_wire_bus_read() {
	unsigned char i = 8;
	unsigned char result = 0x00;
	while (i > 0) {
		result = result >> 1;
		one_wire_bus = 0;
		ONE_WIRE_DELAY_1_US;
		one_wire_bus = 1;
		ONE_WIRE_DELAY_10_US;
		if (one_wire_bus)
			result |= 0x80;
		i--;
		ONE_WIRE_DELAY_10_US;
		ONE_WIRE_DELAY_10_US;
		ONE_WIRE_DELAY_5_US;
	}
	return result;
}

unsigned char one_wire_bus_search_rom(unsigned char alarm_search,
		unsigned char *result, unsigned char n_rom) {
	unsigned char bit, tmp, act = 0, n = n_rom;
	unsigned char i = 0, *p = result; //record the position of the result
	unsigned char map = 0x00, log_map = 0x00, mix = 0; //record the decision of re-act in the rom mix position
	while (n_rom) {
		one_wire_bus_present();

		if (alarm_search)
			one_wire_bus_write(ONE_WIRE_ALARM_SEARCH);
		else
			one_wire_bus_write(ONE_WIRE_SEARCH_ROM);
		map = log_map << (8 - mix);
		log_map = 0xff;
		mix = 0;
		for (bit = 0; bit < 64; bit++) {
			tmp = 0x00;
			//get first bit
			one_wire_bus = 0;
			ONE_WIRE_DELAY_1_US;
			one_wire_bus = 1;
			ONE_WIRE_DELAY_10_US;
			tmp = one_wire_bus;
			tmp = tmp << 1;
			ONE_WIRE_DELAY_10_US;
			ONE_WIRE_DELAY_10_US;
			ONE_WIRE_DELAY_5_US;

			//get secone bit
			one_wire_bus = 0;
			ONE_WIRE_DELAY_1_US;
			one_wire_bus = 1;
			ONE_WIRE_DELAY_10_US;
			tmp += one_wire_bus;
			ONE_WIRE_DELAY_10_US;
			ONE_WIRE_DELAY_10_US;
			ONE_WIRE_DELAY_5_US;

			//decide
			switch (tmp) {
			case 0b00:
				act = map & 0x80 ? 0x01 : 0x00;
				map = map << 1;
				mix++;
				log_map = (log_map << 1) + act;
				//there is a fork of this bit.
				break;
			case 0b01:
				//the bit is sure is 0.
				act = 0;
				break;
			case 0b10:
				//the bit is sure is 1.
				act = 1;
				break;
			default:
				//0b11
				//there is no device on the bus
				return n - n_rom;
				break;
			}

			//save the bit of the rom
			*p = *p | (act << i);
			i++;
			if (i >= 8) {
				p++;
				i = 0;
			}

			//reaction to the bus
			if (act) {				//write 1
				one_wire_bus = 0;
				//ONE_WIRE_DELAY(1);
				one_wire_bus = 1;
				ONE_WIRE_DELAY_10_US;
				ONE_WIRE_DELAY_10_US;
				ONE_WIRE_DELAY_10_US;
			} else {				//write 0
				one_wire_bus = 0;
				ONE_WIRE_DELAY_10_US;
				ONE_WIRE_DELAY_10_US;
				ONE_WIRE_DELAY_10_US;
			}
			one_wire_bus = 1;
		}
		if (log_map == 0xff)
			return n - n_rom + 1;		//there is no other device for search
		log_map++;				//new map for next search rom
		n_rom--;
	}
	return n - n_rom;
}

void one_wire_bus_read_rom(unsigned char *result) {
	unsigned char i;
	one_wire_bus_present();
	one_wire_bus_write(ONE_WIRE_READ_ROM);
	for (i = 0; i < 8; i++) {
		result[i] = one_wire_bus_read();
	}
}

void one_wire_bus_match_rom(unsigned char *rom) {
	unsigned char i;
	one_wire_bus_present();
	one_wire_bus_write(ONE_WIRE_MATCH_ROM);
	for (i = 0; i < 8; i++) {
		one_wire_bus_write(rom[i]);
	}
}

