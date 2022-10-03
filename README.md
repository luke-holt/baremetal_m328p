# Overview
---
Baremetal project for ATMEGA328P microcontroller.

Right now, the project adjusts the brightness of an LED from the voltage level of a potentiometer. USART is used to monitor the MCU, PWM to drive the LED and ADC to read the voltage level from the pot.

The `avr-gcc` toolchain is used along with `avrdude`.

# To do
---
- [ ] Add 1602a LCD support
	- [x] Achieve working example.
	- [ ] Propper command API.
- [x] Refactor Makefile to be scalable
- [ ] Refactor current HAL to be interrupt driven
	- [x] USART
    	- [x] Spend least amount of time in ISR. Set flag and leave. (Event system)
	- [ ] PWM
		- [ ] Pin select for PWM out
	- [ ] ADC
		- [ ] adc i/o select
- [ ] Refactor project structure to support multiple applications
	- [ ] Create examples dirs
	- [ ] Create apps dir
    	- [ ] Refactor makefile to support this new configuration
    	- [ ] Add makefiles for each app
- [ ] Add wiring diagram
- [ ] Add error system
- [x] Implement event system
  - Implemented basic even system where an event loop checks the registered groups for flags and calls the associated handler.
