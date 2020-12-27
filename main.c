/*
 * main.c
 *
 *  Created on: 2020年12月16日
 *      Author: Tom
 */

#include "main.h"

__xdata unsigned int timer0;
__xdata unsigned char usart_buf[256], usart_p, usart_e, i2c_dev[10];
__xdata I2cBus iic;
__xdata I2cMemory i2c_mem;
__xdata SpiBus spi;
__xdata SpiMemory spi_mem;

void iic_test();
void iic_eeprom();
void spi_test();

void usart_send(char *fmt, ...) {
	va_list list;
	va_start(list, fmt);
	usart_p += vsprintf(usart_buf + usart_p + usart_e, fmt, list);
	va_end(list);
}

void timer0_interrupt()
__interrupt 1 {
	timer0++;
	if(timer0==1000) {
		timer0=0;
	}
}

void usart_interrupt()
__interrupt 4 {
	unsigned char t;
	if(RI==1) {
		RI=0;
		t=SBUF;
		SBUF=t;
		//iic_test();
		//iic_eeprom();
		//spi_test();
		//long_t();
		//usart_send("received 0x%02x\r\n",t);
	} else {
		TI=0;
		if(usart_p>0) {
			SBUF=usart_buf[usart_e];
			usart_e++;
			usart_p--;
		}
	}

}

void usart_init() {
	SCON = 0x50;
	TMOD |= 0x20;
	PCON = 0x80;
	AUXR &= 0xBF;//定时器1时钟为Fosc/12,即12T
	AUXR &= 0xFE;//串口1选择定时器1为波特率发生器
	//P3M0=0x02;
	//P3M1=0x01;
	TH1 = 253;
	TL1 = 253;
	TR1 = 1;
	ET1 = 0;
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
	TR0 = 1;
	timer0 = 0;
}

void iic_init() {

	iic.sck = gpio_format(1, 0);
	iic.sda = gpio_format(1, 1);
	i2c_mem.bus = &iic;
	i2c_mem.dev_addr = 0x50;
	i2c_mem.page_size = 8;
	i2c_mem.n_bit_addr = I2C_MEMORY_8_BIT_ADDRESS;
	//iic_test();
	//iic_eeprom();
}

void iic_test() {
	unsigned char i, n;
	n = i2c_scan_7bit_addr_dev(&iic, i2c_dev, 10);
	usart_send("\r\nGet i2c dev addr: ");
	for (i = 0; i < n; i++) {
		usart_send("0x%02x ", i2c_dev[i]);
	}
	usart_send("\r\n");
	//get 0x48 0x50 0x51 0x52 0x53 0x54 0x55 0x56 0x57
}

void iic_eeprom() {
	unsigned char i, buf[25];
	for (i = 0; i < 25; i++) {
		buf[i] = i;
	}
	i = i2c_memory_write(&i2c_mem, 0x08, 25, buf);
	if (i != 25)
		usart_send("write i2c memory fail\r\n");
	for (i = 0; i < 25; i++) {
		buf[i] = 0;
	}
	i = i2c_memory_read(&i2c_mem, 0x08, 25, buf);
	if (i != 25)
		usart_send("read i2c memory fail\r\n");
	usart_send("\r\nbuf:\r\n");
	for (i = 0; i < 25; i++) {
		usart_send("%02x ", i, buf[i]);
	}
	usart_send("\r\n");
}

void spi_main_init() {
	spi.cpha = 0;
	spi.cpol = 0;
	spi.MISO = gpio_format(1, 4);
	spi.MOSI = gpio_format(1, 3);
	spi.MSB_FIRST = 1;
	spi.SCK = gpio_format(1, 2);
	spi.CS = gpio_format(1, 5);
	spi_init(&spi);
	spi_mem.bus = &spi;
	spi_mem.n_bit_address = SPI_MEMORY_8_BIT_ADDRESS;
	spi_mem.page_size = 16;
}

void spi_test() {
	unsigned char buf[25], i, temp;
	spi_set_cs(&spi, 0);
	spi_write(&spi, SPI_MEMORY_WRITE_ENABLE);
	spi_set_cs(&spi, 1);
	usart_send("SPI READ:%02x\r\n", spi_read(&spi));
	spi_memory_read(&spi_mem, 0, buf, 25);
	usart_send("\r\nspi memory test\r\n");
	for (i = 0; i < 25; i++) {
		usart_send("%02x ", buf[i]);
	}
	usart_send("\r\n");
	for (i = 0; i < 25; i++) {
		buf[i] = i;
	}
	temp = spi_memory_write(&spi_mem, 16, buf, 25);
	if (temp == 0)
		usart_send("\r\nWrite Failed\r\n");
}

#define test_crc7(d,c) 	i=crc7_calc(0x##d);i=crc7_calc_end(i<<7);usart_send("crc7("#d")=%02x,0x"#c"\r\n",i)

void main() {
	unsigned int i;
	unsigned long j;
	gpio io = gpio_format(1, 7);
	usart_init();
	//timer0_init();
	iic_init();
	spi_main_init();
	//iic_test();
	//0x9876=4B 0x1234=20 0x1111=22 0x4444=01 0x2222=44 0x3333=66 0x8888=02 0xFEDA=71 0xABCD=24 0xFF=79 F5=23
//	i=crc7_calc(0x9876);
//	i=crc7_calc_end(i<<7);
//	usart_send("crc7(9876)=%02x,0x4B\r\n",i);
//	i=crc7_calc(0xFEDA);
//	i=crc7_calc_end(i<<7);
//	usart_send("crc7(FEDA)=%02x,0x71\r\n",i);
//	i=crc7_calc(0xABCD);
//	i=crc7_calc_end(i<<7);
//	usart_send("crc7(ABCD)=%02x,0x24\r\n",i);
//
//	test_crc7(1234,20);
//	test_crc7(1111,22);
//	test_crc7(3333,66);
//	test_crc7(8888,02);
//
//	i=crc7_calc_end(0xF5<<7);
//	usart_send("crc7(F5)=%02x,0x23\r\n",i);
//	i=crc7_calc_end(0xFF<<7);
//	usart_send("crc7(FF)=%02x,0x79\r\n",i);

//j=0xff;
//	for(i=0;i<20;i++){
//		j=crc16_calc_debug(j<<15,i);
//	usart_send("%2d=%lx\r\n",i,j);
//	}

	j=0xFFFF;
	//j=crc16_calc(j<<16);
	j=crc16_calc_end(j<<16);
	usart_send("%lx\r\n",j);
	while (1) {
		gpio_set(io, 0);
		gpio_set(io, 1);
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
