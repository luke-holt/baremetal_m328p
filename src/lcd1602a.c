#include <avr/io.h>
#include <util/delay.h>

#include "lcd1602a.h"

#define HIGH	1
#define LOW		0

/*
 * Current LCD pinout
 * RS	DD2
 * RW	DD3
 * E	DD4
 * D0	DD5
 * D1	DD6
 * D2	DD7
 * D3	DB0
 * D4	DB2
 * D5	DB3
 * D6	DB4
 * D7	DB5
 */


/**
 * @brief Set RS pin to either HIGH for data register, or LOW
 * for instruction register.
 * 
 * @param state
 * 
 */
static void lcd_set_rs(uint8_t state)
{
	if (state) {
		PORTD |= (1 << PD2);
	} else {
		PORTD &= ~(1 << PD2);
	}
}


/**
 * @brief Set RW pin to either HIGH for read, or LOW for write.
 * 
 * @param state 
 */
static void lcd_set_rw(uint8_t state)
{
	if (state) {
		PORTD &= ~(1 << PD3);
	} else {
		PORTD |= (1 << PD3);
	}
}


static void lcd_clear_ctl_bits(void)
{
	PORTD &= ~((1 << PIN2) | (1 << PIN3) | (1 << PIN4));
}


static void lcd_begin_write(uint8_t byte)
{
	/* TODO: Do this properly, 4hed */
	uint8_t b0, b1, b2, b3, b4, b5, b6, b7;
	b0 = byte & 0x01;
	byte >>= 1;
	b1 = byte & 0x01;
	byte >>= 1;
	b2 = byte & 0x01;
	byte >>= 1;
	b3 = byte & 0x01;
	byte >>= 1;
	b4 = byte & 0x01;
	byte >>= 1;
	b5 = byte & 0x01;
	byte >>= 1;
	b6 = byte & 0x01;
	byte >>= 1;
	b7 = byte & 0x01;
	byte >>= 1;

	PORTD |= (b0 << PIN5) | (b1 << PIN6) | (b2 << PIN7);
	PORTB |= (b3 << PIN0) | (b4 << PIN2) | (b5 << PIN3) | (b6 << PIN4) | (b7 << PIN5);

	/* EN LOW */
	PORTD &= ~(1 << PIN4);
	_delay_us(1);

	/* EN HIGH */
	PORTD |= (1 << PIN4);
	_delay_us(1);
	
	/* EN LOW */
	PORTD &= ~(1 << PIN4);
	_delay_us(1);

}


static void lcd_write_inst(uint8_t cmd)
{
	lcd_set_rs(LOW);
	lcd_set_rw(LOW);
	lcd_begin_write(cmd);
}


static void lcd_write_data(uint8_t byte)
{
	lcd_set_rs(HIGH);
	lcd_set_rw(LOW);
	lcd_begin_write(byte);
}

static void lcd_clear_bf(void)
{
	uint8_t bf;

	lcd_set_rs(LOW);
	lcd_set_rw(HIGH);

	/* Make input (D7 pin) */
	DDRB &= ~(1 << PIN5);

	/* EN LOW */
	PORTD &= ~(1 << PIN4);
	_delay_us(1);

	/* EN HIGH */
	PORTD |= (1 << PIN4);
	_delay_us(1);

	bf = ((PORTB & 0b00100000) == 0b00100000) ? 1 : 0;

	/* EN LOW */
	PORTD &= ~(1 << PIN4);
	_delay_us(1);

	/* Make output again */
	DDRB |= (1 << PIN5);

	/* Blink led if BF was set */
	PORTB |= (bf << PIN5);
	_delay_ms(1000);
	PORTB &= ~(1 << PIN5);
}


void lcd_init(void)
{
	/* RS, RW and E pins set to write, and clear them */
	DDRD |= (1 << DDD2) | (1 << DDD3) | (1 << DDD4);
	lcd_clear_ctl_bits();

	DDRD |= (1 << DDD5) | (1 << DDD6) | (1 << DDD7);
	DDRB |= (1 << DDB0) | (1 << DDB2) | (1 << DDB3) | (1 << DDB4) | (1 << DDB5);

	_delay_ms(2000);

	/* Start up sequence */
	_delay_ms(20);
	lcd_write_inst(0b00110000);
	_delay_ms(10);
	lcd_write_inst(0b00110000);
	_delay_ms(1);
	lcd_write_inst(0b00110000);

	/* Set 8-bit data length, 2 lines, and 5x8 font */
	lcd_write_inst(0b00111000);
	_delay_us(50);

	/* Set display on, cursor on, blinking off */
	lcd_write_inst(0b00001110);
	_delay_us(50);

	/* Clear display */
	lcd_write_inst(0b00000001);
	_delay_ms(5);

	/* Set cursor position to increment and not scroll */
	lcd_write_inst(0b00000110);
	_delay_us(50);

	lcd_clear_bf();

	char c, *hello = "0123456789";

	c = *hello;
	while (c) {
		lcd_write_data((uint8_t)c);
		c = *(++hello);
	}
}
