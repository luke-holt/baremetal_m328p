#include <string.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include <util/delay.h>

/* New hal */
#include "usart.h"

/* Old hal */
#include "pwm.h"
#include "adc.h"
#include "lcd1602a.h"


static usart_driver_api_t usart;
static adc_driver_api_t adc;


ISR (USART_RX_vect)
{
	uint8_t d;
	usart.rx_byte(&d);
	usart.tx_byte(d);

	DDRB |= (1 << DDB3);

	PORTB |= (1 << PB3);
	_delay_ms(1);
	PORTB &= ~(1 << PB3);
}


int main(void)
{
	/* Globally enable interrupts */
	sei();

	usart = usart_get_inst();

	usart.set_baudrate(9600);
	usart.set_frame_cfg(
		USART_NDATA_8,
		USART_PARITY_DISABLED,
		USART_NSTOP_1);
	usart.set_int_enable(1);
	usart.enable();

	adc = adc_get_inst();

	adc.set_prescaler(ADC_PS_128);
	adc.set_int_enable(0);
	adc.enable();


	pwm_init();
	lcd_init();

	lcd_println("hello, friend", LCD_TOP_ROW);
	lcd_shift_down();
	lcd_println("my name is Luke", LCD_TOP_ROW);

	uint16_t adc_value;
	while (1) {
		adc.read(ADC_CH2, &adc_value);

		double dc = adc_value * 100.0 / 1023.0;
		int d = (int)dc;

		pwm_set_dc(d);
	}

	return 0;
}
