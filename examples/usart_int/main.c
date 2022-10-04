#include <avr/interrupt.h>

#include "hal/usart.h"


#define BAUDRATE	9600


int main(void)
{
	/* Get the hal api instance */
	usart_driver_api_t usart = usart_get_instance();

	/* Set baud rate */
	usart.set_baudrate(9600);

	/* Set frame configuration */
	usart.set_frame_cfg(8, 0, 1);

	/* Enable usart interrupts */
	usart.int_enable();

	/* Enable AVR interrupts */
	sei();

	/* Enable transmitter and receiver */
	usart.enable();

	while (1) {
		/* TODO: EVENT: After implementing event system, finish this. */
	}
}


ISR (USART_RX_vect)
{
	/* Receive complete */
}

ISR (USART_TX_vect)
{
	/* Transmit complete */
}

ISR (USART_UDRE_vect)
{
	/* Data register UDR0 empty */
}
