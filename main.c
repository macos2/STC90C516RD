/*
 * main.c
 *
 *  Created on: 2020年12月13日
 *      Author: Tom
 */


#include "main.h"

__xdata lm032l lcd;
__xdata Ds1302 clock_dev;
__xdata unsigned char usart_buf[256],one_wire_dev[256];


void ds18b20_test();
void ds1302_test();

void usart_send(char *fmt,...){
	 va_list list;
	 va_start(list,fmt);
	 usart_buf[255]+=vsprintf(usart_buf+usart_buf[255],fmt,list);
	 va_end(list);

}

void usart_interrupt() __interrupt 4{
	unsigned char t;
	if(RI==1){
		RI=0;
		t=SBUF;
		SBUF=t;
		ds18b20_test();
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
	TMOD=0x20;
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

void lcd_init(){
	lcd.DATA=gpio_format(0,GPIO_ALL_PIN);
	lcd.E=gpio_format(2,7);
	lcd.RS=gpio_format(2,6);
	lcd.RW=gpio_format(2,5);
	lm032l_init(&lcd);
	lm032l_write_string(&lcd,0x00,"Hello",5);
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
	clock_dev.io=gpio_format(3,4);
	clock_dev.rst=gpio_format(3,5);
	clock_dev.sclk=gpio_format(3,6);
	ds1302_set_date(&clock_dev,1,2,3);
	ds1302_set_time(&clock_dev,4,5,6);
}

void ds1302_test(){
	unsigned char i,tmp;
	usart_send("\r\nclock:\r\n");
	for(i=0;i<9;i++){
		tmp=ds1302_read(&clock_dev,0,i);
		usart_send("%02x ",tmp);
	}
	usart_send("\r\n");
}

void main(){
	gpio io=gpio_format(1,7);
	lcd_init();
	usart_init();
	ds18b20_test();
	ds1302_init();
	ds1302_test();

	while(1){
		gpio_set(io,0);
		gpio_set(io,1);
	};
}
