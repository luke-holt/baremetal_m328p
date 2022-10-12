[![pre-commit](https://img.shields.io/badge/pre--commit-enabled-brightgreen?logo=pre-commit&logoColor=white)](https://github.com/pre-commit/pre-commit)

# Overview
Baremetal project for ATMEGA328P microcontroller.

Right now, the project adjusts the brightness of an LED from the voltage level of a potentiometer. USART is used to monitor the MCU, PWM to drive the LED and ADC to read the voltage level from the pot.

The `avr-gcc` toolchain is used along with `avrdude`.

# To do
- [ ] Add 1602a LCD support
	- [x] Achieve working example.
	- [ ] Propper command API.
- [x] Refactor HAL API
  - [x] USART
  - [x] PWM
	- [x] Pin select for PWM out
  - [x] ADC
	- [x] adc i/o select
- [ ] Refactor project structure to support multiple applications
	- [x] Create examples dirs
		- [ ] Add wiring diagrams
	- [ ] Create apps dir
    	- [ ] Refactor makefile to support this new configuration
    	- [ ] Add makefiles for each app
- [ ] Add error system
- [x] Refactor event system to implement a message queue instead of monitoring a set of registered bytes for event flags.
	- [x] Add queue implementation
