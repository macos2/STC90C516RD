/*
 * main.c
 *
 *  Created on: 2020年12月16日
 *      Author: Tom
 */

#include "main.h"

__xdata unsigned int timer0;
__xdata unsigned char usart_buf[128], usart_p, usart_e, i2c_dev[10],timer0_isp;
__xdata SpiBus spi;
__xdata SpiSd spi_sd;
__xdata I2cBus i2c;
__xdata I2cMemory i2c_mem;
__xdata SpiMemory spi_mem;
__xdata unsigned char sd_buf[512];
__xdata unsigned char test;
void iic_test();
void iic_eeprom();
void spi_test();



void usart_send(char *fmt, ...) {
	va_list list;
	va_start(list, fmt);
	while(usart_p!=0);
	usart_p += vsprintf(usart_buf, fmt, list);
	usart_p--;
	usart_e++;
	SBUF=usart_buf[0];
	va_end(list);
}

void timer0_interrupt()
__interrupt 1 {
	timer0++;
	if(timer0==1000) {
		timer0=0;
		usart_send("%d\r\n",timer0_isp++);
	}
}

void usart_interrupt()
__interrupt 4 {
	unsigned char t;
	if(RI==1) {
		RI=0;
		t=SBUF;
		SBUF=t;
		if(t=='1')test=1;
		if(t=='2')test=2;
		if(t=='3')test=3;
		if(t=='4')test=4;
		if(t=='5')test=5;
		if(t=='6')test=6;
	} else {
		TI=0;
		if(usart_p>0) {
			SBUF=usart_buf[usart_e];
			usart_e++;
			usart_p--;
		}else{
			usart_e=0;
			usart_p=0;
		}
	}

}

void usart_init() {
	PCON |= 0x80;		//使能波特率倍速位SMOD
	SCON = 0x50;		//8位数据,可变波特率
	AUXR &= 0xBF;		//定时器1时钟为Fosc/12,即12T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//清除定时器1模式位
	TMOD |= 0x20;		//设定定时器1为8位自动重装方式
	TL1 = 0xFD;		//设定定时初值
	TH1 = 0xFD;		//设定定时器重装值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
	P3M0=0x03;
	P3M1=0x00;
	ES = 1;
	EA = 1;
	usart_e = 0;
	usart_p = 0;
}

void timer0_init() {
	TMOD |= 0x02;
	TL0 = 6;
	TH0 = 6;
	ET0 = 1;
	TR0 = 0;
	timer0 = 0;
	timer0_isp=0;
}

void spi_main_init() {
	spi.cpha = 0;
	spi.cpol = 0;
	spi.MISO = gpio_format(1, 3);
	spi.MOSI = gpio_format(1, 1);
	spi.MSB_FIRST = 1;
	spi.SCK = gpio_format(1, 2);
	spi.CS = gpio_format(1, 0);
	spi_init(&spi);
	spi_sd.spi=&spi;
	spi_mem.bus=&spi;
	spi_mem.n_bit_address=SPI_MEMORY_8_BIT_ADDRESS;
	spi_mem.page_size=8;
}

void i2c_init(){
	i2c.sck=gpio_format(1,0);
	i2c.sda=gpio_format(1,1);
	i2c_mem.bus=&i2c;
	i2c_mem.dev_addr=0x51;
	i2c_mem.n_bit_addr=I2C_MEMORY_8_BIT_ADDRESS;
	i2c_mem.page_size=8;

}

void main() {
	unsigned int i;
	unsigned int j;
	gpio io = gpio_format(1, 7);
	usart_init();
	timer0_init();
	spi_main_init();
	i2c_init();
	test=0;
	while (1) {
		gpio_set(io, 0);
		gpio_set(io, 1);
		if(test==1){
			spi_sd_init(&spi_sd,64,1);
			spi_sd_read(&spi_sd,0,sd_buf,1);
			usart_send("\r\n");
			spi_sd_write(&spi_sd,0,sd_buf,1);
			test=0;

		}
		if(test==2){
			usart_send("read buffer:\r\n");
			for(i=0;i<255;i++){
				usart_send("%02x ",sd_buf[i]);
			}
			test=0;
		}
		if(test==3){
			i=one_wire_bus_search_rom(0,sd_buf,5);
					usart_send("%d device detected\r\n",i);
					j=0;
					while(i>0){
						usart_send("%d %02x %02x %02x %02x %02x %02x %02x",sd_buf[j],sd_buf[j+1],sd_buf[j+2],sd_buf[j+3],sd_buf[j+4],sd_buf[j+5]);
						j+=6;
						i--;
					}
					test=0;

		}
		if(test==4){
			for(i=0;i<255;i++){
				sd_buf[i]=i;
			}
			usart_send("read buffer:\r\n");
			for(i=0;i<255;i++){
				usart_send("%02x ",sd_buf[i]);
			}
			test=0;
		}
		if(test==5){
			i2c_memory_read(&i2c_mem,0x00,256,sd_buf);
			for(i=0;i<256;i++){
				if(i%8==0)usart_send("\r\n");
				usart_send("%02x ",sd_buf[i]);
			}
			for(i=0;i<256;i++){
				sd_buf[i]=i;
			}
			i2c_memory_write(&i2c_mem,0x00,256,sd_buf);
			test=0;
		}
		if(test==6){
			spi.CS=gpio_format(1,4);
			spi_memory_read(&spi_mem,00,sd_buf,256);
			for(i=0;i<256;i++){
				if(i%8==0)usart_send("\r\n");
				usart_send("%02x ",sd_buf[i]);
			}
			for(i=0;i<256;i++){
					sd_buf[i]=i;
				}
			spi_memory_write(&spi_mem,0,sd_buf,256);
			spi.CS=gpio_format(1,0);
			test=0;
		}
	};
}
