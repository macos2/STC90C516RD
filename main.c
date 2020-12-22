/*
 * main.c
 *
 *  Created on: 2020年12月16日
 *      Author: Tom
 */


#include "main.h"

__xdata unsigned int timer0;
__xdata unsigned char usart_buf[256],usart_p,usart_e,i2c_dev[10];
__xdata I2cBus iic;
__xdata I2cMemory i2c_mem;
__xdata SpiBus spi;

void iic_test();
void iic_eeprom();
void spi_test();



void usart_send(char *fmt,...){
	 va_list list;
	 va_start(list,fmt);
	 usart_p+=vsprintf(usart_buf+usart_p,fmt,list);
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
		iic_eeprom();
		//spi_test();
		//long_t();
		usart_send("received 0x%02x\r\n",t);
	}else{
		TI=0;
		if(usart_p>0){
			SBUF=usart_buf[usart_e];
			usart_p-=1;
			usart_e+=1;
		}else{
			usart_e=0;
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
	usart_e=0;
	usart_p=0;
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
	i2c_mem.bus=&iic;
	i2c_mem.dev_addr=0x50;
	i2c_mem.page_size=8;
	i2c_mem.n_bit_addr=I2C_MEMORY_8_BIT_ADDRESS;
	//iic_test();
	//iic_eeprom();
}

void iic_test(){
//	unsigned char ack=1,addr=0b1010000;
//	i2c_start(&iic);
//	ack=i2c_send_7bit_addr(&iic,addr,0);
//	usart_send("send address:%02x ack:%02x\r\n",addr,ack);
//	ack=i2c_write(&iic,0x55);
//	usart_send("send data:%02x ack:%02x\r\n",0x55,ack);
//	i2c_stop(&iic);
	unsigned char i,p=0;
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
		//get 0x48 0x50 0x51 0x52 0x53 0x54 0x55 0x56 0x57
}

void iic_eeprom(){
//	unsigned char i;
//	i2c_start(&iic);
//	usart_send("send 7bit addr:%x\r\n",i2c_send_7bit_addr(&iic,0x50,0));
//	usart_send("send write addr:%x\r\n",i2c_write(&iic,0x00));
//	for(i=0;i<8;i++){
//		usart_send("write data:%x\r\n",i2c_write(&iic,i<<4));
//	}
//	i2c_stop(&iic);
//	i=1;
//	while(i!=0){
//		i2c_start(&iic);
//		i=i2c_send_7bit_addr(&iic,0x50,1);
//		i2c_stop(&iic);
//	}

//	usart_send("read from eeprom\r\n");
//	i2c_start(&iic);
//	usart_send("send 7bit addr:%x\r\n",i2c_send_7bit_addr(&iic,0x50,0));
//	usart_send("send write addr:%x\r\n",i2c_write(&iic,0x00));
//	i2c_start(&iic);
//	usart_send("send 7bit addr:%x\r\n",i2c_send_7bit_addr(&iic,0x50,1));
//	for(i=0;i<8;i++){
//		usart_send("@%x :%02x\r\n",i,i2c_read(&iic,0));
//	}
//	i2c_read(&iic,1);
//	i2c_stop(&iic);

	unsigned char i,buf[25];
	for(i=0;i<25;i++){
		buf[i]=i;
	}
	i=i2c_memory_write(&i2c_mem,0x05,25,buf);
	if(i!=25)usart_send("write i2c memory fail\r\n");

	i=i2c_memory_read(&i2c_mem,0x05,25,buf);
	if(i!=25)usart_send("read i2c memory fail\r\n");
	usart_send("\r\nbuf:\r\n");
	for(i=0;i<25;i++){
		usart_send("@%d: 0x%02x ",i,buf[i]);
	}
	usart_send("\r\n");
}


void spi_main_init(){
	spi.CPHA=0;
	spi.CPOL=0;
	spi.MISO=gpio_format(1,4);
	spi.MOSI=gpio_format(1,3);
	spi.MSB_FIRST=1;
	spi.SCK=gpio_format(1,2);
	spi.CS=gpio_format(1,5);
	spi_init(&spi);
}

void spi_test(){
	spi_write(&spi,0x55);
	spi_write(&spi,0x55);
	usart_send("SPI READ:%02x\r\n",spi_read(&spi));
}


void main(){
	unsigned char i=0xff;
	gpio io=gpio_format(1,7);
	usart_init();
	//timer0_init();
	iic_init();
	//spi_main_init();
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
