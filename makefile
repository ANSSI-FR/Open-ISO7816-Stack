CC=avr-gcc
CFLAGS=-Wall -ansi -pedantic
LDFLAGS=
EXEC_ELF=test.elf
EXEC_HEX=test.hex
SERIAL_PORT=/dev/ttyACM0



all: $(EXEC_HEX)	

upload: all
	avrdude -c arduino -p m328p -P $(SERIAL_PORT) -U flash:w:$(EXEC_HEX)
	
clean:
	rm -f main.o test.o usart.o $(EXEC_ELF) $(EXEC_HEX)


$(EXEC_HEX): $(EXEC_ELF)
	avr-objcopy -O ihex -R .eeprom $< $@
	
	
$(EXEC_ELF): main.o test.o usart.o
	$(CC) -mmcu=atmega328p $^ -o $@


main.o: main.c
	$(CC) -Wall -Os -DF_CPU=16000000UL -mmcu=atmega328p -c -o $@ $<

%.o: %.c %.h const_defines.h
	$(CC) -Wall -Os -DF_CPU=16000000UL -mmcu=atmega328p -c -o $@ $<
	
