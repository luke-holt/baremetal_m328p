#include <avr/io.h>

#include "pwm.h"

#define PWM_MAX		0xFF
#define PWM_MIN		0x00


void pwm_init(pwm_pin_t pin)
{
	switch (pin) {
	case PWM_PIN_PB1:
		TCCR1A |= (2 << COM1A0) | (5 << WGM10); /* Set non-inverting mode and fast-pwm */
		TCCR1B |= (2 << CS10); /* Set prescaler to 8 */
		OCR1A = PWM_MIN; /* Set output compare value to min */
		DDRB |= (1 << DDB1); /* Enable pin as output */
		return;
	case PWM_PIN_PB2:
		TCCR1A |= (2 << COM1B0) | (5 << WGM10);
		TCCR1B |= (2 << CS10);
		OCR1B = PWM_MIN;
		DDRB |= (1 << DDB2);
		return;
	case PWM_PIN_PB3:
		TCCR2A |= (2 << COM2A0) | (5 << WGM20);
		TCCR2B |= (2 << CS20);
		OCR2A = PWM_MIN;
		DDRB |= (1 << DDB3);
		return;
	case PWM_PIN_PD3:
		TCCR2A |= (2 << COM2B0) | (3 << WGM20);
		TCCR2B |= (2 << CS20);
		OCR2B = PWM_MIN;
		DDRD |= (1 << DDD3);
		return;
	case PWM_PIN_PD5:
		TCCR0A |= (2 << COM0B0) | (3 << WGM00);
		TCCR0B |= (2 << CS00);
		OCR0A = PWM_MIN;
		DDRD |= (1 << DDD5);
		return;
	case PWM_PIN_PD6:
		TCCR0A |= (2 << COM0A0) | (3 << WGM00);
		TCCR0B |= (2 << CS00);
		OCR0B = PWM_MIN;
		DDRD |= (1 << DDD6);
		return;
	}
}


void pwm_update_dc(pwm_pin_t pin, const int dc)
{
	int d_adj = dc;

	if (d_adj > 100) d_adj = 100;
	if (d_adj < 0) d_adj = 0;
	
	double d = d_adj / 100.0 * (int)PWM_MAX;

	switch (pin) {
	case PWM_PIN_PB1:
		OCR1A = (int)d;
		break;
	case PWM_PIN_PB2:
		OCR1B = (int)d;
		break;
	case PWM_PIN_PB3:
		OCR2A = (int)d;
		break;
	case PWM_PIN_PD3:
		OCR2B = (int)d;
		break;
	case PWM_PIN_PD5:
		OCR0B = (int)d;
		break;
	case PWM_PIN_PD6:
		OCR0A = (int)d;
		break;
	}
}
