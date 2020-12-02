all:main.hex

main.hex:main.c main.h gpio.rel LM032L.rel lcd12864-sed1565.rel
	sdcc -mmcs51 $^ -o main.ihx
	packihx main.ihx >$@
	
%.rel:%.c %.h
	sdcc -mmcs51 -c $< -o $@