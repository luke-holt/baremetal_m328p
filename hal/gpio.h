#ifndef GPIO_H__
#define GPIO_H__

#include <stdint.h>

typedef enum gpio_mode_t {
  INPUT,
  OUTPUT,
} gpio_mode_t;

typedef enum gpio_state_t {
  LOW,  /* Digital low */
  HIGH, /* Digital high */
} gpio_state_t;

void gpio_pin_mode(volatile unsigned char *ddr, uint8_t pin, gpio_mode_t mode);

void gpio_attach_interrupt(volatile unsigned char *port, uint8_t pin);

void gpio_write(volatile unsigned char *port, uint8_t pin, gpio_state_t state);
void gpio_toggle(volatile unsigned char *port, uint8_t pin);

#endif
