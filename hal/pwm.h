typedef enum pwm_pin_t {
	PWM_PIN_PB1,	/* OC1A */
	PWM_PIN_PB2,	/* OC1B */
	PWM_PIN_PB3,	/* OC2A */
	PWM_PIN_PD3,	/* OC2B */
	PWM_PIN_PD5,	/* OC0B */
	PWM_PIN_PD6,	/* OC0A */
} pwm_pin_t;


/**
 * @brief Initiate PWM for a specified pin.
 * 
 * @param pin 
 */
void pwm_init(pwm_pin_t pin);


/**
 * @brief Update the duty cycle of a specific pin.
 * 
 * @param pin
 * @param dc duty cycle
 */
void pwm_update_dc(pwm_pin_t pin, const int dc);
