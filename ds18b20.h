/*
 * ds18b20.h
 *
 *  Created on: 2020年12月11日
 *      Author: tom
 */

#ifndef DS18B20_H_
#define DS18B20_H_

#include "one_wire_bus.h"

#define DS18B20_CONVERT_T 0x44
#define DS18B20_WRITE_SCRATCHPAD 0x4E
#define DS18B20_READ_SCRATCHPAD 0xbe
#define DS18B20_COPY_SCRATCHPAD 0x48
#define DS18B20_RECALL_E2 0xb8
#define DS18B20_READ_POWER_SUPPLY 0xb4

void ds18b20_convert_t();
void ds18b20_write_scratchpad(unsigned char TL,unsigned char TH,unsigned CFG);
void ds18b20_read_scratchpad(unsigned char *result,unsigned char len);
void ds18b20_copy_scratchpad();
void ds18b20_recall_e2();
unsigned char ds18b20_read_power_supply();

#endif /* DS18B20_H_ */
