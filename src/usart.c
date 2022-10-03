#include <avr/io.h>
#include <avr/interrupt.h>

/* TODO: REMOVE */
#include <util/delay.h>

#include "event.h"

#include "usart.h"

#define UBRR_VALUE(br)	(((F_CPU / (br * 16UL))) - 1)
#define USART_BUF_SIZE		128

/* Flags */
#define USART_FLAG_NEW_CHAR		0x01
#define USART_FLAG_BUF_FULL		0x02
#define USART_CMD_COMPLETE		0x04


static char usart_rx_buf[USART_BUF_SIZE];
static int buf_i;

static uint8_t usart_flags;


static void handler(uint8_t flag)
{
	switch (flag) {
	case USART_FLAG_NEW_CHAR:
		if (usart_rx_buf[buf_i] == 0x0D) {
			/* Carriage return, parse buffer */
		}
		break;
	case USART_FLAG_BUF_FULL:
		usart_tx_str("Buffer full\n\r");
		break;
	}
}


void usart_init(int baud_rate)
{
	buf_i = 0;

	/* Set baud rate */
	UBRR0 = UBRR_VALUE(baud_rate);

	/* Enable transmitter, receiver and rx interrupt */
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);

	/* Character size 8 bit */
	UCSR0C |= (3 << UCSZ00);

	event_handler_register(&usart_flags, &handler);

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
	/* Set flag */
	usart_flags |= USART_FLAG_NEW_CHAR;

	uint8_t c = UDR0;

	if (buf_i < USART_BUF_SIZE) {
		usart_rx_buf[buf_i] = c;
		UDR0 = c;
	} else {
		usart_flags |= USART_FLAG_BUF_FULL;
	}
}
