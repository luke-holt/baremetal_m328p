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

typedef enum gpio_int_sense_ctl_t {
  GPIO_INT_LOW,     /* Interrupt active low */
  GPIO_INT_CHNG,    /* Interrupt active on logical change */
  GPIO_INT_FALLING, /* Interrupt active on falling edge */
  GPIO_INT_RISING,  /* Interrupt active on rising edge */
} gpio_int_sense_ctl_t;

typedef enum gpio_interrupt_t {
  GPIO_INT0,
  GPIO_INT1,
} gpio_interrupt_t;

void gpio_pin_mode(volatile unsigned char *ddr, uint8_t pin, gpio_mode_t mode);

/* Enable INT0 and INT1 hardware interrupts */
void gpio_enable_int(gpio_interrupt_t interrupt,
                     gpio_int_sense_ctl_t sense_ctl);

void gpio_reg_cb(gpio_interrupt_t interrupt, void (*callback)(void));

void gpio_write(volatile unsigned char *port, uint8_t pin, gpio_state_t state);
void gpio_toggle(volatile unsigned char *port, uint8_t pin);

#endif
