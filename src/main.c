#include <avr/io.h>

#include <util/delay.h>

#include "usart.h"
#include "pwm.h"
#include "adc.h"


int main(void)
{
	pwm_init();
	
	/* Init usart with baud rate */
	usart_init(9600);
	
	/* Init adc with prescaler */
	adc_init(ADC_PS_128);

	usart_tx_str("Setup complete.\n\r");

	uint16_t adc_value;

	while (1) {
		adc_read(&adc_value);

		double dc = adc_value * 100.0 / 1023.0;
		int d = (int)dc;

		pwm_set_dc(d);
	}

	return 0;
}
