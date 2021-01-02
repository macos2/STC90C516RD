opt= -mmcs51  --model-large #--model-large #--model-huge # --model-large #--model-medium # -DUSE_FLOATS=1
all:main.hex

clean:
	rm *.rel *.rst *.lst *.sym *.map *.mem *.ihx *.hex *.lk *.asm

main.hex:main.c main.h gpio.rel i2c_bus.rel spi_bus.rel spi_sd.rel i2c_memory.rel LM032L.rel  one_wire_bus.rel ds18b20.rel ds1302.rel spi_memory.rel #lcd12864-sed1565.rel 
	sdcc $(opt) $^ -o main.ihx
	packihx main.ihx >$@
	
%.rel:%.c %.h
	sdcc $(opt) -c $< -o $@
