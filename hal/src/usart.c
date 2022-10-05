#include <avr/io.h>

#include "usart.h"

/* Equations found in table 19-1 of datasheet. */
#define UBRR2BAUD(br)			(((F_CPU / (br * 16UL))) - 1)

#define USART_DEFAULT_BAUD		115200
#define USART_MAX_BAUD			1000000


static inline void set_baudrate(int baud_rate)
{
	UBRR0 = UBRR2BAUD(baud_rate);
}


static void set_frame_cfg(
	usart_ndata_bits_t ndata,
	usart_parity_mode_t parity,
	usart_nstop_bits_t nstop)
{
	if ((ndata + parity + nstop) == 0) {
		/*
		 * Set defaults (8-N-1)
		 * 8 data, no parity, 1 stop.
		 */
		UCSR0C |= (3 << UCSZ00);
	}

	switch (ndata) {
	case USART_NDATA_5:
	 	break;
	case USART_NDATA_6:
		UCSR0C |= (1 << UCSZ00);
		break;
	case USART_NDATA_7:
		UCSR0C |= (2 << UCSZ00);
		break;
	case USART_NDATA_8:
		UCSR0C |= (3 << UCSZ00);
		break;
	default:
		/* TODO: Error, n_data not sup */
		break;
	}

	switch (parity) {
	case USART_PARITY_DISABLED:
		UCSR0C &= ~((1 << UPM00) | (1 << UPM01));
		break;
	case USART_PARITY_EVEN:
		UCSR0C |= (2 << UPM00);
		break;
	case USART_PARITY_ODD:
		UCSR0C |= (3 << UPM00);
	default:
		/* TODO: ERROR, n_parity not sup */
		break;
	}

	switch (nstop) {
	case USART_NSTOP_1:
		UCSR0C &= ~(1 << USBS0);
		break;
	case USART_NSTOP_2:
		UCSR0C |= (1 << USBS0);
		break;
	default:
		/* TODO: ERROR, n_stop not sup */
		break;
	}

	/* Clock polarity, not used in async mode */
	UCSR0C &= ~(1 << UCPOL0);
}


static void set_int_enable(uint8_t en)
{
	if (en == 1) {
		/* Enable RX complete, TX complete and empty data register interrupts */
		UCSR0B |= (1 << RXCIE0)/* | (1 << TXCIE0) | (1 << UDRIE0)*/;
		return;
	}
	
	if (en == 0) {
		/* Disable */
		UCSR0B &= ~((1 << RXCIE0) | (1 << TXCIE0) | (1 << UDRIE0));
		return;
	}

	/* TODO: ERROR: enable must be a 1 or 0 */
}


static inline void enable(void)
{
	/* Enable transmitter, receiver */
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
}


static inline void tx_byte(uint8_t byte)
{
	/* Ensure data buffer empty */
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = byte;
}


static inline void rx_byte(uint8_t *byte)
{
	/* Ensure receive complete */
	while (!(UCSR0A & (1 << RXC0)));
	*byte = UDR0;
}


usart_driver_api_t usart_get_inst(void)
{
	usart_driver_api_t api = {
		.set_baudrate = set_baudrate,
		.set_frame_cfg = set_frame_cfg,
		.set_int_enable = set_int_enable,
		.enable = enable,
		.tx_byte = tx_byte,
		.rx_byte = rx_byte,
	};

	return api;
}
