opt= -mmcs51 --model-medium 
all:main.hex

clean:
	rm *.rel *.rst *.lst *.sym *.map *.mem *.ihx *.hex *.lk *.asm

main.hex:main.c main.h gpio.rel LM032L.rel  one_wire_bus.rel ds18b20.rel ds1302.rel #lcd12864-sed1565.rel
	sdcc $(opt) $^ -o main.ihx
	packihx main.ihx >$@
	
%.rel:%.c %.h
	sdcc $(opt) -c $< -o $@
