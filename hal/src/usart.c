#include <avr/io.h>

#include "usart.h"

/* Equations found in table 19-1 of datasheet. */
#define UBRR2BAUD(br)			(((F_CPU / (br * 16UL))) - 1)

#define USART_DEFAULT_BAUD		115200
#define USART_MAX_BAUD			1000000


static void set_baudrate(int baud_rate)
{
	UBRR0 = UBRR2BAUD(baud_rate);
}


static void set_frame_cfg(uint8_t n_data, uint8_t n_parity, uint8_t n_stop)
{
	if ((n_data + n_parity + n_stop) == 0) {
		/*
		 * Set defaults (8-N-1)
		 * 8 data, no parity, 1 stop.
		 */
		UCSR0C |= (3 << UCSZ00);
	}

	switch (n_data) {
	case 5:
	 	break;
	case 6:
		UCSR0C |= (1 << UCSZ00);
		break;
	case 7:
		UCSR0C |= (2 << UCSZ00);
		break;
	case 8:
		UCSR0C |= (3 << UCSZ00);
		break;
	default:
		/* TODO: Error, n_data not sup */
		break;
	}

	switch (n_parity) {
	case 0:
		/* No parity */
		UCSR0C &= ~((1 << UPM00) | (1 << UPM01));
		break;
	case 1:
		/* Even parity */
		UCSR0C |= (2 << UPM00);
		break;
	case 2:
		/* Odd parity */
		UCSR0C |= (3 << UPM00);
	default:
		/* TODO: ERROR, n_parity not sup */
		break;
	}

	switch (n_stop) {
	case 1:
		/* 1 stop bit */
		UCSR0C &= ~(1 << USBS0);
		break;
	case 2:
		/* 2 stop bit */
		UCSR0C |= (1 << USBS0);
		break;
	default:
		/* TODO: ERROR, n_stop not sup */
		break;
	}

	/* Clock polarity, not used in async mode */
	UCSR0C &= ~(1 << UCPOL0);
}


static void int_enable(uint8_t en)
{
	if (en == 1) {
		/* Enable RX complete, TX complete and empty data register interrupts */
		UCSR0B |= (1 << RXCIE0) | (1 << TXCIE0) | (1 << UDRIE0);
	} else if (en == 0) {
		/* Disable */
		UCSR0B &= ~((1 << RXCIE0) | (1 << TXCIE0) | (1 << UDRIE0));
	} else {
		/* TODO: ERROR: enable must be a 1 or 0 */
	}
}


static void enable(void)
{
	/* Enable transmitter, receiver */
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
}


static void tx_byte(uint8_t byte)
{
	/* Ensure data buffer empty */
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = byte;
}


static void rx_byte(uint8_t *byte)
{
	/* Ensure receive complete */
	while (!(UCSR0A & (1 << RXC0)));
	*byte = UDR0;
}


usart_driver_api_t usart_get_instance(void)
{
	usart_driver_api_t api = {
		.set_baudrate = set_baudrate,
		.set_frame_cfg = set_frame_cfg,
		.int_enable = int_enable,
		.enable = enable,
		.tx_byte = tx_byte,
		.rx_byte = rx_byte,
	};

	return api;
}
