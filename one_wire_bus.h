/*
 * 1wire_bus.h
 *
 *  Created on: 2020年11月30日
 *      Author: tom
 */
#ifndef ONE_WIRE_BUS_H_
#define ONE_WIRE_BUS_H_
#include "gpio.h"

#define ONE_WIRE_SEARCH_ROM 0xF0
#define ONE_WIRE_READ_ROM 0x33
#define ONE_WIRE_MATCH_ROM 0x55
#define ONE_WIRE_SKIP_ROM 0xCC
#define ONE_WIRE_ALARM_SEARCH 0xEC
#define one_wire_bus P35


unsigned char one_wire_bus_present();
void one_wire_bus_write(unsigned char value);
unsigned char one_wire_bus_read();

/*
 * search the device rom on the bus
 *
 * alarm_search !=0 perform alarm searching.
 * *result will save in the search result , which point to the memory has alloced more than n_rom x 8 byte .
 * n_rom limit the number of device of searching.
 *
 * return the number of devices which has been found.
 */


unsigned char one_wire_bus_search_rom(unsigned char alarm_search ,unsigned char *result,unsigned char n_rom);
void one_wire_bus_read_rom(unsigned char *result);
void one_wire_bus_match_rom(unsigned char *rom);

#endif /* 1WIRE_BUS_H_ */
