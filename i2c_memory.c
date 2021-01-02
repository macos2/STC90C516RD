/*
 * i2c_memory.c
 *
 *  Created on: 2020年12月20日
 *      Author: tom
 */


#include "i2c_memory.h"
#define OVERTIME 100

__xdata I2cMemory *__i2c__mem;
__xdata unsigned long __addr;
__xdata unsigned char __buf_size;
__xdata unsigned char *__i2c_mem_buf;

void i2c_memory_set_rw_param(__xdata I2cMemory *mem,__xdata unsigned long addr,__xdata unsigned char buf_size,__xdata unsigned char *i2c_mem_buf){
	__i2c__mem=mem;
	__addr=addr;
	__buf_size=buf_size;
	__i2c_mem_buf=i2c_mem_buf;
}

//unsigned char i2c_memory_read(I2cMemory *mem,unsigned long addr,unsigned char buf_size,unsigned char *buf){
unsigned char i2c_memory_read(){
	__xdata unsigned char i,j=0,ack,*p;


	do{
		i2c_stop(__i2c__mem->bus);
		i2c_start(__i2c__mem->bus);
	ack=i2c_send_7bit_addr(__i2c__mem->bus,__i2c__mem->dev_addr,0);
	j++;
	}while(j<OVERTIME&&ack!=0);
	if(ack!=0){
		i2c_stop(__i2c__mem->bus);
		return 0;
	}

	p=&__addr;
	for(i=0;i<=(__i2c__mem->n_bit_addr);i++){
		i2c_write(__i2c__mem->bus,p[__i2c__mem->n_bit_addr-i]);
	}


	i2c_start(__i2c__mem->bus);
	ack=i2c_send_7bit_addr(__i2c__mem->bus,__i2c__mem->dev_addr,1);
	if(ack!=0){
		i2c_stop(__i2c__mem->bus);
		return 0;
	}

	for(i=0;i<__buf_size;i++){
		*__i2c_mem_buf=i2c_read(__i2c__mem->bus,0);
		__i2c_mem_buf++;
	}
	i2c_read(__i2c__mem->bus,1);//dummy read and no ack for stop
	i2c_stop(__i2c__mem->bus);
	return __buf_size;
}

//unsigned char i2c_memory_write(I2cMemory *mem,unsigned long addr,unsigned char buf_size,unsigned char *buf){
unsigned char i2c_memory_write(){
	__xdata unsigned char i,j=0,ack,*p;

	do{
		i2c_stop(__i2c__mem->bus);
		i2c_start(__i2c__mem->bus);
	ack=i2c_send_7bit_addr(__i2c__mem->bus,__i2c__mem->dev_addr,0);
	j++;
	}while(j<OVERTIME&&ack!=0);

	if(ack!=0){
		i2c_stop(__i2c__mem->bus);
		return 0;
	}

	p=&__addr;
	for(i=0;i<=(__i2c__mem->n_bit_addr);i++){
		i2c_write(__i2c__mem->bus,p[__i2c__mem->n_bit_addr-i]);
	}

	j=__buf_size>__i2c__mem->page_size?__i2c__mem->page_size:__buf_size;
	for(i=0;i<j;i++){
			i2c_write(__i2c__mem->bus,*__i2c_mem_buf);
			__i2c_mem_buf++;
	}

	if(__buf_size>__i2c__mem->page_size){//next page write
				i2c_stop(__i2c__mem->bus);
				//write next page
				i2c_memory_set_rw_param(__i2c__mem,__addr+__i2c__mem->page_size,__buf_size-__i2c__mem->page_size,__i2c_mem_buf);
				j=i2c_memory_write();
				if(j==0){
					i2c_stop(__i2c__mem->bus);
					return 0;
				}
	}

	i2c_stop(__i2c__mem->bus);
	return __buf_size;
}
