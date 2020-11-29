/*
 * main.c
 *
 *  Created on: 2020年11月27日
 *      Author: tom
 */

#include "main.h"
char temp[16];
unsigned char p;
lm032l lcd;
lm032l lcd2;

void timer1_interrupt()
__interrupt 3 {
	SBUF='A';
}

void usart_interrupt()
__interrupt 4 {
	if (RI == 1) {
		if (p < 255) {
			temp[p] = SBUF;
			temp[p + 1] = '\0';
			SBUF = temp[p];
			lm032l_write_data_direct(&lcd,temp[p]);
			lm032l_write_data_direct(&lcd2,temp[p]);
			p++;
		}
		RI = 0;
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

void usart_init() { //115200@12M
	SCON = 0x50;
	PCON = 0x80; //T1 foscx2
	ES = 1;
	EA = 1;
}

void lcd_init(){
	lcd.DATA=gpio_format(1,GPIO_ALL_PIN);
	lcd.E=gpio_format(2,0);
	lcd.RS=gpio_format(2,1);
	lcd.RW=gpio_format(2,2);
	lm032l_init(&lcd);

	lcd2.DATA=gpio_format(1,GPIO_ALL_PIN);
	lcd2.E=gpio_format(2,3);
	lcd2.RS=gpio_format(2,1);
	lcd2.RW=gpio_format(2,2);
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
