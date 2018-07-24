clock1.o: clock1.c
	avr-gcc -c -o wusel.o -Os -mmcu=atmega32  wusel.c

clock1.elf: clock1.o
	avr-gcc -mmcu=atmega32 -o wusel.elf wusel.o

clock1.hex: clock1.elf
#	avr-objcopy -O ihex -R .eeprom clock1.elf clock1.hex
compile: wusel.hex
	avrdude -c flash:w:wusel.hex 
