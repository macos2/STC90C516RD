/*
 * main.c
 *
 *  Created on: 2020年12月16日
 *      Author: Tom
 */


#include "main.h"

__xdata unsigned int timer0;
__xdata unsigned char usart_buf[256];
I2C_BUS iic;

void iic_test();

void usart_send(char *fmt,...){
	 va_list list;
	 va_start(list,fmt);
	 usart_buf[255]+=vsprintf(usart_buf+usart_buf[255],fmt,list);
	 va_end(list);

}

void timer0_interrupt() __interrupt 1{
	timer0++;
	if(timer0==1000){
		timer0=0;
	}
}

void usart_interrupt() __interrupt 4{
	unsigned char t;
	if(RI==1){
		RI=0;
		t=SBUF;
		SBUF=t;
		iic_test();
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

void iic_init(){
	iic.sck=gpio_format(1,0);
	iic.sda=gpio_format(1,1);
}

void iic_test(){
	unsigned char ack=1,addr=0b1010000;
	i2c_start(&iic);
	ack=i2c_send_7bit_addr(&iic,addr,0);
	usart_send("send address:%02x ack:%02x\r\n",addr,ack);
	ack=i2c_write(&iic,0x55);
	usart_send("send data:%02x ack:%02x\r\n",0x55,ack);
	i2c_stop(&iic);
}

void main(){
	unsigned char i=0xff;
	gpio io=gpio_format(1,7);
	usart_init();
	//timer0_init();
	iic_init();
	//iic_test();
	while(1){
		i2c_start(&iic);
		i2c_send_7bit_addr(&iic,0b0101010,1);
		i2c_write(&iic,0x00);
		i2c_stop(&iic);
		while(i--);
		i=0xff;
		while(i--);
		i=0xff;
	};
}
