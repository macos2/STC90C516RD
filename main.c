/*
 * main.c
 *
 *  Created on: 2020年12月16日
 *      Author: Tom
 */


#include "main.h"

__xdata unsigned int timer0;
__xdata unsigned char usart_buf[256],i2c_dev[10];
__xdata I2cBus iic;
__xdata SpiBus spi;

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
		//iic_test();
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
	unsigned char i,p=0;
	iic.sck=gpio_format(1,0);
	iic.sda=gpio_format(1,1);
	for(i=0;i<127;i++){
		i2c_start(&iic);
		if(i2c_send_7bit_addr(&iic,i,1)==0){
			i2c_dev[p]=i;
			p++;
		}
		i2c_stop(&iic);
	}
	usart_send("\r\nGet i2c dev addr: ");
	for(i=0;i<p;i++){
		usart_send("0x%02x ",i2c_dev[i]);
	}
	usart_send("\r\n");
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

void spi_main_init(){
	spi.CPHA=0;
	spi.CPOL=0;
	spi.MISO=gpio_format(1,2);
	spi.MOSI=gpio_format(1,3);
	spi.MSB_FIRST=0;
	spi.SCK=gpio_format(1,4);
	spi.CS=gpio_format(1,5);
	spi_init(&spi);
}


void main(){
	unsigned char i=0xff;
	gpio io=gpio_format(1,7);
	usart_init();
	//timer0_init();
	iic_init();
	spi_main_init();
	//iic_test();
	while(1){
//		i2c_start(&iic);
//		i2c_send_7bit_addr(&iic,0x46,0);
//		i2c_write(&iic,0x33);
//		i2c_write(&iic,0x00);
//		i2c_stop(&iic);
//		while(i--);
//		i=0xff;
//		while(i--);
//		i=0xff;
//		spi_write(&spi,0x55);
//		spi_read(&spi);
//		//usart_send("\r\nread from spi bus:%02x \r\n\0",spi_read(&spi));
//		while(i--);
//		i=0xff;
//		while(i--);
//		i=0xff;
	};
}
