all:main.hex

clean:
	rm *.rel *.rst *.lst *.sym *.map *.mem *.ihx *.hex *.lk *.asm

main.hex:main.c main.h gpio.rel LM032L.rel  one_wire_bus.rel ds18b20.rel #lcd12864-sed1565.rel
	sdcc -mmcs51 $^ -o main.ihx
	packihx main.ihx >$@
	
%.rel:%.c %.h
	sdcc -mmcs51 -c $< -o $@
