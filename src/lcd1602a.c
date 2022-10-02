#include <avr/io.h>
#include <util/delay.h>

#include "lcd1602a.h"

#define HIGH	1
#define LOW		0


/*
 * Buffer containing the current state of the LCD
 * First 16 for top row and last for bottom
 */
static char lcd_cur_state_top[16];
static char lcd_cur_state_bot[16];
static uint8_t len_top;
static uint8_t len_bot;


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


void lcd_println(char *str, uint8_t len, uint8_t line)
{
	if (line) {
		lcd_send((0x80 | 0x40), LOW);
		len_bot = 0;
	} else {
		lcd_send((0x80 | 0x00), LOW);
		len_top = 0;
	}

	int i = 0;
	char c = *str;
	while (i < len) {
		lcd_send(c, HIGH);

		if (line) {
			lcd_cur_state_bot[i] = c;
			len_bot++;
		} else {
			lcd_cur_state_top[i] = c;
			len_top++;
		}

		c = *(++str);
		i++;
	}
}


void lcd_shift_down(void)
{
	/* Clear LCD */
	lcd_send(0x01, LOW);
	_delay_ms(2);

	/* Print top into bottom */
	lcd_println(lcd_cur_state_top, len_top, 1);

	len_bot = len_top;
	len_top = 0;
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

	/* Display on, cursor on, blink off */
	lcd_send(0x0E, LOW);

	/* Entry mode */
	lcd_send(0x06, LOW);

	/* Clear display */
	lcd_send(0x01, LOW);
	_delay_ms(2);
}
