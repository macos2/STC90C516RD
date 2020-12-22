/*
 * i2c_memory.c
 *
 *  Created on: 2020年12月20日
 *      Author: tom
 */


#include "i2c_memory.h"
#define OVERTIME 100

unsigned char i2c_memory_read(I2cMemory *mem,unsigned long addr,unsigned char buf_size,unsigned char *buf){
	unsigned char i,j=0,ack,*p;


	do{
		i2c_stop(mem->bus);
		i2c_start(mem->bus);
	ack=i2c_send_7bit_addr(mem->bus,mem->dev_addr,0);
	j++;
	}while(j<OVERTIME&&ack!=0);
	if(ack!=0){
		i2c_stop(mem->bus);
		return 0;
	}

	p=&addr;
	for(i=0;i<=(mem->n_bit_addr);i++){
		i2c_write(mem->bus,p[mem->n_bit_addr-i]);
	}


	i2c_start(mem->bus);
	ack=i2c_send_7bit_addr(mem->bus,mem->dev_addr,1);
	if(ack!=0){
		i2c_stop(mem->bus);
		return 0;
	}

	for(i=0;i<buf_size;i++){
		*buf=i2c_read(mem->bus,0);
		buf++;
	}
	i2c_read(mem->bus,1);//dummy read and no ack for stop
	i2c_stop(mem->bus);
	return buf_size;
}

unsigned char i2c_memory_write(I2cMemory *mem,unsigned long addr,unsigned char buf_size,unsigned char *buf){
	unsigned char *data,i,j=0,ack,*p;

	do{
		i2c_stop(mem->bus);
		i2c_start(mem->bus);
	ack=i2c_send_7bit_addr(mem->bus,mem->dev_addr,0);
	j++;
	}while(j<OVERTIME&&ack!=0);

	if(ack!=0){
		i2c_stop(mem->bus);
		return 0;
	}

	p=&addr;
	for(i=0;i<=(mem->n_bit_addr);i++){
		i2c_write(mem->bus,p[mem->n_bit_addr-i]);
	}

	data=buf;
	for(i=0;i<buf_size;i++){
			i2c_write(mem->bus,*data);
			data++;
			if((i+1)%8==0){//up to 1 page_size write
				i2c_stop(mem->bus);
				j=i2c_memory_write(mem,addr+mem->page_size,buf_size-mem->page_size,data);
				if(j==0){
					i2c_stop(mem->bus);
					return 0;
				}
			}

	}
	i2c_stop(mem->bus);
	return buf_size;
}
