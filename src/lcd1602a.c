#include <avr/io.h>
#include <util/delay.h>

#include "gpio.h"
#include "lcd1602a.h"

#define HIGH 1
#define LOW 0

/*
 * Buffer containing the current state of the LCD
 * First 16 for top row and last for bottom
 */
static char lcd_cur_state_top[16];
static char lcd_cur_state_bot[16];

static inline void lcd_set_rs(uint8_t state) { gpio_write(&PORTD, PD2, state); }

static void lcd_pulse_en(void) {
  gpio_write(&PORTD, PD3, LOW);
  _delay_us(1);
  gpio_write(&PORTD, PD3, HIGH);
  _delay_us(1);
  gpio_write(&PORTD, PD3, LOW);

  /* Cmd needs >37us to process */
  _delay_us(100);
}

static void lcd_write_4bits(uint8_t bits) {
  gpio_write(&PORTD, PD4, (bits & 1));
  gpio_write(&PORTD, PD5, ((bits >> 1) & 1));
  gpio_write(&PORTD, PD6, ((bits >> 2) & 1));
  gpio_write(&PORTD, PD7, ((bits >> 3) & 1));
  lcd_pulse_en();
}

/**
 * @brief Send 8 bit word to LCD
 *
 * @param byte 8 bit word to send
 * @param rs 0 for instructions. 1 for data.
 */
static void lcd_send(uint8_t byte, uint8_t rs) {
  lcd_set_rs(rs);
  lcd_write_4bits(byte >> 4);
  lcd_write_4bits(byte);
}

void lcd_println(char *str, uint8_t row) {
  char *line;

  if (row == LCD_BOT_ROW) {
    lcd_send((0x80 | 0x40), LOW);
    line = lcd_cur_state_bot;
  } else {
    lcd_send((0x80 | 0x00), LOW);
    line = lcd_cur_state_top;
  }

  /* FIXME: No spaghetti pls */
  char c = *str;
  for (int i = 0; i < 16; i++) {
    if (c) {
      lcd_send(c, HIGH);

      /* Update current state buffers */
      line[i] = c;

      /* Next char */
      c = *(++str);
    } else {
      /* blank characters */
      lcd_send(0x20, HIGH);

      /* Space ascii code */
      line[i] = 0x20;
    }
  }
}

void lcd_shift_down(void) {
  /* Clear LCD */
  lcd_send(0x01, LOW);
  _delay_ms(2);

  /* Print top into bottom */
  lcd_println(lcd_cur_state_top, LCD_BOT_ROW);
}

void lcd_init(void) {
  /* Set RS, E and 4 data pins to output */
  gpio_pin_mode(&DDRD, DDD2, OUTPUT);
  gpio_pin_mode(&DDRD, DDD3, OUTPUT);
  gpio_pin_mode(&DDRD, DDD4, OUTPUT);
  gpio_pin_mode(&DDRD, DDD5, OUTPUT);
  gpio_pin_mode(&DDRD, DDD6, OUTPUT);
  gpio_pin_mode(&DDRD, DDD7, OUTPUT);

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

  /* Display on, cursor off, blink off */
  lcd_send(0x0C, LOW);

  /* Entry mode */
  lcd_send(0x06, LOW);

  /* Clear display */
  lcd_send(0x01, LOW);
  _delay_ms(2);
}
