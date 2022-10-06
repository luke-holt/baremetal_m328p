#include <stdint.h>


typedef enum {
	ADC_PS_2 = 1,
	ADC_PS_4,
	ADC_PS_8,
	ADC_PS_16,
	ADC_PS_32,
	ADC_PS_64,
	ADC_PS_128,
} adc_prescaler_t;


typedef enum {
	ADC_CH0,
	ADC_CH1,
	ADC_CH2,
	ADC_CH3,
	ADC_CH4,
	ADC_CH5,
	ADC_CH6,
	ADC_CH7,
} adc_channel_t;


typedef void (*adc_set_prescaler_t)(adc_prescaler_t ps);
typedef void (*adc_set_int_enable_t)(uint8_t en);
typedef void (*adc_enable_t)(void);
typedef void (*adc_trig_conv_t)(void);
typedef void (*adc_read_t)(adc_channel_t ch, uint16_t *value);


typedef struct {

	adc_set_prescaler_t set_prescaler;
	adc_set_int_enable_t set_int_enable;
	adc_enable_t enable;
	adc_trig_conv_t trig_conv;
	adc_read_t read;

} adc_driver_api_t;


adc_driver_api_t adc_get_inst(void);
