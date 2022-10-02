#include <string.h>

#include <avr/io.h>

#include <util/delay.h>

#include "usart.h"
#include "pwm.h"
#include "adc.h"
#include "lcd1602a.h"


int main(void)
{
	/* Init pwm */
	pwm_init();
	
	/* Init usart with baud rate */
	usart_init(9600);
	
	/* Init adc with prescaler */
	adc_init(ADC_PS_128);

	usart_tx_str("Setup complete.\n\r");

	/* Setup lcd */
	lcd_init();

	/* Print on top row */
	char *msg = "hello, my friend";
	lcd_println(msg, strlen(msg), 0);

	/* wait */
	_delay_ms(2000);

	/* Shift top row down */
	lcd_shift_down();

	/* Print on top row */
	char *next = "My name is Luke";
	lcd_println(next, strlen(next), 0);

	uint16_t adc_value;

	while (1) {
		adc_read(&adc_value);

		double dc = adc_value * 100.0 / 1023.0;
		int d = (int)dc;

		pwm_set_dc(d);
	}

	return 0;
}
