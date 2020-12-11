/*
 * main.c
 *
 *  Created on: 2020年11月27日
 *      Author: tom
 */

#include "main.h"
__xdata char temp[256],temp2[256],ds18b20[256];
__xdata lm032l lcd;
__xdata lm032l lcd2;
__xdata unsigned char p;
__xdata unsigned char one_wire_result,scratchpad[9];

void one_wire_read_rom();
void one_wire_show_search_result();

void timer1_interrupt()
__interrupt 3 {
	SBUF='A';
}

void usart_interrupt()
__interrupt 4 {
	unsigned char tmp;
	if (RI == 1) {
		RI = 0;
		tmp = SBUF;
		lm032l_write_code(&lcd, LM032L_CLEAR);
		lm032l_write_code(&lcd2, LM032L_CLEAR);
		switch (tmp) {
			case '\r':
			temp[0] = '\0';
			p = 0;
			break;
			case '\b':
			if(p>0) {
				p--;
				temp[p] = '\0';
				lm032l_write_string(&lcd, 0x00,"                ", 16);
			}
			break;
			default:
			if (p < 254) {
				temp[p] = tmp;
				p++;
				temp[p] = '\0';
			}
			break;
		}
		if (p > 16) {
			lm032l_write_string(&lcd, 0x00, temp + p - 16, 16);
			lm032l_write_data(&lcd, 0x00, 0b01111111);
		} else {
			lm032l_write_string(&lcd, 0x00, temp, p);
		}
		SBUF = tmp;
		one_wire_read_rom();
	} else {
		TI = 0;
	}

}

void timer1_init() {
	TMOD = 0x20;
	TH1 = 253; //256-fosc(11059200)/12/32/baud(19200)*(2^PCON&0x80>>7)=253
	TL1 = 253;
	//ET1 = 1;
	TR1 = 1;
}

void usart_init() {
	SCON = 0x50;
	PCON = 0x80; //T1 foscx2
	ES = 1;
	EA = 1;
	p = 0;
	temp[p] = '\0';
}

void lcd_init() {
	lcd.DATA = gpio_format(1, GPIO_ALL_PIN);
	lcd.E = gpio_format(3, 3);
	lcd.RS = gpio_format(2, 6);
	lcd.RW = gpio_format(2, 5);
	lm032l_init(&lcd);

	lcd2.DATA = gpio_format(1, GPIO_ALL_PIN);
	lcd2.E = gpio_format(3, 2);
	lcd2.RS = gpio_format(2, 6);
	lcd2.RW = gpio_format(2, 5);
	lm032l_init(&lcd2);
}

void one_wire_read_rom(){
	one_wire_result=one_wire_bus_search_rom(0,ds18b20,6);
	one_wire_show_search_result();
}

void one_wire_show_search_result(){
	unsigned char i;
	temp2[255]=sprintf(temp2,"found %02d",one_wire_result);
	lm032l_write_string(&lcd2,0x00,temp2,temp2[255]);

	one_wire_bus_present();
	one_wire_bus_write(ONE_WIRE_SKIP_ROM);
	ds18b20_write_scratchpad(0x10,0x20,0x7f);

	one_wire_bus_present();
	one_wire_bus_write(ONE_WIRE_SKIP_ROM);
	ds18b20_convert_t();

	one_wire_bus_match_rom(ds18b20);
	ds18b20_read_scratchpad(scratchpad,9);

	for(i=0;i<9;i++){
	temp2[255]=sprintf(temp2,"%02x",scratchpad[i]);
	lm032l_write_string(&lcd2,0x40+i*2,temp2,temp2[255]);
	}
}

void main() {
	timer1_init();
	usart_init();
	lcd_init();
	one_wire_read_rom();
	while (1) {
	};
}
