/*
 * main.c
 *
 *  Created on: 2020年12月16日
 *      Author: Tom
 */


#include "main.h"

__xdata lm032l lcd;
__xdata Ds1302 clock_dev;
__xdata unsigned int timer0;
__xdata unsigned char usart_buf[256],one_wire_dev[256];


void ds18b20_test();
void ds1302_test();

void usart_send(char *fmt,...){
	 va_list list;
	 va_start(list,fmt);
	 usart_buf[255]+=vsprintf(usart_buf+usart_buf[255],fmt,list);
	 va_end(list);

}

void timer0_interrupt() __interrupt 1{
	unsigned char buf[7];
	timer0++;
	if(timer0==400){
		timer0=0;
		ds1302_get_time(&clock_dev,buf);
		lm032l_write_string(&lcd,0x40,"20%02d-%02x-%02x %02x:%02x:%02x",buf[6],buf[4],buf[3],buf[2],buf[1],buf[0]);
	}
}

void usart_interrupt() __interrupt 4{
	unsigned char t;
	if(RI==1){
		RI=0;
		t=SBUF;
		SBUF=t;
		//ds18b20_test();
		ds1302_test();
		usart_send("received 0x%02x\r\n",t);
	}else{
		TI=0;
		if(usart_buf[255]>0){
			SBUF=usart_buf[usart_buf[254]];
			usart_buf[255]-=1;
			usart_buf[254]+=1;
		}else{
			usart_buf[254]=0;
		}
	}

}

void usart_init(){
	SCON=0x50;
	TMOD|=0x20;
	PCON=0x80;
	TH1=253;
	TL1=253;
	TR1=1;
	ET1=0;
	ES=1;
	EA=1;
	usart_buf[254]=0;
	usart_buf[255]=0;
}

void timer0_init(){
	TMOD|=0x02;
	TL0=6;
	TH0=6;
	ET0=1;
	TR0=1;
	timer0=0;
}

void lcd_init(){
	lcd.DATA=gpio_format(1,GPIO_ALL_PIN);
	lcd.E=gpio_format(3,3);
	lcd.RS=gpio_format(2,6);
	lcd.RW=gpio_format(2,5);
	lm032l_init(&lcd);
	lm032l_write_string(&lcd,0x00,"Hello");
}

void ds18b20_test(){
	unsigned char i,j;
	unsigned char num=one_wire_bus_search_rom(0,one_wire_dev,6);
	usart_send("get %d device\r\n",num);
	for(i=0;i<num;i++){
		usart_send("Dev%d: ",i);
		for(j=0;j<8;j++){
			usart_send("%02x ",one_wire_dev[j+8*i]);
		}
		usart_send("\r\n");
	}

}

void ds1302_init(){
	clock_dev.io=gpio_format(1,4);
	clock_dev.rst=gpio_format(1,5);
	clock_dev.sclk=gpio_format(1,6);
	ds1302_set_date(&clock_dev,1,2,3);
	ds1302_set_time(&clock_dev,4,5,6);
}

void ds1302_test(){
	unsigned char buf[7];
	ds1302_get_time(&clock_dev,buf);
	usart_send("\r\nclock:\r\n");
	usart_send("20%02d-%02x-%02x %02x:%02x:%02x",buf[6],buf[4],buf[3],buf[2],buf[1],buf[0]);
	usart_send("\r\n");
}

void main(){
	gpio io=gpio_format(1,7);
	lcd_init();
	usart_init();
	timer0_init();
	ds18b20_test();
	ds1302_init();
	ds1302_test();
	while(1){
		gpio_set(io,0);
		gpio_set(io,1);
	};
}
