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


static void print(char *str)
{
	char c = *str;
	while (c) {
		usart.tx_byte(c);
		c = *(++str);
	}
}


int main(void)
{
	usart = usart_get_instance();

	usart.set_baudrate(9600);
	usart.set_frame_cfg(0, 0, 0);
	usart.set_int_enable(1);
	usart.enable();

	/* Globally enable interrupts */
	sei();

	// print("hello, world\n\r");

	pwm_init();
	adc_init(ADC_PS_128);
	lcd_init();

	lcd_println("hello, friend", LCD_TOP_ROW);
	_delay_ms(2000);
	lcd_shift_down();
	lcd_println("my name is Luke", LCD_TOP_ROW);

	uint16_t adc_value;
	while (1) {
		adc_read(&adc_value);

		double dc = adc_value * 100.0 / 1023.0;
		int d = (int)dc;

		pwm_set_dc(d);
	}

	return 0;
}
