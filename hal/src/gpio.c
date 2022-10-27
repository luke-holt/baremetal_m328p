#include <avr/io.h>

#include "gpio.h"

void gpio_pin_mode(volatile unsigned char *ddr, uint8_t pin, gpio_mode_t mode) {
  *ddr &= ~(1 << pin);
  *ddr |= (mode << pin);
}

void gpio_attach_interrupt(volatile unsigned char *port, uint8_t pin) {}

void gpio_write(volatile unsigned char *port, uint8_t pin, gpio_state_t state) {
  if (state == HIGH) {
    *port |= (1 << pin);
  } else {
    *port &= ~(1 << pin);
  }
}

inline void gpio_toggle(volatile unsigned char *port, uint8_t pin) {
  *port = ~(*port & (1 << pin)) & (1 << pin);
}
