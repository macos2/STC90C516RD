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


unsigned char one_wire_bus_present(gpio bus);
void one_wire_bus_write(gpio bus,unsigned char value);
unsigned char one_wire_bus_read(gpio bus);

/*
 * search the device rom on the bus
 * the result will save in the *result which has alloced more than n_rom x 8byte memory.
 * n_rom limit the number of device of searching.
 */
void one_wire_bus_search_rom(gpio bus,unsigned *result,unsigned char n_rom);

#endif /* 1WIRE_BUS_H_ */
