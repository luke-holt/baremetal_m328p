[![pre-commit](https://img.shields.io/badge/pre--commit-enabled-brightgreen?logo=pre-commit&logoColor=white)](https://github.com/pre-commit/pre-commit)

# Overview
Baremetal project for ATMEGA328P microcontroller.

Right now, the project adjusts the brightness of an LED from the voltage level of a potentiometer. USART is used to monitor the MCU, PWM to drive the LED and ADC to read the voltage level from the pot.

The `avr-gcc` toolchain is used along with `avrdude`.

# To do
- [ ] Add 1602a LCD support
	- [ ] Propper command API.
- [ ] Refactor project structure to support multiple applications
	- [ ] Create apps dir
    	- [ ] Refactor makefile to support this new configuration
    	- [ ] Add makefiles for each app
- [ ] Add error system
