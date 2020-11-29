#include "lcd12864-sed1565.h"

//#define A0 P20 //A0=0 CODE,A0=1 DDRAM
//#define WR P21 //WR=0 WRITE,WR=1 READ
//#define E P23 //E=0 disable,E=1 enable
//#define DATA P1 //code or data

extern void sed1565_delay( ) {
	char i = 0x2f;
	while (i)
		i--;
}

void sed1565_init(lcd12864_sed1565 *lcd ) {
	unsigned char d = sed1565_adc_select + 1;
	unsigned char reset=sed1565_reset;
	sed1565_write_data(lcd,1, &reset, 1);
	sed1565_write_data(lcd,1, &d, 1);
}

unsigned char sed1565_state(lcd12864_sed1565 *lcd ) {

	unsigned char result = 0x00;
	gpio_set(lcd->DATA,0xff);
	gpio_set(lcd->A0,0);
	gpio_set(lcd->E,0);
	gpio_set(lcd->WR,1);
	sed1565_delay();
	gpio_set(lcd->E,1);
	result = gpio_get(lcd->DATA);
	gpio_set(lcd->E,1);
	return result;
}

void sed1565_read_data(lcd12864_sed1565 *lcd, unsigned char col_addr, unsigned char *result,
		unsigned char count) {
	unsigned char i;
	gpio_set(lcd->A0,1); //ddram
	gpio_set(lcd->WR,0); //write
	gpio_set(lcd->E,0);
	gpio_set(lcd->DATA,col_addr);
	gpio_set(lcd->E,1);
	sed1565_delay();
	gpio_set(lcd->E,0);
	gpio_set(lcd->WR,1); //read
	gpio_set(lcd->DATA,0xFF);
	gpio_set(lcd->E,1);
	sed1565_delay();
	//Dummy Read
	gpio_set(lcd->E,0);
	sed1565_delay();
	for (i = 0; i < count; i++) {
		gpio_set(lcd->E,1);
		sed1565_delay();
		result[i] = gpio_get(lcd->DATA);
		i++;
		gpio_set(lcd->E,0);
	}
	return;
}

void sed1565_write_data(lcd12864_sed1565 *lcd, unsigned char iscode, unsigned char *D,
		unsigned char count) {
	unsigned char i;
	gpio_set(lcd->A0, iscode ? 0 : 1);
	gpio_set(lcd->WR,0);
	gpio_set(lcd->E,0);
	for (i = 0; i < count; i++) {
		gpio_set(lcd->DATA,D[i]);
		gpio_set(lcd->E,1);
		sed1565_delay();
		gpio_set(lcd->E,0);
	}

}

void sed1565_set_coluumn(lcd12864_sed1565 *lcd, unsigned char col) {
	unsigned char d[] = { sed1565_column_address_set_hight_bit | (col >> 4),
			sed1565_column_address_set_low_bit | (col & 0x0F), };
	sed1565_write_data(lcd,1, d, 2);
}

void sed1565_set_line(lcd12864_sed1565 *lcd, unsigned char line) {
	unsigned char d = sed1565_start_line_set | (line & 0b00111111);
	sed1565_write_data(lcd,1, &d, 1);
}

void sed1565_set_page(lcd12864_sed1565 *lcd, unsigned char page) {
	unsigned char d = sed1565_page_address_set | (page & 0x0f);
	sed1565_write_data(lcd,1, &d, 1);
}

void sed1565_write_16x16_Font(lcd12864_sed1565 *lcd, unsigned char page, unsigned char col,
		unsigned char *f) {
	sed1565_set_line(lcd,page);
	//sed1565_set_page(page);
	sed1565_set_coluumn(lcd,col);
	sed1565_write_data(lcd,0, f, 16);
	sed1565_set_line(lcd,page+16);
	//sed1565_set_page(page + 1);
	sed1565_set_coluumn(lcd,col);
	sed1565_write_data(lcd,0, f + 16, 16);
}
void sed1565_write_8x16_Font(lcd12864_sed1565 *lcd, unsigned char page, unsigned char col,
		unsigned char *f) {
	sed1565_set_page(lcd,page);
	sed1565_set_coluumn(lcd,col);
	sed1565_write_data(lcd,0, f, 8);
	sed1565_set_page(lcd,page + 1);
	sed1565_set_coluumn(lcd,col);
	sed1565_write_data(lcd,0, f + 8, 8);
}

void sed1565_write_picture(lcd12864_sed1565 *lcd, unsigned char page,unsigned char col,unsigned char w_pix,unsigned char h_pix,unsigned *img){
	unsigned char *i=img;
	unsigned char h;
	h_pix=h_pix/8;
	for(h=0;h<h_pix;h++){
		sed1565_set_page(lcd,page+h);
		sed1565_set_coluumn(lcd,col);
		sed1565_write_data(lcd,0,i,w_pix);
		i+=w_pix;
	}
}
