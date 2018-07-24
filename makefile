wusel.o: wusel.c
	avr-gcc -c -o wusel.o -Os -mmcu=atmega32u4  wusel.c

wusel.elf: wusel.o
	avr-gcc -mmcu=atmega32u4 -o wusel.elf wusel.o

wusel.hex: wusel.elf
#	avr-objcopy -O ihex -R .eeprom wusel.elf wusel.hex
compile: wusel.hex	
#compile,the variable that the file can be called by
#-c which programmer is used to flash the hardware
	avrdude -c flash:w:wusel.hex 
