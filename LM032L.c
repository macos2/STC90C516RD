#include "LM032L.h"

//#define RS P20 //RS=0 CODE,RS=1 DDRAM
//#define RW P21 //RW=0 WRITE,RW=1 READ
//#define E P22 //E=0 disable,E=1 enable
//#define DATA P1 //code or data

extern void lm032l_delay(){
char i=0x4F;
	while(i)i--;
}

void lm032l_init(lm032l *lcd){
	gpio_set(lcd->E,0);
	gpio_set(lcd->RS,0);
	gpio_set(lcd->RW,0);
	gpio_set(lcd->DATA,LM032L_DISPLAY_CTRL|0x01<<2|0x00<<1|0x00<<0);
	gpio_set(lcd->E,1);
	lm032l_delay();
	gpio_set(lcd->E,0);
	gpio_set(lcd->DATA,LM032L_FUNC_CTRL|0x01<<4|0x01<<3|0x01<<2);
	gpio_set(lcd->E,1);
	lm032l_delay();
	gpio_set(lcd->E,0);
	
}

unsigned char lm032l_busy(lm032l *lcd){
	unsigned char result=0x80;
	gpio_set(lcd->E,0);
	gpio_set(lcd->RS,0);
	gpio_set(lcd->RW,1);
	gpio_set(lcd->DATA,0xFF);
	gpio_set(lcd->E,1);
	lm032l_delay();
	result=gpio_get(lcd->DATA);
	gpio_set(lcd->E,0);
	gpio_set(lcd->RW,0);
	return result&0x80;
}

unsigned char lm032l_read_data(lm032l *lcd,unsigned char addr){
	unsigned char result=0x00;
	lm032l_write_code(lcd,addr|LM032L_DDRAM_ADDR);
	gpio_set(lcd->RW,1);
	gpio_set(lcd->RS,1);
	gpio_set(lcd->DATA,0x00);
	gpio_set(lcd->E,1);
	lm032l_delay();
	result=gpio_get(lcd->DATA);
	gpio_set(lcd->E,0);
	return result;
}

void lm032l_write_data(lm032l *lcd,unsigned char addr,unsigned char D){
	lm032l_write_code(lcd,addr|LM032L_DDRAM_ADDR);
	gpio_set(lcd->RS,1);
	gpio_set(lcd->DATA,D);
	gpio_set(lcd->E,1);
	lm032l_delay();
	gpio_set(lcd->E,0);
}

void lm032l_write_data_direct(lm032l *lcd,unsigned char D){
	gpio_set(lcd->E,0);
	gpio_set(lcd->RS,1);
	gpio_set(lcd->DATA,D);
	gpio_set(lcd->E,1);
	lm032l_delay();
	gpio_set(lcd->E,0);
}

void lm032l_write_code(lm032l *lcd,unsigned char D){
	gpio_set(lcd->E,0);
	gpio_set(lcd->RS,0);
	gpio_set(lcd->RW,0);
	gpio_set(lcd->DATA,D);
	gpio_set(lcd->E,1);
	lm032l_delay();
	gpio_set(lcd->E,0);
}

void lm032l_write_string(lm032l *lcd,unsigned char addr,char *d,unsigned char len){
	unsigned char i=0;
	if(len==0)return;
	lm032l_write_data(lcd,addr,d[i]);
	while(i<(len-1)){
		i++;
		lm032l_write_data_direct(lcd,d[i]);
	}
}
