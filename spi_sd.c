/*
 * spi_sd.c
 *
 *  Created on: 2020年12月23日
 *      Author: tom
 */
#include "spi_sd.h"

#define CLEAR_ARGS(x) 	x[0]=0;x[1]=0;x[2]=0;x[3]=0;

unsigned char crc7_calc(unsigned int d);
unsigned char crc7_calc_end(unsigned int d);
unsigned int crc16_calc(unsigned long d);

unsigned char crc7_calc(unsigned int d) {
	unsigned char i = 0;
	unsigned int temp = d, p = 0x8000, crc = sd_crc7_lsh8;
	while (i < 8) {
		//usart_send("%2d:\t%04x\t  %04x\r\n",i,temp,crc);
		if (p & temp) {
			temp = temp ^ crc;
		}
		crc = crc >> 1;
		p = p >> 1;
		i++;
	}
	//usart_send("%2d:\t%04x\t  %04x\r\n\r\n",i,temp,crc);
	return temp;
}
unsigned char crc7_calc_end(unsigned int d) {
	unsigned char i = 0;
	unsigned int temp = d, p = 0x4000, crc = sd_crc7_lsh8 >> 1;
	while (i < 7) {
		//usart_send("%2d:\t%04x\t  %04x\r\n",i,temp,crc);
		if (p & temp) {
			temp = temp ^ crc;
		}
		crc = crc >> 1;
		p = p >> 1;
		i++;
	}
	if (temp & 0x80)
		temp = temp ^ crc;
	//usart_send("%2d:\t%04x\t  %04x\r\n\r\n",i,temp,crc);
	return temp;
}

unsigned int crc16_calc(unsigned long d) {
	unsigned char i = 0;
	unsigned long temp = d, p = 0x80000000, crc = sd_crc16_lsh15;
	while (i < 15) {
		//usart_send("%2d:\t%08lx\t  %08lx\r\n",i,temp,crc);
		if (p & temp) {
			temp = temp ^ crc;
		}
		crc = crc >> 1;
		p = p >> 1;
		i++;
	}
	//usart_send("%2d:\t%08lx\t  %08lx\r\n",i,temp,crc);
	if (temp & 0x10000)
		temp = temp ^ crc;
	//usart_send("%2d:\t%08lx\t  %08lx\r\n\r\n",i,temp,crc);
	return temp;
}

void spi_sd_gen_command(unsigned char cmd, unsigned char *args,
		unsigned char *result) {
	unsigned char crc = 0;
	result[5] = 0x40 | (cmd & 0b111111);
	result[4] = args[3];
	result[3] = args[2];
	result[2] = args[1];
	result[1] = args[0];
	crc = crc7_calc(result[5] << 8 | result[4]);
	crc = crc7_calc(crc << 8 | result[3]);
	crc = crc7_calc(crc << 8 | result[2]);
	crc = crc7_calc(crc << 8 | result[1]);
	crc = crc7_calc_end(crc << 7);
	result[0] = crc << 1 | 0x01;
}

unsigned char spi_sd_send_command(SpiSd *sd, unsigned char cmd,
		unsigned char *args) {
	unsigned char crc = 0, timeout, r1, result[6];
	result[5] = 0x40 | (cmd & 0b111111);
	result[4] = args[3];
	result[3] = args[2];
	result[2] = args[1];
	result[1] = args[0];
	crc = crc7_calc(result[5] << 8 | result[4]);
	crc = crc7_calc(crc << 8 | result[3]);
	crc = crc7_calc(crc << 8 | result[2]);
	crc = crc7_calc(crc << 8 | result[1]);
	crc = crc7_calc_end(crc << 7);
	result[0] = crc << 1 | 0x01;

	usart_send("Send CMD%2d\r\n", cmd);
	spi_write(sd->spi, result[5]);
	spi_write(sd->spi, result[4]);
	spi_write(sd->spi, result[3]);
	spi_write(sd->spi, result[2]);
	spi_write(sd->spi, result[1]);
	spi_write(sd->spi, result[0]);
	timeout = 10;
	r1 = 0xff;
	while (r1 == 0xff && timeout) {
		r1 = spi_read(sd->spi);
		timeout--;
	}
	return r1;
}
unsigned char spi_sd_send_app_command(SpiSd *sd, unsigned char cmd,
		unsigned char *args) {
	unsigned char temp[4], r1;
	CLEAR_ARGS(temp);
	r1 = spi_sd_send_command(sd, 55, temp);
	usart_send("Send CMD55 R1:%02x\r\n", r1);
	r1 = spi_sd_send_command(sd, cmd, args);
	;
	usart_send("Send ACMD%02d R1:%02x\r\n", cmd, r1);
	return r1;
}

unsigned char spi_sd_init(SpiSd *sd,unsigned int block_size_for_sdsd_mmc,unsigned char crc_off) {
	unsigned char r1 = 0xff, r3_r7[4], timeout;
	unsigned char args[4];

	//Send CMD0 + CS=0 set card into idle state
	CLEAR_ARGS(args);
	spi_set_cs(sd->spi, 0);
	r1 = spi_sd_send_command(sd, 0, args);
	usart_send("R1:%02x\r\n", r1);

	//Send CMD8 to determine SD version,1.0 or 2.0
	args[0] = 0x55; //check pattern
	args[1] = 0x01; //voltage supplied 2.6~3.6V
	args[2] = 0;
	args[3] = 0;
	r1 = spi_sd_send_command(sd, 8, args);
	r3_r7[3] = spi_read(sd->spi);
	r3_r7[2] = spi_read(sd->spi);
	r3_r7[1] = spi_read(sd->spi);
	r3_r7[0] = spi_read(sd->spi);
	usart_send("R1:%02x\r\n", r1);
	usart_send("R7:%02x %02x %02x %02x\r\n", r3_r7[3], r3_r7[2], r3_r7[1],
			r3_r7[0]);
	if ((r1 & spi_sd_r1_illegal_cmd) == 0) {
		sd->version = 2;
		usart_send("SD VERSION 2.0\r\n");
		if (r3_r7[1] == 0x01 && r3_r7[0] == 0x55)
			usart_send("SD Support Host Supplied Voltage\r\n");
	} else {
		sd->version = 1;
		usart_send("SD VERSION 1.0\r\n");
	}

	//Send CMD58 to read OCR
	CLEAR_ARGS(args);
	r1 = spi_sd_send_command(sd, 58, args);
	r3_r7[3] = spi_read(sd->spi);
	r3_r7[2] = spi_read(sd->spi);
	r3_r7[1] = spi_read(sd->spi);
	r3_r7[0] = spi_read(sd->spi);
	usart_send("R1:%02x\r\n", r1);
	usart_send("R3:%02x %02x %02x %02x\r\n", r3_r7[3], r3_r7[2], r3_r7[1],
			r3_r7[0]);
	/* R3
	 *  0~6 reserved
	 *  7 Reserved for Low Voltage Range
	 *  8~14 reserved
	 * 15 2.7-2.8
	 * 16 2.8-2.9
	 * 17 2.9-3.0
	 * 18 3.0-3.1
	 * 19 3.1-3.2
	 * 20 3.2-3.3
	 * 21 3.3-3.4
	 * 22 3.4-3.5
	 * 23 3.5-3.6
	 * 24 Switching to 1.8V Accepted (S18A) Only UHS-I card supports this bit.
	 * 25-26 reserved
	 * 27 Over 2TB support Status (CO2T) Only SDUCcard supports this bit.
	 * 28 reserved
	 * 29 UHS-II Card Status
	 * 30 Card Capacity Status (CCS) This  bit  is  valid only when the card power up status bit is set.
	 * 31 Card power up status bit (busy) This bit is set to LOW if the card has not finished the power up routine.
	 */

	//Send ACMD41 to init Card
	args[0] = 0;
	args[1] = 0;
	args[2] = 0;
	if (sd->version == 1)
		args[3] = 0x00;
	else
		args[3] = 0x40;
	r1 = 0xff;
	timeout = 5;
	usart_send("Send ACMD41 with HCS=%d \r\n", sd->version - 1);
	while ((r1 & spi_sd_r1_idle_state == 1) && (timeout > 0)) {
		r1 = spi_sd_send_app_command(sd, 41, args);
		timeout--;
	}
	if (timeout == 0 && (r1 & spi_sd_r1_idle_state) != 0) {
		usart_send("Send ACMD41 with HCS=%d Failed\r\n", sd->version - 1);
		if (sd->version == 2) {
			usart_send("Retry Send ACMD41 with HCS=0\r\n");
			args[0] = 0;
			args[1] = 0;
			args[2] = 0;
			args[3] = 0;
			r1 = 0xff;
			timeout = 5;
			while ((r1 & spi_sd_r1_idle_state == 1) && (timeout > 0)) {
				r1 = spi_sd_send_app_command(sd, 41, args);
				timeout--;
			}
		}
	}
	if ((r1 & spi_sd_r1_idle_state) == 0) {
		usart_send("Send ACMD41 with HCS=%d Success,Card is Ready\r\n",
				sd->version - 1);
	}

	if (sd->version == 2) { //send CMD58 for SD2.0 to Check CCS,
		CLEAR_ARGS(args);
		r1 = spi_sd_send_command(sd, 58, args);
		r3_r7[3] = spi_read(sd->spi);
		r3_r7[2] = spi_read(sd->spi);
		r3_r7[1] = spi_read(sd->spi);
		r3_r7[0] = spi_read(sd->spi);
		usart_send("R1:%02x\r\n", r1);
		usart_send("R3:%02x %02x %02x %02x\r\n", r3_r7[3], r3_r7[2], r3_r7[1],
				r3_r7[0]);
		if (r3_r7[3] & 0x40) {
			usart_send("This is SDHC or SDXC card\r\n");
			sd->block_size = 512;
			sd->is_SDSD = 0;
		} else {
			usart_send("This is SDSD card\r\n");
			sd->is_SDSD = 1;
		}
	}

	if (sd->version == 1 || sd->is_SDSD == 1) {
		args[0] = block_size_for_sdsd_mmc & 0x00ff;
		args[1] = block_size_for_sdsd_mmc >> 8;
		args[2] = 0;
		args[3] = 0;
		r1 = spi_sd_send_command(sd, 16, args);
		if (r1 == 0)
			sd->block_size = block_size_for_sdsd_mmc;
	}
	usart_send("SD Block Size is %d\r\n", sd->block_size);

	if (crc_off) { //send CMD59 turn off CRC check
		r1 = spi_sd_send_command(sd, 59, args);
		if (r1 == 0)
			usart_send("CRC Turn Off\r\n");
		else
			usart_send("CRC Failed to Turn Off\r\n");
	}

	spi_set_cs(sd->spi, 1);
	return 0;
}

unsigned long spi_sd_read(SpiSd *sd,unsigned long block_addr,unsigned char *buf,unsigned long num_block){
	unsigned char r1,crc[2],*p,timeout=255;
	unsigned int j,tmp_crc,tmp;
	unsigned long read=0,i,tmp_crc_param;
	spi_set_cs(sd->spi, 0);
	if(num_block==1)
		r1=spi_sd_send_command(sd,17,&block_addr);//signal block read
	else
		r1=spi_sd_send_command(sd,18,&block_addr);//mul-block read
	if(r1!=0)return 0;

	do{
	r1=spi_read(sd->spi);
	timeout--;
	}	while(timeout!=0&&r1!=0xfe);

	for(i=0;i<num_block;i++){
		p=buf;
		for(j=0;j<sd->block_size;j++){
			*buf=spi_read(sd->spi);
			buf++;
		}
		crc[1]=spi_read(sd->spi);
		crc[0]=spi_read(sd->spi);

		//CRC Check
		tmp_crc_param=p[0];
		tmp_crc_param=tmp_crc_param<<8;
		tmp_crc_param|=p[1];
		tmp_crc_param=tmp_crc_param<<8;
		tmp_crc_param|=p[2];
		tmp_crc_param=tmp_crc_param<<8;
		tmp_crc_param|=p[3];
		tmp_crc=crc16_calc(tmp_crc_param);
		p+=4;
		tmp=(sd->block_size-4)/2;
		for(j=0;j<tmp;j++){
			tmp_crc_param=tmp_crc;
			tmp_crc_param=tmp_crc_param<<8;
			tmp_crc_param|=p[0];
			tmp_crc_param=tmp_crc_param<<8;
			tmp_crc_param|=p[1];
		tmp_crc=crc16_calc(tmp_crc_param);
		p+=2;
		}
		tmp_crc_param=tmp_crc;
		tmp_crc_param=tmp_crc_param<<8;
		tmp_crc_param|=crc[1];
		tmp_crc_param=tmp_crc_param<<8;
		tmp_crc_param|=crc[0];
		tmp_crc=crc16_calc(tmp_crc_param);
		if(tmp_crc!=0)
			usart_send("Block Read CRC Error\r\n");
		read++;
	}
	i=0;//temp args for CMD12
	if(num_block!=1){
		r1=spi_sd_send_command(sd,12,&i);
		if(r1!=0)usart_send("Stop Read Failed\r\n");
	}
	spi_set_cs(sd->spi, 1);
	return read;
}
