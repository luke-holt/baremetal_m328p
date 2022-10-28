#include <avr/interrupt.h>
#include <avr/io.h>

#include "gpio.h"

/* Pointers to hold function callbacks */
static void (*int0_callback)(void);
static void (*int1_callback)(void);

void gpio_pin_mode(volatile unsigned char *ddr, uint8_t pin, gpio_mode_t mode) {
  *ddr &= ~(1 << pin);
  *ddr |= (mode << pin);
}

void gpio_enable_int(gpio_interrupt_t interrupt,
                     gpio_int_sense_ctl_t sense_ctl) {
  if (interrupt == GPIO_INT0) {
    EICRA |= (sense_ctl << ISC00);
    EIMSK |= (1 << INT0);
  } else {
    EICRA |= (sense_ctl << ISC10);
    EIMSK |= (1 << INT1);
  }
}

void gpio_reg_cb(gpio_interrupt_t interrupt, void (*callback)(void)) {
  if (interrupt == GPIO_INT0) {
    int0_callback = callback;
  } else {
    int1_callback = callback;
  }
}

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

ISR(INT0_vect) { int0_callback(); }

ISR(INT1_vect) { int1_callback(); }
