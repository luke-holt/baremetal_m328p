#include <avr/io.h>


enum adc_prescaler {
	ADC_PS_2 = 1,
	ADC_PS_4,
	ADC_PS_8,
	ADC_PS_16,
	ADC_PS_32,
	ADC_PS_64,
	ADC_PS_128,
};



void adc_init(const enum adc_prescaler ps);
void adc_read(uint16_t *val);
