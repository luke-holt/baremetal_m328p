# Overview
---
Baremetal project for ATMEGA328P microcontroller.

Right now, the project adjusts the brightness of an LED from the voltage level of a potentiometer. USART is used to monitor the MCU, PWM to drive the LED and ADC to read the voltage level from the pot.

The `avr-gcc` toolchain is used along with `avrdude`.

# To do
---
- [ ] Add 1602a LCD support
	- [x] Achieve working example.
	- [ ] ...
- [x] Refactor Makefile to be scalable
- [ ] Refactor current HAL to be interrupt driven
	- [x] USART
    	- [ ] Spend least amount of time in ISR. Set flag and leave. (Event system)
	- [ ] PWM
	- [ ] ADC
- [ ] Create examples directory
- [ ] Add wiring diagram
- [ ] Add error system
- [ ] Implement event system
