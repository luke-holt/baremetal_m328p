#include <avr/io.h>
#include <util/delay.h>

#include "lcd1602a.h"

#define HIGH	1
#define LOW		0


static void lcd_set_rs(uint8_t state)
{
	if (state) {
		PORTD |= (1 << PD2);
	} else {
		PORTD &= ~(1 << PD2);
	}
}

static void lcd_pulse_en(void)
{
    PORTD &= ~(1 << PD3);
    _delay_us(1);
    PORTD |= (1 << PD3);
    _delay_us(1);
    PORTD &= ~(1 << PD3);

    /* Cmd needs >37us to process */
    _delay_us(100);
}


static void lcd_write_4bits(uint8_t bits)
{
	PORTD = (PORTD & 0xF) | (bits << 4);
    lcd_pulse_en();
}


/**
 * @brief Send 8 bit word to LCD
 * 
 * @param byte 8 bit word to send
 * @param rs 0 for instructions. 1 for data.
 */
static void lcd_send(uint8_t byte, uint8_t rs)
{
	lcd_set_rs(rs);
	lcd_write_4bits(byte >> 4);
	lcd_write_4bits(byte);
}


void lcd_init(void)
{
	/* Set RS, E and 4 data pins to output */
	DDRD |= (0b111111 << DDD2);
	// lcd_clear_ctl_bits();

    /* Ensure LCD power up */
	_delay_ms(50);

    /* EN and RS LOW */
    lcd_set_rs(LOW);

	lcd_write_4bits(0x03);
    _delay_us(4500);

	lcd_write_4bits(0x03);
    _delay_us(4500);

	lcd_write_4bits(0x03);
    _delay_us(150);

    /* Set to 4 bit mode */
    lcd_write_4bits(0x02);

	/* Display on, cursor on, blink on */
	lcd_send(0x0E, LOW);

	/* Entry mode */
	lcd_send(0x06, LOW);

	/* Clear display */
	lcd_send(0x01, LOW);
	_delay_ms(2);

	/* Return home */
	lcd_send(0x02, LOW);
	_delay_ms(2);

	char *str = "Hello, world!";
	char c = *str;
	while (c) {
		lcd_send(c, HIGH);
		
		/* Next char */
		c = *(++str);

		_delay_ms(500);
	}
}
