all:
	avr-gcc -DF_CPU=16000000UL -Os -mmcu=atmega32u4 wusel.c -o wusel.elf
	avr-objcopy -j .text -j .data -O ihex wusel.elf wusel.hex
	avrdude -p m32u4  -c avrispmkII -P usb -U flash:w:wusel.hex
