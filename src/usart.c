#include <avr/io.h>

#include "usart.h"

#define UBRR_VALUE(br)	(((F_CPU / (br * 16UL))) - 1)


void usart_init(int baud_rate)
{
	/* Set baud rate */
	UBRR0 = UBRR_VALUE(baud_rate);

	/* Enable TXer and RXer */
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);

	/* Character size 8 bit */
	UCSR0C |= (3 << UCSZ00);
}


void usart_tx_char(char c)
{
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;
}


void usart_tx_str(char *str)
{
	char c = *str;
	while (c) {
		usart_tx_char(c);
		c = *(++str);
	}
}


void usart_rx_char(char *c)
{
	while (!(UCSR0A & (1 << RXC0))) {};
	*c = UDR0;
}
