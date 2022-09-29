# Overview
---
Baremetal project for ATMEGA328P microcontroller.

Right now, the project adjusts the brightness of an LED from the voltage level of a potentiometer. USART is used to monitor the MCU, PWM to drive the LED and ADC to read the voltage level from the pot.

The `avr-gcc` compiler is used along with `avrdude`.

# To do
---
- [ ] Add 1602a LCD support
- [x] Refactor Makefile to be scalable
- [ ] Refactor current HAL to be interrupt driven
- [ ] Create examples directory
- [ ] Add wiring diagram
