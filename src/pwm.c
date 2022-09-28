#include <avr/io.h>

#include "pwm.h"

#define MAX			0x03FF
#define MIN			0x0000


void pwm_init(void)
{
	DDRB |= (1 << DDB1);
	OCR1A = MIN;
	TCCR1A |= (1 << COM1A1);
	TCCR1A |= (1 << WGM11) | (1 << WGM10);
	TCCR1B |= (1 << CS11);
}


void pwm_toggle(void)
{
	if (OCR1A == MAX) {
		OCR1A = MIN;
	} else {
		OCR1A = MAX;
	}
}


void pwm_set_dc(int dc)
{
	double val = dc / 100.0 * (int)MAX;
	int v = (int)val;
	OCR1A = v;
}

