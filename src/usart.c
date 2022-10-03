#include <avr/io.h>
#include <avr/interrupt.h>

#include "usart.h"

#define UBRR_VALUE(br)	(((F_CPU / (br * 16UL))) - 1)
#define USART_BUF_SIZE		128

static char usart_rx_buf[USART_BUF_SIZE];
static int rx_buf_len;


void usart_init(int baud_rate)
{
	rx_buf_len = 0;

	/* Set baud rate */
	UBRR0 = UBRR_VALUE(baud_rate);

	/* Enable transmitter, receiver and rx interrupt */
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);

	/* Character size 8 bit */
	UCSR0C |= (3 << UCSZ00);

	sei();
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

ISR (USART_RX_vect)
{
	char rx_byte = UDR0;

	switch (rx_byte) {
	case 0x0D:
		/* Carriage return */

		usart_tx_str("\n\r");

		/* print buffer */
		for (int i = 0; i < rx_buf_len; i++) {
			usart_tx_char(usart_rx_buf[i]);
		}

		rx_buf_len =  0;

		break;
	case 0x7F:
		/* Char sent for "backspace" */

		usart_tx_char(0x08); /* backspace */
		usart_tx_char(0x20); /* space to remove existing char */
		usart_tx_char(0x08); /* backspace */

		rx_buf_len--;
		break;
	default:
		if (rx_buf_len < USART_BUF_SIZE)
		{
			usart_rx_buf[rx_buf_len] = rx_byte;
			rx_buf_len++;
			UDR0 = rx_byte;
		} else {
			usart_tx_str("Buffer full.\n\r");
		}
	}
}
