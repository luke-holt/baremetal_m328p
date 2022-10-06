#include <stdlib.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include <util/delay.h>

#include "event_msg.h"

/* New hal */
#include "usart.h"

/* Old hal */
#include "pwm.h"
#include "adc.h"
#include "lcd1602a.h"


static usart_driver_api_t usart;
static int usart_grpno;

static adc_driver_api_t adc;


ISR (USART_RX_vect)
{
	event_msg_t *msg = malloc(sizeof(msg));
	msg->grpno = usart_grpno;
	msg->eventno = 0;
	msg->ctx = malloc(sizeof(uint8_t));

	usart.rx_byte(msg->ctx);

	event_enqueue(msg);
}


static void usart_handler(event_msg_t *msg)
{
	char *x = (char *)(msg->ctx);
	usart.tx_byte(*x);

	free(x);
	free(msg);
}


int main(void)
{
	usart_grpno = event_register_group(&usart_handler);

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

	event_begin_loop();

	/*
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
	*/

	return 0;
}
