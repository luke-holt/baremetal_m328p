#include <avr/io.h>

#include "adc.h"

#define ADC_MIN_VAL		0x000
#define ADC_MAX_VAL		0x3FF


static void set_prescaler(adc_prescaler_t ps)
{
	/* Clear current prescaler bits */
	ADCSRA &= ~(7 << ADPS0);

	/* Set prescaler bits */
	ADCSRA |= (ps << ADPS0);
}


static void set_int_enable(uint8_t en)
{
	if (en == 1) {
		/* Set interrupt enable bit */
		ADCSRA |= (1 << ADIE);
		return;
	}

	if (en == 0) {
		/* Clear interrupt enable bit */
		ADCSRA &= ~(1 << ADIE);
		return;
	}

	/* TODO: ERROR: value not sup */
}


static void enable(void)
{
	/* Select voltage reference */
	ADMUX |= (1 << REFS0);

	/* Enable ADC and do first conversion */
	ADCSRA |= (1 << ADEN) | (1 << ADSC);
}


static inline void trig_conv(void)
{
	ADCSRA |= (1 << ADSC);
}


static void set_channel(adc_channel_t ch)
{
	/* If current cfg is different than desired */
	if ((ADMUX & (7 << MUX0)) != ch){
		/* Clear cfg */
		ADMUX &= ~(7 << MUX0);

		/* Set channel bits */
		ADMUX |= (ch << MUX0);
	}
}


static void read(adc_channel_t ch, uint16_t *value)
{
	set_channel(ch);

	if ((ADCSRA & (1 << ADIE)) != (1 << ADIE)) {
		trig_conv();

		/* Wait for conversion to complete */
		while (!(ADCSRA & (1 << ADIF)));
	}

	*value = ADC & ADC_MAX_VAL;
}


adc_driver_api_t adc_get_inst(void)
{
	adc_driver_api_t api = {
		.set_prescaler = set_prescaler,
		.set_int_enable = set_int_enable,
		.enable = enable,
		.trig_conv = trig_conv,
		.read = read,
	};

	return api;
}
