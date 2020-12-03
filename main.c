/*
 * main.c
 *
 *  Created on: 2020年11月27日
 *      Author: tom
 */

#include "main.h"
__xdata char temp[256];
unsigned char p;
lm032l lcd;
lm032l lcd2;

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
			p--;
			temp[p] = '\0';
			break;
		default:
			if (p < 254) {
				temp[p] = tmp;
				p++;
				temp[p] = '\0';
			}
			break;
		}
		if (p > 20) {
			lm032l_write_string(&lcd, 0x00, temp + p - 20, 20);
			lm032l_write_data(&lcd, 0x00, 0x19);
		} else {
			lm032l_write_string(&lcd, 0x00, temp, p);
		}
		SBUF = tmp;
		lm032l_write_string(&lcd, 0x40, "LCD", 3);
		lm032l_write_string(&lcd2, 0x40, "LCD2", 4);
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
}

void lcd_init() {
	lcd.DATA = gpio_format(0, GPIO_ALL_PIN);
	lcd.E = gpio_format(2, 7);
	lcd.RS = gpio_format(2, 6);
	lcd.RW = gpio_format(2, 5);
	lm032l_init(&lcd);

	lcd2.DATA = gpio_format(0, GPIO_ALL_PIN);
	lcd2.E = gpio_format(2, 4);
	lcd2.RS = gpio_format(2, 6);
	lcd2.RW = gpio_format(2, 5);
	lm032l_init(&lcd2);
}

void main() {
	timer1_init();
	usart_init();
	lcd_init();
	p = 0;
	temp[p] = '\0';
	while (1) {

	};
}
