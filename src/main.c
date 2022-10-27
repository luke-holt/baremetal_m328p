#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>

#include "event_msg.h"

/* New hal */
#include "adc.h"
#include "gpio.h"
#include "pwm.h"
#include "spi.h"
#include "usart.h"

/* Old hal */
#include "lcd1602a.h"

static usart_driver_api_t usart;
// static int usart_grpno;

// static adc_driver_api_t adc;
// static int adc_grpno;

static spi_driver_api_t spi;
// static int spi_grpno;

/*
ISR(USART_RX_vect) {
  event_msg_t *msg = malloc(sizeof(msg));
  msg->grpno = usart_grpno;
  msg->eventno = 0;
  msg->ctx = malloc(sizeof(uint8_t));

  usart.rx_byte(msg->ctx);

  event_enqueue(msg);
}

ISR(ADC_vect) {
  event_msg_t *msg = malloc(sizeof(msg));
  msg->grpno = adc_grpno;
  msg->eventno = 0;
  msg->ctx = malloc(sizeof(uint16_t));

  adc.read(ADC_CH2, msg->ctx);

  event_enqueue(msg);
}

static void usart_handler(event_msg_t *msg) {
  char *x = (char *)(msg->ctx);
  usart.tx_byte(*x);
}

static void adc_handler(event_msg_t *msg) {
  uint16_t *val = (uint16_t *)(msg->ctx);

  double dc = *val * 100.0 / 1023.0;
  int d = (int)dc;

  pwm_update_dc(PWM_PIN_PB1, d);

  adc.trig_conv();
}
*/

int main(void) {
  // usart_grpno = event_register_group(&usart_handler);
  // adc_grpno = event_register_group(&adc_handler);

  // sei();

  usart = usart_get_inst();

  usart.set_baudrate(9600);
  usart.set_frame_cfg(USART_NDATA_8, USART_PARITY_DISABLED, USART_NSTOP_1);
  usart.set_int_enable(1);
  usart.enable();
  usart.tx_byte(0x41); /* A */

  gpio_pin_mode(&DDRB, DDB1, OUTPUT);

  /*
  adc = adc_get_inst();

  adc.set_prescaler(ADC_PS_128);
  adc.set_int_enable(1);
  adc.enable();

  pwm_init(PWM_PIN_PB1);
  */

  lcd_init();

  lcd_println("hello, friend", LCD_TOP_ROW);
  lcd_shift_down();
  lcd_println("my name is jeff", LCD_TOP_ROW);

  // event_begin_loop();

  spi = spi_get_inst();
  spi_config_t spi_cfg = {
      .data_order = SPI_DORDER_MSB,
      .mode = SPI_MODE_PARENT,
      .cpol = SPI_CPOL_0,
      .cpha = SPI_CPHA_0,
      .crate = SPI_CRATE_125K,
      .int_enable = SPI_INT_ENABLE,
  };
  spi.set_config(&spi_cfg);
  spi.enable();

  char *msg = "Hello, world!";

  while (1) {
    spi.burst_write(msg, strlen(msg));

    gpio_toggle(&PORTB, PB1);

    _delay_ms(1000);
  }

  return 0;
}
