PORT = /dev/ttyUSB0
FLASH_BAUD = 115200
MONITOR_BAUD = 9600

# 8 bits, no parity bit, 1 stop bit
USART_PACKET = 8-N-1

MCU = atmega328p
CFLAGS = -Os -DF_CPU=16000000UL -mmcu=$(MCU) -I include

WARNINGS = -Wall -Wextra -Wshadow -Wpointer-arith -Wbad-function-cast -Wcast-align -Wsign-compare \
			-Waggregate-return -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wunused

CC = avr-gcc
OBJCP = avr-objcopy


app: usart.o pwm.o adc.o src/main.c
	$(CC) $(CFLAGS) $(WARNINGS) -o build/app src/main.c build/usart.o build/pwm.o build/adc.o
	$(OBJCP) -O ihex -R .eeprom build/app build/app.hex

usart.o: include/usart.h src/usart.c
	$(CC) $(CFLAGS) $(WARNINGS) -c src/usart.c -o build/usart.o

pwm.o: include/pwm.h src/pwm.c
	$(CC) $(CFLAGS) $(WARNINGS) -c src/pwm.c -o build/pwm.o

adc.o: include/adc.h src/adc.c
	$(CC) $(CFLAGS) $(WARNINGS) -c src/adc.c -o build/adc.o

clean:
	rm build/*

flash: app
	sudo avrdude -F -V -c arduino -p $(MCU) -P $(PORT) -b $(FLASH_BAUD) -U flash:w:build/app.hex

monitor:
	screen $(PORT) $(MONITOR_BAUD) $(USART_PACKET)
