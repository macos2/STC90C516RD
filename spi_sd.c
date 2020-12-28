/*
 * spi_sd.c
 *
 *  Created on: 2020年12月23日
 *      Author: tom
 */
#include "spi_sd.h"

__xdata unsigned char __sd_respon[17];
__xdata unsigned char __sd_cmd[6];
#define CLEAR_ARGS 	args[0]=0;args[1]=0;args[2]=0;args[3]=0;
#define DUMMY_READ spi_read(sd->spi)

unsigned char crc7_calc(unsigned int d){
	unsigned char i=0;
	unsigned int temp=d,p=0x8000,crc=sd_crc7_lsh8;
	while(i<8){
		//usart_send("%2d:\t%04x\t  %04x\r\n",i,temp,crc);
		if(p&temp){
			temp=temp^crc;
		}
		crc=crc>>1;
		p=p>>1;
		i++;
	}
	//usart_send("%2d:\t%04x\t  %04x\r\n\r\n",i,temp,crc);
	return temp;
}
unsigned char crc7_calc_end(unsigned int d){
	unsigned char i=0;
	unsigned int temp=d,p=0x4000,crc=sd_crc7_lsh8>>1;
	while(i<7){
		//usart_send("%2d:\t%04x\t  %04x\r\n",i,temp,crc);
		if(p&temp){
			temp=temp^crc;
		}
		crc=crc>>1;
		p=p>>1;
		i++;
	}
	if(temp&0x80)temp=temp^crc;
	//usart_send("%2d:\t%04x\t  %04x\r\n\r\n",i,temp,crc);
	return temp;
}

unsigned int crc16_calc(unsigned long d){
	unsigned char i=0;
	unsigned long temp=d,p=0x80000000,crc=sd_crc16_lsh15;
	while(i<15){
		//usart_send("%2d:\t%08lx\t  %08lx\r\n",i,temp,crc);
		if(p&temp){
			temp=temp^crc;
		}
		crc=crc>>1;
		p=p>>1;
		i++;
	}
	//usart_send("%2d:\t%08lx\t  %08lx\r\n",i,temp,crc);
	if(temp&0x10000)temp=temp^crc;
	//usart_send("%2d:\t%08lx\t  %08lx\r\n\r\n",i,temp,crc);
	return temp;
}

void spi_sd_gen_command(unsigned char cmd,unsigned char *args,unsigned char *result){
	unsigned char crc=0;
result[5]=0x40|(cmd&0b111111);
result[4]=args[3];
result[3]=args[2];
result[2]=args[1];
result[1]=args[0];
crc=crc7_calc(result[5]<<8|result[4]);
crc=crc7_calc(crc<<8|result[3]);
crc=crc7_calc(crc<<8|result[2]);
crc=crc7_calc(crc<<8|result[1]);
crc=crc7_calc_end(crc<<7);
result[0]=crc<<1|0x01;
}

void spi_sd_send_command(SpiSd *sd,unsigned char cmd,unsigned char *args){
	unsigned char crc=0,result[6];
result[5]=0x40|(cmd&0b111111);
result[4]=args[3];
result[3]=args[2];
result[2]=args[1];
result[1]=args[0];
crc=crc7_calc(result[5]<<8|result[4]);
crc=crc7_calc(crc<<8|result[3]);
crc=crc7_calc(crc<<8|result[2]);
crc=crc7_calc(crc<<8|result[1]);
crc=crc7_calc_end(crc<<7);
result[0]=crc<<1|0x01;

spi_write(sd->spi,result[5]);
spi_write(sd->spi,result[4]);
spi_write(sd->spi,result[3]);
spi_write(sd->spi,result[2]);
spi_write(sd->spi,result[1]);
spi_write(sd->spi,result[0]);
}


unsigned char spi_sd_init(SpiSd *sd){
	unsigned char r1,r3_r7[4];
	unsigned char args[4];


	CLEAR_ARGS;
	spi_set_cs(sd->spi,0);
	spi_set_cs(sd->spi,1);
	DUMMY_READ;
	spi_set_cs(sd->spi,0);
	spi_sd_send_command(sd,0,args);

	r1=spi_read(sd->spi);
	usart_send("R1:%02x\r\n",r1);

	args[0]=0x55;
	args[1]=0x01;
	args[2]=0;
	args[3]=0;
	spi_sd_send_command(sd,8,args);
	//DUMMY_READ;
	r1=spi_read(sd->spi);
	r3_r7[3]=spi_read(sd->spi);
	r3_r7[2]=spi_read(sd->spi);
	r3_r7[1]=spi_read(sd->spi);
	r3_r7[0]=spi_read(sd->spi);
	usart_send("R1:%02x\r\n",r1);
	usart_send("R7:%02x %02x %02x %02x\r\n",r3_r7[3],r3_r7[2],r3_r7[1],r3_r7[0]);


	if((r1&spi_sd_r1_illegal_cmd)==0){
		sd->version=2;
		if(r3_r7[1]==r3_r7[0]&&r3_r7[0]==0x55)usart_send("SD SUPPORT VOLTAGE\r\n");
	}else{
		sd->version=1;
	}

	CLEAR_ARGS;
	spi_sd_send_command(sd,58,args);
	//DUMMY_READ;
	r1=spi_read(sd->spi);
	r3_r7[3]=spi_read(sd->spi);
	r3_r7[2]=spi_read(sd->spi);
	r3_r7[1]=spi_read(sd->spi);
	r3_r7[0]=spi_read(sd->spi);
	usart_send("R1:%02x\r\n",r1);
	usart_send("R7:%02x %02x %02x %02x\r\n",r3_r7[3],r3_r7[2],r3_r7[1],r3_r7[0]);
	spi_set_cs(sd->spi,1);
	return 0;
}
