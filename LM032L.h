#include "gpio.h"
#include "stdio.h"

#ifndef LM032L_H_
#define LM032L_H_

#define LM032L_CLEAR 0x01
#define LM032L_RESET_CURSOR 0x02
#define LM032L_INPUT_METHOD 0x04 // 00001DS,D=1 move right,S=1 All String move
#define LM032L_DISPLAY_CTRL 0x08 // 0001DCB,D=1 display on.C=1 display cursor,B=1 blink cursor
#define LM032L_STR_CURSOR_MOVE_CTRL 0x10 //0001SR00,S=1 move string,R=1 right direction
#define LM032L_FUNC_CTRL 0x20 //001DNF00,D=1 4bit data bus,N=1 display two line,F=0 5x7 char F=1 5x10 char
#define LM032L_CHAR_ADDR 0x40 //01xx xxxx,xx xxxx=char addr
#define LM032L_DDRAM_ADDR 0x80 //1xxxxxxx,xxxxxxx=DDRAM addr

typedef struct lm032l {
	gpio RS;
	gpio RW;
	gpio E;
	gpio DATA;
}lm032l;


void lm032l_init(lm032l *lcd);
unsigned char lm032l_busy(lm032l *lcd);
unsigned char lm032l_read_data(lm032l *lcd,unsigned char addr);
void lm032l_write_data(lm032l *lcd,unsigned char addr,unsigned char D);
void lm032l_write_data_direct(lm032l *lcd,unsigned char D);
void lm032l_write_code(lm032l *lcd,unsigned char D);
void lm032l_write_string(lm032l *lcd,unsigned char addr,char *fmt,...);

#endif
