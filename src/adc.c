#include <avr/io.h>

#include "adc.h"


void adc_init(const enum adc_prescaler ps)
{
	/* Select voltage reference and channel (pin PC3) */
	ADMUX |= (1 << REFS0) | (3 << MUX0);

	/* Enable ADC, start conversion, set prescaler bits, enable interrupt */
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (ps << ADPS0) | (1 << ADIE);
}


void adc_read(uint16_t *val)
{
	/* Start conversion */
	ADCSRA |= (1 << ADSC);

	while (!(ADCSRA & (1 << ADIF)));

	/* Read first 10 bits of ADC data register */
	*val = ADC & 0x3FF;
}
