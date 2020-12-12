/*
 * ds18b20.c
 *
 *  Created on: 2020年12月11日
 *      Author: tom
 */

#include "ds18b20.h"

void ds18b20_convert_t() {
	one_wire_bus_write(DS18B20_CONVERT_T);
}

void ds18b20_write_scratchpad(unsigned char TL, unsigned char TH, unsigned CFG) {
	one_wire_bus_write(DS18B20_WRITE_SCRATCHPAD);
	one_wire_bus_write(TH);
	one_wire_bus_write(TL);
	one_wire_bus_write(CFG);
	one_wire_bus_present();
}

void ds18b20_read_scratchpad(unsigned char *result, unsigned char len) {
	one_wire_bus_write(DS18B20_READ_SCRATCHPAD);
	if (len > 9)
		len = 9;
	while (len) {
		*result = one_wire_bus_read();
		result++;
		len--;
	}
}
void ds18b20_copy_scratchpad() {
	one_wire_bus_write(DS18B20_COPY_SCRATCHPAD);
}
void ds18b20_recall_e2() {
	unsigned char c = 0x00;
	one_wire_bus_write(DS18B20_RECALL_E2);
	while (c == 0x00) {
		c = one_wire_bus_read();
	}
}

unsigned char ds18b20_read_power_supply() {
	one_wire_bus_write(DS18B20_READ_POWER_SUPPLY);
	return one_wire_bus_read();
}

unsigned char ds18b20_temperature_to_string(unsigned char *value, char *string) {
	unsigned char i = 0;
	unsigned int *temp = value, t = 0;
	if (*temp & 0x8000) {
		string[i] = '-';
		i++;
		*temp = ~(*temp - 1);
	}
	if (*temp & 0x01)
		t += 625;
	if (*temp & 0x02)
		t += 1250;
	if (*temp & 0x04)
		t += 2500;
	if (*temp & 0x08)
		t += 5000;
	i += sprintf(string + i, "%d.", *temp >> 4);
	i += sprintf(string+i,"%04d",t);
	return i;

}
