#include "usart.h"


#define BAUDRATE	9600


int main(void)
{
	/* Get the hal api instance */
	usart_driver_api_t usart = usart_get_instance();

	/* Set baud rate */
	usart.set_baudrate(9600);

	/* Set frame configuration */
	usart.set_frame_cfg(8, 0, 1);
	/* OR */
	// usart.set_frame_cfg(0, 0, 0);
	/* ... to set default 8-N-1 */

	/* Enable transmitter and receiver */
	usart.enable();

	uint8_t c;
	while (1) {
		/* Receive byte */
		usart.rx_byte(&c);

		/* increment it */
		c++;

		/* Send it back */
		usart.tx_byte(c);
	}
}
